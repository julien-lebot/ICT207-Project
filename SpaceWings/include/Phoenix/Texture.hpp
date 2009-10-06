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
#include <Phoenix/Resource.hpp>

namespace Phoenix
{
	class Texture : public Resource
	{
		public:
			Texture(const ResourceManager* const creator,
					const std::string& name,
					const ResourceHandleType handle,
					TextureTarget target);

			Texture(const Texture &copy);
			Texture &operator = (const Texture &);
			virtual ~Texture();

			/// Creates an empty texture for render to texture technique.
			void create(const std::string& name,
						GLuint width = 128,
						GLuint height = 128,
						GLuint depth = 1,
						GLuint internalFormat = GL_RGBA8,
						GLuint format = GL_RGBA);

			/// Binds a texture for rendering. Does not enable texturing, so you might have to do that manually for the moment. @param textureUnit Which texture unit to bind this texture to.
			void bind(const int textureUnit = -1);

			/// Unbind disables texturing for the target on the texture unit specified to bind.
			void unbind();

			const GLuint getID() const { return mID; }

			/** Gets the type of texture 
			*/
			TextureTarget getTextureTarget(void) const { return mTarget; }

			/** Gets the number of mipmaps to be used for this texture.
			*/
			size_t getUseMipmaps(void) const {return mMipmaps;}

			/** Sets the number of mipmaps to be used for this texture.
			@note
			Must be set before calling any 'load' method.
			*/
			void setUseMipMaps(bool mipmaps) { mMipmaps = mipmaps;}

			/** Are mipmaps hardware generated?
			@remarks
			Will only be accurate after texture load, or createInternalResources
			*/
			bool getMipmapsHardwareGenerated(void) const { return mMipmapsHardwareGenerated; }

			/** Returns the height of the texture.
			*/
			size_t getHeight(void) const { return mHeight; }

			/** Returns the width of the texture.
			*/
			size_t getWidth(void) const { return mWidth; }

			/** Returns the depth of the texture (only applicable for 3D textures).
			*/
			size_t getDepth(void) const { return mDepth; }

		protected:
			/// Texture's height
			size_t mHeight;

			/// Texture's width
			size_t mWidth;

			/// Texture's depth
			size_t mDepth;

			/// Does this texture uses mipmaps ?
			bool mMipmaps;

			/// Are the mipmaps hardware generated ? (Subject to terms and conditions: e.g. the hardware / driver supports it)
			bool mMipmapsHardwareGenerated;

			/// The texture target, e.g. TEXTURE_2D etc...
			TextureTarget mTarget;

			/// Which texture unit this texture is currently bound to.
			int	mCurrtUnit;

			/// Represents the texture format in hw. TODO: Encapsulate those in an enum.
			int mInternalFormat, mFormat;

			/// The texture ID. Note that this is OpenGL dependent as DirectX does not use IDs for its textures. A better solution should be found.
			GLuint mID;

			/// Load texels from an image. Precondition: the image must have been previously loaded.
			virtual void loadImpl(const ImagePtr image, const GLuint target);

			/// @copydoc Resource::prepareImpl
			virtual void prepareImpl();

			/// @copydoc Resource::loadImpl
			virtual void loadImpl();

			/// @copydoc Resource::unloadImpl
			virtual void unloadImpl();

			// Should not be in the interface
			GLenum getGLTextureTarget() const;

			// DEPRECATED
			int getValidWrapMode (int clamp);
			int getValidMagFilter(int filter);
			int getValidMinFilter(int filter);
	};

	typedef tr1::shared_ptr<Texture> TexturePtr;
}
#endif