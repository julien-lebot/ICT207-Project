/*
*	Window.hpp
*
*	A class representing a window for rendering.
*
*  Created by Julien Lebot on 23/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Window_hpp__
#define __Window_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Resolution.hpp>
#include <Phoenix/NonCopyable.hpp>
#include <Phoenix/Tuple.hpp>
#include <string>

namespace Phoenix
{
	/**
	 * @brief A rendering window class. Client should subclass it to do anything useful.
	 */
	class _PhoenixExport Window
	: private NonCopyable
	{
	public:
		/// Display flags that are used to configure the window
		enum DisplayFlags
		{
			RGB	= 0,
			SINGLE = 0,
			INDEXED = 1,
			DOUBLE = 2,
			ACCUM = 4,
			ALPHA = 8,
			DEPTH = 16,
			STENCIL = 32,
			MULTISAMPLE = 128
		};

		/**
		 * @brief Default constructor
		 */
		Window();

		/**
		 * @brief The constructor.
		 *
		 * @param parent The parent window, if any. If a parent window is specified, this window will become a child of that window;
		 *				 otherwise it will be a top-level window.
		 * @param name The name of the window, it's also the title that is displayed.
		 * @param res The resolution of the window.
		 * @param fullscreen Whether to go fullscreen or not.
		 */
		Window(Window * const parent,
			   const std::string& name,
			   const Resolution_us& res,
			   bool fullscreen);

		/// Virtual destructor
		virtual ~Window();

		/**
		 * @brief Effectively creates the window and all the associated rendering context.
		 * 
		 */
		void initialize();

		/**
		 * @brief Destroys that window and all associated rendering context. If that window has any children, they'll be destroyed too.
		 */
		void destroy();

		/**
		 * @brief Main rendering method. All rendering calls should be emitted from that method.
		 * The default implementation does nothing.
		 */
		virtual void onDisplay() {}

		/**
		 * @brief The idle method. Whenever the rendering system is idle this method will be called.
		 * The default implementation does nothing.
		 */
		virtual void onIdle() {}

		/**
		 * @brief The reshape method. Whenever the rendering window's size changes, this method will be called.
		 * The default implementation does nothing.
		 *
		 * @param w The new width of the window.
		 * @param h The new height of the window.
		 */
		virtual void onReshape(int w, int h) {}

		/**
		 * @brief The visibility method. Whenever the rendering window becomes visible or invisible, this method will be called.
		 * The default implementation does nothing.
		 *
		 * @param visible Whether or not the window is visible.
		 */
		virtual void onVisibility(bool visible) {}

		/**
		 * @brief The keyboard handling method. Whenever a key is pressed on the keyboard this method will be called.
		 * The default implementation does nothing.
		 *
		 * @param key The key code of the key pressed.
		 * @param x The x coordinate of the mouse.
		 * @param y The y coordinate of the mouse.
		 */
		virtual void onKeyboard(unsigned char key, int x, int y) {}

		/**
		* @brief The keyboard handling method. Whenever a key is depressed on the keyboard this method will be called.
		* The default implementation does nothing.
		*
		* @param key The key code of the key pressed.
		* @param x The x coordinate of the mouse.
		* @param y The y coordinate of the mouse.
		*/
		virtual void onKeyboardUp(unsigned char key, int x, int y) {}

		/**
		 * @brief The special keyboard handling method. Whenever a special key (e.g. arrow up etc...) is pressed this method will be called.
		 * The default implementation does nothing.
		 *
		 * @param key The key code of the key pressed.
		 * @param x The x coordinate of the mouse.
		 * @param y The y coordinate of the mouse.
		 */
		virtual void onSpecialKey(int key, int x, int y) {}

		/**
		* @brief The special keyboard handling method. Whenever a special key (e.g. arrow up etc...) is depressed this method will be called.
		* The default implementation does nothing.
		*
		* @param key The key code of the key pressed.
		* @param x The x coordinate of the mouse.
		* @param y The y coordinate of the mouse.
		*/
		virtual void onSpecialKeyUp(int key, int x, int y) {}

		/**
		 * @brief The mouse motion method. Whenever the mouse moves, this method will be called.
		 * @param x The new x coordinate of the mouse.
		 * @param y The new y coordinate of the mouse.
		 */
		virtual void onMotion(int x, int y) {}

		/**
		* @brief The mouse event method. Whenever the mouse generates an event (such as a button press), this method will be called.
		* @param button An integer representing which button has been pressed on the mouse.
		* @param state An integer representing the state of the mouse
		* @param x The x coordinate of the mouse.
		* @param y The y coordinate of the mouse.
		*/
		virtual void onMouseEvent(int button, int state, int x, int y) {}

		/**
		* @brief The mouse passive method. Whenever the mouse comes to rest, this method will be called.
		* @param x The new x coordinate of the mouse.
		* @param y The new y coordinate of the mouse.
		*/
		virtual void onPassiveMotion(int x, int y) {}

		/**
		 * @brief Gets the handle associated with this window.
		 * @return The handle of the window.
		 */
		int getHandle() const { return mHandle; }

		/**
		 * @brief Gets the name (or title) of the window.
		 * @param The name (or title) of the window.
		 */
		const std::string& getName() const { return mName; }

		/**
		 * @brief Gets the resolution of the window.
		 * @return The resolution of the window.
		 */
		const Resolution_us& getResolution() const { return mResolution; }

		/**
		 * @brief Sets the name of the window.
		 * @param name The new name of the window.
		 */
		void setName(const std::string& name)
		{
			mName = name;
			glutSetWindowTitle(mName.c_str());
		}

		/**
		 * @brief Sets the resolution of the window.
		 * This method has no effect after the window has been initialized.
		 * @param res The new resolution of the window.
		 */
		void setResolution(const Resolution_us& res) { mResolution = res; }

		/**
		 * @brief Sets the display flags for the window.
		 * @param flags The new flags for the window.
		 */
		void setDisplayFlags(const unsigned int& flags)
		{
			mDisplayFlags = flags;
		}

		/**
		 * @brief Returns whether or not this window is initialized.
		 * return A boolean stating whether or not this window is initialized.
		 */
		bool isInitialised() const { return mInitialised; }

	protected:
		/**
		 * @brief Hook method for the subclass to override. It is called just after the window has been initialized.
		 * This can used to do any non-trivial initialization.
		 */
		virtual void initializeImpl() {}

		/**
		* @brief Hook method for the subclass to override. It is called just after the window has been initialized.
		*/
		virtual void destroyImpl() {}

	protected:
		/// The handle of the window
		int mHandle;
		
		/// The resolution of the window
		Resolution_us mResolution;

		/// The name or title of the window
		std::string mName;

		/// The flags that describes the window
		unsigned int mDisplayFlags;

		/// Whether or not the window is initialized.
		bool mInitialised;

		/// Whether or not the window is fullscreen.
		bool mFullscreen;

		/// The parent of that window if any
		Window* mParent;
	};

	typedef tr1::shared_ptr<Window> WindowPtr;
}

#endif // __Window_hpp__