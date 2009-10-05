/*
*	Type2Type.hpp
*
*	A template that maps a type to a type
*
*  Created by Julien Lebot on 23/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Type2Type_hpp__
#define __Type2Type_hpp__

namespace Phoenix
{
	template <typename T>
	struct Type2Type
	{
		typedef T OriginalType;
	};
}

#endif // Type2Type