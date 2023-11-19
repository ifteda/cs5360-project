#include "World.hpp"

void World::addObject(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
}

bool World::intersect(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;
    double closestSoFar = t_max;

    for (const auto& object : objects) {
        if (object->intersect(ray, t_min, closestSoFar, tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}