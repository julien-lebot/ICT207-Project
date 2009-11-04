/*
*	PhysicsStructs.hpp
*
*	Structs used for physics.
*
*	Created by Terje Loe
*/

#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <Phoenix/Vector3.hpp>
#include <Phoenix/Quaternion.hpp>

namespace Phoenix
{
	struct PhysicsState
	{
        // primary
		Math::Vector3f position;
		Math::Vector3f momentum;
		Math::Quaternionf orientation;

        // secondary
        Math::Vector3f velocity;

		// constant
        float mass;
        float inverseMass;

		void recalculate()
        {
              velocity = momentum * inverseMass;
        }
	};

	struct Derivative
	{
		Math::Vector3f velocity;
		Math::Vector3f force;
	};
}

#endif