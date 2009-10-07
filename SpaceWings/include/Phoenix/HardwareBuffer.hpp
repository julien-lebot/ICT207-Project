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
	/**
	 * An abstract class representing a buffer of data that is not stored in the main RAM, but for example, stored in video RAM on the GPU.
	 * This class may become more generic in the future so expect modifications. Also it is planned to have a shadow copy of the data in RAM
	 * for faster updates if the need arises.
	 */
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

		/// The target of the buffer, either a vertex buffer or an index buffer.
		enum Target
		{
			VERTEX,
			INDEX
		};

		/**
		 * @brief Constructs a hardware buffer to store data.
		 * @param size The size to allocate in that buffer for the data.
		 * @param usage The usage of the buffer. Very important, the driver uses that to optimize performance.
		 * @param target The target of the buffer.
		 */
		HardwareBuffer(const std::size_t size,
					   const Usage usage,
					   const Target target);

		/// Virtual destructor as this class should be sub-classed (it is not generic enough atm).
		~HardwareBuffer();

		/// Binds this buffer for rendering
		void bind();

		/// Unbind this buffer
		void unbind();

		/// Returns the OpenGL ID of that buffer.
		GLuint getID();

		/**
		 * @brief Uploads data to the GPU. The size of the data MUST match the size provided in the constructor.
		 * @param data The data to send to the GPU.
		 */
		void upload(const void* data);

		/**
		 * @brief Upload some data to the GPU. The size of the data MUST be <= to the size provided in the constructor.
		 * @param offset An offset in the buffer where the data will be stored.
		 * @param size The size of the data to upload.
		 * @param data The data to send to the GPU.
		 */
		void upload(int offset, std::size_t size, const void* data);

		/**
		 * @brief Maps this buffer into main memory.
		 * @param access Describes how this buffer will be used. Very important for driver optimization.
		 */
		void * map(const Access access);

		/// Unmaps this buffer. DO NOT FORGET TO CALL THIS AFTER MAPPING THE BUFFER.
		void unmap();

		/// Returns the size of the buffer.
		std::size_t getSize() { return mSize; }
	protected:
		/// OpenGL specific ID.
		GLuint mID;

		/// The size of the buffer.
		std::size_t mSize;

		/// What usage are we going to do with this buffer ?
		Usage mUsage;

		/// The target of this buffer for
		Target mTarget;
	};

	typedef tr1::shared_ptr<HardwareBuffer> HardwareBufferPtr;
}

#endif // __HardwareBuffer_hpp__