/*
*	GameEntity.hpp
*
*	GameEntity class. Base entity class
*
*	Created by Terje Loe
*/

#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <Phoenix\base.h>
#include <Phoenix\NonCopyable.hpp>
#include <Phoenix\Model.hpp>
#include <ObjToSwm\SwmReader.hpp>

namespace Phoenix
{
	/* @breif base entity class
	 *
	 * The class is used as the base for GameEntities
	 */
	class GameEntity
	{
	public:
		GameEntity () {}
		virtual GameEntity* create() {}
		virtual void load( Phoenix::ModelFileReader& reader ){}
		void setPossition( float x, float y, float z ) { m_pos.x = x; m_pos.y = y; m_pos.z = z; }
	protected:
		Phoenix::Model m_model;
		Possition m_pos;
	};
}

#endif