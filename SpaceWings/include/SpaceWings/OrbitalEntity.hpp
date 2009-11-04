/*
*	OrbitalEntity.hpp
*
*	PlayerEntity class. Derived entity class
*
*	Created by Terje Loe
*/


#ifndef ORBITALENTITY_HPP
#define ORBITALENTITY_HPP

#include <SpaceWings\GameEntity.hpp>
#include <iostream>

namespace Phoenix
{
	/* @brief derived entity class
	 *
	 * This is the player Entity
	 */
	class OrbitalEntity : public GameEntity
	{
	public:
		OrbitalEntity () {}
		
		~OrbitalEntity(){};
	};
}

#endif