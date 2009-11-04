/*
*	ConfigOpts.hpp
*
*	OpenGL renderer config options
*
*  Created by Julien Lebot on 08/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __ConfigOpts_hpp__
#define __ConfigOpts_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Type2Type.hpp>
#include <boost/any.hpp>
#include <string>
#include <vector>
#include <map>

namespace Phoenix
{
	struct ConfigOption
	{
		template <typename T>
		bool is(const boost::any& operand, Type2Type<T> = Type2Type<T>()) const
		{
			return operand.type() == typeid(T);
		}

		bool is_empty(const boost::any & operand) const
		{
			return operand.empty();
		}

		bool is_char_ptr(const boost::any & operand) const
		{
			try
			{
				boost::any_cast<const char *>(operand);
				return true;
			}
			catch(const boost::bad_any_cast &)
			{
				return false;
			}
		}

		bool is_string(const boost::any & operand) const
		{
			try
			{
				boost::any_cast<std::string>(operand);
				return true;
			}
			catch(const boost::bad_any_cast &)
			{
				return false;
			}
		}
		
		/*
		bool is_string(const boost::any & operand) const
		{
		return boost::any_cast<std::string>(&operand);
		}
		*/

		ConfigOption &addPossibleValue(const boost::any& val)
		{
			possibleValues.push_back(val);
			return *this;
		}

		ConfigOption()
		{}

		ConfigOption(const boost::any& val)
		: value(val)
		{}

		boost::any value;
		std::vector<boost::any> possibleValues;
		// bool readOnly;
	};

	typedef std::map<std::string, ConfigOption> ConfigOptionMap;
}

#endif // __ConfigOpts_hpp__