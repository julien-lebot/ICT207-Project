/*
*	Matrix3.hpp
*
*	A 3D Matrix class.
*
*  Created by Joshua Wilding on 31/08/09.
*  Copyright 2009 Joshua Wilding. All rights reserved.
*
*  Modified by Julien Lebot 31/08/09:
*    - Fixed some little bugs to make it compile (Math::Units stuff)
*
*/

#ifndef __Matrix3_hpp__
#define __Matrix3_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Math.hpp>
#include <Phoenix/Matrix4.hpp>
#include <Phoenix/VectorCommon.hpp>
#include <Phoenix/Vector3.hpp>

#include <cassert>
#include <iostream>

namespace Phoenix {
	namespace Math {

		/**
		 * @brief A row major 3x3 matrix class.
		 * 
		 * Note: The coordinate system is assumed to be right-handed.
		 * 
		 * @param T The template type should be a T quantity such as float or double.
		 */
		template <typename T>
		class _PhoenixExport _MEM_ALIGN_VS(16) Matrix3
		{
		protected:
			union
			{
				T m[3][3];
				T _m[9];
			};

			template <typename T>
			friend class Matrix4;

		public:
			typedef T real_t;
			typedef Matrix3<T> Matrix3_t;

			_INLINE T* const operator&() const { return _m; }
			_INLINE T*		 operator&()       { return _m; }

			/** Default constructor.
			 *	Note: It doesn't initialize the matrix for efficiency.
			 */
			_INLINE Matrix3<T>() {};
			_INLINE explicit Matrix3<T>(const T arr[3][3])
			{
				memcpy(m,arr,9*sizeof(T));
			}

			_INLINE explicit Matrix3<T>(const T arr[9])
			{
				memcpy(m,arr,9*sizeof(T));
			}

			_INLINE Matrix3<T>(const Matrix3<T>& mat)
			{
				memcpy(m,mat.m,9*sizeof(T));
			}

			Matrix3<T>( T f00, T f01, T f02,
						T f10, T f11, T f12,
						T f20, T f21, T f22)
			{
#if 0//defined(USE_INTRINSIC_ASM)
				__m128* mat = static_cast<__m128*> (this);
				mat[0] = _mm_setr_ps(f00, f01, f02, 0.0);
				mat[1] = _mm_setr_ps(f10, f11, f22, 0.0);
				mat[2] = _mm_setr_ps(f20, f21, f22, 0.0);
				mat[3] = _mm_setr_ps(0.0, 0.0, 0.0, 1.0);
#else
				m[0][0] = f00;
				m[0][1] = f01;
				m[0][2] = f02;
				m[1][0] = f10;
				m[1][1] = f11;
				m[1][2] = f12;
				m[2][0] = f20;
				m[2][1] = f21;
				m[2][2] = f22;
#endif
			}

			// member access, allows use of construct mat[r][c]
			_INLINE T* operator[](size_t rc) const
			{
				return (T*) m[rc];
			}

			Vector3<T> getCol(size_t col) const
			{
				assert( 0 <= col && col < 3 );
				return Vector3<T>(m[0][col],
								  m[1][col],
								  m[2][col]);
			}

			void setCol(size_t col, const Vector3<T>& vec)
			{
				assert( 0 <= col && col < 3 );
				m[0][col] = vec.x;
				m[1][col] = vec.y;
				m[2][col] = vec.z;
			}

			void fromAxes(const Vector3<T>& xAxis,
						  const Vector3<T>& yAxis,
						  const Vector3<T>& zAxis)
			{
				setCol(0,xAxis);
				setCol(1,yAxis);
				setCol(2,zAxis);
			}

			// assignment and comparison
			_INLINE Matrix3<T>& operator=(const Matrix3<T>& mat)
			{
				memcpy(m,mat.m,9*sizeof(T));
				return *this;
			}

