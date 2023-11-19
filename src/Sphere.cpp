#include <algorithm>
#include <cmath>

#include "globals.hpp"
#include "Hittable.hpp"

Sphere::Sphere(const Vec3 &center, double radius, const Material *material)
    : center_start(center), center_end(center), radius(radius), Hittable(material)
{
    boundingBox = this->calculateBoundingBox();
}

Sphere::Sphere(const Vec3 &center_start, const Vec3 &center_end, double radius, const Material *material)
    : center_start(center_start), center_end(center_end), radius(radius), Hittable(material)
{
    boundingBox = this->calculateBoundingBox();
}

BoundingBox Sphere::calculateBoundingBox() const
{
    Vec3 radius_vector(radius, radius, radius);

    Vec3 min_point_start = center_start - radius_vector;
    Vec3 max_point_start = center_start + radius_vector;

    Vec3 min_point_end = center_end - radius_vector;
    Vec3 max_point_end = center_end + radius_vector;

    Vec3 overall_min(
        std::min(min_point_start.x, min_point_end.x),
        std::min(min_point_start.y, min_point_end.y),
        std::min(min_point_start.z, min_point_end.z));
    Vec3 overall_max(
        std::max(max_point_start.x, max_point_end.x),
        std::max(max_point_start.y, max_point_end.y),
        std::max(max_point_start.z, max_point_end.z));

    return BoundingBox(overall_min, overall_max);
}

bool Sphere::intersect(const Ray &ray, double tMin, double tMax, HitRecord &rec) const
{
    if (boundingBox.intersect(ray, tMin, tMax))
    {
        Vec3 current_center = center_start + (center_end - center_start) * ray.time;
        Vec3 oc = ray.origin - current_center;

        double a = ray.direction.lengthSquared();
        double halfB = oc.dot(ray.direction);
        double c = oc.lengthSquared() - radius * radius;
        double discriminant = halfB * halfB - a * c;

        if (discriminant > 0)
        {
            double root = std::sqrt(discriminant);
            double tTemp = (-halfB - root) / a;
            if (tTemp < tMax && tTemp > tMin)
            {
                rec.t = tTemp;
                rec.point = ray.at(rec.t);
                rec.normal = (rec.point - current_center) / radius;
                rec.material = material;
                return true;
            }
            tTemp = (-halfB + root) / a;
            if (tTemp < tMax && tTemp > tMin)
            {
                rec.t = tTemp;
                rec.point = ray.at(rec.t);
                rec.normal = (rec.point - current_center) / radius;
                rec.material = material;
                return true;
            }
        }
    }
    return false;
}

void Sphere::moveTo(const Vec3 &pos)
{
    Vec3 offset = pos - center_start;
    translate(offset);
}

Vec3 Sphere::normal(const Vec3 &point) const
{
    return (point - center_start) / radius;
}

void Sphere::translate(const Vec3 &offset)
{
    center_start += offset;
    center_end += offset;
    boundingBox.min += offset;
    boundingBox.max += offset;
}