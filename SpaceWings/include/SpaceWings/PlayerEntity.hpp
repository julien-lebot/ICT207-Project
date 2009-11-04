/*
*	PlayerEntity.hpp
*
*	PlayerEntity class. Derived entity class
*
*	Created by Terje Loe
*/


#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include <SpaceWings/GameEntity.hpp>
#include <iostream>

namespace Phoenix
{
	/* @breif derived entity class
	 *
	 * This is the player Entity
	 */
	class PlayerEntity : public GameEntity
	{
	public:
		PlayerEntity ()	{}
		
		~PlayerEntity(){};
	};
}

#endif