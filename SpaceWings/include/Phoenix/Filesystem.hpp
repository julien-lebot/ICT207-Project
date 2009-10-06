/*
*	Filesystem.hpp
*
*	"The Boost.Filesystem library provides portable facilities to query and manipulate paths, files, and directories.
*
*	The motivation for the library is the need to perform portable script-like operations from within C++ programs.
*	The intent is not to compete with Python, Perl, or shell languages, but rather to provide portable filesystem
*	operations when C++ is already the language of choice. The design encourages, but does not require, safe and portable usage.
*
*	Programs using the library are portable, both in the sense that the syntax of program code is portable,
*	and the sense that the semantics or behavior of code is portable. The generic path grammar is another important aid to portability.
*
*	Usage is safe in the sense that errors cannot be ignored since most functions throw C++ exceptions when errors are detected.
*	This is also convenient for users because it alleviates the need to explicitly check error return codes.
*
*	A proposal, N1975, to include Boost.Filesystem in Technical Report 2 has been accepted by the C++ Standards Committee.
*	The Boost.Filesystem library will stay in alignment with the TR2 Filesystem proposal as it works its way through the TR2 process.
*	Note, however, that namespaces and header granularity differs between Boost.Filesystem and the TR2 proposal."
*
*	http://www.boost.org/doc/libs/1_40_0/libs/filesystem/doc/index.htm
*
*  Created by Julien Lebot on 06/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Filesystem_hpp__
#define __Filesystem_hpp__

#include <Phoenix/Base.h>

#if defined(_USE_BOOST_)
#include <boost/filesystem.hpp>
namespace Phoenix { namespace tr2 = boost; }
#endif

#endif // __Filesystem_hpp__