/*
*	EntityManager.hpp
*
*	EntityManager class.
*
*	Created by Terje Loe
*
*	Edited by Joshua Wilding on 03/11/09
*/

#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <Phoenix\Renderer.hpp>
#include <Phoenix\NonCopyable.hpp>
#include <SpaceWings\GameEntity.hpp>
#include <SpaceWings\PlayerEntity.hpp>
#include <SpaceWings\OrbitalEntity.hpp>
#include <Network\Packet.hpp>
#include <fstream>
#include <string>
#include <vector>

namespace Phoenix
{
	enum EntityType
	{
		// Orbital Body Entity Types
		STAR  = 1,
		PLANET = 2,
		MOON = 4,
		ASTEROIDBELT = 8,
		ASTEROID = 16,
		STARBASE = 32,
		// Static Objects
		//NEBULAE,
		// Dynamic Entity Types
		SPACESHIP = 64,
		//TRADESHIP,
		//COMET
		NUMBER = 7
	};
	
	struct EntityListElement
	{
		std::string name;
		EntityType entityType;
		int parentIndex;
		float size, orbit, year, day, axis;
		PhysicsState state;
		Derivative derivatives;
		GameEntity* gameEntity;

		bool effectedByPhysics () { return (entityType >= SPACESHIP); }
	};

	class EntityManager : public NonCopyable
	{
	public:
		EntityManager () { levelSent = false; }		// Generate template entities
		
		void setup (bool server) { if(server) readFile("test.world"); printEntities (); }
		void getPosition (const EntityListElement& tempELE, Math::Vector3f& tempV )
		{
			tempV += tempELE.state.position;
			if (tempELE.parentIndex != -1)
				getPosition (entityList [tempELE.parentIndex], tempV);
		}
		void readFile ( const std::string& );
		void createEntity ( std::string str ) { createEntity( str, false, 0); }
		void createEntity ( std::string, bool, int );
		void destroyEntity ( std::string );
		void printEntities ();
		void createConstructList ( std::vector<Network::Packet> & list );
		std::vector<EntityListElement> & getEntityListRef(){ return entityList; }
		void renderAll ();
	private:
		std::vector<EntityListElement> entityList;
		bool levelSent;
		SwmReader swmR;
	};
}

#endif