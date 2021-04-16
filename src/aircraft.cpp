#include "aircraft.hpp"

#include "GL/opengl_interface.hpp"

#include <cmath>

void Aircraft::turn_to_waypoint(double delta_time)
{
    if (!waypoints.empty())
    {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1)
        {
            const float d   = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(((target - pos) * (1.f / delta_time)) - speed);
    }
}

void Aircraft::turn(Point3D direction)
{
    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}

unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0)
    {
        const Point3D norm_speed { speed * (1.0f / speed_len) };
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * PI - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces
        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * PI))) + 1) %
               NUM_AIRCRAFT_TILES;
    }
    else
    {
        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal()
{
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal = true;
    has_landed     = true;
}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear()
{
    if (waypoints.size() > 1u)
    {
        const auto it            = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after  = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after)
        {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after)
        {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after)
        {
            landing_gear_deployed = false;
        }
    }
}

template <bool front> void Aircraft::add_waypoint(const Waypoint& wp)
{
    if constexpr (front)
    {
        waypoints.push_front(wp);
    }
    else
    {
        waypoints.push_back(wp);
    }
}

bool Aircraft::is_out_of_sim() const
{
    return has_crashed || (has_landed && waypoints.empty() && !is_at_terminal);
}

bool Aircraft::is_low_on_fuel() const
{
    return fuel < 200;
}

bool Aircraft::move(double delta_time)
{
    if (waypoints.empty())
    {
        // const auto front = false;
        for (const auto& wp : control.get_instructions(*this))
        {
            add_waypoint<false>(wp);
        }
        // waypoints = control.get_instructions(*this);
    }

    if (!is_at_terminal)
    {
        if (is_circling())
        {
            auto direction = control.reserve_terminal(*this);
            if (!direction.empty())
            {
                waypoints = std::move(direction);
            }
        }
        turn_to_waypoint(delta_time);
        // move in the direction of the current speed
        pos += speed * delta_time;

        // if we are close to our next waypoint, strike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
        {
            if (waypoints.front().is_at_terminal())
            {
                arrive_at_terminal();
            }
            else
            {
                operate_landing_gear();
            }
            waypoints.pop_front();
        }

        if (is_on_ground())
        {
            if (!landing_gear_deployed)
            {
                using namespace std::string_literals;
                throw AircraftCrash { flight_number, pos, speed, " crashed into the ground"s };
            }
        }
        else
        {
            fuel -= FUEL_DECR;
            if (fuel < 0.)
            {
                using namespace std::string_literals;
                throw AircraftCrash { flight_number, pos, speed, " has no fuel remaining"s };
            }
            // if we are in the air, but too slow, then we will sink!
            const float speed_len = speed.length();
            if (speed_len < SPEED_THRESHOLD)
            {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
    }

    return true;
}

void Aircraft::display() const
{
    type.texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM }, get_speed_octant());
}

bool Aircraft::has_terminal() const
{
    if (!waypoints.empty())
    {
        return waypoints.back().type == WaypointType::wp_terminal;
    }
    return false;
}

bool Aircraft::is_circling() const
{
    return !has_landed && !has_terminal();
}

void Aircraft::refill(float& fuel_stock)
{
    float missing_fuel = MAX_FUEL - fuel;
    int to_refill;

    to_refill = missing_fuel < fuel_stock ? missing_fuel : fuel_stock;

    fuel_stock -= to_refill;
    fuel += to_refill;

    std::cout << "Aircraft " << get_flight_num() << " refilled with " << missing_fuel << "T" << std::endl;
}
