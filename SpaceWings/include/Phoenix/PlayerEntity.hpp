/*
*	PlayerEntity.hpp
*
*	PlayerEntity class. Derived entity class
*
*	Created by Terje Loe
*/


#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include <Phoenix\GameEntity.hpp>
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
		PlayerEntity () { modelFile = "Raider.swm"; }
		
		/* Creates a new PlayerEntity. Used by the Object Factory
		 * @return PlayerEntity* to the new object
		 */
		PlayerEntity* create() { return new PlayerEntity; }

		/* Loads the model of the object.
		 * @param reader object to be used for file reading.
		 */

	};
}

#endif