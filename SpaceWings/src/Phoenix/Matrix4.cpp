/*
*	Matrix4.hpp
*
*	A 4D Matrix class.
*
*  Created by Julien Lebot on 31/08/09.
*  Copyright 2009 Joshua Wilding. All rights reserved.
*
*  Modified by Julien Lebot 31/08/09
*
*/

#include <Phoenix/Matrix4.hpp>
#include <Phoenix/StaticCheck.hpp>

namespace Phoenix {
	namespace Math {
		const float Matrix4<float>::EPSILON = 1e-06f;
		const double Matrix4<double>::EPSILON = 1e-06f;

		const Matrix4<float> Matrix4<float>::ZERO(0,0,0,0,
												  0,0,0,0,
												  0,0,0,0,
												  0,0,0,0);

		const Matrix4<double> Matrix4<double>::ZERO(0,0,0,0,
													0,0,0,0,
													0,0,0,0,
													0,0,0,0);

		const Matrix4<float> Matrix4<float>::IDENTITY(1,0,0,0,
													  0,1,0,0,
													  0,0,1,0,
													  0,0,0,1);


		const Matrix4<double> Matrix4<double>::IDENTITY(1,0,0,0,
														0,1,0,0,
														0,0,1,0,
														0,0,0,1);

		// Little neat function that secretly test the size of our matrix to make sure (at compile time) that is has the appropriate memory alignment !
		static void compile_time_check()
		{
			PHOENIX_STATIC_CHECK(sizeof(Matrix4<float>)==64,Matrix4f_memory_not_boundary_aligned);
			PHOENIX_STATIC_CHECK(sizeof(Matrix4<double>)==128,Matrix4d_memory_not_boundary_aligned);
		}
	}
}