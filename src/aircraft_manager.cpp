#include "aircraft_manager.hpp"
#include "aircraft.hpp"

AircraftManager::AircraftManager()
    : GL::Displayable { 0 }
{
    GL::move_queue.emplace(this);
    GL::display_queue.emplace_back(this);
}

//AircraftManager* AircraftManager::_instance = nullptr;

/*
AircraftManager* AircraftManager::get_instance()
{
    if(_instance == nullptr)
    {
        _instance = new AircraftManager();
    }
    return _instance;
}
*/


int AircraftManager::aircrafts_airline_count(const std::string& airline)
{
    auto begin = aircrafts.begin();
    auto end   = aircrafts.end();
    return std::count_if(begin, end, [airline](const std::unique_ptr<Aircraft>& aircraft)
                         { return aircraft->get_flight_num().compare(0, airline.size(), airline) == 0; });
}

bool AircraftManager::move(double delta_time)
{

    std::sort(aircrafts.begin(), aircrafts.begin(), [](std::unique_ptr<Aircraft>& a1, std::unique_ptr<Aircraft>& a2){ return a1 < a2; });

    auto begin = aircrafts.begin();
    auto end   = aircrafts.end();

    for(auto it = begin ; it != end ; )
    {
        auto& aircraft = *it;
        try {
            aircraft->move(delta_time);
        }
        catch (const AircraftCrash& crash)
        {
            aircraft->crash();
            crashed_aircrafts_count++;
            std::cerr << crash.what() << std::endl;
        }
        ++it;
    }
    aircrafts.erase(std::remove_if(begin, end, 
        [](std::unique_ptr<Aircraft>& aircraft){ return aircraft->is_out_of_sim(); }), end);
    
/*
    for(auto it = aircrafts.begin() ; it != aircrafts.end();)
    {
        if(!(*it)->move(delta_time))
        { 
            it = aircrafts.erase(it);
            //it->reset();
        }
        else 
        {
            ++it;
        }
    }
*/
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

int AircraftManager::get_required_fuel()
{
    auto missing_fuel = [](int sum, std::unique_ptr<Aircraft>& a)
    {
        if(a->is_low_on_fuel() && !a->has_left_airport())
        {
           return sum + 3000 - a->get_fuel();
        }
        return sum;
    };
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0, missing_fuel);
}
