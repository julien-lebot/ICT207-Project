/*
*	Vector3_test.hpp
*
*	3D Vector test suite
*
*  Created by Julien Lebot on 21/03/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <EngineFoundation/Math/Vector3.hpp>

using namespace EngineFoundation::Math;

BOOST_AUTO_TEST_SUITE( math_test_suite )
BOOST_AUTO_TEST_CASE( vector3_test_ctors )
{
	// Default construction
	Vector3f v0;
	BOOST_CHECK_EQUAL(v0, Vector3f::ZERO);

	// Construction from a bunch of coords
	Vector3f v1(1.5f,3.14f,-15.0f);
	BOOST_CHECK_EQUAL(v1[0], 1.5f);
	BOOST_CHECK_EQUAL(v1[1], 3.14f);
	BOOST_CHECK_EQUAL(v1[2],-15.0f);

	// Construction from a vector
	float coords[3] = {-69.0f,1987.0f,2009.0f};
	Vector3f v2(coords);
	BOOST_CHECK_EQUAL(*(v2.ptr()), -69.0f);
	BOOST_CHECK_EQUAL(*(v2.ptr() + 1), 1987.0f);
	BOOST_CHECK_EQUAL(*(v2.ptr() + 2), 2009.0f);

	// Copy construction
	Vector3f v3(v1);
	BOOST_CHECK_EQUAL(v3[0], 1.5f);
	BOOST_CHECK_EQUAL(v3[1], 3.14f);
	BOOST_CHECK_EQUAL(v3[2],-15.0f);
}

BOOST_AUTO_TEST_CASE( vector3_test_assignments )
{
	Vector3f v1(1.5f,3.14f,-15.0f);

	float coords[3] = {-69.0f,1987.0f,2009.0f};
	Vector3f v2(coords);

	// Assignment of vectors to vectors
	v1 = v2;
	BOOST_CHECK_EQUAL(*(v1.ptr()), -69.0f);
	BOOST_CHECK_EQUAL(*(v1.ptr() + 1), 1987.0f);
	BOOST_CHECK_EQUAL(*(v1.ptr() + 2), 2009.0f);

	// Assignment of a scalar
	v2 = 3.15f;
	BOOST_CHECK_EQUAL(v2[0], 3.15f);
	BOOST_CHECK_EQUAL(v2[1], 3.15f);
	BOOST_CHECK_EQUAL(v2[2], 3.15f);
}

BOOST_AUTO_TEST_CASE( vector3_test_arithmethic )
{
	Vector3f v1(1.5f,3.14f,-15.0f);
	Vector3f v2 = v1;

	// Test addition of 2 vectors together and multiplication
	BOOST_CHECK_EQUAL(v1 + v2, 2 * v1);

	// Test substraction
	Vector3f v4 = v2 - Vector3f(3.0f, -2.3f, 16.8f);
	Vector3f v5 = Vector3f(-1.5f, 5.44f, -31.8f);
	BOOST_CHECK_EQUAL(v4[0], v5[0]);
	BOOST_CHECK_EQUAL(v4, v5);

	// Test division
	BOOST_CHECK_EQUAL(v1 / 2, Vector3f(0.75f, 1.57f, -7.5f));

	// Test inversion
	BOOST_CHECK_EQUAL(-v2, Vector3f(-1.5f, -3.14f, 15.0f));

	// Test addition assignment
	Vector3f v3(0.8f, 0.86f, -5.0f);
	BOOST_CHECK_EQUAL(v1 += v3, Vector3f(2.3f, 4.0f, -20.0f));

	// Test substraction assignment
	BOOST_CHECK_EQUAL(v2 -= v2, Vector3f::ZERO);

	// Test division assignment
	BOOST_CHECK_EQUAL(v1 /= v1, Vector3f::UNIT);
}

BOOST_AUTO_TEST_CASE( vector3_test_equalities )
{
	Vector3f v1(1.5f,3.14f,-15.0f);
	Vector3f v2;
	v2 = 3.15f;

	// Test equality operator
	Vector3f v3(3.15f);
	BOOST_CHECK(v2 == v3);

	BOOST_CHECK(v1 != v2);
	BOOST_CHECK(v1 != v3);
}

BOOST_AUTO_TEST_CASE( vector3_test_math )
{
	Vector3f v1(2.0f, 8.0f, -16.0f);

	// Test magnitude op
	BOOST_CHECK_EQUAL(v1.magnitude(), 18.0f);

	// Test normalise op
	v1.normalise();
	BOOST_CHECK_EQUAL(ceilf(v1.magnitude()), 1.0f);

	// Test cross product op
	//		(y)
	//		(.)->(-x) gives |
	//						v(-z)
	BOOST_CHECK_EQUAL(Vector3f::MINUS_X.cross(Vector3f::Y), Vector3f::MINUS_Z);

	Vector3f v2(1.5f,3.14f,-15.0f);

	// Test dot product op
	BOOST_CHECK_EQUAL(ceilf(sqrtf(v2.dot(v2))), ceilf(v2.magnitude()));
}

BOOST_AUTO_TEST_SUITE_END()