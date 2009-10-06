/*
*	Window.cpp
*
*	A class representing a window for rendering.
*
*  Created by Julien Lebot on 23/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Window.hpp>

using namespace Phoenix;

Window::Window()
: mDisplayFlags(RGB | DOUBLE | DEPTH),
  mHandle(-1),
  mInitialised(false),
  mParent(NULL),
  mResolution(Resolution_us(640,480)),
  mFullscreen(false),
  mName("Phoenix Render Window !")
{}

Window::Window(Window * const parent,
			   const std::string& name,
			   const Resolution_us& res,
			   bool fullscreen)
: mDisplayFlags(RGB | DOUBLE | DEPTH),
 mHandle(-1),
 mInitialised(false),
 mParent(parent),
 mResolution(res),
 mFullscreen(fullscreen),
 mName(name)
{}

Window::~Window()
{
	destroy();
}

void Window::initialize() 
{
	glutInitDisplayMode(mDisplayFlags);
	glutInitWindowSize(mResolution.getWidth(), mResolution.getHeight());
	glutInitWindowPosition(0, 0);
	if (mParent != NULL)
		mHandle = glutCreateSubWindow(mParent->getHandle(), 0, 0, mResolution.getWidth(), mResolution.getHeight());
	else
		mHandle = glutCreateWindow(mName.c_str());

	glutSetWindowTitle(mName.c_str());

	if (mFullscreen)
		glutFullScreen();

	mInitialised = true;
	assert(checkGLErrors());
	initializeImpl();
	assert(checkGLErrors());
}

void Window::destroy()
{
	if (mHandle)
		glutDestroyWindow(mHandle);
	mInitialised = false;
	destroyImpl();
}