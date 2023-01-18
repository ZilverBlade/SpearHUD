#pragma once

#include <ghudcpp/types.h>

namespace GHUD {
	namespace Math {
        template<typename T, size_t N>
        static constexpr inline T Dot(const Vec<T, N>& a, const Vec<T, N>& b) {
            T result{};
            for (size_t i = 0; i < N; i++) {
                result += a[i] * b[i];
            }
            return result;
        }
        template<typename T, size_t N>
        static constexpr inline T Length(const Vec<T, N>& a) {
            T lensq{};
            for (size_t i = 0; i < N; i++) {
                lensq += a[i] * a[i];
            }
            return sqrt(lensq);
        }
        template<typename T, size_t N>
        static constexpr inline Vec<T, N> Normalize(const Vec<T, N>& a) {
            const T lensq = Length(a);
            return a / lensq;
        }
	};
}