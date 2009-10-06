/*
*	Texture.hpp
*
*	A texture class.
*
*  Created by Julien Lebot on 11/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Texture_hpp__
#define __Texture_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Resource.hpp>
#include <Phoenix/RendererCommon.hpp>
#include <Phoenix/Image.hpp>					// TODO:Forward declaration

namespace Phoenix
{
	class Texture
	{
		public:
			Texture(TextureTarget target = TEXTURE_2D);
			Texture(const Texture &copy);
			Texture &operator = (const Texture &);
			~Texture();

			/// Load one texture from a file
			void load(const std::string& path, bool mipmap = true);

			/// Load a texture from an image
			void load(const ImagePtr image, bool mipmap = true);

			/// Load a cube or 3D texture
			void load(const ImageList images, bool mipmap = true);

			/// Creates a 1D texture
			void create(const std::string& name,
						GLuint width,
						GLuint internalFormat = GL_RGBA8,
						GLuint format = GL_RGBA);

			/// Creates a 2D texture
			void create(const std::string& name,
						GLuint width,
						GLuint height,
						GLuint internalFormat = GL_RGBA8,
						GLuint format = GL_RGBA);

			/// Creates a Cube texture
			void create(const std::string& name,
						GLuint width,
						GLuint height,
						GLuint depth,
						GLuint internalFormat = GL_RGBA8,
						GLuint format = GL_RGBA);

			//void copyCurrentBuffer(int newTarget = -1);

			void bind(const int textureUnit = -1);
			void unbind();

			//void bind(HardwarePixelBuffer *buffer);

			const  GLuint  getID() const;

			const GLuint  getTarget() const;
			const GLuint  getHeight() const;
			const GLuint  getWidth()  const;
			const GLuint  getDepth()  const;

			//void    setAnisoLevel(int level);
			GLuint  getAnisoLevel();
			void    destroy();

		protected:
			GLuint			mHeight,
							mWidth,
							mDepth,
							mAniso,
							mID;

			TextureTarget	mTarget;

			int				mCurrtUnit;

			void loadImpl(const ImagePtr image, GLuint target, bool mipmap);

			GLenum getGLTextureTarget() const;

			int getValidWrapMode (int clamp);
			int getValidMagFilter(int filter);
			int getValidMinFilter(int filter);
	};
}
#endif