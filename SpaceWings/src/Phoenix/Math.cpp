/*
*	Math.cpp
*
*	Implementation of various math functions.
*
*  Created by Julien Lebot on 21/03/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Math.hpp>

#if defined(USE_INTRINSIC_ASM)
#include <xmmintrin.h>
#endif

namespace Phoenix {
namespace Math {

unsigned int getClosestPow2(unsigned int power)
{
	unsigned int nPow2 = 1;

	while(power > nPow2)
		nPow2 = (nPow2 << 1);

	return nPow2;
}

float sqroot(float val)
{
	if (val == 1.0)
		return 1.0;

#if defined(USE_INTRINSIC_ASM)
	return _mm_cvtss_f32( /// Converts back to float
		   _mm_sqrt_ss( /// Computes the square root
		   _mm_load_ss(&val)	/// Converts from float to __m128
		)
		);
#elif defined(USE_SSE)
	float result = 0.0f;
	__asm
	{
		movss       xmm0,dword ptr [val]
		sqrtss      xmm0,xmm0 
		movss		dword ptr [result], xmm0
	}
	return result;
#else
	return sqrt(val);
#endif
}

float rsqroot(float val)
{
	if (val == 1.0)
		return 1.0;

#if defined(USE_INTRINSIC_ASM)
	return _mm_cvtss_f32( /// Converts back to float
		_mm_rsqrt_ss( /// Computes the reverse square root
		_mm_load_ss(&val)	/// Converts from float to __m128
		)
		);
#elif defined(USE_SSE)
	float result = 0.0f;
	__asm
	{
		movss       xmm0,dword ptr [val]
		rsqrtss     xmm0,xmm0 
		movss		dword ptr [result], xmm0
	}
	return result;
#else
	return 1.0f/sqroot(val);
#endif
}

float cosine(const Units::Radians &value)
{
	return cos((float)value.value());
}

float sine(const Units::Radians &value)
{
	return sin((float)value.value());
}

float tangent(const Units::Radians &value)
{
	return tan((float)value.value());
}

float arctangent(const Units::Radians &value)
{
	return atan((float)value.value());
}

float arcsine(const Units::Radians &value)
{
	return asin((float)value.value());
}

float arcosine(const Units::Radians &value)
{
	return acos((float)value.value());
}

float convertPolar (const float &tempX, const float &tempY)
{
	float angle;
	if (tempX == 0.0)					// check if x is close to zero
	{
		if (tempY > 0.0)
			angle = PI / 2.0;
		else
			angle = 3.0 * PI / 2.0;
	}
	else {
		angle = atan (tempY / tempX);
		if (tempX < 0.0)
			angle += PI;
		if (angle > 2 * PI)
			angle -= (2.0 * PI);
	}

	return (angle);
}

}

}