			bool operator==(const Matrix3<T>& mat) const
			{
				for(size_t row = 0; row < 3; row++)
				{
					for(size_t col = 0; col < 3; col++)
					{
						if( m[row][col] != mat.m[row][col] )
							return false;
					}
				}

				return true;
			}

			_INLINE bool operator!=(const Matrix3<T>& mat) const
			{
				return !operator==(mat);
			}

			// arithmetic operations
			Matrix3<T> operator+(const Matrix3<T>& mat) const
			{
				Matrix3<T>  sum;
				for(size_t row = 0; row < 3; row++)
				{
					for(size_t col = 0; col < 3; col++)
					{
						sum.m[row][col] = m[row][col] + mat.m[row][col];
					}
				}
				return sum;
			}

			Matrix3<T> operator-(const Matrix3<T>& mat) const
			{
				Matrix3<T> delta;
				for(size_t row = 0; row < 3; row++)
				{
					for(size_t col = 0; col < 3; col++)
					{
						delta.m[row][col] = m[row][col] - mat.m[row][col];
					}
				}
				return delta;
			}

			Matrix3<T> operator*(const Matrix3<T>& rkMatrix) const
			{
				Matrix3<T>  kProd;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
					{
						kProd.m[iRow][iCol] =
							m[iRow][0]*rkMatrix.m[0][iCol] +
							m[iRow][1]*rkMatrix.m[1][iCol] +
							m[iRow][2]*rkMatrix.m[2][iCol];
					}
				}
				return kProd;
			}

			// matrix * vector [3x3 * 3x1 = 3x1]
			Vector3<T> operator*(const Vector3<T>& rkVector) const
			{
				Vector3<T> kProd;
				for (size_t iRow = 0; iRow < 3; iRow++)
				{
					kProd[iRow] =
						m[iRow][0]*rkVector[0] +
						m[iRow][1]*rkVector[1] +
						m[iRow][2]*rkVector[2];
				}
				return kProd;
			}

			// vector * matrix [1x3 * 3x3 = 1x3]
			friend Vector3<T> operator*(const Vector3<T>& rkVector,
										const Matrix3<T>& rkMatrix)
			{
				Vector3<T>  kProd;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					kProd[iRow] =
						rkVector[0]*rkMatrix.m[0][iRow] +
						rkVector[1]*rkMatrix.m[1][iRow] +
						rkVector[2]*rkMatrix.m[2][iRow];
				}
				return kProd;
			}

