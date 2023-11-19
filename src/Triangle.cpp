#include <algorithm>
#include <limits>

#include "Hittable.hpp"

Triangle::Triangle(const Vec3 &v0,
                   const Vec3 &v1,
                   const Vec3 &v2,
                   const Material *material)
    : v0_start(v0), v0_end(v0),
      v1_start(v1), v1_end(v1),
      v2_start(v2), v2_end(v2), Hittable(material)
{
    boundingBox = this->calculateBoundingBox();
}

Triangle::Triangle(const Vec3 &v0_start, const Vec3 &v0_end,
                   const Vec3 &v1_start, const Vec3 &v1_end,
                   const Vec3 &v2_start, const Vec3 &v2_end,
                   const Material *material)
    : v0_start(v0_start), v0_end(v0_end),
      v1_start(v1_start), v1_end(v1_end),
      v2_start(v2_start), v2_end(v2_end), Hittable(material)
{
    boundingBox = this->calculateBoundingBox();
}

BoundingBox Triangle::calculateBoundingBox() const
{
    Vec3 min_point_start(
        std::min({v0_start.x, v1_start.x, v2_start.x}),
        std::min({v0_start.y, v1_start.y, v2_start.y}),
        std::min({v0_start.z, v1_start.z, v2_start.z}));
    Vec3 max_point_start(
        std::max({v0_start.x, v1_start.x, v2_start.x}),
        std::max({v0_start.y, v1_start.y, v2_start.y}),
        std::max({v0_start.z, v1_start.z, v2_start.z}));

    Vec3 min_point_end(
        std::min({v0_end.x, v1_end.x, v2_end.x}),
        std::min({v0_end.y, v1_end.y, v2_end.y}),
        std::min({v0_end.z, v1_end.z, v2_end.z}));
    Vec3 max_point_end(
        std::max({v0_end.x, v1_end.x, v2_end.x}),
        std::max({v0_end.y, v1_end.y, v2_end.y}),
        std::max({v0_end.z, v1_end.z, v2_end.z}));

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

bool Triangle::intersect(const Ray &ray, double tMin, double tMax, HitRecord &rec) const
{
    if (boundingBox.intersect(ray, tMin, tMax))
    {
        Vec3 v0 = v0_start + (v0_end - v0_start) * ray.time;
        Vec3 v1 = v1_start + (v1_end - v1_start) * ray.time;
        Vec3 v2 = v2_start + (v2_end - v2_start) * ray.time;

        Vec3 v0v1 = v1 - v0;
        Vec3 v0v2 = v2 - v0;
        Vec3 pvec = ray.direction.cross(v0v2);
        double det = v0v1.dot(pvec);

        if (det < std::numeric_limits<double>::epsilon() && det > -std::numeric_limits<double>::epsilon())
            return false;

        double invDet = 1 / det;
        Vec3 tvec = ray.origin - v0;
        double u = tvec.dot(pvec) * invDet;

        if (u < 0 || u > 1)
            return false;

        Vec3 qvec = tvec.cross(v0v1);
        double v = ray.direction.dot(qvec) * invDet;

        if (v < 0 || u + v > 1)
            return false;

        rec.t = v0v2.dot(qvec) * invDet;
        rec.point = ray.at(rec.t);
        rec.normal = v0v1.cross(v0v2).normalize();
        rec.material = material;
        rec.setFaceNormal(ray, rec.normal);

        return (rec.t >= tMin) && (rec.t <= tMax);
    }
    return false;
}

void Triangle::moveTo(const Vec3 &pos)
{
    Vec3 centroid_start = (v0_start + v1_start + v2_start) / 3.0;
    Vec3 offset = pos - centroid_start;
    translate(offset);
}

Vec3 Triangle::normal(const Vec3 &point) const
{
    return (v1_start - v0_start).cross(v2_start - v0_start).normalize();
}

void Triangle::translate(const Vec3 &offset)
{
    v0_start += offset;
    v0_end += offset;
    v1_start += offset;
    v1_end += offset;
    v2_start += offset;
    v2_end += offset;
    boundingBox.min += offset;
    boundingBox.max += offset;
}