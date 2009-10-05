/*
 *	CorePlatorm.h
 *
 *	Defines lots of platform and OS specific macros.
 *	Inspired from OGREPlatform.h
 *
 *  Created by Julien Lebot on 20/03/09.
 *  Copyright 2009 Julien Lebot. All rights reserved.
 *
 */

#ifndef __CorePlatform_H__
#define __CorePlatform_H__

#define _PLATFORM_WIN32_		1
#define _PLATFORM_LINUX_		2
#define _PLATFORM_APPLE_		3

#define _COMPILER_MSVC_			1
#define _COMPILER_GNUC_			2
#define _COMPILER_BORL_			3

#define _ARCH_32_				1
#define _ARCH_64_				2

#if defined (_MSC_VER)
#	define _COMPILER_	_COMPILER_MSVC_
#	define _COMPILER_VER_ _MSC_VER
#	define _COMPILER_STR_	"Microsoft Visual C/C++"

#elif defined (__GNUC__)
#	define _COMPILER_	_COMPILER_GNUC_
#	define _COMPILER_VER_ (((__GNUC__)*100) + \
						(__GNUC_MINOR__*10) + \
						__GNUC_PATCHLEVEL__)
#	define _COMPILER_STR_	"GNU GCC"

#elif defined (__BORLANDC__)
#	define _COMPILER_	_COMPILER_BORL_
#	define _COMPILER_VER_ __BCPLUSPLUS__
#	define __FUNCTION__ __FUNC__
#	define _COMPILER_STR_	"Borland"

#else
#	pragma error "No known compiler. Abort! Abort!"
#endif

#if _COMPILER_ == _COMPILER_MSVC_
#	if _COMPILER_VER_ >= 1200
#		define FORCEINLINE __forceinline
#	endif
#elif defined(__MINGW32__)
#	if !defined(FORCEINLINE)
#		define FORCEINLINE __inline
#	endif
#else
#	define FORCEINLINE __inline
#endif

#if defined(__WIN32__) || defined(_WIN32)
#	define _PLATFORM_ _PLATFORM_WIN32_
#	define _PLATFORM_STR_	"Microsoft Windows"

#elif defined (__APPLE_CC__)
#	define _PLATFORM_ _PLATFORM_APPLE_
#	define _PLATFORM_STR_	"Apple Inc. Mac OS"

#else
#	define _PLATFORM_ _PLATFORM_LINUX_
#	define _PLATFORM_STR_	"GNU/Linux"
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define _ARCH_TYPE_ _ARCH_64_
#	define _ARCH_TYPE_STR_	"64"
#else
#   define _ARCH_TYPE_ _ARCH_32_
#	define _ARCH_TYPE_STR_	"32"
#endif

#if defined( __MINGW32__ ) && !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__)
#	    define _UNICODE_SUPPORT_ 1
#   else
#       define _UNICODE_SUPPORT_ 0
#   endif
#else
#	define _UNICODE_SUPPORT_ 1
#endif

#endif	//__CorePlatform_H__