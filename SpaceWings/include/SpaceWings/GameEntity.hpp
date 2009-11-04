/*
*	GameEntity.hpp
*
*	GameEntity class. Base entity class
*
*	Created by Terje Loe
*/

#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <Phoenix/base.h>
#include <Phoenix/HardwareBuffer.hpp>
#include <Phoenix/Model.hpp>
#include <Phoenix/RenderOperation.hpp>
#include <ObjToSwm/SwmReader.hpp>
#include <Phoenix/Vector3.hpp>
#include <Phoenix/Quaternion.hpp>

namespace Phoenix
{
	struct PhysicsState
	{
        // primary
		Phoenix::Math::Vector3f position;
		Phoenix::Math::Vector3f momentum;
		Phoenix::Math::Quaternionf orientation;

        // secondary
        Phoenix::Math::Vector3f velocity;

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
		Phoenix::Math::Vector3f velocity;
		Phoenix::Math::Vector3f force;
	};

	/* @breif base entity class
	 *
	 * The class is used as the base for GameEntities
	 */
	class GameEntity
	{
	public:
		GameEntity (){ modelFile = "models/"; }
		virtual ~GameEntity (){ delete m_renderOp.vertexData; }
		void load (SwmReader & swmR);
		RenderOperation & getRenderOperation() { return m_renderOp; }

		bool isLoaded() { return loaded; }

		void setModelFile(std::string str) { modelFile += str; }
		std::string getModelFile() { return modelFile; }
	protected:
		std::string modelFile;
		
		std::vector<GLfloat> mdlVertices;
		std::vector<GLfloat> mdlNormals;
		std::vector<GLfloat> mdlTexCoords;
		
		std::vector<int> mtlOffsets;
		std::vector<std::string> mtlOffsetNames;
		std::vector<Phoenix::Material> mtlList;
		
		RenderOperation m_renderOp;

		bool loaded;

	};
}

#endif