/*
*	GLTextureManager.hpp
*
*	A manager of OpenGL texture resources.
*
*  Created by Julien Lebot on 18/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __GLTextureManager_hpp__
#define __GLTextureManager_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/TextureManager.hpp>

namespace Phoenix
{
	class GLTextureManager
		: public TextureManager
	{
		GLTextureManager();
		~GLTextureManager();
	protected:
		ResourcePtr createImpl(const std::string& name, const ResourceHandleType handle);

		void createDefaultTexture();

		GLuint mDefaultTexID;
	};
}

#endif // __GLTextureManager_hpp__