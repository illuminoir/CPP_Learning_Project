#include "aircraft_factory.hpp"
#include "aircraft_manager.hpp"
#include "tower.hpp"
#include "aircraft.hpp"


std::unique_ptr<Aircraft> AircraftFactory::create_random_aircraft(Tower& tower)
{
    return create_aircraft(*(aircraft_types[rand() % 3]), tower);
}

bool AircraftFactory::flight_is_registered(std::string flight) const
{
    auto it = registered_flights.find(flight);

    if(it == registered_flights.end()){
        return false;
    }
    return true;
}

std::string AircraftFactory::generate_unique_flight_number()
{
    std::string flight;
    do {
        flight = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    } while(flight_is_registered(flight));

    registered_flights.emplace(flight);

    return flight;
}

std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(const AircraftType& type, Tower& tower)
{
    //AircraftManager* manager = AircraftManager::get_instance();

    const std::string flight_number = generate_unique_flight_number();
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    //manager->add_aircraft();
    return std::make_unique<Aircraft>(type, flight_number, start, direction, tower);
}