/*
*	Matrix3.cpp
*
*	Declares the constants defined in Vector3.hpp
*
*  Created by Joshua Wilding on 31/08/09.
*  Copyright 2009 Joshua Wilding. All rights reserved.
*
*/

#include <Phoenix/Matrix3.hpp>

namespace Phoenix {
	namespace Math {
		const float Matrix3<float>::EPSILON = 1e-06f;
		const double Matrix3<double>::EPSILON = 1e-06f;
		
		const Matrix3<float> Matrix3<float>::ZERO(0,0,0,0,0,0,0,0,0);
		const Matrix3<double> Matrix3<double>::ZERO(0,0,0,0,0,0,0,0,0);

		const Matrix3<float> Matrix3<float>::IDENTITY(1,0,0,0,1,0,0,0,1);
		const Matrix3<double> Matrix3<double>::IDENTITY(1,0,0,0,1,0,0,0,1);
	}
}