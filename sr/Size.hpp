//
//  SoftwareRenderer
//

#ifndef SIZE_H
#define SIZE_H

#include <cstddef>
#include <type_traits>
#include "Vector.hpp"

namespace sr
{
    template <std::size_t N, typename T> class Size final
    {
    public:
        T v[N]{0};

        constexpr Size() noexcept {}

        template <typename ...A>
        explicit constexpr Size(A... args) noexcept:
            v{args...}
        {
        }

        template <std::size_t X = N, std::size_t N2, typename std::enable_if<(X != N2)>::type* = nullptr>
        explicit Size(const Size<N2, T>& size) noexcept
        {
            for (std::size_t i = 0; i < N && i < N2; ++i)
                v[i] = size.v[i];
        }

        explicit constexpr Size(const Vector<N, T>& vec) noexcept:
            v{vec.v}
        {
        }

        T& operator[](std::size_t index) noexcept { return v[index]; }
        constexpr T operator[](std::size_t index) const noexcept { return v[index]; }

        template <std::size_t X = N, typename std::enable_if<(X >= 1)>::type* = nullptr>
        T& width() noexcept { return v[0]; }

        template <std::size_t X = N, typename std::enable_if<(X >= 1)>::type* = nullptr>
        constexpr T width() const noexcept { return v[0]; }

        template <std::size_t X = N, typename std::enable_if<(X >= 2)>::type* = nullptr>
        T& height() noexcept { return v[1]; }

        template <std::size_t X = N, typename std::enable_if<(X >= 2)>::type* = nullptr>
        constexpr T height() const noexcept { return v[1]; }

        template <std::size_t X = N, typename std::enable_if<(X >= 3)>::type* = nullptr>
        T& depth() noexcept { return v[2]; }

        template <std::size_t X = N, typename std::enable_if<(X >= 3)>::type* = nullptr>
        constexpr T depth() const noexcept { return v[2]; }

        void scale(const Vector<N, T>& scale) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                v[i] *= scale.v[i];
        }

        const Size operator+(const Size& size) const noexcept
        {
            Size result = *this;
            for (std::size_t i = 0; i < N; ++i)
                result.v[i] += size.v[i];
            return result;
        }

        Size& operator+=(const Size& size) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                v[i] += size.v[i];
            return *this;
        }

        const Size operator-(const Size& size) const noexcept
        {
            Size result = *this;
            for (std::size_t i = 0; i < N; ++i)
                result.v[i] -= size.v[i];
            return result;
        }

        Size& operator-=(const Size& size) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                v[i] -= size.v[i];
            return *this;
        }

        const Size operator-() const noexcept
        {
            Size result = *this;
            for (auto& c : result.v)
                c = -c;
            return result;
        }

        const Size operator*(const T scalar) const noexcept
        {
            Size result(*this);
            for (auto& c : result.v)
                c *= scalar;
            return result;
        }

        Size& operator*=(const T scalar) noexcept
        {
            for (auto& c : v)
                c *= scalar;
            return *this;
        }

        const Size operator/(const T scalar) const noexcept
        {
            Size result(*this);
            for (auto& c : result.v)
                c /= scalar;
            return result;
        }

        Size& operator/=(const T scalar) noexcept
        {
            for (auto& c : v)
                c /= scalar;
            return *this;
        }

        bool operator<(const Size& size) const noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                if (v[i] < size.v[i]) return true;
                else if (size.v[i] < v[i]) return false;
            }

            return false;
        }

        bool operator==(const Size& size) const noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                if (v[i] != size.v[i]) return false;
            return true;
        }

        bool operator!=(const Size& size) const noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                if (v[i] != size.v[i]) return true;
            return false;
        }

        bool isZero() const noexcept
        {
            for (const auto& c : v)
                if (c != T(0)) return false;
            return true;
        }

        T volume() const noexcept
        {
            T result = 0;
            for (const auto& c : v)
                result *= c;
            return result;
        }
    };

    template <std::size_t N, typename T>
    inline const Size<N, T> operator*(const Size<N, T>& size, const Vector<N, T>& v) noexcept
    {
        Size<N, T> result = size;
        for (std::size_t i = 0; i < N; ++i)
            result.v[i] *= v.v[i];
        return result;
    }

    template <std::size_t N, typename T>
    inline const Size<N, T> operator/(const Size<N, T>& size, const Vector<N, T>& v) noexcept
    {
        Size<N, T> result = size;
        for (std::size_t i = 0; i < N; ++i)
            result.v[i] /= v.v[i];
        return result;
    }

    using Size2U = Size<2, std::uint32_t>;
    using Size3U = Size<3, std::uint32_t>;
    using Size2F = Size<2, float>;
    using Size3F = Size<3, float>;
}

#endif
