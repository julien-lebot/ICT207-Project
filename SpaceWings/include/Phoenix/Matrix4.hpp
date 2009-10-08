/*
*	Matrix4.hpp
*
*	A 4D Matrix class.
*
*  Created by Julien Lebot on 31/08/09.
*  Copyright 2009 Joshua Wilding. All rights reserved.
*
*  Modified by Julien Lebot 31/08/09
*
*/

#ifndef __Matrix4_hpp__
#define __Matrix4_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Math.hpp>
#include <Phoenix/Matrix3.hpp>
#include <iostream>

namespace Phoenix {
	namespace Math {
		/**
		* @brief A row major 4x4 matrix class.
		* 
		* Note: The coordinate system is assumed to be right-handed.
		* 
		* @param T The template type should be a T quantity such as float or double.
		*/
		template <typename T>
		class _PhoenixExport _MEM_ALIGN_VS(16) Matrix4
		{
		protected:
			union
			{
				T m[4][4];
				T _m[16];
			};

			template <typename T>
			friend class Matrix3;
		public:
			typedef T real_t;
			typedef Matrix4<real_t> Matrix4_t;

			friend std::ostream& operator<<(std::ostream& os, const Matrix4_t& mat)
			{
				os  << "[" << mat[0][0] << "," << mat[0][1] << "," << mat[0][2] << "," << mat[0][3] << "]" << std::endl
					<< "[" << mat[1][0] << "," << mat[1][1] << "," << mat[1][2] << "," << mat[1][3] << "]" << std::endl
					<< "[" << mat[2][0] << "," << mat[2][1] << "," << mat[2][2] << "," << mat[2][3] << "]" << std::endl
					<< "[" << mat[3][0] << "," << mat[3][1] << "," << mat[3][2] << "," << mat[3][3] << "]";
				return os;
			}

			/** Default constructor.
			 *	Note: It doesn't initialize the matrix for efficiency.
			 */
			_INLINE Matrix4<T>() {};
			_INLINE explicit Matrix4<T>(const T arr[4][4])
			{
				memcpy(m,arr,16*sizeof(T));
			}

			_INLINE explicit Matrix4<T>(const T arr[16])
			{
				memcpy(m,arr,16*sizeof(T));
			}

			_INLINE Matrix4<T>(const Matrix4_t& mat)
			{
				memcpy(m,mat.m,16*sizeof(T));
			}

			Matrix4<T>( T f00, T f01, T f02, T f03,
						T f10, T f11, T f12, T f13,
						T f20, T f21, T f22, T f23,
						T f30, T f31, T f32, T f33)
			{
#if 0//defined(USE_INTRINSIC_ASM)
				__m128* mat = (__m128*)(this);
				mat[0] = _mm_setr_ps(f00, f01, f02, f03);
				mat[1] = _mm_setr_ps(f10, f11, f22, f13);
				mat[2] = _mm_setr_ps(f20, f21, f22, f23);
				mat[3] = _mm_setr_ps(f30, f31, f32, f33);
#else
				m[0][0] = f00;
				m[0][1] = f01;
				m[0][2] = f02;
				m[0][3] = f03;
				m[1][0] = f10;
				m[1][1] = f11;
				m[1][2] = f12;
				m[1][3] = f13;
				m[2][0] = f20;
				m[2][1] = f21;
				m[2][2] = f22;
				m[2][3] = f23;
				m[3][0] = f30;
				m[3][1] = f31;
				m[3][2] = f32;
				m[3][3] = f33;
#endif
			}

			// member access, allows use of construct mat[r][c]
			_INLINE const T* const operator[](size_t rc) const
			{
				return (T*) m[rc];
			}

			_INLINE T* operator[](size_t rc)
			{
				return (T*) m[rc];
			}

			_INLINE T* const operator&() const { return _m; }
			_INLINE T*		 operator&()       { return _m; }

