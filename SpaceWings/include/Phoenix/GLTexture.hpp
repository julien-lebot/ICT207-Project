/*
*	GLTexture.hpp
*
*	An OpenGL texture class.
*
*  Created by Julien Lebot on 11/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __GLTexture_hpp__
#define __GLTexture_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Texture.hpp>
#include <Phoenix/GLObject.hpp>

namespace Phoenix
{
	// Texture operation policy
	class TextureOps
	{
	public:
		GLuint generateID()
		{
			GLuint GLID;
			glGenTextures(1, &GLID);
			return GLID;
		}

		void bind(const GLuint GLID, const GLenum target) const
		{
			glBindTexture(target, GLID);
		}

		void destroy(GLuint GLID)
		{
			glDeleteTextures(1, &GLID);
		}

	protected:
		~TextureOps() {}	// Subclass responsible for destroying resource themselves
	};

	// Partial template specialization for texture operations
	template
		<
			class ObjectBase
		>
	class GLObject<TextureOps, ObjectBase>
	: private TextureOps,
	  public virtual ObjectBase
	{
	public:
		/**
		* @brief Default constructor, generates an ID for the resource/object.
		*/
		GLObject()
		: mID(-1)
		{}

		/**
		* @brief Virtual destructor which de-allocates the resource/object.
		*/
		virtual ~GLObject()
		{
			if (mID != -1)
				destroy(mID);
		}

		void create()
		{
			mID = generateID();
		}

		/**
		* @brief Binds the object/resource for use with OpenGL.
		*
		* @param target The texture type to be bound.
		*/
		void bind(const GLenum target) const
		{
			TextureOps::bind(mID, target);
		}

		/**
		* @brief Gets the ID associated with that object/resource.
		*/
		GLsizei getID() const
		{
			return mID;
		}

	protected:
		void destroy()
		{
			destroy(mID);
		}

	protected:
		GLsizei mID;	// The OpenGL id for that resource/object
	};

	class GLTexture
	: public Texture
	{
	public:
		GLTexture(ResourceManager* creator, const std::string& name, const ResourceHandleType handle);
		~GLTexture();
		GLenum getTarget() const;
		bool loadImg(const Image &image);
		void bind();
	};

	typedef tr1::shared_ptr<GLTexture> GLTexturePtr;
}

#endif // __GLTexture_hpp__