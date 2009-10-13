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

Texture::Texture(const ResourceManager* const creator,
				 const std::string& name,
				 const ResourceHandleType handle,
				 TextureTarget target)
: Resource(creator, name, handle),
  mHeight(128),
  mWidth(128),
  mDepth(64),
  mTarget(target),
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
/*
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
*/
void Texture::create(const std::string& name,
			GLuint width,
			GLuint height,
			GLuint depth,
			GLuint internalFormat,
			GLuint format)
{
	prepareImpl();

	switch (internalFormat)
	{
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		switch(mTarget)
		{
		case TEXTURE_1D:
			glCompressedTexImage1DARB(GL_TEXTURE_1D, 0, format, 
									  width, 0,
									  (internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ?
									  ((width +3)/4)*((height +3)/4)*8 :
									  ((width +3)/4)*((height +3)/4)*16, NULL);
			break;
		case TEXTURE_2D:
			glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, format,
									  width, height, 0, 
									  (internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ?
									  ((width +3)/4)*((height +3)/4)*8 :
									  ((width +3)/4)*((height +3)/4)*16, NULL);
			break;
		case TEXTURE_3D:
			glCompressedTexImage3DARB(GL_TEXTURE_3D, 0, format,
									  width, height, depth, 0, 
									  (internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ?
									  ((width +3)/4)*((height +3)/4)*8 :
									  ((width +3)/4)*((height +3)/4)*16, NULL);
			break;
		case TEXTURE_CUBE_MAP:
			for(int face=0; face<6; face++)
			{
				glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format,
										  width, height, 0, 
										  (internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ?
										  ((width +3)/4)*((height +3)/4)*8 :
										  ((width +3)/4)*((height +3)/4)*16, NULL);
			}
		break;
		};
	break;
	default:
		switch(mTarget)
		{
		case TEXTURE_1D:
			glTexImage1D(GL_TEXTURE_1D, 0, format,
						 width, 0, 
						 GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			break;
		case TEXTURE_2D:
			glTexImage2D(GL_TEXTURE_2D, 0, format,
						 width, height, 0, 
						 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			break;
		case TEXTURE_3D:
			glTexImage3D(GL_TEXTURE_3D, 0, format,
						 width, height, depth, 0, 
						 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			break;
		case TEXTURE_CUBE_MAP:
			for(int face=0; face<6; face++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format,
							 width, height, 0, 
							 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			}
		break;
		};
	break;
	}

	// update class state at last
	mWidth = width;
	mHeight = height;
	mDepth = depth;
	mInternalFormat = internalFormat;
	mFormat = format;
}

void Texture::bind(const int textureUnit)
{
  if(!mID)
    return;

  if(textureUnit > -1)
    glActiveTextureARB(GL_TEXTURE0_ARB + textureUnit);

  glBindTexture(GL_TEXTURE_2D, mID);

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

Texture::~Texture()
{
  unloadImpl();
}

void Texture::prepareImpl()
{
	glGenTextures(1, &mID);
	glBindTexture(getGLTextureTarget(), mID);

	// Set some default values, those can be changed later on during rendering
	// The reason for this is that it allows on-the-fly changes of the texture filtering and makes it easier for the
	// renderer to apply texture techniques. J.L.
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_S,     GL_REPEAT);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_T,     GL_REPEAT);
	glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_R,     GL_REPEAT);
}

void Texture::loadImpl(const ImagePtr image, const GLuint target)
{
	switch(image->getInternalFormat()) 
	{
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		if(GLEE_ARB_texture_compression &&
			GLEE_EXT_texture_compression_s3tc)
		{

			if(GLEE_SGIS_generate_mipmap && mMipmaps)
			{
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2DARB(target,
				0,
				image->getInternalFormat(),
				image->getWidth(), image->getHeight(),
				0,
				((image->getWidth() +3)/4)*((image->getHeight() +3)/4)*16,
				image->getDataBuffer());
		}
		else
		{
			unloadImpl();
		} 
		break;

	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
		if(GLEE_ARB_texture_compression &&
			GLEE_EXT_texture_compression_s3tc)
		{
			if(GLEE_SGIS_generate_mipmap && mMipmaps)
			{
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			}

			glCompressedTexImage2DARB(target, 
				0,
				GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
				image->getWidth(), image->getHeight(),
				0,
				((image->getWidth() +3)/4)*((image->getHeight() +3)/4)*8,
				image->getDataBuffer());
		}
		else
		{
			unloadImpl();
		} 
		break;

	default:
		if(mMipmaps)
		{
			if (mMipmapsHardwareGenerated)
			{
				// Generates mipmaps in hw
				if (glGenerateMipmapEXT)
					glGenerateMipmapEXT(target);
				else
					glTexParameteri(target, GL_GENERATE_MIPMAP, GL_TRUE);
			}
			else
			{
				// Generates mipmaps in sw
				gluBuild2DMipmaps(target,
					image->getInternalFormat(),
					image->getWidth(),
					image->getHeight(),
					image->getFormat(),
					GL_UNSIGNED_BYTE,
					image->getDataBuffer());
			}
		}
		else if (mMipmapsHardwareGenerated && !glGenerateMipmapEXT)
		{
			// Disable mipmap generation in hw
			glTexParameteri(target, GL_GENERATE_MIPMAP, GL_FALSE);
		}

		glTexImage2D(target,
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

void Texture::loadImpl()
{
	if(!mID)
		return;

	// How do we do TEXTURE_3D ?? J.L.
	if (mTarget == TEXTURE_CUBE_MAP)
	{
		std::string baseName, extension, filePath = getFilePath();
		std::size_t pos = filePath.find_last_of(".");
		baseName = filePath.substr(0, pos);
		if(pos != std::string::npos)
			extension = filePath.substr(pos+1);

		static const std::string facesSuffixes[6] = { "PX", "NX", "PY", "NY", "PZ", "NZ"};
		std::vector<ImagePtr> images(6);

		for (std::size_t i = 0; i < 6; ++i)
		{
			std::string finalName = baseName + facesSuffixes[i];
			if (!extension.empty())
				finalName += ("." + extension);
			images[i] = ImagePtr(new Image(finalName));
			images[i]->load();

			loadImpl(images[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i);
		}

		mHeight = images[5]->getHeight();
		mWidth  = images[5]->getWidth();
		mDepth  = images[5]->getDepth();
	}
	else
	{
		ImagePtr image = ImagePtr(new Image(getFilePath()));

		assert(checkGLErrors());
		assert(image->load());
		assert(checkGLErrors());

		loadImpl(image, getGLTextureTarget());

		mHeight = image->getHeight();
		mWidth  = image->getWidth();
		mDepth  = image->getDepth();
	}
}

void Texture::unloadImpl()
{
	glDeleteTextures(1, &mID);
}