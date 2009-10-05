/*
*	Image.cpp
*
*	A class representing an image.
*	Image class code borrowed from http://developer.nvidia.com/object/nature_scene.html
*
*  Modified by Julien Lebot on 11/09/09.
*  Copyright 2005 Nvidia Corp. All rights reserved.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Image.hpp>

#define deleteObject(A){ if(A){ delete   A; A = NULL; } }
#define deleteArray(A) { if(A){ delete[] A; A = NULL; } }

using namespace Phoenix;

Image::Image(const std::string &path)
: mPath(path),
  mBuffer(NULL),
  mInternalFormat(0),
  mComponents(0),
  mFormat(0),
  mHeight(0),
  mWidth(0),
  mDepth(0)
{

}

Image::Image(const Image &image)
: mPath(image.mPath),
  mInternalFormat(image.mInternalFormat),
  mComponents(image.mComponents),
  mFormat(image.mFormat),
  mHeight(image.mHeight),
  mWidth(image.mWidth),
  mDepth(image.mDepth)
{
	setDataBuffer(image.mBuffer);
}

Image &Image::operator=(const Image &image)
{
	mPath = image.mPath;
	mInternalFormat = image.mInternalFormat;
	mComponents = image.mComponents;
	mFormat = image.mFormat;
	mHeight = image.mHeight;
	mWidth = image.mWidth;
	mDepth = image.mDepth;
	setDataBuffer(image.mBuffer);

	return *this;
}

void Image::setPath(const std::string &path)
{
	mPath = path;
}

static bool findString(const char *source, const char *target){
	if(source){
		const char *str = source;
		int len = int(strlen(target));
		while ((str = strstr(str, target))){
			str += len;
			if (*str == ' ' || *str == '\0')
				return true;
		}
	}
	return false;
}

static bool isJPEG(const char* filename){
	return findString(filename, "jpg" ) ||
		findString(filename, "JPG" ) ||
		findString(filename, "jpeg") ||
		findString(filename, "JPEG");
}

static bool isTGA(const char* filename){
	return findString(filename, "tga"  )||
		findString(filename, "TGA"  )||
		findString(filename, "targa")||
		findString(filename, "TARGA");
}

static bool isPNG(const char* filename){
	return findString(filename, "png"  )||
		findString(filename, "PNG"  );
}

static bool isDDS(const char* filename){
	return findString(filename, "dds"  )||
		findString(filename, "DDS"  );
}

bool Image::load()
{
	if(isTGA(mPath.file_string().c_str()))
		return loadTGA();

	if(isDDS(mPath.file_string().c_str()))
		return loadDDS();

	return false;
}

const GLuint   Image::getComponentsCount() const { return mComponents;      }
const GLuint   Image::getInternalFormat()  const { return mInternalFormat; }
const GLubyte* Image::getDataBuffer()      const { return mBuffer;		   }
const GLuint   Image::getFormat()          const { return mFormat;		   }

const GLushort Image::getHeight() const {return mHeight;}
const GLushort Image::getWidth()  const {return mWidth; }
const GLushort Image::getDepth()  const {return mDepth; }

Image::~Image()
{
	if(mBuffer)
	{
		deleteArray(mBuffer);
	}
}

void Image::setDataBuffer(const GLubyte *buffer)
{
	if(!mBuffer || !mWidth || !mHeight || !mComponents)
		return;

	deleteArray(mBuffer);

	int size = (mInternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? ((mWidth+3)/4)*((mHeight+3)/4)*8 : 
			  ((mInternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) ||
			   (mInternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)) ? ((mWidth+3)/4)*((mHeight+3)/4)*16 :
																		mWidth*mHeight*mComponents;
	mBuffer = new GLubyte[size];
	memcpy(mBuffer, buffer, size);
}

void Image::setFormat(GLuint f)
{ 
	mFormat = f; 
}

void Image::setInternalFormat(GLuint iformat)
{
	mInternalFormat = iformat;
}

void Image::setComponentsCount(GLuint c)
{
	mComponents = c < 1 ? 1 : c > 4 ? 4 : c; 
}