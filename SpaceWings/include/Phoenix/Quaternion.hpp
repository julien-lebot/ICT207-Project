/*
Class Name:

CQuaternion.

Created by:

Allen Sherrod (Programming Ace of www.UltimateGameProgramming.com).

Modified by:
Julien Lebot and Joshua Wilding to work with the Phoenix engine on 25/09/2009

Description:

This class is used to create a quaternion to be used with rotations.
*/


#ifndef QUATERNION_H
#define QUATERNION_H

#define GET_RADIANS(degree) (float)((degree * M_PI) / 180.0f)   // Will convert degrees to radians.

#include <cmath>                                            // Math header file.
#include <Phoenix/Base.h>
#include <Phoenix/Math.hpp>
#include <Phoenix/Matrix4.hpp>
#include <Phoenix/Matrix3.hpp>

namespace Phoenix
{
	namespace Math
	{
		template <class qType>
		class Quaternion 
		{
		public:
			Quaternion()
			{
				// Initialize each member variables.
				x = y = z = 0.0f;
				w = 1.0f;
			}

			Quaternion(qType xAxis, qType yAxis, qType zAxis, qType wAxis) 
			{
				// Initialize each member variables.
				x = xAxis; y = yAxis; z = zAxis;
				w = wAxis;
			}

			Quaternion<qType> &operator=(const Matrix4<qType> &matrix)
			{

				w = 0.5f * sqrtf(matrix[0][0] + matrix[1][1] + matrix[2][2] + matrix[3][3]);
				w  = w  < 0.0001f ? 0.0004f : 4.0f*w;

				x  = (matrix[1][2] - matrix[2][1])/w;
				y  = (matrix[2][0] - matrix[0][2])/w;
				z  = (matrix[0][1] - matrix[1][0])/w;
				w /= 4.0f;

				return *this;
			}

			Quaternion<qType> &operator=(const Quaternion<qType> &q)
			{
				// This will make this quaternion equal to q.
				w = q.w; x = q.x; y = q.y; z = q.z;
				return *this;
			}

			Quaternion<qType> operator*(const Quaternion  &q)
			{
				// To multiply a quaternion you must first do the dot and cross product
				// of the 2 quaternions then add/subract them to a result.
				Quaternion<qType>  result;

				result.x = w * q.x + x * q.w + y * q.z - z * q.y;
				result.y = w * q.y - x * q.z + y * q.w + z * q.x;
				result.z = w * q.z + x * q.y - y * q.x + z * q.w;
				result.w = w * q.w - x * q.x - y * q.y - z * q.z;

				return result;
			}

			// rotation of a vector by a quaternion
			Vector3<qType> operator*(const Vector3<qType>& rkVector) const
			{
				// nVidia SDK implementation
				Vector3<qType> uv, uuv;
				Vector3<qType> qvec(x, y, z);
				uv = qvec.cross(rkVector);
				uuv = qvec.cross(uv);
				uv *= (2.0f * w);
				uuv *= 2.0f;

				return rkVector + uv + uuv;
			}

