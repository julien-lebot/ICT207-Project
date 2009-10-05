/*
*	GLTexture.cpp
*
*	An OpenGL texture class.
*
*  Created by Julien Lebot on 11/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/GLTexture.hpp>

using namespace Phoenix;

GLTexture::GLTexture(ResourceManager* creator, const std::string& name, const ResourceHandleType handle)
: Texture(creator,name,handle)
{

}

GLTexture::~GLTexture()
{

}

GLenum GLTexture::getTarget() const
{
	switch(mTarget)
	{
	case TEXTURE_1D:
		return GL_TEXTURE_1D;
	case TEXTURE_2D:
		return GL_TEXTURE_2D;
	case TEXTURE_3D:
		return GL_TEXTURE_3D;
	case TEXTURE_CUBE_MAP:
		return GL_TEXTURE_CUBE_MAP;
	default:
		return 0;
	}
}

bool GLTexture::loadImg(const Image &image)
{
	// TODO: check max number of mipmaps supported
	glTexParameteri(getTarget(), GL_TEXTURE_MAX_LEVEL, mNumMipmaps);

	if(/*(mUsage & TU_AUTOMIPMAP) &&*/
		mNumRequestedMipmaps && mMipmapsHardwareGenerated)
	{
		glTexParameteri(getTarget(), GL_GENERATE_MIPMAP, GL_TRUE);
	}
/*
	switch(image.getInternalFormat())
	{
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		if(GLEW_ARB_texture_compression &&
			GLEW_EXT_texture_compression_s3tc)
		{
			if(GLEW_SGIS_generate_mipmap && mipmap)
			{
				glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(getGLTextureTarget(), GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2D(target,
				0,
				GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
				image.getWidth(), image.getHeight(), 
				0, 
				((image.getWidth() +3)/4)*((image.getHeight() +3)/4)*16, 
				image.getDataBuffer());
		}
		else
		{
			destroy();
			return false; // TODO: log
		} 
		break;

	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		if(GLEE_ARB_texture_compression &&
			GLEE_EXT_texture_compression_s3tc)
		{

			if(GLEE_SGIS_generate_mipmap && mipmap)
			{
				glTexParameteri(quickFix, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(quickFix, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2DARB(target,
				0,
				GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 
				image.getWidth(), image.getHeight(), 
				0, 
				((image.getWidth() +3)/4)*((image.getHeight() +3)/4)*16,
				image.getDataBuffer());
		}
		else
		{
			destroy();
			return Logger::writeErrorLog("GL_EXT_texture_compression_s3tc not supported");
		} 
		break;

	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
		if(GLEE_ARB_texture_compression &&
			GLEE_EXT_texture_compression_s3tc)
		{
			if(GLEE_SGIS_generate_mipmap && mipmap)
			{
				glTexParameteri(quickFix, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(quickFix, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2DARB(target, 
				0,
				GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 
				image.getWidth(), image.getHeight(), 
				0,
				((image.getWidth() +3)/4)*((image.getHeight() +3)/4)*8, 
				image.getDataBuffer());
		}
		else
		{
			destroy();
			return Logger::writeErrorLog("GL_EXT_texture_compression_s3tc not supported");
		} 
		break;

	default: 
		if(mipmap)
			gluBuild2DMipmaps(target,
			image.getInternalFormat(),
			image.getWidth(),
			image.getHeight(),
			image.getFormat(),
			GL_UNSIGNED_BYTE,
			image.getDataBuffer());
		else
			glTexImage2D(target, 0,  image.getInternalFormat(), image.getWidth(), 
			image.getHeight(), 0, image.getFormat(), 
			GL_UNSIGNED_BYTE, image.getDataBuffer());

	}
*/
	return true;
}
