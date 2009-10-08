/*
*	Matrix3.cpp
*
*	Declares the constants defined in Matrix3.hpp
*
*  Created by Joshua Wilding on 31/08/09.
*  Copyright 2009 Joshua Wilding. All rights reserved.
*
*/

#include <Phoenix/Matrix3.hpp>
#include <Phoenix/StaticCheck.hpp>

namespace Phoenix {
	namespace Math {
		const float Matrix3<float>::EPSILON = 1e-06f;
		const double Matrix3<double>::EPSILON = 1e-06f;
		
		const Matrix3<float> Matrix3<float>::ZERO(0,0,0,0,0,0,0,0,0);
		const Matrix3<double> Matrix3<double>::ZERO(0,0,0,0,0,0,0,0,0);

		const Matrix3<float> Matrix3<float>::IDENTITY(1,0,0,0,1,0,0,0,1);
		const Matrix3<double> Matrix3<double>::IDENTITY(1,0,0,0,1,0,0,0,1);

		// Little neat function that secretly test the size of our matrix to make sure (at compile time) that is has the appropriate memory alignment !
		static void compile_time_check()
		{
			PHOENIX_STATIC_CHECK(sizeof(Matrix3<float>)==48,Matrix3f_memory_not_boundary_aligned);
			PHOENIX_STATIC_CHECK(sizeof(Matrix3<double>)==80,Matrix3d_memory_not_boundary_aligned);
		}
	}
}