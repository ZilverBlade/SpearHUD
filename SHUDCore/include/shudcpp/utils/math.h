#pragma once

#include <shudcpp/types.h>

namespace SHUD {
    struct Transform {
        fvec2 mTransformOffset = { 0.0f, 0.0f }; // transform offset px
        fvec2 mPosition = { 0.f, 0.f }; // position px
        fvec2 mScale = { 64.0f, 64.0f }; // size px
        float mRotation = 0.0f; // rotation in radians
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
            static_assert(std::numeric_limits<T>::is_iec559 && "Only floating point types can have normalized vectors!");
            const T lensq = Length(a);
            assert(lensq > std::numeric_limits<T>::epsilon() && "Length is 0! Cannot normalize vector!");
            return a / lensq;
        }
        template<typename T, size_t N>
        static constexpr inline Vec<T, N> Abs(const Vec<T, N>& a) {
            Vec<T, N> result;
            for (size_t i = 0; i < N; i++) {
                bool negative = a[i] < 0.0f;
                result[i] = -static_cast<float>(negative) * a[i] + static_cast<float>(!negative) * a[i];
            }
            return result;
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
            const inline fvec2& operator[] (size_t i) const noexcept {
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

        static constexpr inline fvec2 operator*(const Transform2x2& m, fvec2 v) noexcept {
            fvec2 result;
            fvec2 mulMr0 = { m[0][0], m[1][0] };
            fvec2 mulMr1 = { m[0][1], m[1][1] };

            result[0] = Dot(mulMr0, v);
            result[1] = Dot(mulMr1, v);
            return result;
        }
        static constexpr inline Transform2x2 operator*(const Transform2x2& a, const Transform2x2& b) noexcept {
            Transform2x2 result;
            fvec2 mulAr0 = { a[0][0], a[1][0] };
            fvec2 mulAr1 = { a[0][1], a[1][1] };
            fvec2 mulBc0 = { b[0][0], b[0][1] };
            fvec2 mulBc1 = { b[1][0], b[1][1] };

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