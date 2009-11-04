/*
*	GameLogicIntegrator.hpp
*
*	GameLogicIntegrator class.
*
*	Created by Terje Loe
*/

#ifndef GAMELOGICINTEGRATOR_HPP
#define GAMELOGICINTEGRATOR_HPP

#include <Phoenix/Timer.hpp>
#include <Phoenix/Input.hpp>
#include <SpaceWings/GameEntity.hpp>
#include <Phoenix/EntityManager.hpp>

namespace Phoenix
{
	const float dampening = 10;
	
	class GameLogicIntegrator
	{
	public:
		GameLogicIntegrator() : accumulator(0.0f), t(0.0f), dt(0.01f), render(true)
		{
			timer.start();
			currentTime = (float) timer.getElapsedTimeInMilliSec();
		}

		Phoenix::Derivative evaluate(Phoenix::PhysicsState & state, float t, float dt, const Phoenix::Derivative & derivative);
		Phoenix::Derivative evaluate(const Phoenix::PhysicsState & state, float t);

		void integrate(Phoenix::PhysicsState & state, float t, float dt);
		void forces(const Phoenix::PhysicsState & state, Math::Vector3f & force);
		void controls(const Phoenix::Input & input, const Phoenix::PhysicsState & state, Math::Vector3f & force);
		
		void logicLoop(std::vector<Phoenix::EntityListElement> & entityList);
		void updateOrbitPosition(Phoenix::EntityListElement & listElement);
		void updateRotation (Phoenix::EntityListElement & listElement);

		bool timeToRender() { return render; }

		void setClientInput(Input & inp) { clientInput = inp; }
		void setServerInput(Input & inp) { serverInput = inp; }
		
		Input & getClientInput() { return clientInput; }
		Input & getServerInput() { return serverInput; }
	private:
		Phoenix::Input clientInput;
		Phoenix::Input serverInput;

		Phoenix::Timer timer;
		float t;
		float dt;
		
		float accumulator;
		float currentTime;

		float newTime;
		float deltaTime;

		Phoenix::PhysicsState previousState;
		Phoenix::PhysicsState currentState;

		std::vector<Phoenix::EntityListElement>::iterator currentEntityListElem;

		bool render;
	};
}

#endif