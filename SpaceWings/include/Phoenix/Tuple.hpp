/*
*	Tuple.hpp
*
*	A wrapper around boost::tuple or tr1::tuple
*
*  Created by Julien Lebot on 24/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Tuple_hpp__
#define __Tuple_hpp__

#include <Phoenix/Base.h>

#if defined(_USE_BOOST_)
#include <boost/tuple/tuple.hpp>
namespace Phoenix { namespace tr1 = boost; }
#else
#if _PLATFORM_ == _PLATFORM_WIN32_
#include <tuple>
#else
#include <tr1/tuple>
#endif
namespace Phoenix { namespace tr1 = std::tr1; }
#endif

#endif // __Tuple_hpp__