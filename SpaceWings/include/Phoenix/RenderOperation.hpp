/*
*	RenderOperation.hpp
*
*	A class representing a render operation: e.g. an operation that draws something on the screen.
*
*  Created by Julien Lebot on 07/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __RenderOperation_hpp__
#define __RenderOperation_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/HarwareBufferData.hpp>

namespace Phoenix
{
	struct RenderOperation
	{
		enum PrimitiveType
		{
			POINTS,
			LINES,
			LINE_STRIP,
			TRIANGLES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN
		};

		VertexData *vertexData;
		IndexData *indexData;
		PrimitiveType primitiveType;
		bool indexed;
	};
}

#endif // __RenderOperation_hpp__