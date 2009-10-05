/*
*	ConsoleGL.cpp
*
*	OpenGL console
*
*  Created by Julien Lebot on 07/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/ConsoleGL.hpp>
#include <iostream>

using namespace Phoenix;

ConsoleGL::ConsoleGL(std::size_t maxCmdHistory,
				     std::size_t maxTxtHistory,
				     bool echo,
				     ConsoleFunction defaultFunction,
				     int cmdLineIdx)
: Console(maxCmdHistory, maxTxtHistory, echo, defaultFunction, cmdLineIdx)
{
}

ConsoleGL::~ConsoleGL()
{
}

void ConsoleGL::render()
{
	for(std::list<std::string>::const_iterator iter = mTxtBuf.begin(); iter != mTxtBuf.end(); ++iter)
	{
		std::cout << *iter << "\n";
	}

}