/*
*	Camera.cpp
*
*	A class representing a camera for rendering.
*
*  Created by Julien Lebot on 25/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Camera.hpp>
#include <Phoenix/Matrix3.hpp>
#include <iostream>

using namespace Phoenix;
using namespace Phoenix::Math;

Camera::Camera()
: mPos(0,0,-5),
mView(0,0,0),
mUp(0,1,0),
mRight(1,0,0),
mViewMatrix(Camera::Matrix4_t::IDENTITY),
mYawFixed(false),
mYawFixedAxis(Camera::Vector3_t::Y)
{
}

Camera::~Camera()
{

}

void Camera::setPosition(const Camera::Vector3_t& vec)
{
	mPos = vec;
}

const Camera::Vector3_t& Camera::getPosition() const
{
	return mPos;
}

void Camera::move(const Camera::Vector3_t& vec)
{
	mPos += vec;
	mView += vec;
}

void Camera::moveRelative(const Camera::Vector3_t& vec)
{
#if _USE_QUATERNIONS_
	Camera::Vector3_t transformation = mOrientation * vec;
	mPos += transformation;
#else
	Camera::Vector3_t transformation, direction = mView - mPos;
	direction.normalize();

	// fwd / bwd in our direction
	transformation = direction * vec.z;
	mPos += transformation;
	mView += transformation;

	// Computes the right vector
	mRight = mUp.cross(direction);
	mRight.normalize();

	// Strafe
	transformation = mRight * vec.x;
	mPos += transformation;
	mView += transformation;
#endif
}

void Camera::setDirection(const Camera::Vector3_t& vec)
{
	mView = vec;
}

Camera::Vector3_t Camera::getDirection() const
{
	return mView;
}

Camera::Vector3_t Camera::getUp() const
{
	return mUp;
}

Camera::Vector3_t Camera::getRight() const
{
	return mRight;
}

void Camera::lookAt(const Camera::Vector3_t& targetPoint)
{

}

void Camera::roll(const Math::Units::Radians& angle)
{
	rotate(Camera::Vector3_t::X, angle);	
}

void Camera::yaw(const Math::Units::Radians& angle)
{
	Camera::Vector3_t yAxis;

	if (mYawFixed)
	{
		// Rotate around fixed yaw axis
		yAxis = mYawFixedAxis;
	}
	else
	{
#if _USE_QUATERNIONS_
		// Rotate around local Y axis
		yAxis = mOrientation * Camera::Vector3_t::Y;
#else
		Camera::Vector3_t direction = mView - mPos;
		direction.normalize();

		mRight = mUp.cross(direction);
		mRight.normalize();

		yAxis = mRight.cross(direction);
#endif
	}

	rotate(yAxis, angle);
}

void Camera::pitch(const Math::Units::Radians& angle)
{
#if _USE_QUATERNIONS_
	Camera::Vector3_t axis = mOrientation * Camera::Vector3_t::X;
#else
	Camera::Vector3_t direction = mView - mPos;
	Camera::Vector3_t axis = direction.cross(mUp);
	axis.normalize();
#endif
	rotate(axis, angle);
}

void Camera::rotate(const Camera::Vector3_t& axis, const Math::Units::Radians& angle)
{
#if _USE_QUATERNIONS_
	//mOrientation.rotatef(angle.value(), axis.x, axis.y, axis.z);
	//return;
	Camera::Quaternion_t q;
	q.fromAngleAxis(angle, axis);
	rotate(q);
#else
	Camera::Vector3_t newView, direction = mView - mPos;
	Matrix3f rotMat;

	// Calculate the sine and cosine of the angle once
	float cosTheta = cosine(angle);
	float sinTheta = sine(angle);

	/// INFO: the "classic" rotation matrix about an arbitrary axis, here in its 3x3 form
	///
	///		| t * x^2 + c			t * x * y - s * z		t * x * z + s * y |	  |x|	|rotated_x|
	///		| t * x * y + s * z		t * y^2 + c				t * y * z - s * x |	* |y| =	|rotated_y|
	///		| t * x * z - s & y		t * y * z + s * x		t * z^2 + c		  |   |z|	|rotated_z|
	///
	///		where
	///		t = 1 - cos(θ)
	///		c = cos(θ)
	///		s = sin(θ)
	///

	rotMat[0][0] = (cosTheta + (1 - cosTheta) * (axis.x) * (axis.x));
	rotMat[0][1] = ((1 - cosTheta) * axis.x * axis.y - axis.z * sinTheta);
	rotMat[0][2] = ((1 - cosTheta) * axis.x * axis.z + axis.y * sinTheta);

	rotMat[1][0] = ((1 - cosTheta) * axis.x * axis.y + axis.z * sinTheta);
	rotMat[1][1] = (cosTheta + (1 - cosTheta) * (axis.y) * (axis.y));
	rotMat[1][2] = ((1 - cosTheta) * axis.y * axis.z - axis.x * sinTheta);

	rotMat[2][0] = ((1 - cosTheta) * axis.x * axis.z - axis.y * sinTheta);
	rotMat[2][1] = ((1 - cosTheta) * axis.y * axis.z + axis.x * sinTheta);
	rotMat[2][2] = (cosTheta + (1 - cosTheta) * (axis.z) * (axis.z));

	newView = rotMat * direction;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	mView = mPos + newView;
#endif
}

void Camera::rotate(const Quaternion_t& q)
{
	Quaternion_t qnorm = q;
	qnorm.normalize();
	mOrientation = qnorm * mOrientation;
}

void Camera::setFixedYawAxis(bool useFixed, const Camera::Vector3_t& fixedAxis)
{

}

void Camera::updateViewMatrix()
{
#if _USE_QUATERNIONS_
	// View matrix is:
	//
	//  [ Lx  Uy  Dz  Tx  ]
	//  [ Lx  Uy  Dz  Ty  ]
	//  [ Lx  Uy  Dz  Tz  ]
	//  [ 0   0   0   1   ]
	//
	// Where T = -(Transposed(Rot) * Pos)

	// This is most efficiently done using 3x3 Matrices
	Camera::Matrix3_t rot = mOrientation.toMatrix_3x3();

	// Make the translation relative to new axes
	//Camera::Matrix3_t rotT = rot.transpose();
	//Camera::Vector3_t trans = -rotT * mPos;

	// Make final matrix
	//mViewMatrix = Camera::Matrix4_t::IDENTITY;
	mViewMatrix = mOrientation.toMatrix_4x4(); // fills upper 3x3
	mViewMatrix[3][0] = mPos.x;
	mViewMatrix[3][1] = mPos.y;
	mViewMatrix[3][2] = mPos.z;

	//mViewMatrix = mViewMatrix.getTranspose();
	//mViewMatrix = mViewMatrix.inverse();
#elif _USE_CUSTOM_VIEW_MATRIX_
	// Taken from the Red Book http://www.opengl.org/resources/faq/technical/lookat.cpp
	Camera::Vector3_t* xaxis = (Camera::Vector3_t*)(mViewMatrix[0]),
					 * up = (Camera::Vector3_t*)(mViewMatrix[1]),
					 * at = (Camera::Vector3_t*)(mViewMatrix[2]);

	// Compute the direction vector
	*at  = mView - mPos;
	at->normalize();

	// up x direction = right
	*xaxis = at->cross(mUp);
	xaxis->normalize();

	*up = xaxis->cross(*at);

	// Point down the Z axis
	*at *= -1.f;

	mViewMatrix[0][3] = 0.f;
	mViewMatrix[1][3] = 0.f;
	mViewMatrix[2][3] = 0.f;

	mViewMatrix[3][0] = mPos.x;
	mViewMatrix[3][1] = mPos.y;
	mViewMatrix[3][2] = mPos.z;
	mViewMatrix[3][3] = 1.f;

	mViewMatrix = mViewMatrix.inverse();
#else
	mViewMatrix = mOrientation.toMatrix_4x4();

	mXAxis = Camera::Vector3_t(mViewMatrix[0][0], mViewMatrix[1][0], mViewMatrix[2][0]);
	mYAxis = Camera::Vector3_t(mViewMatrix[0][1], mViewMatrix[1][1], mViewMatrix[2][1]);
	mZAxis = Camera::Vector3_t(mViewMatrix[0][2], mViewMatrix[1][2], mViewMatrix[2][2]);
	//mviewDir = -m_zAxis;

	mViewMatrix[3][0] = -mXAxis.dot(mPos);
	mViewMatrix[3][1] = -mYAxis.dot(mPos);
	mViewMatrix[3][2] = -mZAxis.dot(mPos);
#endif
}

const Camera::Matrix4_t& Camera::getViewMatrix()
{
	return mViewMatrix;
}

void Camera::rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
{
	Quaternion_t rot;

	// Rotate camera about the world y axis.
	// Note the order the quaternions are multiplied. That is important!
	if (headingDegrees != 0.0f)
	{
		rot.fromAngleAxis(headingDegrees * Units::radians, Vector3_t::Y);
		mOrientation = rot * mOrientation;
	}

	// Rotate camera about its local x axis.
	// Note the order the quaternions are multiplied. That is important!
	if (pitchDegrees != 0.0f)
	{
		rot.fromAngleAxis(pitchDegrees * Units::radians, Vector3_t::X);
		mOrientation = mOrientation * rot;
	}

	updateViewMatrix();
}

void Camera::doLook()
{
	glMatrixMode(GL_MODELVIEW);
#if (_USE_CUSTOM_VIEW_MATRIX_ == 1)
	updateViewMatrix();
	glLoadMatrixf(&mViewMatrix);
#elif (_USE_QUATERNIONS_ == 1)
	glLoadIdentity();
	glMultMatrixf(&mViewMatrix);
#else
	gluLookAt(mPos.x, mPos.y, mPos.z,
			  mView.x, mView.y, mView.z,
			  mUp.x, mUp.y, mUp.z);
#endif
}