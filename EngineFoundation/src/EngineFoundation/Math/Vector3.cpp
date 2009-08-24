/*
*	Vector3.cpp
*
*	Declares the constants defined in Vector3.hpp
*
*  Created by Julien Lebot on 21/08/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <EngineFoundation/Math/Vector3.hpp>

namespace EngineFoundation {
	namespace Math {
		const Vector3<float> Vector3<float>::ZERO(0, 0, 0);
		const Vector3<double> Vector3<double>::ZERO(0, 0, 0);

		const Vector3<float> Vector3<float>::X(1, 0, 0);
		const Vector3<double> Vector3<double>::X(1, 0, 0);

		const Vector3<float> Vector3<float>::Y(0, 1, 0);
		const Vector3<double> Vector3<double>::Y(0, 1, 0);

		const Vector3<float> Vector3<float>::Z(0, 0, 1);
		const Vector3<double> Vector3<double>::Z(0, 0, 1);

		const Vector3<float> Vector3<float>::MINUS_X(-1, 0, 0);
		const Vector3<double> Vector3<double>::MINUS_X(-1, 0, 0);

		const Vector3<float> Vector3<float>::MINUS_Y( 0,-1, 0);
		const Vector3<double> Vector3<double>::MINUS_Y( 0,-1, 0);

		const Vector3<float> Vector3<float>::MINUS_Z( 0, 0,-1);
		const Vector3<double> Vector3<double>::MINUS_Z( 0, 0,-1);

		const Vector3<float> Vector3<float>::UNIT(1, 1, 1);
		const Vector3<double> Vector3<double>::UNIT(1, 1, 1);
	}
}