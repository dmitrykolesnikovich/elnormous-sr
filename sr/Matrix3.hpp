//
//  SoftwareRenderer
//

#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"

namespace sr
{
    class Matrix3
    {
    public:
        static const Matrix3 IDENTITY;
        static const Matrix3 ZERO;

        float m[9];

        Matrix3()
        {
            *this = Matrix3::ZERO;
        }

        Matrix3(float m11, float m12, float m13,
                float m21, float m22, float m23,
                float m31, float m32, float m33);

        Matrix3(const float* array);

        Matrix3(const Matrix3& copy);

        float& operator[](size_t index) { return m[index]; }
        float operator[](size_t index) const { return m[index]; }

        static void createScale(const Vector2& scale, Matrix3& dst);
        static void createScale(float xScale, float yScale, Matrix3& dst);
        static void createRotation(float angle, Matrix3& dst);
        static void createTranslation(const Vector2& translation, Matrix3& dst);
        static void createTranslation(float xTranslation, float yTranslation, Matrix3& dst);

        void add(float scalar);
        void add(float scalar, Matrix3& dst);
        void add(const Matrix3& matrix);
        static void add(const Matrix3& m1, const Matrix3& m2, Matrix3& dst);

        float determinant() const;
        bool invert();
        bool invert(Matrix3& dst) const;

        bool isIdentity() const;

        void multiply(float scalar);
        void multiply(float scalar, Matrix3& dst) const;
        static void multiply(const Matrix3& matrix, float scalar, Matrix3& dst);
        void multiply(const Matrix3& matrix);
        static void multiply(const Matrix3& m1, const Matrix3& m2, Matrix3& dst);

        void negate()
        {
            negate(*this);
        }

        void negate(Matrix3& dst) const
        {
            dst.m[0] = -dst.m[0];
            dst.m[1] = -dst.m[1];
            dst.m[2] = -dst.m[2];
            dst.m[3] = -dst.m[3];
            dst.m[4] = -dst.m[4];
            dst.m[5] = -dst.m[5];
            dst.m[6] = -dst.m[6];
            dst.m[7] = -dst.m[7];
            dst.m[8] = -dst.m[8];
        }

        void rotate(float angle);
        void rotate(float angle, Matrix3& dst) const;
        void scale(float value);
        void scale(float value, Matrix3& dst) const;
        void scale(float xScale, float yScale);
        void scale(float xScale, float yScale, Matrix3& dst) const;
        void scale(const Vector2& s);
        void scale(const Vector2& s, Matrix3& dst) const;

        void set(float m11, float m12, float m13,
                 float m21, float m22, float m23,
                 float m31, float m32, float m33);
        void set(const float* array);
        void setIdentity();
        void setZero();

        void subtract(const Matrix3& matrix);

        static void subtract(const Matrix3& m1, const Matrix3& m2, Matrix3& dst);

        void transformPoint(Vector2& point) const
        {
            transformVector(point.x, point.y, 1.0F, point);
        }

        void transformPoint(const Vector2& point, Vector2& dst) const
        {
            transformVector(point.x, point.y, 1.0F, dst);
        }

        void transformVector(Vector2& vector) const
        {
            transformVector(vector.x, vector.y, 0.0F, vector);
        }

        void transformVector(const Vector2& vector, Vector2& dst) const
        {
            transformVector(vector.x, vector.y, 0.0F, dst);
        }

        void transformVector(float x, float y, float z, Vector2& dst) const
        {
            Vector3 t;
            transformVector(Vector3(x, y, z), t);
            dst = Vector2(t.x, t.y);
        }

        void transformVector(Vector3& vector) const
        {
            transformVector(vector, vector);
        }

        void transformVector(const Vector3& vector, Vector3& dst) const;

        void translate(float x, float y);
        void translate(float x, float y, Matrix3& dst) const;
        void translate(const Vector2& t);
        void translate(const Vector2& t, Matrix3& dst) const;

        void transpose();
        void transpose(Matrix3& dst) const;

        inline Matrix3 operator+(const Matrix3& matrix) const
        {
            Matrix3 result(*this);
            add(result, matrix, result);
            return result;
        }

        inline Matrix3& operator+=(const Matrix3& matrix)
        {
            add(matrix);
            return *this;
        }

        inline Matrix3 operator-(const Matrix3& matrix) const
        {
            Matrix3 result(*this);
            subtract(result, matrix, result);
            return result;
        }

        inline Matrix3& operator-=(const Matrix3& matrix)
        {
            subtract(matrix);
            return *this;
        }

        inline Matrix3 operator-() const
        {
            Matrix3 result(*this);
            negate(result);
            return result;
        }

        inline Matrix3 operator*(const Matrix3& matrix) const
        {
            Matrix3 result(*this);
            multiply(result, matrix, result);
            return result;
        }

        inline Matrix3& operator*=(const Matrix3& matrix)
        {
            multiply(matrix);
            return *this;
        }

        inline bool operator==(const Matrix3& matrix)
        {
            return m[0] == matrix.m[0] &&
                   m[1] == matrix.m[1] &&
                   m[2] == matrix.m[2] &&
                   m[3] == matrix.m[3] &&
                   m[4] == matrix.m[4] &&
                   m[5] == matrix.m[5] &&
                   m[6] == matrix.m[6] &&
                   m[7] == matrix.m[7] &&
                   m[8] == matrix.m[8];
        }

        inline bool operator!=(const Matrix3& matrix)
        {
            return m[0] != matrix.m[0] ||
                   m[1] != matrix.m[1] ||
                   m[2] != matrix.m[2] ||
                   m[3] != matrix.m[3] ||
                   m[4] != matrix.m[4] ||
                   m[5] != matrix.m[5] ||
                   m[6] != matrix.m[6] ||
                   m[7] != matrix.m[7] ||
                   m[8] != matrix.m[8];
        }
    };

    inline Vector2& operator*=(Vector2& v, const Matrix3& m)
    {
        m.transformVector(v);
        return v;
    }

    inline Vector2 operator*(const Matrix3& m, const Vector2& v)
    {
        Vector2 x;
        m.transformVector(v, x);
        return x;
    }

    inline Vector3& operator*=(Vector3& v, const Matrix3& m)
    {
        m.transformVector(v);
        return v;
    }

    inline Vector3 operator*(const Matrix3& m, const Vector3& v)
    {
        Vector3 x;
        m.transformVector(v, x);
        return x;
    }
}
