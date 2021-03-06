#include "aircraft_factory.hpp"

#include "aircraft.hpp"
#include "aircraft_manager.hpp"
#include "tower.hpp"

std::unique_ptr<Aircraft> AircraftFactory::create_random_aircraft(Tower& tower)
{
    return create_aircraft(*(aircraft_types[rand() % 3]), tower);
}

bool AircraftFactory::flight_is_registered(const std::string& flight) const
{
    auto it = registered_flights.find(flight);

    if (it == registered_flights.end())
    {
        return false;
    }
    return true;
}

std::string AircraftFactory::generate_unique_flight_number()
{
    std::string flight;
    do
    {
        flight = airlines[std::rand() % NUM_AIRLINES] +
                 std::to_string(MIN_FLIGHT_NUMBER + (rand() % MAX_FLIGHT_NUMBER));
    }
    while (flight_is_registered(flight));

    registered_flights.emplace(flight);

    return flight;
}

std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(const AircraftType& type, Tower& tower)
{
    const std::string flight_number = generate_unique_flight_number();
    const float angle               = (rand() % 1000) * 2 * PI / 1000.f; // random angle between 0 and 2pi
    const Point3D start             = Point3D { std::sin(angle), std::cos(angle), 0.f } * 3.f + Point3D { 0.f, 0.f, 2.f };
    const Point3D direction         = (-start).normalize();

    auto fuel = fuel_rd_dist(rengine);

    return std::make_unique<Aircraft>(type, flight_number, start, direction, tower, fuel);
}