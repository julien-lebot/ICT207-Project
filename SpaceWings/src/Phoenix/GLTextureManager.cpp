/*
*	GLTextureManager.cpp
*
*	A manager of OpenGL texture resources.
*
*  Created by Julien Lebot on 18/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/GLTextureManager.hpp>

using namespace Phoenix;

GLTextureManager::GLTextureManager()
: TextureManager(),
  mDefaultTexID(0)
{
	createDefaultTexture();
}

GLTextureManager::~GLTextureManager()
{
	glDeleteTextures(1, &mDefaultTexID);
}

ResourcePtr GLTextureManager::createImpl(const std::string& name, const ResourceHandleType handle)
{
	return ResourcePtr();
}

void GLTextureManager::createDefaultTexture()
{

}