/*
*	VectorCommon.hpp
*
*	Defines common definitions for all vector classes.
*
*  Created by Julien Lebot on 21/03/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __VectorCommon_h__
#define __VectorCommon_h__

#if defined(USE_INTRINSIC_ASM)
#include <xmmintrin.h>
#endif

namespace Phoenix {
	namespace Math {
		//namespace details {
			/// Copied from xmmintrin.h, because it is useful for pure asm readability
			/// It's same version as the Intel's version, except that it's in reverse order
			#define SHUFFLE(fp3,fp2,fp1,fp0) (((fp0) << 6) | ((fp1) << 4) | \
				((fp2) << 2) | ((fp3)))

			/// Very useful unnamed enum to ease readability of shufps
			enum
			{
				VEC_X = 0,
				VEC_Y = 1,
				VEC_Z = 2,
				VEC_W = 3
			};
		//}
	}
}

#endif // __VectorCommon_h__