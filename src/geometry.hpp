#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <utility>

template <typename Type, const int dimension> class Point
{
public:
    std::array<Type, dimension> values;

    Point() = default;

    template <typename... T> Point(Type first, T&&... args) : values { first, std::forward<T>(args)... }
    {
        static_assert(sizeof...(args) + 1 == dimension,
                      "Incorrect amount of arguments when constructing Point object");
    }

    Type& x() { return values[0]; }
    Type x() const { return values[0]; }

    Type& y()
    {
        static_assert(dimension >= 2, "y coordinate only available when dimension is higher than 1");
        return values[1];
    }
    Type y() const
    {
        static_assert(dimension >= 2, "y coordinate only available when dimension is higher than 1");
        return values[1];
    }

    Type& z()
    {
        static_assert(dimension >= 3, "z coordinate only exists when dimension is higher than 2");
        return values[2];
    }
    Type z() const
    {
        static_assert(dimension >= 3, "z coordinate only exists when dimension is higher than 2");
        return values[2];
    }

    Point& operator+=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::plus<Type>());
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       std::minus<Type>());
        return *this;
    }

    Point& operator*=(const Type scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](Type val) { return val * scalar; });
        return *this;
    }

    Point& operator*=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       std::multiplies<Type>());
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
    Point operator*(const Type scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    std::string to_string()
    {
        std::string msg;

        using namespace std::string_literals;
        return "("s +
               std::accumulate(std::next(values.begin()), values.end(), std::to_string(values[0]),
                               [](const std::string& acc, float val)
                               { return acc + ","s + std::to_string(val); }) +
               ")"s;
    }

    float length() const
    {
        return std::sqrt(std::reduce(values.begin(), values.end(), 0.f,
                                     [](float acc, float val) { return acc + (val * val); }));
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
        Point result = *this;
        result *= (-1);
        return result;
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
    p1 *= 3; // ou 3.f, 3.0 en fonction du type de Point
    p3 *= 2;

    // Point3D { 0, 0 };
}