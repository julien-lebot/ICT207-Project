#include <Phoenix/GameLogicIntegrator.hpp>

using namespace Phoenix;
using namespace Math;

Derivative GameLogicIntegrator::evaluate(PhysicsState & state, float t, float dt, const Derivative &derivative)
{
	state.position += derivative.velocity * dt;
	state.momentum += derivative.force * dt;
	state.recalculate();
	
	Derivative output;
	output.velocity = state.velocity;
	forces(state, output.force);
	return output;
}

Derivative GameLogicIntegrator::evaluate(const PhysicsState & state, float t)
{
	Derivative output;
	output.velocity = state.velocity;
	forces(state, output.force);
	return output;
}

void GameLogicIntegrator::integrate(PhysicsState & state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);

	state.position += 1.0f/6.0f * dt * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
	state.momentum += 1.0f/6.0f * dt * (a.force + 2.0f*(b.force + c.force) + d.force);

	state.recalculate();
}

void GameLogicIntegrator::forces(const PhysicsState & state, Vector3f & force)
{
	force -= dampening * state.velocity;
	if((*currentEntityListElem).name == "SERVERPLAYER")
		controls(serverInput, state, force);
	else if((*currentEntityListElem).name == "CLIENTPLAYER")
		controls(clientInput, state, force);
			
	
}

void GameLogicIntegrator::controls(const Input & input, const PhysicsState & state, Vector3f & force)
{
	if(input.rollLeft)
	{
		
	}
	if(input.rollRight)
	{
	
	}
	if(input.accelerate)
	{
		force.z += 1000.0f;
	}
	if(input.breaks)
	{
	
	}

	//deltaX;
	//deltaY;
}

void GameLogicIntegrator::logicLoop(std::vector<Phoenix::EntityListElement> & entityList)
{
	render = !render;

	if(render == false)
	{
		for(std::vector<Phoenix::EntityListElement>::iterator iter = entityList.begin(); iter != entityList.end(); iter++ )
		{
			currentEntityListElem = iter;
			if((*iter).effectedByPhysics())
			{
				integrate((*iter).state, t, dt);
				t += dt;
				accumulator -= dt;
			}
			else if ((*iter).orbit && ((*iter).state.position.x + (*iter).state.position.y + (*iter).state.position.z) == 0.0)
			{
				(*iter).state.position.x = (*iter).orbit;
			}
			else
			{
				updateOrbitPosition((*iter));
				updateRotation((*iter));
			}
		}
	}
}
/*
void GameLogicIntegrator::logicLoop(std::vector<Phoenix::EntityListElement> & entityList)
{
	if(render)
	{
		newTime = timer.getElapsedTimeInMilliSec();
		deltaTime = newTime - currentTime;

		currentTime = newTime;
		accumulator += deltaTime;

		render = false;
	}

	if(accumulator >= dt)
	{
		for(std::vector<Phoenix::EntityListElement>::iterator iter = entityList.begin(); iter != entityList.end(); iter++ )
		{
			currentEntityListElem = iter;
			if((*iter).effectedByPhysics())
			{
				integrate((*iter).state, t, dt);
				t += dt;
				accumulator -= dt;
			}
			else
			{
				if ((*iter).year)
					updateOrbitPosition((*iter));
				if ((*iter).day)
					updateRotation((*iter));
			}
		}
	}
	else
		render = true;
}
*/
void GameLogicIntegrator::updateOrbitPosition (Phoenix::EntityListElement & listElement)
{
	float angle = convertPolar (listElement.state.position.x, listElement.state.position.z) + 2.0 * PI / listElement.year;
	listElement.state.position.x = listElement.orbit * cos (angle);
	listElement.state.position.z = listElement.orbit * sin (angle);
}

void GameLogicIntegrator::updateRotation (Phoenix::EntityListElement & listElement)
{
	double angle = 2.0 * PI / listElement.day * timer.getElapsedTimeInMilliSec () / 100.0;

	Math::Vector3f axis = Vector3f (sin (listElement.axis), cos (listElement.axis), sin (listElement.axis));
	axis.normalize ();
	listElement.state.orientation.fromAngleAxis (angle, axis);
}