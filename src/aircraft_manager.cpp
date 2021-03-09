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


bool AircraftManager::move(double delta_time)
{

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