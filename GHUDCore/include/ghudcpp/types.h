#pragma once
#include <array>

namespace GHUD {
	using byte = unsigned char;

	using uint8 = unsigned char;
	using int8 = signed char;

	using uint16 = unsigned short;
	using int16 = signed short;

	using uint32 = unsigned int;
	using int32 = signed int;

#ifdef _MSC_VER
	using uint64 = unsigned long long;
	using int64 = signed long long;
#else
	using uint64 = unsigned long;
	using int64 = signed long;
#endif

    template<typename T, size_t N>
    struct Vec {
        std::array<T, N> array;
        constexpr T& operator[](size_t i) noexcept { return array[i]; }
        constexpr const T& operator[](size_t i) const noexcept { return array[i]; }
    };

    template<typename T>
    struct Vec<T, 2> {
        T x, y;

        constexpr T& operator[](size_t i) noexcept {
            switch (i) {
            case 1: return y;
            default: return x;
            }
        }
        constexpr const T& operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return y;
            default: return x;
            }
        }
    };

    template<typename T>
    struct Vec<T, 3> {
        T x, y, z;

        constexpr T& operator[](size_t i) noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            default: return x;
            }
        }

        constexpr const T& operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            default: return x;
            }
        }
    };

    template<typename T>
    struct Vec<T, 4> {
        T x, y, z, w;

        constexpr T& operator[](size_t i) noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: return x;
            }
        }

        constexpr const T& operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: return x;
            }
        }
    };

    template<>
    struct Vec<uint8, 4> {
        uint8 r, g, b, a;

        constexpr Vec(float r, float g, float b, float a) : 
            r{ static_cast<uint8>((static_cast<uint32>(r * 255.0) & 0x000000FF)) },
            g{ static_cast<uint8>((static_cast<uint32>(g * 255.0) & 0x000000FF)) },
            b{ static_cast<uint8>((static_cast<uint32>(b * 255.0) & 0x000000FF)) },
            a{ static_cast<uint8>((static_cast<uint32>(a * 255.0) & 0x000000FF)) } 
        {};

        constexpr Vec(uint8 r, uint8 g, uint8 b, uint8 a) : r{ r }, g{ g }, b{ b }, a{ a } 
        {};

        constexpr Vec(const Vec<float, 4>& col) : Vec(col.x, col.y, col.z, col.w) 
        {};

        constexpr Vec(uint32 col) : 
            r{ static_cast<uint8>((col & 0xFF000000) >> 24) },
            g{ static_cast<uint8>((col & 0x00FF0000) >> 16) },
            b{ static_cast<uint8>((col & 0x0000FF00) >> 8) },
            a{ static_cast<uint8>(col & 0x000000FF) } 
        {};

        constexpr uint8& operator[](size_t i) noexcept {
            switch (i) {
            case 1: return g;
            case 2: return b;
            case 3: return a;
            default: return r;
            }
        }

        constexpr const uint8& operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return g;
            case 2: return b;
            case 3: return a;
            default: return r;
            }
        }
    };

    template<>
    struct Vec<uint8, 3> {
        uint8 r, g, b;

        constexpr Vec(float r, float g, float b) :
            r{ static_cast<uint8>((static_cast<uint32>(r * 255.0) & 0x000000FF)) },
            g{ static_cast<uint8>((static_cast<uint32>(g * 255.0) & 0x000000FF)) },
            b{ static_cast<uint8>((static_cast<uint32>(b * 255.0) & 0x000000FF)) } 
        {};
        constexpr Vec(uint8 r, uint8 g, uint8 b) : r{ r }, g{ g }, b{ b } {};
        constexpr Vec(const Vec<float, 3>& col) : Vec(col.x, col.y, col.z) {};
        constexpr Vec(uint32 col) : 
            r{ static_cast<uint8>((col & 0xFF000000) >> 24) },
            g{ static_cast<uint8>((col & 0x00FF0000) >> 16) },
            b{ static_cast<uint8>((col & 0x0000FF00) >> 8) } 
        {};

        constexpr uint8& operator[](size_t i) noexcept {
            switch (i) {
            case 1: return g;
            case 2: return b;
            default: return r;
            }
        }

        constexpr const uint8& operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return g;
            case 2: return b;
            default: return r;
            }
        }
    };

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator+(const Vec<T, N>& a, const Vec<T, N>& b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] + b[i];
        }

        return result;
    }

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator-(const Vec<T, N>& a, const Vec<T, N>& b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] - b[i];
        }

        return result;
    }

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator*(const Vec<T, N>& a, const Vec<T, N>& b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] * b[i];
        }

        return result;
    }

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator/(const Vec<T, N>& a, const Vec<T, N>& b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] / b[i];
        }

        return result;
    }

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator+(const Vec<T, N>& a, T b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] + b;
        }

        return result;
    }

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator-(const Vec<T, N>& a, T b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] - b;
        }

        return result;
    }

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator*(const Vec<T, N>& a, T b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] * b;
        }

        return result;
    }

    template<typename T, size_t N>
    constexpr inline Vec<T, N> operator/(const Vec<T, N>& a, T b) {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = a[i] / b;
        }

        return result;
    }

    using fvec2 = Vec<float, 2>;
    using fvec3 = Vec<float, 3>;
    using fvec4 = Vec<float, 4>;
    using dvec2 = Vec<double, 2>;
    using dvec3 = Vec<double, 3>;
    using dvec4 = Vec<double, 4>;
    using rgba = Vec<uint8, 4>;
    using rgb = Vec<uint8, 3>;
}