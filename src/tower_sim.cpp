#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}


void TowerSimulation::create_aircraft()
{
    assert(airport);

    aircraft_manager.add_aircraft(aircraft_factory.create_random_aircraft(airport->get_tower()));
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('b', []() { GL::ticks_per_sec++; 
                                       std::cout<<"fps : " << GL::ticks_per_sec<<std::endl; });
    GL::keystrokes.emplace('n', []()
                                { 
                                    if(GL::ticks_per_sec > 1){ 
                                        GL::ticks_per_sec--;
                                        std::cout<<"fps : " << GL::ticks_per_sec<<std::endl;} 
                                });
    GL::keystrokes.emplace('a', []() { GL::speed_incr_factor += 0.1; 
                                       std::cout<<"speed : " << GL::speed_incr_factor<<std::endl; });
    GL::keystrokes.emplace('z', []() { GL::speed_incr_factor -= 0.1; 
                                       std::cout<<"speed : " << GL::speed_incr_factor<<std::endl; });
    GL::keystrokes.emplace('m', [this]() 
                                { 
                                    std::cout << aircraft_manager.get_crashed_aircrafts_count() << " aircrafts crashed" << std::endl;
                                });

    const auto& airlines = aircraft_factory.get_airlines();

    for(size_t i = 0 ; i < airlines.size() ; ++i)
    {
        GL::keystrokes.emplace('0' + i, [airlines, this, i]() { std::cout << "Airline " << airlines[i] << " : " <<
                                                        aircraft_manager.aircrafts_airline_count(airlines[i]) << std::endl; });
    }
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [input, keystroke] : GL::keystrokes)
    {
        std::cout << input << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    assert(airport == nullptr);

    airport = new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() },
                            aircraft_manager };
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    aircraft_factory.init_aircraft_types();

    GL::loop();
}
