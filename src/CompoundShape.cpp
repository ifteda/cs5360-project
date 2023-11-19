#include "Hittable.hpp"

CompoundShape::CompoundShape(const std::vector<std::shared_ptr<Triangle>> &tris, const Material *material) : Hittable(material)
{
    for (const std::shared_ptr<Triangle> &tri_ptr : tris)
    {
        triangles.push_back(std::make_shared<Triangle>(*tri_ptr));
    }
    boundingBox = this->calculateBoundingBox();
}

BoundingBox CompoundShape::calculateBoundingBox() const
{
    if (triangles.empty())
        return BoundingBox();

    BoundingBox box = triangles[0]->calculateBoundingBox();
    for (const auto &tri : triangles)
    {
        box = BoundingBox::surroundingBox(box, tri->calculateBoundingBox());
    }
    return box;
}

bool CompoundShape::intersect(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    bool hitAnything = false;
    double closestT = t_max;

    for (const auto &tri : triangles)
    {
        if (tri->intersect(ray, t_min, closestT, rec))
        {
            hitAnything = true;
            closestT = rec.t;
        }
    }

    return hitAnything;
}

void CompoundShape::moveTo(const Vec3 &pos)
{
    Vec3 centroid = boundingBox.centroid();
    Vec3 offset = pos - centroid;
    translate(offset);
}

Vec3 CompoundShape::normal(const Vec3 &point) const
{
    return Vec3(0, 0, 0);
}

void CompoundShape::translate(const Vec3 &offset)
{
    for (auto &tri : triangles)
    {
        tri->translate(offset);
    }

    boundingBox.min += offset;
    boundingBox.max += offset;
}