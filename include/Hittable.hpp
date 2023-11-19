#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>
#include <vector>

#include "BoundingBox.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Material;

struct HitRecord
{
    Vec3 point;
    Vec3 normal;
    const Material *material;
    double t;
    bool frontFace;

    inline void setFaceNormal(const Ray &r, const Vec3 &outwardNormal)
    {
        frontFace = r.direction.dot(outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    const Material *material;
    BoundingBox boundingBox;

    Hittable(const Material *material) : material(material) {}

    virtual BoundingBox calculateBoundingBox() const = 0;
    virtual bool intersect(const Ray &ray, double t_min, double t_max, HitRecord &rec) const = 0;
    virtual void moveTo(const Vec3 &pos) = 0;
    virtual Vec3 normal(const Vec3 &point) const = 0;
    virtual void translate(const Vec3 &offset) = 0;
};

class Sphere : public Hittable
{
public:
    Vec3 center_start, center_end;
    double radius;

    Sphere(const Vec3 &center, double radius, const Material *material);

    Sphere(const Vec3 &center_start, const Vec3 &center_end, double radius, const Material *material);

    BoundingBox calculateBoundingBox() const override;
    bool intersect(const Ray &ray, double tMin, double tMax, HitRecord &rec) const override;
    void moveTo(const Vec3 &pos) override;
    Vec3 normal(const Vec3 &point) const override;
    void translate(const Vec3 &offset) override;
};

class Triangle : public Hittable
{
public:
    Vec3 v0_start, v0_end, v1_start, v1_end, v2_start, v2_end;

    Triangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Material *material);
    Triangle(const Vec3 &v0_start, const Vec3 &v0_end,
             const Vec3 &v1_start, const Vec3 &v1_end,
             const Vec3 &v2_start, const Vec3 &v2_end,
             const Material *material);

    BoundingBox calculateBoundingBox() const override;
    bool intersect(const Ray &ray, double tMin, double tMax, HitRecord &rec) const override;
    void moveTo(const Vec3 &pos) override;
    Vec3 normal(const Vec3 &point) const override;
    void translate(const Vec3 &offset) override;
};

class CompoundShape : public Hittable
{
public:
    std::vector<std::shared_ptr<Triangle>> triangles;

    CompoundShape(const std::vector<std::shared_ptr<Triangle>> &triangles, const Material *material);

    BoundingBox calculateBoundingBox() const override;
    bool intersect(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    void moveTo(const Vec3 &pos) override;
    Vec3 normal(const Vec3 &point) const override;
    void translate(const Vec3 &offset) override;
};

#endif