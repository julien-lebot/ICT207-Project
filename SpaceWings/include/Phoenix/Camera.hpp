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
	class _PhoenixExport Camera
	{
		enum CameraBehavior
		{
			CAMERA_BEHAVIOR_FIRST_PERSON,
			CAMERA_BEHAVIOR_SPECTATOR,
			CAMERA_BEHAVIOR_FLIGHT,
			CAMERA_BEHAVIOR_ORBIT
		};
	public:
		typedef Math::Vector3f Vector3_t;
		typedef Math::Quaternionf Quaternion_t;
		typedef Math::Matrix3f Matrix3_t;
		typedef Math::Matrix4f Matrix4_t;

		Camera();
		virtual ~Camera();

		void lookAt(const Vector3_t &target);
		void lookAt(const Vector3_t &eye, const Vector3_t &target, const Vector3_t &up);
		void move(float dx, float dy, float dz);
		void move(const Vector3_t &direction, const Vector3_t &amount);
		void perspective(float fovx, float aspect, float znear, float zfar);
		void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);
		void rotateSmoothly(float headingDegrees, float pitchDegrees, float rollDegrees);
		void undoRoll();
		void updatePosition(const Vector3_t &direction, float elapsedTimeSec);
		void zoom(float zoom, float minZoom, float maxZoom);

		// Getter methods.

		_INLINE const Vector3_t &Camera::getAcceleration() const
		{ return m_acceleration; }

		_INLINE Camera::CameraBehavior Camera::getBehavior() const
		{ return m_behavior; }

		_INLINE const Vector3_t &Camera::getCurrentVelocity() const
		{ return m_currentVelocity; }

		_INLINE const Vector3_t &Camera::getPosition() const
		{ return m_eye; }

		_INLINE float Camera::getOrbitMinZoom() const
		{ return m_orbitMinZoom; }

		_INLINE float Camera::getOrbitMaxZoom() const
		{ return m_orbitMaxZoom; }

		_INLINE float Camera::getOrbitOffsetDistance() const
		{ return m_orbitOffsetDistance; }

		_INLINE const Quaternion_t &Camera::getOrientation() const
		{ return m_orientation; }

		_INLINE float Camera::getRotationSpeed() const
		{ return m_rotationSpeed; }

		_INLINE const Matrix4_t &Camera::getProjectionMatrix() const
		{ return m_projMatrix; }

		_INLINE const Vector3_t &Camera::getVelocity() const
		{ return m_velocity; }

		_INLINE const Vector3_t &Camera::getViewDirection() const
		{ return m_viewDir; }

		_INLINE const Matrix4_t &Camera::getViewMatrix() const
		{ return m_viewMatrix; }

		_INLINE const Matrix4_t &Camera::getViewProjectionMatrix() const
		{ return m_viewProjMatrix; }

		_INLINE const Vector3_t &Camera::getXAxis() const
		{ return m_xAxis; }

		_INLINE const Vector3_t &Camera::getYAxis() const
		{ return m_yAxis; }

		_INLINE const Vector3_t &Camera::getZAxis() const
		{ return m_zAxis; }

		_INLINE bool Camera::preferTargetYAxisOrbiting() const
		{ return m_preferTargetYAxisOrbiting; }

		// Setter methods.

		void setAcceleration(const Vector3_t &acceleration);
		void setBehavior(CameraBehavior newBehavior);
		void setCurrentVelocity(const Vector3_t &currentVelocity);
		void setCurrentVelocity(float x, float y, float z);
		void setOrbitMaxZoom(float orbitMaxZoom);
		void setOrbitMinZoom(float orbitMinZoom);
		void setOrbitOffsetDistance(float orbitOffsetDistance);
		void setOrbitPitchMaxDegrees(float orbitPitchMaxDegrees);
		void setOrbitPitchMinDegrees(float orbitPitchMinDegrees);
		void setOrientation(const Quaternion_t &newOrientation);
		void setPosition(const Vector3_t &newEye);
		void setPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting);
		void setRotationSpeed(float rotationSpeed);
		void setVelocity(const Vector3_t &velocity);
		void setVelocity(float x, float y, float z);

	private:
		void rotateFirstPerson(float headingDegrees, float pitchDegrees);
		void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);
		void rotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees);
		void updateVelocity(const Vector3_t &direction, float elapsedTimeSec);
		void updateViewMatrix();

		static const float DEFAULT_ROTATION_SPEED;
		static const float DEFAULT_FOVX;
		static const float DEFAULT_ZNEAR;
		static const float DEFAULT_ZFAR;
		static const float DEFAULT_ORBIT_MIN_ZOOM;
		static const float DEFAULT_ORBIT_MAX_ZOOM;
		static const float DEFAULT_ORBIT_OFFSET_DISTANCE;
		static const Vector3_t WORLD_XAXIS;
		static const Vector3_t WORLD_YAXIS;
		static const Vector3_t WORLD_ZAXIS;

		CameraBehavior m_behavior;
		bool m_preferTargetYAxisOrbiting;
		float m_accumPitchDegrees;
		float m_savedAccumPitchDegrees;
		float m_rotationSpeed;
		float m_fovx;
		float m_aspectRatio;
		float m_znear;
		float m_zfar;
		float m_orbitMinZoom;
		float m_orbitMaxZoom;
		float m_orbitOffsetDistance;
		float m_firstPersonYOffset;
		Vector3_t m_eye;
		Vector3_t m_savedEye;
		Vector3_t m_target;
		Vector3_t m_targetYAxis;
		Vector3_t m_xAxis;
		Vector3_t m_yAxis;
		Vector3_t m_zAxis;
		Vector3_t m_viewDir;
		Vector3_t m_acceleration;
		Vector3_t m_currentVelocity;
		Vector3_t m_velocity;
		Quaternion_t m_orientation;
		Quaternion_t m_savedOrientation;
		Matrix4_t m_viewMatrix;
		Matrix4_t m_projMatrix;
		Matrix4_t m_viewProjMatrix;
	};
}

#endif // __Camera_hpp__