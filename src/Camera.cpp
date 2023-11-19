#include <cmath>
#include <random>

#include "Camera.hpp"
#include "globals.hpp"
#include "Utility.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Depth of Field Requirement */
Camera::Camera(const Vec3 &origin, const Vec3 &target, const Vec3 &up, double verticalFOV, double aspectRatio, double aperture, double focusDistance)
{
    double theta = verticalFOV * M_PI / 180.0;
    double viewportHeight = 2.0 *  tan(theta/2);;
    double viewportWidth = aspectRatio * viewportHeight;

    this->origin = origin;
    this->lensRadius = aperture / 2.0;

    this->w = (origin - target).normalize();
    this->u = up.cross(w).normalize();
    this->v = w.cross(u);

    horizontal = focusDistance * viewportWidth * u;
    vertical = focusDistance * viewportHeight * v;
    lowerLeftCorner = origin - horizontal/2 - vertical/2 - focusDistance * w;
}

Ray Camera::getRay(double s, double t, double time) const
{
    Vec3 rd = lensRadius * util.randomPointInUnitDisk();
    Vec3 offset = u * rd.x + v * rd.y;
    return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset, time);
}