/*
*	TextureManager.cpp
*
*	A manager of texture resources.
*
*  Created by Julien Lebot on 17/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/TextureManager.hpp>
#include <Phoenix/Texture.hpp>

using namespace Phoenix;

// Instantiates the Singleton's data member to avoid linking errors
template<> TextureManager* Singleton<TextureManager, CustomCreated, DefaultLifetime>::mInstance = NULL;
template<> bool Singleton<TextureManager, CustomCreated, DefaultLifetime>::mDestroyed = false;
template<> boost::recursive_mutex Singleton<TextureManager, CustomCreated, DefaultLifetime>::singletonMutex;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{}

std::pair<ResourcePtr, bool> TextureManager::createGet(const std::string& name,
													   const TextureTarget textureTarget,
													   const int numMipmaps, 
													   const bool isAlpha)
{
	std::pair<ResourcePtr, bool> retVal = ResourceManager::createGet(name);

	if (retVal.second)
	{
		Texture* texture = dynamic_cast<Texture*> (retVal.first.get());
		texture->setTextureTarget(textureTarget);
		texture->setNumMipmaps(numMipmaps);
	}

	return retVal;
}

TexturePtr TextureManager::load(const std::string& name,
								const TextureTarget textureTarget,
								const int numMipmaps, 
								const bool isAlpha
								/*PixelFormat desiredFormat*/)
{
	std::pair<ResourcePtr, bool> retVal = createGet(name, textureTarget, numMipmaps,isAlpha);
	Texture* texture = dynamic_cast<Texture*> (retVal.first.get());
	texture->load();
	return TexturePtr(texture);
}