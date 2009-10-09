/*
*	HarwareBufferData.hpp
*
*	Behind that mouthful name, are 2 classes that are the highest level of abstraction of the data contained in a hardware buffer.
*	Those 2 classes encapsulate all the necessary data for a rendering operation.
*
*  Created by Julien Lebot on 07/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __HarwareBufferData_hpp__
#define __HarwareBufferData_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/BufferElementGroup.hpp>
#include <Phoenix/HardwareBuffer.hpp>
#include <map>

namespace Phoenix
{
	struct VertexData
	{
		typedef std::map<unsigned short, HardwareBufferPtr> HardwareBufferMap;

		std::size_t start, /// Start is an offset to where the vertices start
					count; /// Count is how many vertices we have

		/// A group of elementary data
		BufferElementGroup bufferElementGroup;

		/// Binds the source ID of the elements with an actual hardware buffer. This is because multiple elements can share the same buffer.
		HardwareBufferMap bufferBinding;
	};

	struct IndexData
	{
		std::size_t start,	/// Start is an offset to where the index starts
					count;	/// Count is how many indices we have

		/// Shared pointer to the index buffer
		HardwareBufferPtr indexBuffer;
	};
}

#endif // __HarwareBufferData_hpp__