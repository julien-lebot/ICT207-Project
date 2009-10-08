/*
 *	Base.h
 *
 *	Header to be included by all project files.
 *	Can be used as a precompiled header. Contains
 *	lots of project specific macros.
 *
 *  Created by Julien Lebot on 19/08/09.
 *  Copyright 2009 Julien Lebot. All rights reserved.
 *
 */

#ifndef __Base_h__
#define __Base_h__

/**
 * Multi threading:
 * ===============
 * To enable multi threading define _THREAD_SUPPORT
 * This may pull headers, define macros and instantiate object code that will allow the program to run in a multi threaded environment.
 * It depends heavily on Boost libraries and it has been tested with Boost 1.36.
 *
 * Assembly / SSE:
 * ==============
 * To enable the use of intrinsic assembly in the math classes define USE_INTRINSIC_ASM
 * To use hand coded SSE wherever possible in the assembly code define USE_SSE
 * Note that this will take precedence on intrinsic assembly. Both can be defined at the same time.
 *
 * Memory Manager:
 * ==============
 * To disable Paul Nettle's memory manager define _NO_MEMORY_MANAGER_
 * This will work only in release mode because in debug mode the memory manager is replaced by the CRT debug heap anyway (This is Windows specific).
 *
 * Boost libraries:
 * ==============
 * Lots of part of the code depends on some Boost library; the most notable ones are shared_ptr, tuple, fusion, any, filesystem, function, signals2.
 * The use of Boost is controlled by the macro _USE_BOOST_.
 * Note that when Boost is disabled the code will expect some form of compensation for the missing features:
 * For example shared_ptr will automagically switch to TR1's shared_ptr, but you still need to have a compiler that supports it.
 * However some features can hardly be replaced and will always depend on Boost, like filesystem (even though it's scheduled for inclusion in TR2,
 * but TR2 is still in the drawing board stage !).
 * This in general should not be a problem since most of Boost libraries (like signals2) are headers only and take nearly no space at all.
 * In all the other case, precompiled libraries are provided. They are based on Boost 1.36.
 *
 * Other dependencies:
 * ==================
 * The C/C++ standard libraries, the STL, GLee, GLUT, GLU, OpenGL.
 */

/// Platform definitions
#include <Phoenix/CorePlatform.h>

#if _PLATFORM_ == _PLATFORM_WIN32_
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <cstdio>
	#include <cstdlib>
	#if 0	// Not using GLEW anymore it messed with me. J.L.
		#include <GL/glew.h>
		#include <GL/wglew.h>
	#else
		#include <GL/GLee.h>
	#endif
	#include <GL/glu.h>
	#include <GL/glut.h>
#elif _PLATFORM_ == _PLATFORM_LINUX_
	#include <GL/glew.h>
	#include <GL/glu.h>
	#define GL_GLEXT_PROTOTYPES
#elif _PLATFORM_ == _PLATFORM_APPLE_
	#include <GL/glew.h>
	#include <OpenGL/glu.h>
#endif

/// This is here mainly because it's used all over the place and we want to save the hassle of
/// having to include it manually every time.
/// For the other TR1 classes, there should be a separate header file.
#if defined(__cplusplus)
	#if defined(_USE_BOOST_)
		#include <boost/shared_ptr.hpp>
		namespace Phoenix { namespace tr1 = boost; }
	#else
		#if _PLATFORM_ == _PLATFORM_WIN32_
			#include <memory>
		#else
			#include <tr1/memory>
		#endif
		namespace Phoenix { namespace tr1 = std::tr1; }
	#endif

	#ifdef _DEBUG
		// Memory manager conflicts with CRT's debug heap
		#define _NO_MEMORY_MANAGER_
	#endif
	#if !defined(_NO_MEMORY_MANAGER_)
		// Standard includes MUST come first
		#if defined(__cplusplus)
			#include <cstdio>
			#include <cstdlib>
		#else
			#include <stdio.h>
			#include <stdlib.h>
		#endif
		#include <Phoenix/MemoryManager/mmgr.h>			// Paul Nettle's memory manager
	#endif
#endif

