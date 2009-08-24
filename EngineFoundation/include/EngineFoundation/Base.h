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

#define _NO_MEMORY_MANAGER_
#if defined(_DEBUG) && !defined(_NO_MEMORY_MANAGER_)
#include <stdio.h>						// Standard includes MUST come first
#include <stdlib.h>						//
#include <MemoryManager/mmgr.h>			// Paul Nettle's memory manager (does not quite work atm, leave commented out)
#endif

#define USE_INTRINSIC_ASM
//#define USE_SSE			// Don't use hand coded ASM for now, it's buggy

#include <CoreUtils/CorePlatform.h>		// Platform definitions

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

#if _PLATFORM_ == _PLATFORM_WIN32_
#undef _DLL_EXPORT
#if defined(_MAKE_DLL_)
#define _DLL_EXPORT __declspec(dllexport) extern
#else
#define _DLL_IMPORT __declspec(dllimport) extern
#endif
#elif _PLATFORM_ == _PLATFORM_APPLE_
#if defined(__MWERKS__)
#define _DLL_EXPORT __declspec(export) extern
#endif
#endif

#if !defined(_DLL_EXPORT)
#define _DLL_EXPORT extern
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

// Define name
#define	_PROJECT_NAME	"Core3D"

// Define version
#define _VERSION_MAJOR 1
#define _VERSION_MINOR 0
#define _VERSION_PATCH 1
#define _VERSION_SUFFIX "Alpha"
#define _VERSION_NAME "Cuber"

#define _VERSION    ((_VERSION_MAJOR << 16) | (_VERSION_MINOR << 8) | _VERSION_PATCH)

_EXTERN_C_END_

#endif // __Base_h__