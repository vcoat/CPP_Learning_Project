#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>






template <typename Type, unsigned int Dimension>
struct Point{

    std::array<Type,Dimension> values;

    Point() = default;
    template<typename ...Types, std::enable_if_t<std::conjunction<std::is_arithmetic<Types> ...>::value,bool> = true>
    Point(const Types& ...l) : values { static_cast<Type>(l)... } {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() { return values[1]; }
    float y() const { return values[1]; }

    float& z() { return values[2]; }
    float z() const { return values[2]; }

    Point<Type, Dimension>& operator+=(const Point<Type, Dimension>& other){
        std::transform(std::begin(values),std::end(values), std::begin(other.values),std::begin(values), [](Type a,Type b){
            return a + b;
        });
        return *this;
    }

    Point<Type, Dimension>& operator-=(const Point<Type, Dimension>& other){
        std::transform(std::begin(values),std::end(values), std::begin(other.values),std::begin(values), [](Type a,Type b){
            return a - b;
        });
        return *this;
    }

    Point<Type, Dimension>& operator*=(const Point<Type, Dimension>& other){
        std::transform(std::begin(values),std::end(values), std::begin(other.values),std::begin(values), [](Type a,Type b){
            return a * b;
        });
        return *this;
    }

    Point<Type, Dimension>& operator*=(const Type scalar){
        std::transform(std::begin(values),std::end(values),std::begin(values), [scalar](Type a){
            return a * scalar;
        });
        return *this;
    }

    Point<Type, Dimension> operator+(const Point<Type, Dimension>& other) const{
        Point<Type, Dimension> result = *this;
        result += other;
        return result;
    }

    Point<Type, Dimension> operator-(const Point<Type, Dimension>& other) const{
        Point<Type, Dimension> result = *this;
        result -= other;
        return result;
    }

    Point<Type, Dimension> operator*(const Point<Type, Dimension>& other) const{
        Point<Type, Dimension> result = *this;
        result *= other;
        return result;
    }

    Point<Type, Dimension> operator*(const Type scalar) const{
        Point<Type, Dimension> result = *this;
        result *= scalar;
        return result;
    }

    Point<Type, Dimension> operator-() const{
        Point<Type, Dimension> result = *this;
        result *= -1;
        return result;
    }

    float length() const { return std::sqrt(std::inner_product(std::begin(values),std::end(values),std::begin(values),(Type)0)); }

    float distance_to(const Point<Type, Dimension>& other) const { return (*this - other).length(); }

    Point<Type, Dimension>& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point<Type, Dimension>& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
    

};

using Point2D = Point<float,2u>;
using Point3D = Point<float,3u>;

// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}