#if !defined(_EXTERN_C_BEGIN_)
	#if defined(__cplusplus)
		#define _EXTERN_C_BEGIN_ extern "C" {
		#define _EXTERN_C_END_   }
	#else
		#define _EXTERN_C_BEGIN_
		#define _EXTERN_C_END_
	#endif
#endif

_EXTERN_C_BEGIN_

#if !defined(NULL)
	#if _PLATFORM_ == _PLATFORM_LINUX_
		#define NULL __null
	#elif defined(__cplusplus)
		#define NULL 0
	#else
		#define NULL ((void *)0)
	#endif
#endif

#if !defined(__cplusplus)
	#if !defined(TRUE)
		#define TRUE	1
	#endif
	#if !defined(FALSE)
		#define FALSE	0
	#endif
#endif

/// This truly is taken from Ogre, I found it very helpful in making the different targets (DLL, static libs etc...)
#if _PLATFORM_ == _PLATFORM_WIN32_
#	if defined(PHOENIX_STATIC_LIB)
#   	define _PhoenixExport
#   	define _PhoenixPrivate
#   else
#   	if defined(PHOENIX_NONCLIENT_BUILD)
#       	define _PhoenixExport __declspec(dllexport)
#   	else
#           if defined(__MINGW32__)
#               define _PhoenixExport
#           else
#       	    define _PhoenixExport __declspec(dllimport)
#           endif
#   	endif
#   	define _PhoenixPrivate
#	endif
#endif

#if _PLATFORM_ == _PLATFORM_APPLE_ || _PLATFORM_ == _PLATFORM_LINUX_
#    define _PhoenixExport  __attribute__ ((visibility("default")))
#    define _PhoenixPrivate __attribute__ ((visibility("hidden")))
#endif

#if !defined(_INLINE)
	#if _COMPILER_ == _COMPILER_GNUC_ && (__GNUC__ == 4) && !defined(_DEBUG)
		#define _INLINE __inline__ __attribute__((always_inline))
	#elif _COMPILER_ == _COMPILER_GNUC_
		#define _INLINE __inline__
	#elif defined(__MWERKS__)/* || defined(__cplusplus)*/
		#define _INLINE inline
	#elif _COMPILER_ == _COMPILER_MSVC_
		#define _INLINE __inline
	#elif _PLATFORM_ == _PLATFORM_WIN32_
		#define _INLINE __inline__
	#endif
#endif

#if !defined(_MEM_ALIGN_VS) || !defined(_MEM_ALIGN_GCC)
	#if _COMPILER_ == _COMPILER_MSVC_
		#define _MEM_ALIGN_VS(bytes) __declspec(align(bytes))
		#define _MEM_ALIGN_GCC(bytes)
	#elif _COMPILER_ == _COMPILER_GNUC_
		#define _MEM_ALIGN_VS(bytes)
		#define _MEM_ALIGN_GCC(bytes) __attribute__ ((aligned(bytes)))
	#else
		#define _MEM_ALIGN_VS(bytes)
		#define _MEM_ALIGN_GCC(bytes)
	#endif
#endif

