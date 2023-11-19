#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <random>

#include "Vec3.hpp"

class Utility {
public:
    Utility();

    double randomDouble();
    double randomDouble(double min, double max);
    Vec3 randomPointInUnitDisk();
    Vec3 randomUnitSphere();
};

#endif