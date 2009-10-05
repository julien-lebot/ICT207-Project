/*
*	Image.hpp
*
*	A class representing an image.
*	Image class code borrowed from http://developer.nvidia.com/object/nature_scene.html
*
*  Modified by Julien Lebot on 11/09/09.
*  Copyright 2005 Nvidia Corp. All rights reserved.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Image_hpp__
#define __Image_hpp__

#include <Phoenix/Base.h>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace Phoenix
{
	class Image
	{
	private:
		boost::filesystem::path		 mPath;
		GLubyte						*mBuffer;
		GLuint						 mInternalFormat,
									 mComponents,
									 mFormat;
		GLushort					 mHeight,
									 mWidth,
									 mDepth;
	public:
		Image(const std::string &path = "");
		Image(const Image &image);
		Image &operator=(const Image &image);
		~Image();

		void setPath(const std::string &path);
		std::string getPath() const { return mPath.file_string(); }

		void setDataBuffer(const GLubyte* );

		bool load();

		const GLuint   getComponentsCount() const;
		const GLuint   getInternalFormat()  const;
		const GLubyte* getDataBuffer()      const;
		const GLuint   getFormat()          const;

		const GLushort getHeight() const;
		const GLushort getWidth()  const;
		const GLushort getDepth()  const;

	private:
		bool loadDDS();

		bool loadTGA();
		bool loadUncompressed8BitTGA();
		bool loadCompressedTrueColorTGA();
		bool loadUncompressedTrueColorTGA();

		void setInternalFormat(GLuint);
		void setComponentsCount(GLuint);

		void setFormat(GLuint);
		void setHeight(unsigned short height){mHeight = height;}
		void setWidth(unsigned short width){mWidth = width;}
		void setDepth(unsigned short depth){mDepth = depth;}
	};

	typedef tr1::shared_ptr<Image> ImagePtr;
	typedef std::vector<ImagePtr> ImageList;
}

#endif // __Image_hpp__