/// Useful threading defines taken from Ogre
#define _AUTO_MUTEX_NAME mutex
#if defined(_THREAD_SUPPORT)
	#define _AUTO_MUTEX mutable boost::recursive_mutex _AUTO_MUTEX_NAME
	#define _LOCK_AUTO_MUTEX boost::recursive_mutex::scoped_lock autoMutexLock(_AUTO_MUTEX_NAME)
	#define _MUTEX(name) mutable boost::recursive_mutex name
	#define _STATIC_MUTEX(name) static boost::recursive_mutex name
	#define _STATIC_MUTEX_INSTANCE(name) boost::recursive_mutex name
	#define _LOCK_MUTEX(name) boost::recursive_mutex::scoped_lock nameLock(name)
	#define _LOCK_MUTEX_NAMED(mutexName, lockName) boost::recursive_mutex::scoped_lock lockName(mutexName)
	// like _AUTO_MUTEX but mutex held by pointer
	#define _AUTO_SHARED_MUTEX mutable boost::recursive_mutex *_AUTO_MUTEX_NAME
	#define _LOCK_AUTO_SHARED_MUTEX assert(_AUTO_MUTEX_NAME); boost::recursive_mutex::scoped_lock autoMutexLock(*_AUTO_MUTEX_NAME)
	#define _NEW_AUTO_SHARED_MUTEX assert(!_AUTO_MUTEX_NAME); _AUTO_MUTEX_NAME = new boost::recursive_mutex()
	#define _DELETE_AUTO_SHARED_MUTEX assert(_AUTO_MUTEX_NAME); delete _AUTO_MUTEX_NAME
	#define _COPY_AUTO_SHARED_MUTEX(from) assert(!_AUTO_MUTEX_NAME); _AUTO_MUTEX_NAME = from
	#define _SET_AUTO_SHARED_MUTEX_NULL _AUTO_MUTEX_NAME = 0
	#define _MUTEX_CONDITIONAL(mutex) if (mutex)
	#define _THREAD_SYNCHRONISER(sync) boost::condition sync
	#define _THREAD_WAIT(sync, lock) sync.wait(lock)
	#define _THREAD_NOTIFY_ONE(sync) sync.notify_one()
	#define _THREAD_NOTIFY_ALL(sync) sync.notify_all()
	// Thread-local pointer
	#define _THREAD_POINTER(T, var) boost::thread_specific_ptr<T> var
	#define _THREAD_POINTER_SET(var, expr) var.reset(expr)
	#define _THREAD_POINTER_DELETE(var) var.reset(0)
	#define _THREAD_POINTER_GET(var) var.get()
#else
	#define _AUTO_MUTEX
	#define _LOCK_AUTO_MUTEX
	#define _MUTEX(name)
	#define _STATIC_MUTEX(name)
	#define _STATIC_MUTEX_INSTANCE(name)
	#define _LOCK_MUTEX(name)
	#define _LOCK_MUTEX_NAMED(mutexName, lockName)
	#define _AUTO_SHARED_MUTEX
	#define _LOCK_AUTO_SHARED_MUTEX
	#define _NEW_AUTO_SHARED_MUTEX
	#define _DELETE_AUTO_SHARED_MUTEX
	#define _COPY_AUTO_SHARED_MUTEX(from)
	#define _SET_AUTO_SHARED_MUTEX_NULL
	#define _MUTEX_CONDITIONAL(name) if(true)
	#define _THREAD_SYNCHRONISER(sync) 
	#define _THREAD_WAIT(sync, lock) 
	#define _THREAD_NOTIFY_ONE(sync) 
	#define _THREAD_NOTIFY_ALL(sync) 
	#define _THREAD_POINTER(T, var) T* var
	#define _THREAD_POINTER_SET(var, expr) var = expr
	#define _THREAD_POINTER_DELETE(var) _DELETE var; var = 0
	#define _THREAD_POINTER_GET(var) var
#endif

#if _COMPILER_ == _COMPILER_MSVC_
#define _C_CALL_ __cdecl
#else
#define _C_CALL_
#endif

/// Define name: Pretty Horrible Object-oriented Engine No-one Intend to eXecute
#define	_PROJECT_NAME_	"Phoenix"

/// Define version
#define _VERSION_MAJOR_ 1
#define _VERSION_MINOR_ 0
#define _VERSION_PATCH_ 1
#define _VERSION_SUFFIX_ "Alpha"
#define _VERSION_NAME_ "Cuber"

/// Version as a numeral
#define _VERSION_    ((_VERSION_MAJOR_ << 16) | (_VERSION_MINOR_ << 8) | _VERSION_PATCH_)

/// Defines an offset in a buffer
#if !defined(ARRAY_SIZE)
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif

/// Defines a macro that gives the number of elements in a static array
#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

#define checkGLErrors()	checkGLErrors_(__FILE__,__LINE__,__FUNCTION__)

static
bool checkGLErrors_(const char* file, const unsigned int line, const char* function)
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		printf("OpenGL Error: %s::%s::%d->%s\n", file, function, line, errString);
		return false;
	}
	return true;
}

_EXTERN_C_END_

#endif // __Base_h__