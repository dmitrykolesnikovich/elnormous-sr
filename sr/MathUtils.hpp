//
//  SoftwareRenderer
//

#pragma once

#include <cstdint>
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace sr
{
    const float TAU = 6.28318530717958647692F;
    const float PI = 3.14159265358979323846F;
    const float FLOAT_SMALL = 1.0e-37F;
    const float EPSILON = 1.19209290e-07F;
    const float SQRT2 = 1.4142135623730950488F;

    inline float lerp(float v0, float v1, float t)
    {
        return (1.0F - t) * v0 + t * v1;
    }

    inline float smoothStep(float a, float b, float t)
    {
        float remapSmoothStep = t * t * (3 - 2 * t);
        return lerp(a, b, remapSmoothStep);
    }

    inline bool isPOT(uint32_t x)
    {
        return (x != 0) && (((x - 1) & x) == 0);
    }

    inline uint32_t nextPOT(uint32_t x)
    {
        x = x - 1;
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >>16);
        return x + 1;
    }

    template<typename T> inline int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    inline float degToRad(float x)
    {
        return x * 0.0174532925F;
    }

    inline float radToDeg(float x)
    {
        return x * 57.29577951F;
    }

    template<class T>
    inline T clamp(T x, T lo, T hi)
    {
        return (x < lo) ? lo : ((x > hi) ? hi : x);
    }

    static const uint64_t INITIAL_FNV = 2166136261U;
    static const uint64_t FNV_MULTIPLE = 16777619;

    // Fowler / Noll / Vo (FNV) hash
    inline uint64_t fnvHash(uint64_t s)
    {
        uint64_t hash = INITIAL_FNV;
        for (uint64_t i = 0; i < sizeof(uint64_t); i++)
        {
            hash = hash ^ (reinterpret_cast<uint8_t*>(&s)[i]); // xor the low 8 bits
            hash = hash * FNV_MULTIPLE; // multiply by the magic number
        }
        return hash;
    }

    inline Vector3 barycentric(Vector2 a, Vector2 b, Vector2 c, Vector2 p)
    {
        Vector3 s[2];

        s[0].x = c.x - a.x;
        s[0].y = b.x - a.x;
        s[0].z = a.x - p.x;

        s[1].x = c.y - a.y;
        s[1].y = b.y - a.y;
        s[1].z = a.y - p.y;

        Vector3 u = Vector3::cross(s[0], s[1]);

        if (std::abs(u.z) > 1e-2) // dont forget that u.z is integer. If it is zero then triangle ABC is degenerate
            return Vector3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

        return Vector3(-1.0F, 1.0F, 1.0F);
    }
}
