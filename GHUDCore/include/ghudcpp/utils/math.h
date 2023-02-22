#pragma once

#include <ghudcpp/types.h>

namespace GHUD {
    struct Transform {
        fvec2 mTransformOffset = { 0.0f, 0.0f };
        fvec2 mPosition = { 0.5f, 0.5f };
        fvec2 mScale = { 1.0f, 1.0f };
    };
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

        class Transform2x2 { // Collumn major order 2x2 transform matrix
        public:
            Transform2x2() = default;
            Transform2x2(fvec2 scale, float rotation) {
                const float s = sin(rotation);
                const float c = cos(rotation);

                mColumn0[0] = scale.x * c;
                mColumn0[1] = scale.x * s;
                mColumn1[0] = scale.y * (-s);
                mColumn1[1] = scale.y * c;
            }
            Transform2x2(fvec2 column0, fvec2 column1) : mColumn0(column0), mColumn1(column1) 
            {
            }
            inline const fvec2& operator[] (size_t i) const noexcept {
                assert(i < 2 && "Array index out of vector range");
                return reinterpret_cast<const fvec2*>(this)[i];
            }
            inline fvec2& operator[] (size_t i) noexcept {
                assert(i < 2 && "Array index out of vector range");
                return reinterpret_cast<fvec2*>(this)[i];
            }
        private:
            fvec2 mColumn0{ 1.0, 0.0 };
            fvec2 mColumn1{ 0.0, 1.0 };
        };


        static constexpr inline Transform2x2 operator*(const Transform2x2& a, const Transform2x2& b) {
            Transform2x2 result{};
            fvec2 mulAr0 = { a[0][0], a[1][0] };
            fvec2 mulAr1 = { a[0][1], a[1][1] };
            fvec2 mulBc0 = { a[0][0], a[0][1] };
            fvec2 mulBc1 = { a[1][0], a[1][1] };

            result[0][0] = Dot(mulAr0, mulBc0);
            result[0][1] = Dot(mulAr1, mulBc0);
            result[1][0] = Dot(mulAr0, mulBc1);
            result[1][1] = Dot(mulAr1, mulBc1);
            return result;
        }

        static inline Transform2x2 Inverse(const Transform2x2& matrix) noexcept {
            float a = matrix[0][0];
            float b = matrix[1][0];
            float c = matrix[0][1];
            float d = matrix[1][1];
            float determinant = a * d - b * c;
            float invDeterminant = 1.0 / determinant;
            return {
                fvec2(d, -c) * invDeterminant,
                fvec2(-b, a) * invDeterminant
            };
        }
	};
}