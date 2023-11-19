#ifndef VEC3_HPP
#define VEC3_HPP

class Vec3
{
public:
    double x, y, z; // r, g, b in context of colors 

    Vec3();
    Vec3(double x, double y, double z);

    Vec3 operator+(const Vec3 &other) const;
    Vec3 operator-() const;
    Vec3 operator-(const Vec3 &other) const;
    Vec3 operator*(double scalar) const;
    Vec3 operator*(const Vec3 &other) const;
    Vec3 operator/(double scalar) const;
    Vec3 &operator+=(const Vec3 &other);
    Vec3 &operator/=(double scalar);
    double& operator[](int i);
    const double& operator[](int i) const;

    double length() const;
    double lengthSquared() const;
    double dot(const Vec3 &other) const;
    Vec3 cross(const Vec3 &other) const;
    Vec3 normalize() const;
};

Vec3 operator*(double scalar, const Vec3 &vec);

#endif