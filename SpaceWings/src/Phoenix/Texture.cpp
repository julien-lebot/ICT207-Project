#include <Phoenix/Texture.hpp>

using namespace Phoenix;

GLenum Texture::getGLTextureTarget() const
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

Texture::Texture(TextureTarget target)
: mHeight(128),
  mWidth(128),
  mDepth(64),
  mTarget(target),
  mAniso(0.0f),
  mID(0),
  mCurrtUnit(-1)
{
}

Texture::Texture(const Texture & texture)
{
  this->operator=(texture);
}

Texture &Texture::operator= (const Texture & copy)
{
  if(this != &copy)
  {
		// Nothing yet, TODO
  }
  return *this;
}

void Texture::load(const std::string& path, bool mipmap)
{
	assert(checkGLErrors());
	ImagePtr image = ImagePtr(new Image(path));
	assert(checkGLErrors());
	load(image, mipmap);
}

void Texture::load(const ImagePtr image, bool mipmap)
{
	assert(checkGLErrors());
	glGenTextures(1, &mID);
	glBindTexture(getGLTextureTarget(), mID);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_S,     GL_REPEAT);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_T,     GL_REPEAT);
	assert(checkGLErrors());
	loadImpl(image, getGLTextureTarget(), mipmap);
	assert(checkGLErrors());
	mHeight = image->getHeight();
	mWidth  = image->getWidth();
	mDepth  = image->getDepth();
	assert(checkGLErrors());
}

void Texture::load(const ImageList images, bool mipmap)
{
	glGenTextures(1, &mID);
	glBindTexture(getGLTextureTarget(), mID);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_S,     GL_REPEAT);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_T,     GL_REPEAT);

	int i = 0;
	for (ImageList::const_iterator cit = images.begin(); cit != images.end(); ++cit)
	{
		loadImpl(*cit, GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, mipmap);

		if(i == 5)
		{
			mHeight = (*cit)->getHeight();
			mWidth  = (*cit)->getWidth();
			mDepth  = 1;
		}
		i++;
	}
}

void Texture::create(const std::string& name,
			GLuint width,
			GLuint internalFormat,
			GLuint format)
{

}

void Texture::create(const std::string& name,
			GLuint width,
			GLuint height,
			GLuint internalFormat,
			GLuint format)
{

}

void Texture::create(const std::string& name,
			GLuint width,
			GLuint height,
			GLuint depth,
			GLuint internalFormat,
			GLuint format)
{

}

void Texture::bind(const int textureUnit)
{
  if(!mID)
    return;

  if(textureUnit > -1)
    glActiveTextureARB(GL_TEXTURE0_ARB + textureUnit);

  glBindTexture(mTarget, mID);

  mCurrtUnit = textureUnit;
}

void Texture::unbind()
{
  if(mID > 0 )
  {
    if(mCurrtUnit > -1)
      glActiveTextureARB(GL_TEXTURE0_ARB + mCurrtUnit);

    glDisable(mTarget);
  }
}

const GLuint Texture::getID() const 
{ 
  return mID;
}

const GLuint  Texture::getTarget() const { return mTarget; }

const GLuint Texture::getHeight() const { return mHeight;  }
const GLuint Texture::getWidth()  const { return mWidth;   }
const GLuint Texture::getDepth()  const { return mDepth;   } 

int Texture::getValidWrapMode(int clamp)
{
  return (clamp == GL_CLAMP          ) ? GL_CLAMP           :
         (clamp == GL_REPEAT         ) ? GL_REPEAT          :
         (clamp == GL_CLAMP_TO_EDGE  ) ? GL_CLAMP_TO_EDGE   :
         (clamp == GL_CLAMP_TO_BORDER) ? GL_CLAMP_TO_BORDER : GL_REPEAT;
}

int Texture::getValidMagFilter(int filter)
{
  return (filter == GL_NEAREST ) ? GL_NEAREST  : GL_LINEAR;
}

