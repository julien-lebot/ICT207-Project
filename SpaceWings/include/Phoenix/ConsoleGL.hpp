/*
*	ConsoleGL.hpp
*
*	OpenGL console
*
*  Created by Julien Lebot on 07/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __ConsoleGL_hpp__
#define __ConsoleGL_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Console.hpp>

namespace Phoenix
{
	class ConsoleGL : public Console
	{
	public:
		ConsoleGL(std::size_t maxCmdHistory = 20,
				  std::size_t maxTxtHistory = 50,
				  bool echo = false,
				  ConsoleFunction defaultFunction = NULL,
				  int cmdLineIdx = 0);
		~ConsoleGL();
		void render();
	};
}

#endif // __ConsoleGL_hpp__