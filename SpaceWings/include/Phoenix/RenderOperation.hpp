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
#include <Phoenix/Material.hpp>

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
		std::vector<int> mtlOffsets;				//added by Terje
		std::vector<std::string> mtlOffsetNames;	//added by Terje
		std::vector<Phoenix::Material> mtlList;		//added by Terje
		
		VertexData *vertexData;
		IndexData *indexData;
		PrimitiveType primitiveType;
		bool indexed;
	};
}

#endif // __RenderOperation_hpp__