//
//  SoftwareRenderer
//

#include <cmath>
#include <cassert>
#include <algorithm>
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"
#include "Color.hpp"
#include "MathUtils.hpp"

namespace sr
{
    const Vector3 Vector3::ZERO(0.0F, 0.0F, 0.0F);
    const Vector3 Vector3::ONE(1.0F, 1.0F, 1.0F);
    const Vector3 Vector3::UNIT_X(1.0F, 0.0F, 0.0F);
    const Vector3 Vector3::UNIT_Y(0.0F, 1.0F, 0.0F);
    const Vector3 Vector3::UNIT_Z(0.0F, 0.0F, 1.0F);
    const Vector3 Vector3::NEGATIVE_UNIT_X(-1.0F, 0.0F, 0.0F);
    const Vector3 Vector3::NEGATIVE_UNIT_Y(0.0F, -1.0F, 0.0F);
    const Vector3 Vector3::NEGATIVE_UNIT_Z(0.0F, 0.0F, -1.0F);

    Vector3::Vector3(const Vector3& p1, const Vector3& p2)
    {
        set(p1, p2);
    }

    Vector3::Vector3(const Vector2& vec):
        x(vec.x), y(vec.y)
    {
    }

    Vector3::Vector3(const Vector2& vec, float aZ):
        x(vec.x), y(vec.y), z(aZ)
    {
    }

    Vector3& Vector3::operator=(const Vector2& vec)
    {
        x = vec.x;
        y = vec.y;
        z = 0.0F;

        return *this;
    }

    Vector3::Vector3(const Vector4& vec):
        x(vec.x), y(vec.y), z(vec.z)
    {
    }

    Vector3& Vector3::operator=(const Vector4& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;

        return *this;
    }

    Vector3::Vector3(const Color& color)
    {
        x = color.r;
        y = color.g;
        z = color.b;
    }

    Vector3& Vector3::operator=(const Color& color)
    {
        x = color.r;
        y = color.g;
        z = color.b;

        return *this;
    }

    float Vector3::angle(const Vector3& v1, const Vector3& v2)
    {
        float dx = v1.y * v2.z - v1.z * v2.y;
        float dy = v1.z * v2.x - v1.x * v2.z;
        float dz = v1.x * v2.y - v1.y * v2.x;

        return atan2f(sqrtf(dx * dx + dy * dy + dz * dz) + FLOAT_SMALL, dot(v1, v2));
    }

    void Vector3::clamp(const Vector3& min, const Vector3& max)
    {
        assert(!(min.x > max.x || min.y > max.y || min.z > max.z));

        // clamp the x value
        if (x < min.x)
            x = min.x;
        if (x > max.x)
            x = max.x;

        // clamp the y value
        if (y < min.y)
            y = min.y;
        if (y > max.y)
            y = max.y;

        // clamp the z value
        if (z < min.z)
            z = min.z;
        if (z > max.z)
            z = max.z;
    }

    void Vector3::clamp(const Vector3& vec, const Vector3& min, const Vector3& max, Vector3& dst)
    {
        assert(!(min.x > max.x || min.y > max.y || min.z > max.z));

        // clamp the x value
        dst.x = vec.x;
        if (dst.x < min.x)
            dst.x = min.x;
        if (dst.x > max.x)
            dst.x = max.x;

        // clamp the y value
        dst.y = vec.y;
        if (dst.y < min.y)
            dst.y = min.y;
        if (dst.y > max.y)
            dst.y = max.y;

        // clamp the z value
        dst.z = vec.z;
        if (dst.z < min.z)
            dst.z = min.z;
        if (dst.z > max.z)
            dst.z = max.z;
    }

    float Vector3::distance(const Vector3& vec) const
    {
        float dx = vec.x - x;
        float dy = vec.y - y;
        float dz = vec.z - z;

        return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    float Vector3::length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    void Vector3::normalize()
    {
        float n = x * x + y * y + z * z;
        if (n == 1.0F) // already normalized
            return;

        n = sqrtf(n);
        if (n < EPSILON) // too close to zero
            return;

        n = 1.0F / n;
        x *= n;
        y *= n;
        z *= n;
    }

    void Vector3::smooth(const Vector3& target, float elapsedTime, float responseTime)
    {
        if (elapsedTime > 0)
        {
            *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
        }
    }

    float Vector3::getMin() const
    {
        return std::min(x, std::min(y, z));
    }

    float Vector3::getMax() const
    {
        return std::max(x, std::max(y, z));
    }
}
