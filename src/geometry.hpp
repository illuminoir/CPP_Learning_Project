#pragma once

#include <algorithm>
#include <numeric>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

template<typename type, const int dimension>
class Point {
public:
    std::array<type, dimension>values;

    Point() = default;

    Point(type x, type y) : values { x, y }
    {
        //static_assert(dimension == 2, "Initializing a Point2D requires two arguments");
    }

    Point(type x, type y, type z) : values { x, y, z }
    {
        //static_assert(dimension != 3, "Initializing a Point3D requires three arguments");
    }

    type& x() { return values[0]; }
    type x() const { return values[0]; }

    type& y() { return values[1]; }
    type y() const { return values[1]; }

    type& z() { return values[2]; }
    type z() const 
    { 
        static_assert(dimension >= 3, "z() coordinate only exists when dimension is bigger than 2");
        return values[2]; 
    }

    Point& operator+=(const Point other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::plus<type>());
        return *this;
    }

    Point& operator-=(const Point other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::minus<type>());
        return *this;
    }

    Point& operator*=(const type scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(), [scalar](type val){ return val * scalar; });
        return *this;
    }

    Point& operator*=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::multiplies<type>());
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }
        
    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;

    }
    Point operator*(const type scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    std::string to_string()
    {
        std::string msg;

        msg += "(";
        msg += std::to_string(x());
        msg += ",";
        msg += std::to_string(y());
        if(dimension == 3)
        {
            msg += ",";
            msg += std::to_string(z());
        }
        msg += ")";

        return msg;
    }

    float length() const 
    {
        return std::sqrt(std::reduce(values.begin(), values.end(), 0.f, [](float v1, float v2){ return v1 + (v2*v2); }));
    }

    float distance_to(const Point& other) const { return (*this - other).length(); }
    
    Point& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }

    Point operator-() const
    { 
        std::transform(v)
        if(dimension == 2)
        {
            return Point { -x(), -y() }; 
        }
        else
        {
            return Point { -x(), -y(), -z() };
        }
    }

};

using Point2D = Point<float, 2>;
using Point3D = Point<float, 3>;


// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}

inline void test_generic_points()
{
    Point<float, 2> p1;
    Point<float, 2> p2;
    auto p3 = p1 + p2;
    p1 += p2;
    p1 *= 3; //ou 3.f, 3.0 en fonction du type de Point
    p3 *= 2;

    //Point3D { 0, 0 };
}