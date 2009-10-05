/*
*	Texture.hpp
*
*	An API independent texture class.
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
	/**
	 * @brief An API independent abstract texture class.
	 */
	class Texture
		: public Resource
	{
	public:
		Texture(ResourceManager* creator, const std::string& name, const ResourceHandleType handle)
		: Resource(creator, name, handle),
		  mHeight(0),
		  mWidth(0),
		  mDepth(0),
		  mNumRequestedMipmaps(0),
		  mNumMipmaps(0),
		  mMipmapsHardwareGenerated(false),
		  mTarget(TEXTURE_2D)
		{
		}

		virtual ~Texture()
		{

		}

		virtual void loadImage(const ImagePtr image) {}

		virtual void loadImages(const ImageList images) {}

		/** Sets the type of texture; can only be changed before load() 
		*/
		virtual void setTextureTarget(TextureTarget target) { mTarget = target; }

		/** Gets the type of texture 
		*/
		virtual TextureTarget getTextureTarget(void) const { return mTarget; }

		/** Gets the number of mipmaps to be used for this texture.
		*/
		virtual size_t getNumMipmaps(void) const {return mNumMipmaps;}

		/** Sets the number of mipmaps to be used for this texture.
		@note
		Must be set before calling any 'load' method.
		*/
		virtual void setNumMipmaps(size_t num) { mNumRequestedMipmaps = mNumMipmaps = num;}

		/** Are mipmaps hardware generated?
		@remarks
		Will only be accurate after texture load, or createInternalResources
		*/
		virtual bool getMipmapsHardwareGenerated(void) const { return mMipmapsHardwareGenerated; }

		/** Returns the height of the texture.
		*/
		virtual size_t getHeight(void) const { return mHeight; }

		/** Returns the width of the texture.
		*/
		virtual size_t getWidth(void) const { return mWidth; }

		/** Returns the depth of the texture (only applicable for 3D textures).
		*/
		virtual size_t getDepth(void) const { return mDepth; }

	protected:
		size_t mHeight;
		size_t mWidth;
		size_t mDepth;

		size_t mNumRequestedMipmaps;
		size_t mNumMipmaps;
		bool mMipmapsHardwareGenerated;

		TextureTarget mTarget;
	};

	typedef tr1::shared_ptr<Texture> TexturePtr;
}

#endif // __Texture_hpp__