			Quaternion<qType> operator*(qType fScalar) const
			{
				return Quaternion(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
			}

			void slerp(Quaternion &q1, Quaternion &q2, float t)
			{
				float o, co, so, scale0, scale1;
				float qi[4];


				// Do a linear interpolation between two quaternions (0 <= t <= 1).
				co = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;  // dot product

				if (co < 0)
				{
					co = -co;
					qi[0] = -q2.x;
					qi[1] = -q2.y;
					qi[2] = -q2.z;
					qi[3] = -q2.w;
				}
				else
				{
					qi[0] = q2.x;
					qi[1] = q2.y;
					qi[2] = q2.z;
					qi[3] = q2.w;
				}

				// If the quaternions are really close, do a simple linear interpolation.
				if ((1 - co) <= 0.0001f)
				{
					scale0 = 1 - t;
					scale1 = t;
				}
				else
				{
					// Otherwise SLERP.
					o      = (float) acos(co);
					so     = sinf(o);
					scale0 = sinf((1 - t) * o) / so;
					scale1 = sinf(t * o) / so;
				}

				// Calculate interpolated quaternion:

				x = scale0 * q1.x + scale1 * qi[0];
				y = scale0 * q1.y + scale1 * qi[1];
				z = scale0 * q1.z + scale1 * qi[2];
				w = scale0 * q1.w + scale1 * qi[3];
			} 

			Quaternion<qType> conjugate()
			{
				// The Conjugate is basically all axis negated but the w.
				return Quaternion<qType> (-x, -y, -z, w);
			}

			void rotatef(float angle, float xAxis, float yAxis, float zAxis)
			{
				// Pretty much what is going on here is that if there is any axis that is not
				// a 0 or a 1 (meaning its not normalized) then we want to normalize it.
				// I created a if statement because I thought this would be better than normalizing
				// it every time this function is called, which would result in a lot of expensive
				// sqrt() call.  This is just in case the user forgot to use only normalized values.
				if((xAxis != 0 && xAxis != 1) ||
					(yAxis != 0 && yAxis != 1) ||
					(zAxis != 0 && zAxis != 1))
				{
					float length = (float)sqrt(xAxis * xAxis + yAxis * yAxis + zAxis * zAxis);
					xAxis /= length; yAxis /= length; zAxis /= length;
				}

				// Convert the angle degrees into radians.
				//float angle = GET_RADIANS(amount);

				// Call this once for optimization, just like using the if statement to determine if
				// we should normalize.
				float sine = (float)sin(angle / 2.0f);

				// Create the quaternion.
				x = xAxis * sine;
				y = yAxis * sine;
				z = zAxis * sine;
				w = (float)cos(angle / 2.0f);

				// Normalize the quaternion.
				float length = 1 / (float)sqrt(x * x + y * y + z * z + w * w);
				x *= length;
				y *= length;
				z *= length;
			}

			Matrix4<qType> toMatrix_4x4() const
			{
				Matrix4<qType> pMatrix;

				// Calculate the first row.
				pMatrix[0][0]  = 1.0f - 2.0f * (y * y + z * z); 
				pMatrix[0][1]  = 2.0f * (x * y + z * w);
				pMatrix[0][2]  = 2.0f * (x * z - y * w);
				pMatrix[0][3]  = 0.0f;  

				// Calculate the second row.
				pMatrix[1][0]  = 2.0f * (x * y - z * w);  
				pMatrix[1][1]  = 1.0f - 2.0f * (x * x + z * z); 
				pMatrix[1][2]  = 2.0f * (z * y + x * w);  
				pMatrix[1][3]  = 0.0f;  

				// Calculate the third row.
				pMatrix[2][0]  = 2.0f * (x * z + y * w);
				pMatrix[2][1]  = 2.0f * (y * z - x * w);
				pMatrix[2][2] = 1.0f - 2.0f * (x * x + y * y);  
				pMatrix[2][3] = 0.0f;  

				// Calculate the fourth row.
				pMatrix[3][0] = 0;  
				pMatrix[3][1] = 0;  
				pMatrix[3][2] = 0;  
				pMatrix[3][3] = 1.0f;
				return pMatrix;
			}

			Matrix3<qType> toMatrix_3x3() const
			{
				Matrix3<qType> pMatrix;

				// Calculate the first row.
				pMatrix[0][0]  = 1.0f - 2.0f * (y * y + z * z); 
				pMatrix[0][1]  = 2.0f * (x * y + z * w);
				pMatrix[0][2]  = 2.0f * (x * z - y * w);

				// Calculate the second row.
				pMatrix[1][0]  = 2.0f * (x * y - z * w);  
				pMatrix[1][1]  = 1.0f - 2.0f * (x * x + z * z); 
				pMatrix[1][2]  = 2.0f * (z * y + x * w);  

				// Calculate the third row.
				pMatrix[2][0]  = 2.0f * (x * z + y * w);
				pMatrix[2][1]  = 2.0f * (y * z - x * w);
				pMatrix[2][2] = 1.0f - 2.0f * (x * x + y * y);

				return pMatrix;
			}

			void fromAngleAxis(const Math::Units::Radians& rfAngle,
							   const Math::Vector3<qType>& rkAxis)
			{
				// assert:  axis[] is unit length
				//
				// The quaternion representing the rotation is
				//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

				Math::Units::Radians fHalfAngle(0.5*rfAngle);
				qType fSin = Math::sine(fHalfAngle);
				w = Math::cosine(fHalfAngle);
				x = fSin*rkAxis.x;
				y = fSin*rkAxis.y;
				z = fSin*rkAxis.z;
			}

			qType norm() const
			{
				return w*w+x*x+y*y+z*z;
			}

			qType normalize()
			{
				qType len = norm();
				qType factor = (1.0f / Math::sqroot(len));
				*this = *this * factor;
				return len;
			}

			qType x, y, z, w;                      // x, y , z, and w axis.
		};
		typedef Quaternion<float> Quaternionf;
		typedef Quaternion<double> Quaterniond;
	}
}
#endif

// Copyright September 2003
// All Rights Reserved!
// Allen Sherrod
// ProgrammingAce@UltimateGameProgramming.com
// www.UltimateGameProgramming.com