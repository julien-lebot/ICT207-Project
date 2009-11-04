/*
*	Input.hpp
*
*	Input struct.
*
*	Created by Terje Loe
*/

#ifndef INPUT_HPP
#define INPUT_HPP

namespace Phoenix
{
	struct Input
	{
		bool rollLeft;
		bool rollRight;
		bool accelerate;
		bool breaks;
		float deltaX;
		float deltaY;
		float previousX;
		float previousY;
	};
}

#endif