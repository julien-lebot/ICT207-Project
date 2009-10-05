/*
*	TextureManager.hpp
*
*	A manager of texture resources.
*
*  Created by Julien Lebot on 17/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __TextureManager_hpp__
#define __TextureManager_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/ResourceManager.hpp>
#include <Phoenix/Texture.hpp>
#include <Phoenix/RendererCommon.hpp>
#include <Phoenix/Singleton.hpp>

namespace Phoenix
{
	/**
	 * @brief A class that manages texture resources.
	 */
	class TextureManager
	: public ResourceManager,
	  public Singleton<TextureManager, CustomCreated, DefaultLifetime>
	{
	public:
		/// Default constructor is public since client can't instantiate that class anyway (it's abstract).
		TextureManager();
		virtual ~TextureManager();

		virtual std::pair<ResourcePtr, bool> createGet(const std::string& name,
													   const TextureTarget textureTarget = TEXTURE_2D,
													   const int numMipmaps = -1, 
													   const bool isAlpha = false
													   /*PixelFormat desiredFormat = PF_UNKNOWN*/);

		virtual TexturePtr load(const std::string& name,
								const TextureTarget textureTarget = TEXTURE_2D,
								const int numMipmaps = -1, 
								const bool isAlpha = false
								/*PixelFormat desiredFormat = PF_UNKNOWN*/);
	protected:
	};
}

#endif // __TextureManager_hpp__