/*
*	Color.hpp
*
*	OpenGL color class
*
*  Created by Julien Lebot on 08/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Color_hpp__
#define __Color_hpp__

#include <Phoenix/Base.h>
#include <GL/gl.h>

namespace Phoenix
{
	struct Color
	{
	public:
		static const Color zero;
		static const Color black;
		static const Color white;
		static const Color red;
		static const Color green;
		static const Color blue;

		explicit Color( float red = 1.0f,
						float green = 1.0f,
						float blue = 1.0f,
						float alpha = 1.0f )
			: r(red), g(green), b(blue), a(alpha)
		{ }

		bool operator==(const Color& rhs) const;
		bool operator!=(const Color& rhs) const;

		float r,g,b,a;
	};
}

#endif // __Color_hpp__