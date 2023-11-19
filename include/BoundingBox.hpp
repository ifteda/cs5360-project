#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include "Ray.hpp"
#include "Vec3.hpp"

/* Bounding Volume Requirement */
class BoundingBox
{
public:
    Vec3 min, max;

    BoundingBox();
    BoundingBox(const Vec3 &a, const Vec3 &b);

    Vec3 centroid() const;
    bool intersect(const Ray &ray, double t_min, double t_max) const;
    static BoundingBox surroundingBox(const BoundingBox &box1, const BoundingBox &box2);
};

#endif