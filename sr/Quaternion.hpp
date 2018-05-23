//
//  SoftwareRenderer
//

#pragma once

#include "Matrix4.hpp"
#include "Vector.hpp"

namespace sr
{
    class Quaternion
    {
    public:
        static const Quaternion IDENTITY;
        static const Quaternion ZERO;

        float x = 0.0F;
        float y = 0.0F;
        float z = 0.0F;
        float w = 0.0F;

        Quaternion()
        {
        }

        Quaternion(float initX, float initY, float initZ, float initW):
            x(initX), y(initY), z(initZ), w(initW)
        {
        }

        Quaternion(const Quaternion& copy):
            x(copy.x), y(copy.y), z(copy.z), w(copy.w)
        {
        }

        Quaternion operator*(const Quaternion& q) const
        {
            return Quaternion( x * q.w + y * q.z - z * q.y + w * q.x,
                              -x * q.z + y * q.w + z * q.x + w * q.y,
                               x * q.y - y * q.x + z * q.w + w * q.z,
                              -x * q.x - y * q.y - z * q.z + w * q.w);
        }

        const Quaternion& operator*=(const Quaternion& q)
        {
            float tempX =  x * q.w + y * q.z - z * q.y + w * q.x;
            float tempY = -x * q.z + y * q.w + z * q.x + w * q.y;
            float tempZ =  x * q.y - y * q.x + z * q.w + w * q.z;
            float tempW = -x * q.x - y * q.y - z * q.z + w * q.w;

            x = tempX;
            y = tempY;
            z = tempZ;
            w = tempW;

            return *this;
        }

        Quaternion operator*(float scalar) const
        {
            return Quaternion(x * scalar,
                              y * scalar,
                              z * scalar,
                              w * scalar);
        }

        const Quaternion& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;

            return *this;
        }

        Quaternion operator/(float scalar) const
        {
            return Quaternion(x / scalar,
                              y / scalar,
                              z / scalar,
                              w / scalar);
        }

        const Quaternion& operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;

            return *this;
        }

        inline Quaternion operator-() const
        {
            return Quaternion(-x, -y, -z, -w);
        }

        inline Quaternion operator+(const Quaternion& q) const
        {
            Quaternion result(*this);
            result.x += q.x;
            result.y += q.y;
            result.z += q.z;
            result.w += q.w;

            return result;
        }

        inline Quaternion& operator+=(const Quaternion& q)
        {
            x += q.x;
            y += q.y;
            z += q.z;
            w += q.w;

            return *this;
        }

        inline Quaternion operator-(const Quaternion& q) const
        {
            Quaternion result(*this);
            result.x -= q.x;
            result.y -= q.y;
            result.z -= q.z;
            result.w -= q.w;

            return result;
        }

        inline Quaternion& operator-=(const Quaternion& q)
        {
            x -= q.x;
            y -= q.y;
            z -= q.z;
            w -= q.w;

            return *this;
        }

        inline bool operator==(const Quaternion& q) const
        {
            return x == q.x && y == q.y && z == q.z && w == q.w;
        }

        inline bool operator!=(const Quaternion& q) const
        {
            return x != q.x || y != q.y || z != q.z || w != q.w;
        }

        inline void negate()
        {
            x = -x;
            y = -y;
            z = -z;
            w = -w;
        }

        inline void conjugate()
        {
            x = -x;
            y = -y;
            z = -z;
        }

        inline void invert()
        {
            float n2 = x * x + y * y + z * z + w * w; // norm squared

            if (n2 == 0.0F) return;

            // conjugate divided by norm squared
            x = -x / n2;
            y = -y / n2;
            z = -z / n2;
            w = w / n2;
        }

        float getNorm();
        void normalize();
        void rotate(float angle, Vector3F axis);
        void getRotation(float& angle, Vector3F& axis);

        void setEulerAngles(const Vector3F& angles);
        Vector3F getEulerAngles() const;
        float getEulerAngleX() const;
        float getEulerAngleY() const;
        float getEulerAngleZ() const;

        inline Vector3F operator*(const Vector3F& vector) const
        {
            return rotateVector(vector);
        }

        inline Vector3F rotateVector(const Vector3F& vector) const
        {
            Vector3F q(x, y, z);
            Vector3F t = 2.0F * Vector3F::cross(q, vector);
            Vector3F result = vector + (w * t) + Vector3F::cross(q, t);
            return result;
        }

        inline Vector3F getRightVector() const
        {
            return rotateVector(Vector3F(1.0F, 0.0F, 0.0F));
        }

        inline Vector3F getUpVector() const
        {
            return rotateVector(Vector3F(0.0F, 1.0F, 0.0F));
        }

        inline Vector3F getForwardVector() const
        {
            return rotateVector(Vector3F(0.0F, 0.0F, 1.0F));
        }

        Matrix4 getMatrix() const
        {
            Matrix4 result;

            float wx = w * x;
            float wy = w * y;
            float wz = w * z;

            float xx = x * x;
            float xy = x * y;
            float xz = x * z;

            float yy = y * y;
            float yz = y * z;

            float zz = z * z;

            result.m[0] = 1.0F - 2.0F * (yy + zz);
            result.m[4] = 2.0F * (xy - wz);
            result.m[8] = 2.0F * (xz + wy);
            result.m[12] = 0.0F;

            result.m[1] = 2.0F * (xy + wz);
            result.m[5] = 1.0F - 2.0F * (xx + zz);
            result.m[9] = 2.0F * (yz - wx);
            result.m[13] = 0.0F;

            result.m[2] = 2.0F * (xz - wy);
            result.m[6] = 2.0F * (yz + wx);
            result.m[10] = 1.0F - 2.0F * (xx + yy);
            result.m[14] = 0.0F;

            result.m[3] = 0.0F;
            result.m[7] = 0.0F;
            result.m[11] = 0.0F;
            result.m[15] = 1.0F;

            return result;
        }

        Quaternion& lerp(const Quaternion& q1, const Quaternion& q2, float t)
        {
            const float scale = 1.0F - t;
            return (*this = (q1 * scale) + (q2 * t));
        }
    };
}
