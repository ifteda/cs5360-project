#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "Camera.hpp"
#include "globals.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "Utility.hpp"
#include "Vec3.hpp"
#include "World.hpp"

// constants
const int MAX_DEPTH = 50;

// mutable
int numFrames = 2; // adjustable via args
Vec3 currentBgTop = Vec3(0.53, 0.81, 0.98);
Vec3 currentBgBottom = Vec3(0.53, 0.81, 0.98);

std::shared_ptr<CompoundShape> loadObject(std::string modelFilePath, Material *objMaterial)
{
    std::cout << "Loading file: " << modelFilePath << std::endl;

    Object *obj = new Object(modelFilePath);

    std::cout << "Successfully parsed .obj file\n"
              << "Generating triangles..."
              << std::endl;

    std::vector<std::shared_ptr<Triangle>> triangles;

    for (const auto &face : obj->getFaces())
    {
        auto tri = std::make_shared<Triangle>(Vec3(face[0].x, face[0].y, face[0].z),
                                              Vec3(face[1].x, face[1].y, face[1].z),
                                              Vec3(face[2].x, face[2].y, face[2].z),
                                              objMaterial);
        triangles.push_back(tri);
    }

    std::cout << "Successfully loaded " << modelFilePath << "!" << std::endl;

    return std::make_shared<CompoundShape>(triangles, objMaterial);
}

