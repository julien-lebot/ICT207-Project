/*
*	Math.hpp
*
*	Defines some commonly used math function and their
*	faster asm equivalents.
*
*  Created by Julien Lebot on 21/03/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Math_h__
#define __Math_h__

#include <Phoenix/Base.h>

// On MSVC this brings the definitions for some mathematical constants, to be checked on other systems. J.L.
#define _USE_MATH_DEFINES
#include <cmath>

#include <boost/units/base_units/angle/arcminute.hpp>
#include <boost/units/base_units/angle/arcsecond.hpp>
#include <boost/units/base_units/angle/degree.hpp>
#include <boost/units/base_units/angle/gradian.hpp>
#include <boost/units/base_units/angle/revolution.hpp>
#include <boost/units/base_units/angle/radian.hpp>
#include <boost/units/base_units/angle/steradian.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/quantity.hpp>

namespace Phoenix {
	namespace Math {
		namespace Units {
			#define DEFINE_ANGLE_UNIT(type_name,unit_name_,dimension_)																\
			typedef boost::units::make_system<boost::units::angle::unit_name_ ## _base_unit>::type    unit_name_ ## system_;        \
			typedef boost::units::unit<boost::units::dimension_ ## _dimension,unit_name_ ## system_> unit_name_ ## _ ## dimension_;	\
			static const unit_name_ ## _ ## dimension_    unit_name_ ## s;															\
			typedef boost::units::quantity<unit_name_ ## _ ## dimension_>	type_name;

			DEFINE_ANGLE_UNIT(Arcminutes,arcminute,plane_angle)
			DEFINE_ANGLE_UNIT(Arcseconds,arcsecond,plane_angle)
			DEFINE_ANGLE_UNIT(Degrees,degree,plane_angle)
			DEFINE_ANGLE_UNIT(Gradians,gradian,plane_angle)
			DEFINE_ANGLE_UNIT(Radians,radian,plane_angle)
			DEFINE_ANGLE_UNIT(Revolutions,revolution,plane_angle)
			DEFINE_ANGLE_UNIT(Steradians,steradian,plane_angle)

			#undef DEFINE_ANGLE_UNIT
		} // namespace Units

		const float PI = 3.1415926535897932384626433832795;

		/** 
		 * @brief Returns the same number if it is a power of
		 * two. Returns a larger integer if it is not a 
		 * power of two. The larger integer is the next
		 * highest power of two.
		 *
		 * @param power The absolute value of the power.
		 * @return An unsigned int that represents the absolute closest value to the power of two of the argument.
		 */

		unsigned int getClosestPow2(unsigned int power);

		// These functions probably need some re-working as they are made to work with floats only atm
		// Should they be templated or just doubled up ? J.L.
		float sqroot(float val);
		float rsqroot(float val);
		float cosine(const Units::Radians &value);
		float sine(const Units::Radians &value);
		float tangent(const Units::Radians &value);
		float arctangent(const Units::Radians &value);
		float arcsine(const Units::Radians &value);
		float arcosine(const Units::Radians &value);
		float convertPolar (const float &tempX, const float &tempY);

		/// Equality between 2 floating point numbers using tolerance
		template <typename T>
		bool TEqual(T a, T b, T tolerance )
		{
			if (std::fabs(b - a) <= tolerance)
				return true;
			else
				return false;
		}
	} // namespace Math
} // namespace Phoenix

#endif // __Math_h__