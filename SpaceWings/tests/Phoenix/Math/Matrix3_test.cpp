/*
*	Matrix3_test.hpp
*
*	3D Matrix test suite
*
*  Created by Julien Lebot on 31/03/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <boost/test/auto_unit_test.hpp>
#include <Phoenix/Math/Matrix3.hpp>

using namespace Phoenix::Math;

BOOST_AUTO_TEST_SUITE( math_test_suite )
BOOST_AUTO_TEST_CASE( matrix3_test_ctors )
{
	// Test construction from an array of floats
	float mat1[3][3] = {{1.0f,0.0f,0.0f},
						{0.0f,1.0f,0.0f},
						{0.0f,0.0f,1.0f}};
	Matrix3f m1 = Matrix3f(mat1);
	BOOST_CHECK_EQUAL(m1, Matrix3f::IDENTITY);

	// Test copy construction, this is also a way to initialize the matrix since the default constructor does not do it.
	Matrix3f m2 = Matrix3f(Matrix3f::ZERO);
	BOOST_CHECK_EQUAL(m2.determinant(), 0);

	// Test construction from arguments
	Matrix3f m3 = Matrix3f(1.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 1.0f);
	BOOST_CHECK_EQUAL(m3.determinant(), 1.0f);

	// Test assignment
	m2 = m3;
	BOOST_CHECK_EQUAL(m2, m3);
}

BOOST_AUTO_TEST_CASE( matrix3_test_arithmetic )
{
	// Test determinant
	Matrix3f m1 = Matrix3f(2.0f, -1.0f, 3.0f,
						   1.0f,  1.0f, 6.0f,
						   0.0f,  2.0f, 2.0f);
	BOOST_CHECK_EQUAL(m1.determinant(), -12.0f);

	// Test inversion
	BOOST_CHECK_EQUAL(Matrix3f( 3.0f, 0.0f, 0.0f,
								4.0f, 5.0f, 0.0f,
							   -1.0f, 2.0f, 6.0f).inverse(),
					  Matrix3f( 1.0f/3.0f, 0.0f, 0.0f,
							   -4.0f/15.0f, 1.0f/5.0f, 0.0f,
							   13.0f/90.0f, -1.0f/15.0f, 1.0f/6.0f));

	// Test multiplication 1
	m1 = m1 * 1.0f;
	BOOST_CHECK_EQUAL(m1.determinant(), -12.0f);

	// Test multiplication 2
	m1 = m1 * 2.0f;
	BOOST_CHECK_EQUAL(m1.determinant(), -96.0f);

	// Test accessing vector as a 2 dimensional array
	BOOST_CHECK_EQUAL(m1[1][2], 12.0f);

	// Test accessor
	BOOST_CHECK_EQUAL(m1.getCol(1), Vector3f(-2.0f, 2.0f, 4.0f));

	// Test addition
	BOOST_CHECK_EQUAL(m1 + Matrix3f::IDENTITY, Matrix3f(5.0f, -2.0f, 6.0f,
														2.0f,  3.0f, 12.0f,
														0.0f,  4.0f, 5.0f));

	// Test substraction
	BOOST_CHECK_EQUAL(m1 - m1, Matrix3f::ZERO);

	// Test accessing and changing one number in the matrix
	m1[2][0] = -3.0f;
	BOOST_CHECK_EQUAL(m1[2][0], -3.0f);

	// Test negating the matrix (eg -A)
	BOOST_CHECK_EQUAL(-m1, Matrix3f(-4.0f,  2.0f, -6.0f,
									-2.0f, -2.0f, -12.0f,
									 3.0f, -4.0f, -4.0f));

	// Test transposition (eg A^T)
	BOOST_CHECK_EQUAL(m1.transpose(), Matrix3f( 4.0f, 2.0f, -3.0f,
											   -2.0f, 2.0f,  4.0f,
											    6.0f, 12.0f, 4.0f));
}

BOOST_AUTO_TEST_CASE( matrix3_test_output )
{
	Matrix3f m1 = Matrix3f(1.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 1.0f);
	std::cout << m1 << std::endl;
}
BOOST_AUTO_TEST_SUITE_END()