/*
*	Vector3.hpp
*
*	A 3D Vector class.
*
*  Created by Julien Lebot on 19/08/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Vector3_hpp__
#define __Vector3_hpp__

#include <EngineFoundation/Base.h>
#include <EngineFoundation/Math/Math.hpp>
#include <EngineFoundation/Math/VectorCommon.hpp>

#include <cassert>
#include <iostream>

namespace EngineFoundation {
	namespace Math {

		/**
		 * @brief Definition of a 3D vector.
		 * The vector is aligned on a 16 bytes memory boundary for performance,
		 * so technically it's similar to a 4D vector, but some operations can only
		 * be performed on 3D vectors and not 4D ones.
		 *
		 * Note: Maybe this should be aligned on a 32bytes memory boundary for better performance on x86 ? J.L.
		 *
		 * @param T The template type should be a real quantity such as float or double.
		 */
		template <typename T>
		class _MEM_ALIGN_VS(16) Vector3
		{
		protected:
			T x, y, z;
		public:
			// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
			T magnitude()
			{
				T *val = &x;
#if defined(USE_SSE)
				T result = 0.0f;

				__asm
				{
					mov		eax, dword ptr [val]

						movaps	xmm0, [eax]

					// x^2 y^2 z^2 w^2
					mulps	xmm0, xmm0

						// xmm1 = x^2, y^2, z^2, w^2
						movaps	xmm1, xmm0

						// xmm0 = z^2, w^2, x^2, y^2
						shufps	xmm0, xmm0, SHUFFLE(VEC_Z, VEC_W, VEC_X, VEC_Y)

						// xmm0 = (x^2 + z^2), (y^2 + w^2), (z^2 + x^2), (w^2 + y^2)
						addss	xmm0, xmm1

						// xmm1 = y^2, x^2, w^2, z^2
						shufps	xmm1, xmm1, SHUFFLE(VEC_Y, VEC_X, VEC_W, VEC_Z)

						// xmm0 = (x^2 + z^2) + y^2, (y^2 + w^2) + x^2, (z^2 + x^2) + w^2, (w^2 + y^2) + z^2
						addss	xmm0, xmm1

						// xmm0 = sqrt(x^2 + z^2 + y^2), ...
						sqrtss	xmm0, xmm0

						movss	dword ptr [result], xmm0
				}

				return result;
#elif defined(USE_INTRINSIC_ASM)
				__m128 m0, m1;
				__m128 *__mVec = (__m128*)val;

				// Squares the vector
				m0 = _mm_mul_ps(*__mVec, *__mVec);
				m1 = m0;

				m0 = _mm_shuffle_ps(m0, m0, SHUFFLE(VEC_Z, VEC_W, VEC_X, VEC_Y));
				m0 = _mm_add_ss(m0, m1);

				m1 = _mm_shuffle_ps(m1, m1, SHUFFLE(VEC_Y, VEC_X, VEC_W, VEC_Z));
				m0 = _mm_add_ss(m0, m1);

				// Computes the square root and convert back to a float
				return _mm_cvtss_f32(_mm_sqrt_ss(m0));
#else
				return sqroot(((x * x) + 
							   (y * y) + 
							   (z * z)) * Units::radians);
#endif
			}

			void normalise()
			{
				T invSqrtVec = rsqroot(((x * x) + 
										(y * y) + 
										(z * z)) * Units::radians);
				x *= invSqrtVec;
				y *= invSqrtVec;
				z *= invSqrtVec;
			}
			
			_INLINE Vector3()
			: x(0.0f),
			  y(0.0f),
			  z(0.0f)
			{
			}

			_INLINE Vector3(const T fX, const T fY, const T fZ)
			: x(fX),
			  y(fY),
			  z(fZ)
			{
			}

			_INLINE explicit Vector3(const T coords[3])
			: x(coords[0]),
			  y(coords[1]),
			  z(coords[2])
			{
			}

			_INLINE explicit Vector3(const int coords[3])
			{
				x = static_cast<T>(coords[0]);
				y = static_cast<T>(coords[1]);
				z = static_cast<T>(coords[2]);
			}

			_INLINE explicit Vector3(T* const r)
			: x(r[0]),
			  y(r[1]),
			  z(r[2])
			{
			}

			_INLINE explicit Vector3(const T scalar)
			: x(scalar),
			  y(scalar),
			  z(scalar)
			{
			}

			_INLINE Vector3(const Vector3<T>& rhs)
			: x(rhs.x),
			  y(rhs.y),
			  z(rhs.z)
			{
			}

			_INLINE T operator [](const size_t i) const
			{
				assert( i < 3 );

				return *(&x+i);
			}

			_INLINE T& operator [](const size_t i)
			{
				assert( i < 3 );
				return *(&x+i);
			}

			/// Pointer accessor for direct copying
			_INLINE T* ptr()
			{
				return &x;
			}

			/// Pointer accessor for direct copying
			_INLINE const T* ptr() const
			{
				return &x;
			}

			_INLINE Vector3<T>& operator =(const Vector3<T>& rhs)
			{
#if defined(USE_INTRINSIC_ASM)
				_mm_store_ps(&x, (*(__m128*)&(rhs.x)));
#else
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator =(const T scalar)
			{
				x = scalar;
				y = scalar;
				z = scalar;

				return *this;
			}

			_INLINE bool operator ==(const Vector3<T>& rhs) const
			{
				return (x == rhs.x && y == rhs.y && z == rhs.z);
			}

			_INLINE bool operator !=(const Vector3<T>& rhs) const
			{
				return (x != rhs.x || y != rhs.y || z != rhs.z);
			}

			// arithmetic operations
			_INLINE Vector3<T> operator +(const Vector3<T>& rhs) const
			{
				return Vector3<T>(
					x + rhs.x,
					y + rhs.y,
					z + rhs.z);
			}

			_INLINE Vector3<T> operator -(const Vector3<T>& rhs) const
			{
				return Vector3<T>(
					x - rhs.x,
					y - rhs.y,
					z - rhs.z);
			}

			_INLINE Vector3<T> operator *(const T fScalar) const
			{
				return Vector3<T>(
					x * fScalar,
					y * fScalar,
					z * fScalar);
			}

			_INLINE Vector3<T> operator *(const Vector3<T>& rhs) const
			{
				return Vector3<T>(
					x * rhs.x,
					y * rhs.y,
					z * rhs.z);
			}

			_INLINE Vector3<T> operator /(const T scalar) const
			{
				assert(scalar != 0.0);

				T fInv = 1.0f / scalar;

				return Vector3<T>(
					x * fInv,
					y * fInv,
					z * fInv);
			}

			_INLINE Vector3<T> operator /(const Vector3<T>& rhs) const
			{
				return Vector3<T>(
					x / rhs.x,
					y / rhs.y,
					z / rhs.z);
			}

			_INLINE const Vector3<T>& operator +() const
			{
				return *this;
			}

			_INLINE Vector3<T> operator -() const
			{
				return Vector3<T>(-x, -y, -z);
			}

			// overloaded binary and friendly operators to help Vector3
			_INLINE friend Vector3<T> operator *(const T scalar, const Vector3<T>& rhs)
			{
				return Vector3<T>(
					scalar * rhs.x,
					scalar * rhs.y,
					scalar * rhs.z);
			}

			_INLINE friend Vector3<T> operator /(const T scalar, const Vector3<T>& rhs)
			{
				return Vector3<T>(
					scalar / rhs.x,
					scalar / rhs.y,
					scalar / rhs.z);
			}

			_INLINE friend Vector3<T> operator +(const Vector3<T>& lhs, const T rhs)
			{
				return Vector3<T>(
					lhs.x + rhs,
					lhs.y + rhs,
					lhs.z + rhs);
			}

			_INLINE friend Vector3<T> operator +(const T lhs, const Vector3<T>& rhs)
			{
				return Vector3<T>(
					lhs + rhs.x,
					lhs + rhs.y,
					lhs + rhs.z);
			}

			_INLINE friend Vector3<T> operator -(const Vector3<T>& lhs, const T rhs)
			{
				return Vector3<T>(
					lhs.x - rhs,
					lhs.y - rhs,
					lhs.z - rhs);
			}

			_INLINE friend Vector3<T> operator -(const T lhs, const Vector3<T>& rhs)
			{
				return Vector3<T>(
					lhs - rhs.x,
					lhs - rhs.y,
					lhs - rhs.z);
			}

			// arithmetic updates
			_INLINE Vector3<T>& operator +=(const Vector3<T>& rhs)
			{
#if defined(USE_INTRINSIC_ASM)
				*((__m128*)&x) = _mm_add_ps(*((__m128*)&x), *((__m128*)(&rhs.x)));
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movaps	xmm0, dword ptr [_x]
					addps	xmm0, dword ptr [rhs]
					movaps	dword ptr [_x], xmm0
				}
#else
				x += rhs.x;
				y += rhs.y;
				z += rhs.z;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator +=(const T scalar)
			{
#if defined(USE_INTRINSIC_ASM)
				__m128 __mScalar = _mm_set_ps1(scalar);
				*((__m128*)&x) = _mm_add_ps(*((__m128*)&x), __mScalar);
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movss	xmm0, scalar
					shufps	xmm0, xmm0, SHUFFLE(VEC_X, VEC_X, VEC_X, VEC_X)
					movaps	xmm1, dword ptr [_x]
					addps	xmm0, xmm1
					movaps	dword ptr [_x], xmm0
				}
#else
				x += scalar;
				y += scalar;
				z += scalar;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator -=(const Vector3<T>& rhs)
			{
#if defined(USE_INTRINSIC_ASM)
				*((__m128*)&x) = _mm_sub_ps(*((__m128*)&x), *((__m128*)&(rhs.x)));
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movaps	xmm0, dword ptr [_x]
					subps	xmm0, dword ptr [rhs]
					movaps	dword ptr [_x], xmm0
				}
#else
				x -= rhs.x;
				y -= rhs.y;
				z -= rhs.z;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator -=(const T scalar)
			{
#if defined(USE_INTRINSIC_ASM)
				__m128 __mScalar = _mm_set_ps1(scalar);
				*((__m128*)&x) = _mm_sub_ps(*((__m128*)&x), __mScalar);
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movss	xmm0, scalar
					shufps	xmm0, xmm0, SHUFFLE(VEC_X, VEC_X, VEC_X, VEC_X)
					movaps	xmm1, dword ptr [_x]
					subps	xmm0, xmm1
					movaps	dword ptr [_x], xmm0
				}
#else
				x -= scalar;
				y -= scalar;
				z -= scalar;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator *=(const T scalar)
			{
#if defined(USE_INTRINSIC_ASM)
				__m128 __mScalar = _mm_set_ps1(scalar);
				*((__m128*)&x) = _mm_mul_ps(*((__m128*)&x), __mScalar);
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movss	xmm0, scalar
					shufps	xmm0, xmm0, SHUFFLE(VEC_X, VEC_X, VEC_X, VEC_X)
					movaps	xmm1, dword ptr [_x]
					mulps	xmm0, xmm1
					movaps	dword ptr [_x], xmm0
				}
#else
				x *= scalar;
				y *= scalar;
				z *= scalar;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator *=(const Vector3<T>& rhs)
			{
#if defined(USE_INTRINSIC_ASM)
				*((__m128*)&x) = _mm_mul_ps(*((__m128*)&x), *((__m128*)&(rhs.x)));
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movaps	xmm0, dword ptr [_x]
					mulps	xmm0, dword ptr [rhs]
					movaps	dword ptr [_x], xmm0
				}
#else
				x *= rhs.x;
				y *= rhs.y;
				z *= rhs.z;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator /=(const T scalar)
			{
				assert(scalar != 0.0);

#if defined(USE_INTRINSIC_ASM)
				__m128 __mScalar = _mm_set_ps1(scalar);
				*((__m128*)&x) = _mm_div_ps(*((__m128*)&x), __mScalar);
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movss	xmm0, scalar
					shufps	xmm0, xmm0, SHUFFLE(VEC_X, VEC_X, VEC_X, VEC_X)
					movaps	xmm1, dword ptr [_x]
					divps	xmm0, xmm1
					movaps	dword ptr [_x], xmm0
				}
#else
				T fInv = 1.0 / scalar;

				x *= fInv;
				y *= fInv;
				z *= fInv;
#endif
				return *this;
			}

			_INLINE Vector3<T>& operator /=(const Vector3<T>& rhs)
			{
#if defined(USE_INTRINSIC_ASM)
				*((__m128*)&x) = _mm_div_ps(*((__m128*)&x), *((__m128*)&(rhs.x)));
#elif defined(USE_SSE)
				T *_x = &x;
				__asm
				{
					movaps	xmm0, dword ptr [_x]
					divps	xmm0, dword ptr [rhs]
					movaps	dword ptr [_x], xmm0
				}
#else
				x /= rhs.x;
				y /= rhs.y;
				z /= rhs.z;
#endif
				return *this;
			}
			
			_INLINE Vector3<T> cross(const Vector3<T> &rhs) const
			{
				return Vector3<T>(
					y * rhs.z - z * rhs.y,
					z * rhs.x - x * rhs.z,
					x * rhs.y - y * rhs.x);
			}
			
			_INLINE T dot(const Vector3<T> &rhs)
			{
				return x * rhs.x + y * rhs.y + z * rhs.z;
			}

			/// Constants
			static const Vector3<T> ZERO;
			static const Vector3<T> X;
			static const Vector3<T> Y;
			static const Vector3<T> Z;
			static const Vector3<T> MINUS_X;
			static const Vector3<T> MINUS_Y;
			static const Vector3<T> MINUS_Z;
			static const Vector3<T> UNIT;

			inline friend std::ostream& operator <<(std::ostream& os, const Vector3<T>& vec)
			{
				os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
				return os;
			}
		};

		typedef Vector3<float> Vector3f;
		typedef Vector3<double> Vector3d;
	}
}

#endif // __Vector3_hpp__