#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <algorithm>
#include <cmath>

#include "globals.hpp"
#include "Hittable.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Material
{
public:
    bool emissive = false;

    virtual ~Material() {}

    // default. overridden for emissive materials
    virtual Vec3 emitted(const Vec3 &point) const
    {
        return Vec3(0, 0, 0);
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;

    static Vec3 reflect(const Vec3 &v, const Vec3 &n)
    {
        return v - 2 * v.dot(n) * n;
    }

    static Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat)
    {
        double cos_theta = fmin((-uv).dot(n), 1.0);
        Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
        Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.lengthSquared())) * n;
        return r_out_perp + r_out_parallel;
    }

    static double schlick(double cosine, double ref_idx)
    {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class Dielectric : public Material
{
public:
    double ref_idx; // index of refraction
    /* examples:
        vacuum: 1
        ice: 1.31
        water: 1.33
        acrylic glass: 1.49
        window glass: 1.52
        diamond: 2.42
    */

    explicit Dielectric(double ri) : ref_idx(ri) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        attenuation = Vec3(1.0, 1.0, 1.0); // no absorption
        double etai_over_etat = (rec.frontFace) ? (1.0 / ref_idx) : ref_idx;

        Vec3 unit_direction = r_in.direction.normalize();
        double cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0)
        {
            Vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray(rec.point, reflected);
            return true;
        }

        double reflect_prob = schlick(cos_theta, etai_over_etat);
        if (util.randomDouble() < reflect_prob)
        {
            Vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray(rec.point, reflected);
            return true;
        }

        Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = Ray(rec.point, refracted);
        return true;
    }
};

class Translucent : public Dielectric
{
public:
    Vec3 tint;

    Translucent(double ri, const Vec3 &color)
        : Dielectric(ri), tint(color) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        attenuation = tint;
        Vec3 unit_direction = r_in.direction.normalize();

        double etai_over_etat = (rec.frontFace) ? (1.0 / ref_idx) : ref_idx;
        double cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0 || schlick(cos_theta, etai_over_etat) > util.randomDouble())
        {
            Vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray(rec.point, reflected);
            return true;
        }

        Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = Ray(rec.point, refracted);
        return true;
    }
};

class Emissive : public Material
{
public:
    Vec3 emit;

    explicit Emissive(const Vec3 &color) : Material(), emit(color) { emissive = true; }

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        return false;
    }

    Vec3 emitted(const Vec3 &point) const override
    {
        return emit;
    }
};

class Lambertian : public Material
{
public:
    Vec3 albedo;

    explicit Lambertian(const Vec3 &a) : albedo(a) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        Vec3 scatter_direction = rec.normal + util.randomUnitSphere();
        scattered = Ray(rec.point, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material
{
public:
    Vec3 albedo;
    double fuzz;

    Metal(const Vec3 &a, double f) : albedo(a), fuzz(std::min(f, 1.0)) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = reflect(r_in.direction.normalize(), rec.normal);
        scattered = Ray(rec.point, reflected + fuzz * util.randomUnitSphere());
        attenuation = albedo;
        return scattered.direction.dot(rec.normal) > 0;
    }
};

class MixedMaterial : public Material
{
public:
    std::shared_ptr<Material> mat1;
    std::shared_ptr<Material> mat2;
    double blend; // range: [0 (full mat1), 1 (full mat2)]

    MixedMaterial(std::shared_ptr<Material> m1, std::shared_ptr<Material> m2, double factor) : mat1(m1), mat2(m2), blend(factor) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        if (util.randomDouble() < blend)
        {
            return mat2->scatter(r_in, rec, attenuation, scattered);
        }
        else
        {
            return mat1->scatter(r_in, rec, attenuation, scattered);
        }
    }

    Vec3 emitted(const Vec3 &point) const override
    {
        return (1.0 - blend) * mat1->emitted(point) + blend * mat2->emitted(point);
    }
};

#endif