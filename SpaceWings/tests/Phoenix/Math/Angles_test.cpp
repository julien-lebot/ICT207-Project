/*
*	Angles_test.hpp
*
*	Boost Angles test suite.
*
*  Created by Julien Lebot on 21/03/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <boost/test/auto_unit_test.hpp>

// Boost Test declaration and Checking macros
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/units/systems/si.hpp>

#include <boost/units/conversion.hpp>
#include <boost/units/io.hpp>
#include <Phoenix/Math/Math.hpp>
#include <iostream>

using namespace boost::units;
using namespace Phoenix::Math;
using namespace Phoenix::Math::Units;

BOOST_AUTO_TEST_SUITE( math_test_suite )
BOOST_AUTO_TEST_CASE( angles_conversion_test )
{
	std::cout << "Testing angle base units..." << std::endl;
    
	Arcseconds	as(1.0*arcseconds);
    Arcminutes	am(1.0*arcminutes);
    Degrees		d(1.0*degrees);
    Gradians	g(1.0*gradians);
    Radians		r(1.0*radians);
    Revolutions rev(1.0*revolutions);

    std::cout << as << " = " << quantity<si::plane_angle>(as) << std::endl
              << am << " = " << quantity<si::plane_angle>(am) << std::endl
              << d << " = " << quantity<si::plane_angle>(d) << std::endl
              << g << " = " << quantity<si::plane_angle>(g) << std::endl
              << r << " = " << quantity<si::plane_angle>(r) << std::endl
              << rev << " = " << quantity<si::plane_angle>(rev) << std::endl
              << std::endl;

    std::cout << rev << "/" << as << " = " << quantity<si::dimensionless>(rev/as) << std::endl
              << rev << "/" << am << " = " << quantity<si::dimensionless>(rev/am) << std::endl
              << rev << "/" << d << " = " << quantity<si::dimensionless>(rev/d) << std::endl
              << rev << "/" << g << " = " << quantity<si::dimensionless>(rev/g) << std::endl
              << rev << "/" << r << " = " << quantity<si::dimensionless>(rev/r) << std::endl
              << std::endl;

    // conversions only work with exponent of +/- 1 in scaled_base_unit?
    std::cout << as << " = " << Arcseconds(as) << std::endl
              << am << " = " << Arcseconds(am) << std::endl
              << d << " = " << Arcseconds(d) << std::endl
              << rev << " = " << Arcseconds(rev) << std::endl
              << std::endl;
              
    // conversions only work with exponent of +/- 1 in scaled_base_unit? see arcsecond.hpp
    std::cout << as << " = " << Arcminutes(as) << std::endl
              << am << " = " << Arcminutes(am) << std::endl
              << d << " = " << Arcminutes(d) << std::endl
              << rev << " = " << Arcminutes(rev) << std::endl
              << std::endl;

    std::cout << as << " = " << Degrees(as) << std::endl
              << am << " = " << Degrees(am) << std::endl
              << d << " = " << Degrees(d) << std::endl
              << rev << " = " << Degrees(rev) << std::endl
              << std::endl;
              
    std::cout << as << " = " << Revolutions(as) << std::endl
              << am << " = " << Revolutions(am) << std::endl
              << d << " = " << Revolutions(d) << std::endl
              << rev << " = " << Revolutions(rev) << std::endl
              << std::endl;
              
    Steradians sa1(1.0*steradians);
    
    std::cout << sa1 << std::endl
              << std::endl;
}

BOOST_AUTO_TEST_CASE( angles_math_test )
{
	BOOST_CHECK_EQUAL(getClosestPow2(3), 4);

	BOOST_CHECK_EQUAL(sqroot(1.0*radians), sqrt((float)1.0));
	BOOST_CHECK_EQUAL(rsqroot(1.0*radians), 1.0/sqrt((float)1.0));
	BOOST_CHECK_EQUAL(cosine(1.0*radians), cos((float)1.0));
	BOOST_CHECK_EQUAL(sine(1.0*radians), sin((float)1.0));
	BOOST_CHECK_EQUAL(tangent(1.0*radians), tan((float)1.0));
	BOOST_CHECK_EQUAL(arctangent(1.0*radians), atan((float)1.0));
	BOOST_CHECK_EQUAL(arcsine(1.0*radians), asin((float)1.0));
	BOOST_CHECK_EQUAL(arcosine(1.0*radians), acos((float)1.0));
}

BOOST_AUTO_TEST_SUITE_END()