			_INLINE Matrix4_t operator+(const Matrix4_t &rhs) const
			{
				Matrix4_t r;

				r.m[0][0] = m[0][0] + rhs.m[0][0];
				r.m[0][1] = m[0][1] + rhs.m[0][1];
				r.m[0][2] = m[0][2] + rhs.m[0][2];
				r.m[0][3] = m[0][3] + rhs.m[0][3];

				r.m[1][0] = m[1][0] + rhs.m[1][0];
				r.m[1][1] = m[1][1] + rhs.m[1][1];
				r.m[1][2] = m[1][2] + rhs.m[1][2];
				r.m[1][3] = m[1][3] + rhs.m[1][3];

				r.m[2][0] = m[2][0] + rhs.m[2][0];
				r.m[2][1] = m[2][1] + rhs.m[2][1];
				r.m[2][2] = m[2][2] + rhs.m[2][2];
				r.m[2][3] = m[2][3] + rhs.m[2][3];

				r.m[3][0] = m[3][0] + rhs.m[3][0];
				r.m[3][1] = m[3][1] + rhs.m[3][1];
				r.m[3][2] = m[3][2] + rhs.m[3][2];
				r.m[3][3] = m[3][3] + rhs.m[3][3];

				return r;
			}

			_INLINE Matrix4_t operator-(const Matrix4_t &rhs) const
			{
				Matrix4_t r;
				r.m[0][0] = m[0][0] - rhs.m[0][0];
				r.m[0][1] = m[0][1] - rhs.m[0][1];
				r.m[0][2] = m[0][2] - rhs.m[0][2];
				r.m[0][3] = m[0][3] - rhs.m[0][3];

				r.m[1][0] = m[1][0] - rhs.m[1][0];
				r.m[1][1] = m[1][1] - rhs.m[1][1];
				r.m[1][2] = m[1][2] - rhs.m[1][2];
				r.m[1][3] = m[1][3] - rhs.m[1][3];

				r.m[2][0] = m[2][0] - rhs.m[2][0];
				r.m[2][1] = m[2][1] - rhs.m[2][1];
				r.m[2][2] = m[2][2] - rhs.m[2][2];
				r.m[2][3] = m[2][3] - rhs.m[2][3];

				r.m[3][0] = m[3][0] - rhs.m[3][0];
				r.m[3][1] = m[3][1] - rhs.m[3][1];
				r.m[3][2] = m[3][2] - rhs.m[3][2];
				r.m[3][3] = m[3][3] - rhs.m[3][3];

				return r;
			}

			/** Assignment from 3x3 matrix.
			*/
			_INLINE void operator=(const Matrix3<T>& mat3)
			{
				m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
				m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
				m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
			}

			Matrix4<T> getTranspose() const
			{
				Matrix4<T> kTranspose;
				for (size_t iRow = 0; iRow < 4; iRow++)
				{
					for (size_t iCol = 0; iCol < 4; iCol++)
						kTranspose[iRow][iCol] = m[iCol][iRow];
				}
				return kTranspose;
			}

			Matrix4<T> inverse() const
			{
				T m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
				T m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
				T m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
				T m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

				T v0 = m20 * m31 - m21 * m30;
				T v1 = m20 * m32 - m22 * m30;
				T v2 = m20 * m33 - m23 * m30;
				T v3 = m21 * m32 - m22 * m31;
				T v4 = m21 * m33 - m23 * m31;
				T v5 = m22 * m33 - m23 * m32;

				T t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
				T t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
				T t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
				T t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

				T invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

				T d00 = t00 * invDet;
				T d10 = t10 * invDet;
				T d20 = t20 * invDet;
				T d30 = t30 * invDet;

				T d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
				T d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
				T d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
				T d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

				v0 = m10 * m31 - m11 * m30;
				v1 = m10 * m32 - m12 * m30;
				v2 = m10 * m33 - m13 * m30;
				v3 = m11 * m32 - m12 * m31;
				v4 = m11 * m33 - m13 * m31;
				v5 = m12 * m33 - m13 * m32;

				T d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
				T d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
				T d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
				T d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

				v0 = m21 * m10 - m20 * m11;
				v1 = m22 * m10 - m20 * m12;
				v2 = m23 * m10 - m20 * m13;
				v3 = m22 * m11 - m21 * m12;
				v4 = m23 * m11 - m21 * m13;
				v5 = m23 * m12 - m22 * m13;

				T d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
				T d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
				T d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
				T d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

				return Matrix4<T>(
					d00, d01, d02, d03,
					d10, d11, d12, d13,
					d20, d21, d22, d23,
					d30, d31, d32, d33);
			}

			static const T EPSILON;
			static const Matrix4<T> ZERO;
			static const Matrix4<T> IDENTITY;
		};
		typedef Matrix4<float> Matrix4f;
		typedef Matrix4<double> Matrix4d;
	}
}
#endif