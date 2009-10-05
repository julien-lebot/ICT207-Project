/*
*	GLObject.hpp
*
*	An OpenGL object that implements basic abstraction for OpenGL concepts such
*	as textures or buffers etc...
*
*  Created by Julien Lebot on 11/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __GLObject_hpp__
#define __GLObject_hpp__

#include <Phoenix/Base.h>

namespace Phoenix
{
	/** @brief An OpenGL object that implements basic abstraction for OpenGL concepts such
	 *	as textures or buffers etc...
	 *
	 * @param	ObjectOpsPolicy A policy that implements the basic lifetime operations of an OpenGL object:</br>
	 *			- GLuint generateID(), generates an ID from OpenGL for that object.</br>
	 *			- void destroy(GLuint id), deletes the OpenGL object and frees the ID.</br>
	 *			- void bind(GLuint id), binds the object for use.</br>
	 *
	 * @param	ObjectBase A template class that allows the object to have an abstract base class that is API independent.
	 */
	template 
		<
			class ObjectOpsPolicy,
			class ObjectBase
		>
	class GLObject
	: private ObjectOpsPolicy,
	  public virtual ObjectBase
	{
	public:
		/**
		 * @brief Default constructor, generates an ID for the resource/object.
		 */
		GLObject()
		: mID(generateID())
		{}

		/**
		 * @brief Virtual destructor which de-allocates the resource/object.
		 */
		virtual ~GLObject()
		{
			destroy(mID);
		}

		/**
		 * @brief Binds the object/resource for use with OpenGL.
		 */
		void bind() const
		{
			bind(mID);
		}

		/**
		* @brief Gets the ID associated with that object/resource.
		*/
		GLsizei getID() const
		{
			return mID;
		}

	protected:
		GLsizei mID;	// The OpenGL id for that resource/object
	};
}

#endif // __GLObject_hpp__