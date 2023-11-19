#include <algorithm>

#include "BoundingBox.hpp"
#include "globals.hpp"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(const Vec3 &a, const Vec3 &b) : min(a), max(b) {}

Vec3 BoundingBox::centroid() const
{
    return 0.5 * (min + max);
}

bool BoundingBox::intersect(const Ray &ray, double t_min, double t_max) const
{
    for (int axis = 0; axis < 3; axis++)
    {
        double invD = 1.0f / ray.direction[axis];
        double t0 = (min[axis] - ray.origin[axis]) * invD;
        double t1 = (max[axis] - ray.origin[axis]) * invD;
        if (invD < 0.0f)
        {
            std::swap(t0, t1);
        }
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_min > t_max)
        {
            return false;
        }
    }
    numBVIntersections.fetch_add(1);
    return true;
}

BoundingBox BoundingBox::surroundingBox(const BoundingBox &box1, const BoundingBox &box2)
{
    Vec3 small(
        std::min(box1.min.x, box2.min.x),
        std::min(box1.min.y, box2.min.y),
        std::min(box1.min.z, box2.min.z));

    Vec3 big(
        std::max(box1.max.x, box2.max.x),
        std::max(box1.max.y, box2.max.y),
        std::max(box1.max.z, box2.max.z));

    return BoundingBox(small, big);
}