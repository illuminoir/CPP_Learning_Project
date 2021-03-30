#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft_factory.hpp"
#include "aircraft.hpp"
#include "GL/displayable.hpp"
#include <vector>
#include <memory>
#include <tuple>

class AircraftManager : public GL::DynamicObject, public GL::Displayable {
    private:
        std::vector<std::unique_ptr<Aircraft>> aircrafts;
        unsigned int crashed_aircrafts_count = 0;

    public:
        AircraftManager();
        int aircrafts_airline_count(const std::string& airline);

        bool move(double delta_time) override;
        void display() const override;
        void add_aircraft(std::unique_ptr<Aircraft> aircraft);
        inline unsigned int get_crashed_aircrafts_count() const { return crashed_aircrafts_count; }

        float get_required_fuel();
        bool compare_two_aircrafts(std::unique_ptr<Aircraft>& a1, std::unique_ptr<Aircraft>& a2);
};


