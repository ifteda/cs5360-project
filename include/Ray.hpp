#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

class Ray
{
public:
    Vec3 origin, direction;
    double time;

    Ray();
    Ray(const Vec3 &origin, const Vec3 &direction, double time = 0.0);

    Vec3 at(double t) const;
};

inline Ray::Ray() : origin(Vec3()), direction(Vec3()), time(0.0) {}

inline Ray::Ray(const Vec3 &origin, const Vec3 &direction, double time) 
    : origin(origin), direction(direction), time(time) {}

inline Vec3 Ray::at(double t) const 
{
    return origin + t * direction;
}

#endif