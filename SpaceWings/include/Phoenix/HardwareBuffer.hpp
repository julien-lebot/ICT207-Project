/*
*	HardwareBuffer.hpp
*
*	An abstract class representing a buffer of data that is not stored in the main RAM,
*	but for example, stored in video RAM on the GPU.
*
*  Created by Julien Lebot on 11/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __HardwareBuffer_hpp__
#define __HardwareBuffer_hpp__

#include <Phoenix/Base.h>

namespace Phoenix
{
	class _PhoenixExport HardwareBuffer
	{
	public:
		/// Usage pattern for the hardware buffer.
		enum Usage
		{
			STREAM_DRAW,
			STREAM_READ,
			STREAM_COPY,
			STATIC_DRAW,
			STATIC_READ,
			STATIC_COPY,
			DYNAMIC_DRAW,
			DYNAMIC_READ,
			DYNAMIC_COPY
		};

		/// Access type for the harware buffer when mapped in main RAM.
		enum Access
		{
			READ_ONLY,
			WRITE_ONLY,
			READ_WRITE
		};

		enum Target
		{
			VERTEX,
			INDEX
		};

		HardwareBuffer(const std::size_t size,
					   const Usage usage,
					   const Target target);
		~HardwareBuffer();
		void bind();
		void unbind();
		GLuint getID();
		void upload(const void* data);
		void upload(int offset, std::size_t size, const void* data);
		void * map(const Access access);
		void unmap();
		std::size_t getSize() { return mSize; }
	protected:
		GLuint mID;
		std::size_t mSize;
		Usage mUsage;
		Target mTarget;
	};
}

#endif // __HardwareBuffer_hpp__