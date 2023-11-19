#include "Utility.hpp"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(0.0, 1.0);

Utility::Utility() {}

double Utility::randomDouble()
{
    return dis(gen);
}

double Utility::randomDouble(double min, double max)
{
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

Vec3 Utility::randomPointInUnitDisk()
{
    while (true)
    {
        auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (p.lengthSquared() >= 1)
            continue;
        return p;
    }
}

Vec3 Utility::randomUnitSphere()
{
    Vec3 point;
    do
    {
        point = 2.0 * Vec3(randomDouble(), randomDouble(), randomDouble()) - Vec3(1, 1, 1);
    } while (point.lengthSquared() >= 1.0);
    return point;
}