#include "geometry.hpp"

#include <stdexcept>
#include <string>

class AircraftCrash : public std::runtime_error
{
public:
    AircraftCrash(std::string flight_number, Point3D pos, Point3D speed, std::string reason) :
        std::runtime_error { build_error_msg(flight_number, pos, speed, reason) }
    {}

private:
    static std::string build_error_msg(std::string flight_number, Point3D pos, Point3D speed,
                                       std::string reason)
    {
        using namespace std::string_literals;

        std::string msg;
        msg += "Aircraft ";
        msg += flight_number;
        msg += " crashed at position ";
        msg += pos.to_string();
        msg += " and speed ";
        msg += speed.to_string();
        msg += " because it";
        msg += reason;

        return msg;
    }
};