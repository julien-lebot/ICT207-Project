/*
*	Root.hpp
*
*	The root class object
*
*  Created by Julien Lebot on 23/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Root_hpp__
#define __Root_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Window.hpp>
#include <Phoenix/Singleton.hpp>
#include <Phoenix/LogManager.hpp>

namespace Phoenix
{
	/**
	 * @brief The root class object is the entry point to the rendering system.
	 */
	class _PhoenixExport Root
	: public Singleton<Root>
	{
		// Needed for the singleton to be created since the ctor/dtor are private (it's a concrete class after all).
		friend struct CreateUsingNew<Root>;
	public:
		/// Initialise the system, it does important setup and must be called before everything else.
		bool initialize(const std::string& logFile = "Phoenix.log",
						const std::string& cfgFile = "Phoenix.cfg");

		/**
		 * @brief Adds a new window to the render system, that window can subsequently be used for rendering.
		 * The window does not have to be initialized when it's given to the rendering system but it must exist (as in allocated).
		 * This allows the client to derive their class from the Window class and customize it.
		 * Note that a strong reference will be kept by the system so you should not have to care about destroying the window yourself.
		 *
		 * @param window A pointer to the window to be added.
		 * @Param autoConfigure Whether or not to let the system auto configure the window. Default (true) is recommended.
		 */
		void addWindow(WindowPtr window,
					   bool autoConfigure = true);

		/**
		 * @brief Shuts down the system. You do not have to call this directly unless you must (e.g. if you need to shutdown the system prematurely as a
		 * result of an error for example).
		 */
		void shutdown();

		/**
		 * @brief Enters main event processing loop. After this call (which does NOT return), events will be dispatched to the appropriate windows.
		 */
		void enterMainLoop();

		/// Returns whether the system is initialized and ready to be used.
		bool isInitialised();

		/**
		 * @brief Sets whether the idle event will be dispatched. Not all clients need the idle event, since they can have a time to replace it.
		 *
		 * @param enabled Whether to enable or disable the idle event.
		 */
		void setIdle(bool enabled);

		/// In case the idle event dispatching is enabled, tells the system that the current active window will handle the idle events from now on.
		void setIdle2ActiveWindow(void);

		/// Get a reference to the log manager. This is a convenience method
		LogManager& getLogManager() { return LogManager::instance(); }

	private:
		/// Private default constructor
		Root();

		/// Private destructor
		~Root();

		/// GLUT display callback
		static void CallBackDisplayFunc(void);

		/// GLUT idle callback
		static void CallBackIdleFunc(void); 

		/// GLUT keyboard callback
		static void CallBackKeyboardUpFunc(unsigned char key, int x, int y);
		static void CallBackKeyboardFunc(unsigned char key, int x, int y);

		/// GLUT motion callback
		static void CallBackMotionFunc(int x, int y);

		/// GLUT mouse callback
		static void CallBackMouseFunc(int button, int state, int x, int y);

		/// GLUT passive motion callback
		static void CallBackPassiveMotionFunc(int x, int y);

		/// GLUT reshape callback
		static void CallBackReshapeFunc(int w, int h); 

		/// GLUT special func callback
		static void CallBackSpecialUpFunc(int key, int x, int y);
		static void CallBackSpecialFunc(int key, int x, int y);

		/// GLUT visibility func callback
		static void CallBackVisibilityFunc(int visible);

		/// The handle of the window that is handling idle events
		static int mCurrentIdleWindow;

		/// Whether or not the idle event is enabled
		static bool mIdleFuncEnabled;

		/// Whether or not the initialization has been done
		bool mInitDone;
	};
}

#endif // __Root_hpp__