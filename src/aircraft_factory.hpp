#pragma once

#include "GL/texture.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include "aircraft_types.hpp"

#include <array>
#include <random>

class Aircraft;
class Tower;

class AircraftFactory
{
    private:
        std::unordered_set<std::string> registered_flights;
        bool flight_is_registered(std::string) const;
        std::string generate_unique_flight_number();
        std::mt19937 rengine;
        std::uniform_real_distribution<float> range;
    
    public:
        const static size_t NUM_AIRLINES = 8;
        std::array<std::string, NUM_AIRLINES> airlines = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
        static const size_t NUM_AIRCRAFT_TYPES = 3;
        AircraftType* aircraft_types[NUM_AIRCRAFT_TYPES] {};
        const std::array<std::string, NUM_AIRLINES> get_airlines() { return airlines; };


        void init_aircraft_types()
        {
            aircraft_types[0] = new AircraftType { .2f, .5f, .2f, MediaPath { "l1011_48px.png" } };
            aircraft_types[1] = new AircraftType { .2f, .5f, .2f, MediaPath { "b707_jat.png" } };
            aircraft_types[2] = new AircraftType { .8f, 1.6f, .8f, MediaPath { "concorde_af.png" } };
        }
        AircraftFactory()
        : rengine {std::random_device{}()}
        , range{150.f,3000.f}
        {
        }

        [[nodiscard]] std::unique_ptr<Aircraft> create_random_aircraft(Tower& tower);

        [[nodiscard]] std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type, Tower& tower);

};