			// matrix * scalar
			Matrix3<T> operator*(T fScalar) const
			{
				Matrix3<T>  kProd;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
						kProd[iRow][iCol] = fScalar*m[iRow][iCol];
				}
				return kProd;
			}

			// scalar * matrix
			friend Matrix3<T> operator*(T fScalar, const Matrix3<T>& rkMatrix)
			{
				Matrix3<T>  kProd;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
						kProd[iRow][iCol] = fScalar*rkMatrix.m[iRow][iCol];
				}
				return kProd;
			}

			Matrix3<T> operator-() const
			{
				Matrix3<T> neg;
				for(size_t row = 0; row < 3; row++)
				{
					for(size_t col = 0; col < 3; col++)
						neg[row][col] = -m[row][col];
				}
				return neg;
			}

			// utilities
			Matrix3<T> transpose() const
			{
				Matrix3<T>  kTranspose;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
						kTranspose[iRow][iCol] = m[iCol][iRow];
				}
				return kTranspose;
			}

			bool inverse(Matrix3<T>& mat, T tol = EPSILON) const
			{
				// Invert a 3x3 using cofactors.  This is about 8 times faster than
				// the Numerical Recipes code which uses Gaussian elimination.

				mat[0][0] = m[1][1]*m[2][2] -
							m[1][2]*m[2][1];
				mat[0][1] = m[0][2]*m[2][1] -
							m[0][1]*m[2][2];
				mat[0][2] = m[0][1]*m[1][2] -
							m[0][2]*m[1][1];
				mat[1][0] = m[1][2]*m[2][0] -
							m[1][0]*m[2][2];
				mat[1][1] = m[0][0]*m[2][2] -
							m[0][2]*m[2][0];
				mat[1][2] = m[0][2]*m[1][0] -
							m[0][0]*m[1][2];
				mat[2][0] = m[1][0]*m[2][1] -
							m[1][1]*m[2][0];
				mat[2][1] = m[0][1]*m[2][0] -
							m[0][0]*m[2][1];
				mat[2][2] = m[0][0]*m[1][1] -
							m[0][1]*m[1][0];

				T det =
					m[0][0]*mat[0][0] +
					m[0][1]*mat[1][0]+
					m[0][2]*mat[2][0];

				if(std::abs(det) <= tol)
					return false;

				T invDet = 1.0f/det;
				for(size_t row = 0; row < 3; row++)
				{
					for(size_t col = 0; col < 3; col++)
						mat[row][col] *= invDet;
				}

				return true;
			}

			Matrix3<T> inverse(T tol = EPSILON) const
			{
				Matrix3<T> result = Matrix3<T> ::ZERO;
				inverse(result, tol);
				return result;
			}

			T determinant() const
			{
				T fCofactor00 = m[1][1]*m[2][2] -
								m[1][2]*m[2][1];
				T fCofactor10 = m[1][2]*m[2][0] -
								m[1][0]*m[2][2];
				T fCofactor20 = m[1][0]*m[2][1] -
								m[1][1]*m[2][0];

				T fDet =
					m[0][0]*fCofactor00 +
					m[0][1]*fCofactor10 +
					m[0][2]*fCofactor20;

				return fDet;
			}

			/*
			// matrix must be orthonormal
			void toAxisAngle(Vector3<T>& rkAxis, Math::Units::Radians& rfRadians) const
			{
				// Let(x,y,z) be the unit-length axis and let A be an angle of rotation.
				// The rotation matrix is R = I + sin(A)*P +(1-cos(A))*P^2 where
				// I is the identity and
				//
				//       +-        -+
				//   P = |  0 -z +y |
				//       | +z  0 -x |
				//       | -y +x  0 |
				//       +-        -+
				//
				// If A > 0, R represents a counterclockwise rotation about the axis in
				// the sense of looking from the tip of the axis vector towards the
				// origin.  Some algebra will show that
				//
				//   cos(A) =(trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
				//
				// In the event that A = pi, R-R^t = 0 which prevents us from extracting
				// the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
				// P^2 =(R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
				// z^2-1.  We can solve these for axis(x,y,z).  Because the angle is pi,
				// it does not matter which sign you choose on the square roots.

				T fTrace = m[0][0] + m[1][1] + m[2][2];
				T fCos = 0.5*(fTrace-1.0);
				rfRadians = Math::arcosine(fCos);  // in [0,PI]

				if(rfRadians > Math::Units::Radians(0.0))
				{
					if(rfRadians < Math::Units::Radians(M_1_PI * Math::Units::radians))
					{
						rkAxis.x = m[2][1]-m[1][2];
						rkAxis.y = m[0][2]-m[2][0];
						rkAxis.z = m[1][0]-m[0][1];
						rkAxis.normalize();
					}
					else
					{
						// angle is PI
						float fHalfInverse;
						if( m[0][0] >= m[1][1] )
						{
							// r00 >= r11
							if( m[0][0] >= m[2][2] )
							{
								// r00 is maximum diagonal term
								rkAxis.x = 0.5*Math::sqroot(m[0][0] - m[1][1] - m[2][2] + 1.0);
								fHalfInverse = 0.5/rkAxis.x;
								rkAxis.y = fHalfInverse*m[0][1];
								rkAxis.z = fHalfInverse*m[0][2];
							}
							else
							{
								// r22 is maximum diagonal term
								rkAxis.z = 0.5*Math::sqroot(m[2][2] - m[0][0] - m[1][1] + 1.0);
								fHalfInverse = 0.5/rkAxis.z;
								rkAxis.x = fHalfInverse*m[0][2];
								rkAxis.y = fHalfInverse*m[1][2];
							}
						}
						else
						{
							// r11 > r00
							if( m[1][1] >= m[2][2] )
							{
								// r11 is maximum diagonal term
								rkAxis.y = 0.5*Math::sqroot(m[1][1] - m[0][0] - m[2][2] + 1.0);
								fHalfInverse  = 0.5/rkAxis.y;
								rkAxis.x = fHalfInverse*m[0][1];
								rkAxis.z = fHalfInverse*m[1][2];
							}
							else
							{
								// r22 is maximum diagonal term
								rkAxis.z = 0.5*Math::sqroot(m[2][2] - m[0][0] - m[1][1] + 1.0);
								fHalfInverse = 0.5/rkAxis.z;
								rkAxis.x = fHalfInverse*m[0][2];
								rkAxis.y = fHalfInverse*m[1][2];
							}
						}
					}
				}
				else
				{
					// The angle is 0 and the matrix is the identity.  Any axis will
					// work, so just use the x-axis.
					rkAxis.x = 1.0;
					rkAxis.y = 0.0;
					rkAxis.z = 0.0;
				}
			}

			_INLINE void toAxisAngle(Vector3<T>& rkAxis,
									 Math::Units::Degrees& rfAngle) const
			{
				Math::Units::Radians r;
				toAxisAngle(rkAxis, r);
				rfAngle = r;
			}

			void fromAxisAngle(const Vector3<T>&  rkAxis,
							   const Math::Units::Radians &rad)
			{
				T fCos = Math::cosine(rad);
				T fSin = Math::sine(rad);
				T fOneMinusCos = 1.0-fCos;
				T fX2 = rkAxis.x*rkAxis.x;
				T fY2 = rkAxis.y*rkAxis.y;
				T fZ2 = rkAxis.z*rkAxis.z;
				T fXYM = rkAxis.x*rkAxis.y*fOneMinusCos;
				T fXZM = rkAxis.x*rkAxis.z*fOneMinusCos;
				T fYZM = rkAxis.y*rkAxis.z*fOneMinusCos;
				T fXSin = rkAxis.x*fSin;
				T fYSin = rkAxis.y*fSin;
				T fZSin = rkAxis.z*fSin;

				m[0][0] = fX2*fOneMinusCos+fCos;
				m[0][1] = fXYM-fZSin;
				m[0][2] = fXZM+fYSin;
				m[1][0] = fXYM+fZSin;
				m[1][1] = fY2*fOneMinusCos+fCos;
				m[1][2] = fYZM-fXSin;
				m[2][0] = fXZM-fYSin;
				m[2][1] = fYZM+fXSin;
				m[2][2] = fZ2*fOneMinusCos+fCos;
			}

			// Determines if this matrix involves a scaling.
			_INLINE bool hasScale() const
			{
				// check magnitude of column vectors(==local axes)
				T t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
				if(!Math::TEqual(t, 1.0, 1e-04))
					return true;
				t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
				if(!Math::TEqual(t, 1.0, 1e-04))
					return true;
				t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
				if(!Math::TEqual(t, 1.0, 1e-04))
					return true;

				return false;
			}
			*/

			static const T EPSILON;
			static const Matrix3<T> ZERO;
			static const Matrix3<T> IDENTITY;

			inline friend std::ostream& operator <<(std::ostream& os, const Matrix3<T>& mat)
			{
				os		  << "Matrix3(" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << ")" <<
				std::endl << "       (" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << ")" <<
				std::endl << "       (" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << ")";
				return os;
			}
		};

		typedef Matrix3<float> Matrix3f;
		typedef Matrix3<double> Matrix3d;
	}
}

#endif // __Matrix3_hpp__