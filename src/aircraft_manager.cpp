#include "aircraft_manager.hpp"
#include "aircraft.hpp"

AircraftManager::AircraftManager()
    : GL::Displayable { 0 }
{
    GL::move_queue.emplace(this);
    GL::display_queue.emplace_back(this);
}

int AircraftManager::aircrafts_airline_count(const std::string& airline)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(), [&airline](const std::unique_ptr<Aircraft>& aircraft)
                         { return aircraft->get_flight_num().compare(0, airline.size(), airline) == 0; });
}

bool AircraftManager::compare_two_aircrafts(std::unique_ptr<Aircraft>& a1, std::unique_ptr<Aircraft>& a2)
{   
    return std::make_tuple(a1->has_terminal(), a1->get_fuel()) < std::make_tuple(a2->has_terminal(), a2->get_fuel()); 
}

bool AircraftManager::move(double delta_time)
{
    assert(delta_time != 0);

    std::sort(aircrafts.begin(), aircrafts.begin(),
                    [this]
                    (std::unique_ptr<Aircraft>& a1, std::unique_ptr<Aircraft>& a2)
                    { return compare_two_aircrafts(a1,a2); });

    for(std::unique_ptr<Aircraft>& aircraft : aircrafts)
    {
        try {
            aircraft->move(delta_time);
        }
        catch (const AircraftCrash& crash)
        {
            aircraft->crash();
            crashed_aircrafts_count++;
            std::cerr << crash.what() << std::endl;
        }
    }
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(), 
        [](std::unique_ptr<Aircraft>& aircraft){ return aircraft->is_out_of_sim(); }), aircrafts.end());

    return false;
}

void AircraftManager::display() const
{
    for(auto it = aircrafts.begin() ; it != aircrafts.end() ; ++it)
    {
        (*it)->display();
    }
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

float AircraftManager::get_required_fuel()
{
    auto missing_fuel = [](float sum, const std::unique_ptr<Aircraft>& a)
    {
        if(a->is_low_on_fuel() && !a->has_left_airport())
        {
           return sum + 3000.f - a->get_fuel();
        }
        return sum;
    };
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0, missing_fuel);
}