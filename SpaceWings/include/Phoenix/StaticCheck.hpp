/*
*	StaticCheck.hpp
*
*	A compile time assertion mechanism based on Andrei Alexandrescu's Loki's LOKI_STATIC_CHECK.
*
*  Created by Julien Lebot on 08/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __StaticCheck_hpp__
#define __StaticCheck_hpp__

namespace Phoenix
{
	template<int> struct CompileTimeError;
	template<> struct CompileTimeError<true> {};
}

#define PHOENIX_STATIC_CHECK(expr, msg) \
{ Phoenix::CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; }

#endif // __StaticCheck_hpp__