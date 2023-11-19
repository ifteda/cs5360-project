#include <cmath>
#include <random>

#include "Vec3.hpp"

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3 &other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(double scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator*(const Vec3 &other) const
{
    return Vec3(x * other.x, y * other.y, z * other.z);
}

Vec3 Vec3::operator/(double scalar) const
{
    return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3 &Vec3::operator+=(const Vec3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3 &Vec3::operator/=(double scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

double &Vec3::operator[](int i)
{
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    if (i == 2)
        return z;
}

const double &Vec3::operator[](int i) const
{
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    if (i == 2)
        return z;
}

double Vec3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

double Vec3::lengthSquared() const
{
    return x * x + y * y + z * z;
}

double Vec3::dot(const Vec3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const
{
    return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

Vec3 Vec3::normalize() const
{
    double len = length();
    return Vec3(x / len, y / len, z / len);
}

Vec3 operator*(double scalar, const Vec3 &vec)
{
    return vec * scalar;
}