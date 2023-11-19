#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>
#include <vector>

#include "Hittable.hpp"

class World {
private:
    std::vector<std::shared_ptr<Hittable>> objects;

public:
    void addObject(std::shared_ptr<Hittable> object);
    bool intersect(const Ray& ray, double t_min, double t_max, HitRecord& rec) const;
};

#endif
