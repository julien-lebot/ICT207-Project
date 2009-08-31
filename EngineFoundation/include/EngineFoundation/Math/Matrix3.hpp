/*
*	Matrix3.hpp
*
*	A 3D Matrix class.
*
*  Created by Joshua Wilding on 31/08/09.
*  Copyright 2009 Joshua Wilding. All rights reserved.
*
*/

#ifndef __Matrix3_hpp__
#define __Matrix3_hpp__

#include <EngineFoundation/Base.h>
#include <EngineFoundation/Math/Math.hpp>
#include <EngineFoundation/Math/VectorCommon.hpp>
#include <EngineFoundation/Math/Vector3.hpp>

#include <cassert>
#include <iostream>

namespace EngineFoundation {
	namespace Math {

		/**
		 * @brief A 3x3 matrix which can represent rotations around axes.
		 * 
		 * Note: The coordinate system is assumed to be right-handed.
		 * 
		 * @param T The template type should be a T quantity such as float or double.
		 */
		template <typename T>
		class _MEM_ALIGN_VS(16) Matrix3
		{
		public:
			/** Default constructor.
			 *	Note: It doesn't initialize the matrix for efficiency.
			 */
			_INLINE Matrix3<T>() {};
			_INLINE explicit Matrix3<T>(const T arr[3][3])
			{
				memcpy(m,arr,9*sizeof(T));
			}
			_INLINE Matrix3<T>(const Matrix3<T>& rkMatrix)
			{
				memcpy(m,rkMatrix.m,9*sizeof(T));
			}
			Matrix3<T>(T fEntry00, T fEntry01, T fEntry02,
						T fEntry10, T fEntry11, T fEntry12,
						T fEntry20, T fEntry21, T fEntry22)
			{
				m[0][0] = fEntry00;
				m[0][1] = fEntry01;
				m[0][2] = fEntry02;
				m[1][0] = fEntry10;
				m[1][1] = fEntry11;
				m[1][2] = fEntry12;
				m[2][0] = fEntry20;
				m[2][1] = fEntry21;
				m[2][2] = fEntry22;
			}

			// member access, allows use of construct mat[r][c]
			_INLINE T* operator[](size_t iRow) const
			{
				return(T*)m[iRow];
			}
			/*_INLINE operator T*()
			{
				return(T*)m[0];
			}*/
			Vector3<T> getCol(size_t iCol) const
			{
				assert( 0 <= iCol && iCol < 3 );
				return Vector3<T>(m[0][iCol],m[1][iCol],m[2][iCol]);
			}
			void setCol(size_t iCol, const Vector3<T>& vec)
			{
				assert( 0 <= iCol && iCol < 3 );
				m[0][iCol] = vec.x;
				m[1][iCol] = vec.y;
				m[2][iCol] = vec.z;
			}

			void fromAxes(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis)
			{
				setColumn(0,xAxis);
				setColumn(1,yAxis);
				setColumn(2,zAxis);
			}

			// assignment and comparison
			_INLINE Matrix3<T>& operator=(const Matrix3<T>& rkMatrix)
			{
				memcpy(m,rkMatrix.m,9*sizeof(T));
				return *this;
			}

			bool operator==(const Matrix3<T>& rkMatrix) const
			{
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
					{
						if( m[iRow][iCol] != rkMatrix.m[iRow][iCol] )
							return false;
					}
				}

				return true;
			}

			_INLINE bool operator!=(const Matrix3<T>& rkMatrix) const
			{
				return !operator==(rkMatrix);
			}

			// arithmetic operations
			Matrix3<T> operator+(const Matrix3<T>& rkMatrix) const
			{
				Matrix3<T>  kSum;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
					{
						kSum.m[iRow][iCol] = m[iRow][iCol] +
							rkMatrix.m[iRow][iCol];
					}
				}
				return kSum;
			}

