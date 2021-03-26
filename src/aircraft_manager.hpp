#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft_factory.hpp"
#include "aircraft.hpp"
#include "GL/displayable.hpp"
#include <vector>
#include <memory>

class AircraftManager : public GL::DynamicObject, public GL::Displayable {
    private:
        //static AircraftManager* _instance;
        std::vector<std::unique_ptr<Aircraft>> aircrafts;
        int crashed_aircrafts_count = 0;

    public:
        AircraftManager();
        ~AircraftManager(){}
        //static AircraftManager* get_instance();
        int aircrafts_airline_count(const std::string& airline);

        bool move(double delta_time) override;
        void display() const override;
        void add_aircraft(std::unique_ptr<Aircraft> aircraft);
        //void create(const Tower& tower) const;
        inline int get_crashed_aircrafts_count() const { return crashed_aircrafts_count; }

        int get_required_fuel();
};


