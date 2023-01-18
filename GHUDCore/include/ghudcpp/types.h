#pragma once
#include <array>
#include <cassert>

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
                                                                  
        constexpr inline Vec<T, N> operator+=(const Vec<T, N>& a) noexcept {
            return *this = *this + a;                             
        }                                                         
                                                                  
        constexpr inline Vec<T, N> operator-=(const Vec<T, N>& a) noexcept {
            return *this = *this - a;                             
        }                                                         
                                                                  
        constexpr inline Vec<T, N> operator*=(const Vec<T, N>& a) noexcept {
            return *this = *this * a;                             
        }                                                         
                                                                  
        constexpr inline Vec<T, N> operator/=(const Vec<T, N>& a) noexcept {
            return *this = *this / a;
        }

        constexpr inline Vec<T, N> operator+=(T a) noexcept {
            return *this = *this + a;
        }

        constexpr inline Vec<T, N> operator-=(T a) noexcept {
            return *this = *this - a;         
        }                                     
                                              
        constexpr inline Vec<T, N> operator*=(T a) noexcept {
            return *this = *this * a;         
        }                                     
                                              
        constexpr inline Vec<T, N> operator/=(T a) noexcept {
            return *this = *this / a;
        }
    };

    template<typename T>
    struct Vec<T, 2> {
        T x{}, y{};
        Vec() = default;

        constexpr Vec<T, 2>(float vx, float vy) noexcept :
            x(vx), y(vy)
        {}

        constexpr T& operator[](size_t i) noexcept {
            assert(i <= 2 && "Array index out of vector range");
            return *reinterpret_cast<T*>(this + i * sizeof(T));
        }
        constexpr const T& operator[](size_t i) const noexcept {
            assert(i <= 2 && "Array index out of vector range");
            return *reinterpret_cast<const T*>(this + i * sizeof(T));
        }
    };

    template<typename T>
    struct Vec<T, 3> {
        T x{}, y{}, z{};
        Vec() = default;

        constexpr Vec<T, 3>(float vx, float vy, float vz) noexcept :
            x(vx), y(vy), z(vz)
        {}

        constexpr T& operator[](size_t i) noexcept {
            assert(i <= 3 && "Array index out of vector range");
            return *reinterpret_cast<T*>(this + i * sizeof(T));
        }
        constexpr const T& operator[](size_t i) const noexcept {
            assert(i <= 3 && "Array index out of vector range");
            return *reinterpret_cast<const T*>(this + i * sizeof(T));
        }
    };

    template<typename T>
    struct Vec<T, 4> {
        T x{}, y{}, z{}, w{};
        Vec() = default;

        constexpr Vec<T, 4>(float vx, float vy, float vz, float vw) noexcept :
            x(vx), y(vy), z(vz), w(vw)
        {}

        constexpr T& operator[](size_t i) noexcept {
            assert(i <= 4 && "Array index out of vector range");
            return *reinterpret_cast<T*>(this + i * sizeof(T));
        }
        constexpr const T& operator[](size_t i) const noexcept {
            assert(i <= 4 && "Array index out of vector range");
            return *reinterpret_cast<const T*>(this + i * sizeof(T));
        }
    };

    template<>
    struct Vec<uint8, 4> {
        uint8 r{}, g{}, b{}, a{};
        Vec() = default;

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

        inline uint8& operator[](size_t i) noexcept {
            assert(i <= 4 && "Array index out of vector range");
            return *reinterpret_cast<uint8*>(this + i);
        }
        inline const uint8& operator[](size_t i) const noexcept {
            assert(i <= 4 && "Array index out of vector range");
            return *reinterpret_cast<const uint8*>(this + i);
        }
    };

    template<>
    struct Vec<uint8, 3> {
        uint8 r{}, g{}, b{};
        Vec() = default;
        constexpr Vec(float r, float g, float b) :
            r{ static_cast<uint8>((static_cast<uint32>(r * 255.0) & 0x000000FF)) },
            g{ static_cast<uint8>((static_cast<uint32>(g * 255.0) & 0x000000FF)) },
            b{ static_cast<uint8>((static_cast<uint32>(b * 255.0) & 0x000000FF)) } 
        {};
        constexpr Vec(const Vec<float, 3>& col) : Vec(col.x, col.y, col.z) {};
        constexpr Vec(uint8 r, uint8 g, uint8 b) : r{ r }, g{ g }, b{ b } {};
        constexpr Vec(uint32 col) : 
            r{ static_cast<uint8>((col & 0xFF000000) >> 24) },
            g{ static_cast<uint8>((col & 0x00FF0000) >> 16) },
            b{ static_cast<uint8>((col & 0x0000FF00) >> 8) } 
        {};

        inline uint8& operator[](size_t i) noexcept {
            assert(i <= 3 && "Array index out of vector range");
            return *reinterpret_cast<uint8*>(this + i);
        }
        inline const uint8& operator[](size_t i) const noexcept {
            assert(i <= 3 && "Array index out of vector range");
            return *reinterpret_cast<const uint8*>(this + i);
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
    using RGBAColor = Vec<uint8, 4>;
    using RGBColor = Vec<uint8, 3>;
}