			Matrix3<T> operator-(const Matrix3<T>& rkMatrix) const
			{
				Matrix3<T>  kDiff;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
					{
						kDiff.m[iRow][iCol] = m[iRow][iCol] -
							rkMatrix.m[iRow][iCol];
					}
				}
				return kDiff;
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
				return Matrix3<T>::IDENTITY;
			}

			// vector * matrix [1x3 * 3x3 = 1x3]
			friend Vector3<T> operator*(const Vector3<T>& rkVector,
										const Matrix3<T>& rkMatrix)
			{
				Vector3<T>  kProd;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					kProd[iRow] =
						rkPoint[0]*rkMatrix.m[0][iRow] +
						rkPoint[1]*rkMatrix.m[1][iRow] +
						rkPoint[2]*rkMatrix.m[2][iRow];
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
				Matrix3<T>  kNeg;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
						kNeg[iRow][iCol] = -m[iRow][iCol];
				}
				return kNeg;
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

			bool inverse(Matrix3<T>& rkInverse, T fTolerance = 1e-06) const
			{
				// Invert a 3x3 using cofactors.  This is about 8 times faster than
				// the Numerical Recipes code which uses Gaussian elimination.

				rkInverse[0][0] = m[1][1]*m[2][2] -
								  m[1][2]*m[2][1];
				rkInverse[0][1] = m[0][2]*m[2][1] -
								  m[0][1]*m[2][2];
				rkInverse[0][2] = m[0][1]*m[1][2] -
								  m[0][2]*m[1][1];
				rkInverse[1][0] = m[1][2]*m[2][0] -
								  m[1][0]*m[2][2];
				rkInverse[1][1] = m[0][0]*m[2][2] -
								  m[0][2]*m[2][0];
				rkInverse[1][2] = m[0][2]*m[1][0] -
								  m[0][0]*m[1][2];
				rkInverse[2][0] = m[1][0]*m[2][1] -
								  m[1][1]*m[2][0];
				rkInverse[2][1] = m[0][1]*m[2][0] -
								  m[0][0]*m[2][1];
				rkInverse[2][2] = m[0][0]*m[1][1] -
								  m[0][1]*m[1][0];

				T fDet =
					m[0][0]*rkInverse[0][0] +
					m[0][1]*rkInverse[1][0]+
					m[0][2]*rkInverse[2][0];

				if(Math::Abs(fDet) <= fTolerance)
					return false;

				T fInvDet = 1.0/fDet;
				for(size_t iRow = 0; iRow < 3; iRow++)
				{
					for(size_t iCol = 0; iCol < 3; iCol++)
						rkInverse[iRow][iCol] *= fInvDet;
				}

				return true;
			}

			Matrix3<T> inverse(T fTolerance = 1e-06) const
			{
				Matrix3<T>  kInverse = Matrix3<T> ::ZERO;
				Inverse(kInverse,fTolerance);
				return kInverse;
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

			// matrix must be orthonormal
			void toAxisAngle(Vector3<T>& rkAxis, Radian& rfAngle) const
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

				if( rfRadians > Radian(0.0) )
				{
					if( rfRadians < Radian(Math::PI) )
					{
						rkAxis.x = m[2][1]-m[1][2];
						rkAxis.y = m[0][2]-m[2][0];
						rkAxis.z = m[1][0]-m[0][1];
						rkAxis.normalise();
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
								rkAxis.x = 0.5*Math::Sqrt(m[0][0] -
									m[1][1] - m[2][2] + 1.0);
								fHalfInverse = 0.5/rkAxis.x;
								rkAxis.y = fHalfInverse*m[0][1];
								rkAxis.z = fHalfInverse*m[0][2];
							}
							else
							{
								// r22 is maximum diagonal term
								rkAxis.z = 0.5*Math::Sqrt(m[2][2] -
									m[0][0] - m[1][1] + 1.0);
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
								rkAxis.y = 0.5*Math::Sqrt(m[1][1] -
									m[0][0] - m[2][2] + 1.0);
								fHalfInverse  = 0.5/rkAxis.y;
								rkAxis.x = fHalfInverse*m[0][1];
								rkAxis.z = fHalfInverse*m[1][2];
							}
							else
							{
								// r22 is maximum diagonal term
								rkAxis.z = 0.5*Math::Sqrt(m[2][2] -
									m[0][0] - m[1][1] + 1.0);
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
									 Math::Units::Degree& rfAngle) const
			{
				Math::Units::Radians r;
				toAxisAngle( rkAxis, r );
				rfAngle = r;
			}

			void fromAxisAngle(const Vector3<T>&  rkAxis,
							   const Math::Units::Radians& fRadians)
			{
				T fCos = Math::cosine(fRadians);
				T fSin = Math::sine(fRadians);
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

			/** Determines if this matrix involves a scaling. */
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

			static const T EPSILON;
			static const Matrix3<T> ZERO;
			static const Matrix3<T> IDENTITY;

			inline friend std::ostream& operator <<(std::ostream& os, const Matrix3<T>& mat)
			{
				os << "Matrix3(" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << ")";
				os << "       (" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << ")";
				os << "       (" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << ")";
				return os;
			}

		protected:
			T m[3][3];

			// for faster access
			//friend class Matrix4;
		};

		typedef Matrix3<float> Matrix3f;
		typedef Matrix3<double> Matrix3d;
	}
}

#endif // __Matrix3_hpp__