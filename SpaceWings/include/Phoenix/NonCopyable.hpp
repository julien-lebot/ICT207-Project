/*
*	NonCopyable.hpp
*
*	A root type that disables copy.
*
*  Created by Julien Lebot on 17/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __NonCopyable_hpp__
#define __NonCopyable_hpp__

#include <Phoenix/Base.h>

namespace Phoenix
{
	class _PhoenixExport NonCopyable
	{
	protected:
		NonCopyable() {}
		~NonCopyable() {}

	private:
		NonCopyable(const NonCopyable& copy);
		const NonCopyable& operator=(const NonCopyable& copy);
	};
}

#endif // __NonCopyable_hpp__