double clamp(double value, double min, double max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

Vec3 interpolate(const Vec3 &start, const Vec3 &end, int frame, int numFrames)
{
    return start + (end - start) * (((double)frame) / std::max(numFrames - 1, 1));
}

Vec3 rayColor(const Ray &ray, const World &world, int depth)
{
    if (depth <= 0)
    {
        return currentBgTop;
    }

    HitRecord rec;

    if (world.intersect(ray, 0.001, std::numeric_limits<double>::infinity(), rec))
    {
        numObjectIntersections.fetch_add(1);

        Vec3 attenuation;
        Ray scattered;

        if (rec.material->emissive)
        {
            return rec.material->emitted(rec.point);
        }
        else if (rec.material->scatter(ray, rec, attenuation, scattered))
        {
            return attenuation * rayColor(scattered, world, depth - 1);
        }
        return Vec3(0, 0, 0); // no scattering or emission
    }

    // gradient sky
    Vec3 unitDirection = ray.direction.normalize();
    double t = 0.5 * (unitDirection.y + 1.0);
    return (1.0 - t) * currentBgBottom + t * currentBgTop;
}

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        try
        {
            numFrames = std::stod(argv[1]);
        }
        catch (const std::exception &e)
        {
            std::cout << "Invalid format for given number of frames. Using 2 frames." << std::endl;
            numFrames = 2;
        }
    }

    // start & end colors
    const Vec3 SUN_COLOR_START = Vec3(1, 1, 0.9);
    const Vec3 SUN_COLOR_END = Vec3(0.9, 0.39, 0.28);
    const Vec3 MOON_COLOR_START = Vec3(0.88, 0.82, 0.75);
    const Vec3 MOON_COLOR_END = Vec3(0.88, 0.88, 0.9);
    const Vec3 BG_TOP_START = Vec3(0.53, 0.81, 0.98);
    const Vec3 BG_TOP_END = Vec3(0.12, 0.15, 0.3);
    const Vec3 BG_BOTTOM_START = Vec3(0.9, 0.95, 1.0);
    const Vec3 BG_BOTTOM_END = Vec3(0.2, 0.25, 0.5);
    // sphere colors & materials
    Vec3 pastelRed(0.98, 0.6, 0.6);
    Vec3 pastelOrange(0.98, 0.7, 0.58);
    Vec3 pastelBlue(0.6, 0.6, 0.98);
    Vec3 pastelPurple(0.85, 0.6, 0.98);
    Vec3 pastelPink(0.98, 0.6, 0.85);
    auto redLambertian = std::make_shared<Lambertian>(pastelRed);
    auto orangeMetal = std::make_shared<Metal>(pastelOrange, 0.1);
    auto blueLambertian = std::make_shared<Lambertian>(pastelBlue);
    auto purpleTranslucent = std::make_shared<Translucent>(2.4, pastelPurple);
    auto pinkMetal = std::make_shared<Metal>(pastelPink, 0.2);
    auto mixedMaterial = std::make_shared<MixedMaterial>(
        std::make_shared<Metal>(pastelPink, 0.1),
        std::make_shared<Lambertian>(pastelPurple),
        0.5);

    // start & end positions
    const Vec3 SUN_POSITION_START = Vec3(0, 8, -22);
    const Vec3 SUN_POSITION_END = Vec3(0, -16, -22);
    const Vec3 MOON_POSITION_START = Vec3(0, -16, -24);
    const Vec3 MOON_POSITION_END = Vec3(0, 8, -24);
    const Vec3 OBJ_POSITION_START = Vec3(0, -1.8, 18);
    const Vec3 OBJ_POSITION_END = Vec3(0, -1.8, -16);
    // sphere positions
    const Vec3 SPHERE1_START = Vec3(-12, -1.5, 8);
    const Vec3 SPHERE1_END = Vec3(-12, -1.5, -17);
    const Vec3 SPHERE2_START = Vec3(-8, -1.5, 9);
    const Vec3 SPHERE2_END = Vec3(-8, -1.5, -16.5);
    const Vec3 SPHERE3_START = Vec3(-4, -1.5, 10);
    const Vec3 SPHERE3_END = Vec3(-4, -1.5, -16);
    const Vec3 SPHERE4_START = Vec3(4, -1.5, 12);
    const Vec3 SPHERE4_END = Vec3(4, -1.5, -15);
    const Vec3 SPHERE5_START = Vec3(8, -1.5, 13);
    const Vec3 SPHERE5_END = Vec3(8, -1.5, -17.5);
    const Vec3 SPHERE6_START = Vec3(12, -1.5, 14);
    const Vec3 SPHERE6_END = Vec3(12, -1.5, -17);

    try
    {
        // camera setup
        const int imageWidth = 640;
        const int imageHeight = 480;
        const int samplesPerPixel = 100;
        Vec3 lookFrom(0, 0, 20);
        Vec3 lookAt(0, 0, 0);
        Vec3 up(0, 1, 0);
        double verticalFOV = 50;
        double aspectRatio = double(imageWidth) / double(imageHeight);
        double aperture = 0.0;
        double focusDistance = (lookFrom - lookAt).length();
        Camera camera(lookFrom, lookAt, up, verticalFOV, aspectRatio, aperture, focusDistance);

        std::vector<std::shared_ptr<Hittable>> hittables;

        // sun
        Emissive sunMaterial(SUN_COLOR_START);
        auto sun = std::make_shared<Sphere>(SUN_POSITION_START, 6.0, &sunMaterial);
        hittables.push_back(sun);
        // moon
        Emissive moonMaterial(MOON_COLOR_START);
        auto moon = std::make_shared<Sphere>(MOON_POSITION_START, 4.0, &moonMaterial);
        hittables.push_back(moon);

        // objects floating in water
        // CompoundShape loaded from .obj file
        Emissive objMaterial(Vec3(1.0, 0.8745, 0.8));
        auto obj = loadObject("../../common/objects/cube.obj", &objMaterial);
        hittables.push_back(obj);
        // spheres
        auto sphere1 = std::make_shared<Sphere>(SPHERE1_START, 0.6, redLambertian.get());
        auto sphere2 = std::make_shared<Sphere>(SPHERE2_START, 0.7, orangeMetal.get());
        auto sphere3 = std::make_shared<Sphere>(SPHERE3_START, 0.8, blueLambertian.get());
        auto sphere4 = std::make_shared<Sphere>(SPHERE4_START, 0.5, purpleTranslucent.get());
        auto sphere5 = std::make_shared<Sphere>(SPHERE5_START, 0.6, pinkMetal.get());
        auto sphere6 = std::make_shared<Sphere>(SPHERE6_START, 0.7, mixedMaterial.get());
        hittables.push_back(sphere1);
        hittables.push_back(sphere2);
        hittables.push_back(sphere3);
        hittables.push_back(sphere4);
        hittables.push_back(sphere5);
        hittables.push_back(sphere6);

        // surface
        auto reflectiveWater = std::make_shared<Metal>(Vec3(0.9, 0.9, 1.0), 0.02);
        auto refractiveWater = std::make_shared<Translucent>(1.33, Vec3(0, 0.22, 0.66));
        auto surfaceMaterial = std::make_shared<MixedMaterial>(reflectiveWater, refractiveWater, 0.5);
        Vec3 surfaceBottomLeft(-30, -2, -20);
        Vec3 surfaceBottomRight(30, -2, -20);
        Vec3 surfaceTopLeft(-30, -2, 20);
        Vec3 surfaceTopRight(30, -2, 20);
        auto surfaceTri1 = std::make_shared<Triangle>(surfaceBottomLeft, surfaceBottomRight, surfaceTopLeft, surfaceMaterial.get());
        auto surfaceTri2 = std::make_shared<Triangle>(surfaceTopLeft, surfaceBottomRight, surfaceTopRight, surfaceMaterial.get());
        hittables.push_back(surfaceTri1);
        hittables.push_back(surfaceTri2);

        // backdrop
        auto translucentBackdrop = std::make_shared<Translucent>(1.33, Vec3(0.8, 0.8, 0.9));
        auto lambertBackdrop = std::make_shared<Lambertian>(Vec3(0.98, 0.98, 1));
        auto backdropMaterial = std::make_shared<MixedMaterial>(lambertBackdrop, translucentBackdrop, 0.5);
        Vec3 backdropBottomLeft(-40, -4, -30);
        Vec3 backdropBottomRight(40, -4, -30);
        Vec3 backdropTopLeft(-40, 30, -30);
        Vec3 backdropTopRight(40, 30, -30);
        auto backropTri1 = std::make_shared<Triangle>(backdropBottomLeft, backdropBottomRight, backdropTopLeft, backdropMaterial.get());
        auto backropTri2 = std::make_shared<Triangle>(backdropTopLeft, backdropBottomRight, backdropTopRight, backdropMaterial.get());
        hittables.push_back(backropTri1);
        hittables.push_back(backropTri2);

        std::cout << "Rendering images..." << std::endl;

        for (int frame = 0; frame < numFrames; ++frame)
        {
            printf("\nPreparing frame %lu...\n", frame);
            World world;

            // Reset metrics
            numRays.store(0);
            numBVIntersections.store(0);
            numObjectIntersections.store(0);
            clock_t timeStart = clock();

            // update sun, moon, & sky colors
            Vec3 currentSunColor = interpolate(SUN_COLOR_START, SUN_COLOR_END, frame, numFrames);
            Vec3 currentMoonColor = interpolate(MOON_COLOR_START, MOON_COLOR_END, frame, numFrames);
            currentBgTop = interpolate(BG_TOP_START, BG_TOP_END, frame, numFrames);
            currentBgBottom = interpolate(BG_BOTTOM_START, BG_BOTTOM_END, frame, numFrames);
            sunMaterial.emit = currentSunColor;
            moonMaterial.emit = currentMoonColor;

            // update sun & moon positions
            Vec3 currentSunPosition = interpolate(SUN_POSITION_START, SUN_POSITION_END, frame, numFrames);
            Vec3 currentMoonPosition = interpolate(MOON_POSITION_START, MOON_POSITION_END, frame, numFrames);
            sun->moveTo(currentSunPosition);
            moon->moveTo(currentMoonPosition);

            // update floating object positions
            Vec3 currentObjPosition = interpolate(OBJ_POSITION_START, OBJ_POSITION_END, frame, numFrames);
            obj->moveTo(currentObjPosition);
            // spheres
            Vec3 currentSphere1Position = interpolate(SPHERE1_START, SPHERE1_END, frame, numFrames);
            sphere1->moveTo(currentSphere1Position);
            Vec3 currentSphere2Position = interpolate(SPHERE2_START, SPHERE2_END, frame, numFrames);
            sphere2->moveTo(currentSphere2Position);
            Vec3 currentSphere3Position = interpolate(SPHERE3_START, SPHERE3_END, frame, numFrames);
            sphere3->moveTo(currentSphere3Position);
            Vec3 currentSphere4Position = interpolate(SPHERE4_START, SPHERE4_END, frame, numFrames);
            sphere4->moveTo(currentSphere4Position);
            Vec3 currentSphere5Position = interpolate(SPHERE5_START, SPHERE5_END, frame, numFrames);
            sphere5->moveTo(currentSphere5Position);
            Vec3 currentSphere6Position = interpolate(SPHERE6_START, SPHERE6_END, frame, numFrames);
            sphere6->moveTo(currentSphere6Position);

            // add objects to world
            for (auto &hittable : hittables)
            {
                world.addObject(hittable);
            }

            std::string frameFilename = "frames/output_" + std::to_string(frame) + ".ppm";
            std::ofstream file(frameFilename);
            file << "P3\n"
                 << imageWidth << " " << imageHeight << "\n255\n";

            for (int j = imageHeight - 1; j >= 0; --j)
            {
                for (int i = 0; i < imageWidth; ++i)
                {
                    Vec3 color(0, 0, 0);
                    for (int s = 0; s < samplesPerPixel; ++s)
                    {
                        double u = double(i + util.randomDouble()) / double(imageWidth - 1);
                        double v = double(j + util.randomDouble()) / double(imageHeight - 1);
                        double time = util.randomDouble(0.0, 1.0);

                        Ray ray = camera.getRay(u, v, time);
                        numRays.fetch_add(1);
                        color += rayColor(ray, world, MAX_DEPTH);
                    }

                    color /= double(samplesPerPixel);
                    int ir = static_cast<int>(255.99 * clamp(sqrt(color.x), 0.0, 1.0));
                    int ig = static_cast<int>(255.99 * clamp(sqrt(color.y), 0.0, 1.0));
                    int ib = static_cast<int>(255.99 * clamp(sqrt(color.z), 0.0, 1.0));
                    file << ir << " " << ig << " " << ib << "\n";
                }
            }

            clock_t timeEnd = clock();

            printf("Completed frame %lu!\n", frame);
            printf("Metrics\n-------\n");
            printf("Render Time                     : %04.2f (sec)\n", (float)(timeEnd - timeStart) / CLOCKS_PER_SEC);
            printf("Rays Cast                       : %lu\n", numRays.load());
            printf("Bounding Volume Intersections   : %lu\n", numBVIntersections.load());
            printf("Successful Object Intersections : %lu\n", numObjectIntersections.load());
            printf("----------------------------------------------\n");
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}