int Texture::getValidMinFilter(int filter)
{
  return (filter == GL_NEAREST                ) ? GL_NEAREST                :
         (filter == GL_LINEAR_MIPMAP_LINEAR   ) ? GL_LINEAR_MIPMAP_LINEAR   :
         (filter == GL_NEAREST_MIPMAP_LINEAR  ) ? GL_NEAREST_MIPMAP_LINEAR  :
         (filter == GL_LINEAR_MIPMAP_NEAREST  ) ? GL_LINEAR_MIPMAP_NEAREST  :
         (filter == GL_NEAREST_MIPMAP_NEAREST ) ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR;
}

GLuint Texture::getAnisoLevel()
{
  return mAniso;
}

void Texture::destroy()
{
    glDeleteTextures(1, &mID);
}

Texture::~Texture()
{
  destroy();
}

void Texture::loadImpl(const ImagePtr image, GLuint target, bool mipmap)
{
	if(!mID)
		return;
	assert(checkGLErrors());
	assert(image->load());
	assert(checkGLErrors());
	switch(image->getInternalFormat()) 
	{
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		if(GLEE_ARB_texture_compression && GLEE_EXT_texture_compression_s3tc)
		{
			if(GLEE_SGIS_generate_mipmap && mipmap)
			{
				glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(getGLTextureTarget(), GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2DARB(getGLTextureTarget(),
									  0,
									  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
									  image->getWidth(), image->getHeight(),
									  0,
									  ((image->getWidth() +3)/4)*((image->getHeight() +3)/4)*16,
									  image->getDataBuffer());
		}
		else
		{
			destroy();
		} 
		break;

	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		if(GLEE_ARB_texture_compression &&
			GLEE_EXT_texture_compression_s3tc)
		{

			if(GLEE_SGIS_generate_mipmap && mipmap)
			{
				glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(getGLTextureTarget(), GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2DARB(getGLTextureTarget(),
									  0,
									  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
									  image->getWidth(), image->getHeight(),
									  0,
									  ((image->getWidth() +3)/4)*((image->getHeight() +3)/4)*16,
									  image->getDataBuffer());
		}
		else
		{
			destroy();
		} 
		break;

	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
		if(GLEE_ARB_texture_compression &&
			GLEE_EXT_texture_compression_s3tc)
		{
			if(GLEE_SGIS_generate_mipmap && mipmap)
			{
				glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(getGLTextureTarget(), GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2DARB(getGLTextureTarget(), 
									  0,
									  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 
									  image->getWidth(), image->getHeight(), 
									  0,
									  ((image->getWidth() +3)/4)*((image->getHeight() +3)/4)*8, 
									  image->getDataBuffer());
		}
		else
		{
			destroy();
		} 
		break;

	default:
		if(mipmap)
		{
			if (glGenerateMipmapEXT)
				glGenerateMipmapEXT(getGLTextureTarget());
			else
				glTexParameteri(getGLTextureTarget(), GL_GENERATE_MIPMAP, GL_TRUE);
			/* TODO: Add param for software generated mipmaps
			gluBuild2DMipmaps(getGLTextureTarget(),
							  image->getInternalFormat(),
							  image->getWidth(),
							  image->getHeight(),
							  image->getFormat(),
							  GL_UNSIGNED_BYTE,
							  image->getDataBuffer());
							  */
		}
		else if (!glGenerateMipmapEXT)
		{
			glTexParameteri(getGLTextureTarget(), GL_GENERATE_MIPMAP, GL_FALSE);
		}

		glTexImage2D(getGLTextureTarget(),
					 0,
					 image->getInternalFormat(),
					 image->getWidth(), 
					 image->getHeight(),
					 0,
					 image->getFormat(), 
					 GL_UNSIGNED_BYTE,
					 image->getDataBuffer());
		assert(checkGLErrors());
	}
}