/*
*	Root.cpp
*
*	The root class object
*
*  Created by Julien Lebot on 23/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Root.hpp>

using namespace Phoenix;

// The list of windows
WindowPtr windows[256];

// Instantiate the static members to avoid linking errors
int Phoenix::Root::mCurrentIdleWindow = 0;
bool Phoenix::Root::mIdleFuncEnabled = false;

template<> Root* Singleton<Phoenix::Root>::mInstance = NULL;
template<> bool Singleton<Phoenix::Root>::mDestroyed = false;

#if defined(_THREAD_SUPPORT)
template<> boost::recursive_mutex Singleton<Phoenix::Root>::singletonMutex;
#endif

bool Root::initialize(const std::string& logFile,
					  const std::string& cfgFile)
{
	// Dummy argv/argv pair
	int argc = 1;
	char *argv = "";
	glutInit(&argc, &argv);

	LogManager::instance().setDefaultLog(logFile);

	//LogManager::instance().getDefaultLog()->write(_PROJECT_NAME_ + std::string(" initializing ..."));

	//LogManager::instance().getDefaultLog()->write(std::string("version ") + _VERSION_MAJOR_ + "." + _VERSION_MINOR_ + "." + _VERSION_PATCH_ + " " + "(" + _VERSION_NAME_ + ")");

	mInitDone = true;
	return true;
}

void Root::enterMainLoop()
{
	glutMainLoop();
}

void Root::addWindow(WindowPtr window, bool autoConfigure)
{
	// Make sure the window is initialized or face a core dump
	if (!window->isInitialised())
		window->initialize();

	// Store the window in our window list
	windows[window->getHandle()] = window;

	// These need to be called every time a window is added
	glutDisplayFunc(CallBackDisplayFunc);
	glutIdleFunc(CallBackIdleFunc);
	glutKeyboardUpFunc(CallBackKeyboardUpFunc);
	glutKeyboardFunc(CallBackKeyboardFunc);
	glutSpecialUpFunc(CallBackSpecialUpFunc);
	glutSpecialFunc(CallBackSpecialFunc);
	glutMouseFunc(CallBackMouseFunc);
	glutMotionFunc(CallBackMotionFunc);
	glutPassiveMotionFunc(CallBackPassiveMotionFunc);
	glutReshapeFunc(CallBackReshapeFunc); 
	glutVisibilityFunc(CallBackVisibilityFunc);
}

void Root::shutdown()
{
}

bool Root::isInitialised()
{
	return mInitDone;
}

Root::Root()
{
}

Root::~Root()
{
}


void Root::CallBackDisplayFunc(void)
{
	int windowID = glutGetWindow();
	windows[windowID]->onDisplay();
}

void Root::CallBackIdleFunc(void)
{
	if(mIdleFuncEnabled && mCurrentIdleWindow)
	{
		glutSetWindow(mCurrentIdleWindow);
		windows[mCurrentIdleWindow]->onIdle();
	}
}

void Root::CallBackKeyboardFunc(unsigned char key, int x, int y)
{
	int windowID = glutGetWindow();
	windows[windowID]->onKeyboard(key, x, y);
}

void Root::CallBackKeyboardUpFunc(unsigned char key, int x, int y)
{
	int windowID = glutGetWindow();
	windows[windowID]->onKeyboardUp(key, x, y);
}

void Root::CallBackMotionFunc(int x, int y)
{
	int windowID = glutGetWindow();
	windows[windowID]->onMotion(x, y);
}

void Root::CallBackMouseFunc(int button, int state, int x, int y)
{
	int windowID = glutGetWindow();
	windows[windowID]->onMouseEvent(button, state, x, y);
}

void Root::CallBackPassiveMotionFunc(int x, int y)
{
	int windowID = glutGetWindow();
	windows[windowID]->onPassiveMotion(x, y);
}

void Root::CallBackReshapeFunc(int w, int h)
{
	int windowID = glutGetWindow();
	windows[windowID]->onReshape(w, h);
}
 
void Root::CallBackSpecialFunc(int key, int x, int y)
{
	int windowID = glutGetWindow();
	windows[windowID]->onSpecialKey(key, x, y);
}

void Root::CallBackSpecialUpFunc(int key, int x, int y)
{
	int windowID = glutGetWindow();
	windows[windowID]->onSpecialKeyUp(key, x, y);
}
  
void Root::CallBackVisibilityFunc(int visible)
{
	int windowID = glutGetWindow();
	windows[windowID]->onVisibility(visible ? true : false);
}

void Root::setIdle(bool enabled)
{
	mIdleFuncEnabled = enabled;
}

void Root::setIdle2ActiveWindow(void)
{
	mCurrentIdleWindow = glutGetWindow();
}