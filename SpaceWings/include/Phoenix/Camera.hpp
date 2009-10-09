/*
*	Camera.hpp
*
*	A class representing a camera for rendering.
*
*  Created by Julien Lebot on 25/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Camera_hpp__
#define __Camera_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Math.hpp>
#include <Phoenix/Vector3.hpp>
#include <Phoenix/Matrix3.hpp>
#include <Phoenix/Matrix4.hpp>
#include <Phoenix/Quaternion.hpp>

#define _USE_CUSTOM_VIEW_MATRIX_ 1
#define _USE_QUATERNIONS_ 0

namespace Phoenix
{
	class Camera
	{
	public:
		typedef Math::Vector3f Vector3_t;
		typedef Math::Quaternionf Quaternion_t;
		typedef Math::Matrix3f Matrix3_t;
		typedef Math::Matrix4f Matrix4_t;

		Camera();
		virtual ~Camera();

		/**
		* Sets the camera's position.
		*/
		void setPosition(const Vector3_t& vec);

		/** Retrieves the camera's position.
		*/
		const Vector3_t& getPosition() const;

		/** Moves the camera's position by the vector offset provided along world axes.
		*/
		void move(const Vector3_t& vec);

		/** Moves the camera's position by the vector offset provided along it's own axes (relative to orientation).
		*/
		void moveRelative(const Vector3_t& vec);

		/** Sets the camera's direction vector.
		*/
		void setDirection(const Vector3_t& vec);

		/* Gets the camera's direction.
		*/
		Vector3_t getDirection() const;

		/** Gets the camera's up vector.
		*/
		Vector3_t getUp() const;

		/** Gets the camera's right vector.
		*/
		Vector3_t getRight() const;

		/** Points the camera at a location in worldspace.
		@remarks
		This is a helper method to automatically generate the
		direction vector for the camera, based on it's current position
		and the supplied look-at point.
		@param
		targetPoint A vector specifying the look at point.
		*/
		void lookAt(const Vector3_t& targetPoint);

		/** Rolls the camera anticlockwise, around its local z axis.
		*/
		void roll(const Math::Units::Radians& angle);

		/** Rotates the camera anticlockwise around it's local y axis.
		*/
		void yaw(const Math::Units::Radians& angle);

		/** Pitches the camera up/down anticlockwise around it's local z axis.
		*/
		void pitch(const Math::Units::Radians& angle);

		/** Rotate the camera around an arbitrary axis.
		*/
		void rotate(const Vector3_t& axis, const Math::Units::Radians& angle);

		/** Rotate the camera around an arbitrary axis using a Quaternion.
		*/
		void rotate(const Quaternion_t& q);

		/** Tells the camera whether to yaw around it's own local Y axis or a 
		fixed axis of choice.
		@remarks
		This method allows you to change the yaw behaviour of the camera
		- by default, the camera yaws around a fixed Y axis. This is 
		often what you want - for example if you're making a first-person 
		shooter, you really don't want the yaw axis to reflect the local 
		camera Y, because this would mean a different yaw axis if the 
		player is looking upwards rather than when they are looking
		straight ahead. You can change this behaviour by calling this 
		method, which you will want to do if you are making a completely
		free camera like the kind used in a flight simulator. 
		@param
		useFixed If true, the axis passed in the second parameter will 
		always be the yaw axis no matter what the camera orientation. 
		If false, the camera yaws around the local Y.
		@param
		fixedAxis The axis to use if the first parameter is true.
		*/
		void setFixedYawAxis(bool useFixed, const Vector3_t& fixedAxis = Vector3_t::Y);

		void updateViewMatrix();

		const Matrix4_t& getViewMatrix();

		virtual void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);

		void doLook();
	protected:
		Vector3_t mPos;

		/// The view matrix
		Matrix4_t mViewMatrix;

		/// The projection matrix
		Matrix4_t mProjMatrix;

		/// The view projection matrix (view * proj)
		Matrix4_t mViewProjMatrix;

		/// Camera orientation, quaternion style
		Quaternion_t mOrientation;

		Vector3_t mUp, mRight, mView;

		/// The camera local axis
		Vector3_t mXAxis, mYAxis, mZAxis;

		/// Whether to yaw around a fixed axis.
		bool mYawFixed;
		/// Fixed axis to yaw around
		Vector3_t mYawFixedAxis;
	};
}

#endif // __Camera_hpp__