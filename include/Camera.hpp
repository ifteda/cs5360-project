#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Ray.hpp"
#include "Vec3.hpp"

class Camera
{
public:
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lensRadius;

    Camera(const Vec3& origin, const Vec3& target, const Vec3& up, double verticalFOV, double aspectRatio, double aperture, double focusDistance);

    Ray getRay(double s, double t, double time) const;
};

#endif