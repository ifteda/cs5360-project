#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <algorithm>
#include <random>
#include <vector>

#include "Vec3.hpp"

class Perlin
{
public:
    Perlin()
    {
        ranvec.reserve(256);
        for (int i = 0; i < 256; ++i)
        {
            ranvec.push_back(Vec3::random(-1, 1).normalize());
        }
        p = perlinGeneratePerm();
    }

    double noise(const Vec3 &point) const
    {
        double u = point.x - floor(point.x);
        double v = point.y - floor(point.y);
        double w = point.z - floor(point.z);

        int i = floor(point.x);
        int j = floor(point.y);
        int k = floor(point.z);

        Vec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[p[(i + di) & 255] ^ p[(j + dj) & 255] ^ p[(k + dk) & 255]];

        return perlinInterp(c, u, v, w);
    }

private:
    std::vector<int> p;
    std::vector<Vec3> ranvec;

    static std::vector<int> perlinGeneratePerm()
    {
        std::vector<int> p(256);
        for (int i = 0; i < 256; ++i)
        {
            p[i] = i;
        }

        permute(p, 256);
        return p;
    }

    static void permute(std::vector<int> &p, int n)
    {
        std::random_device rd;
        std::mt19937 g(rd());

        for (int i = n - 1; i > 0; i--)
        {
            int target = int(g() % (i + 1));
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    double perlinInterp(Vec3 c[2][2][2], double u, double v, double w) const
    {
        double uu = u * u * (3 - 2 * u);
        double vv = v * v * (3 - 2 * v);
        double ww = w * w * (3 - 2 * w);

        double accum = 0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                {
                    Vec3 weight(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight);
                }
        return accum;
    }

    double dot(const Vec3 &v1, const Vec3 &v2) const
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
};

#endif