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
    auto begin = aircrafts.begin();
    auto end   = aircrafts.end();
    aircrafts.erase(std::remove_if(begin, end, 
        [delta_time](std::unique_ptr<Aircraft>& aircraft){ return !(aircraft->move(delta_time));}), end);
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