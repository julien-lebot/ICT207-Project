/*
*	Console_Test.cpp
*
*	Console test suite
*
*  Created by Julien Lebot on 07/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <Phoenix/Utils/ConsoleGL.hpp>
#include <iostream>

#include <boost/spirit/include/qi.hpp>

#include <boost/fusion/functional.hpp>
#include <boost/fusion/include/fused_function_object.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/include/adapter.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/sequence/io/out.hpp>
#include <boost/fusion/include/out.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/container/generation.hpp>
#include <boost/fusion/include/generation.hpp>
#include <boost/fusion/container/generation/make_map.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/algorithm/query/find.hpp>
#include <boost/fusion/include/find.hpp>

#include <boost/utility/result_of.hpp>

#include <Phoenix/Utils/Atomic.hpp>

template <typename T>
struct ConfigOption
{
	std::string name;
	T value;
	std::vector<T> possibleValues;
};

struct Opts
{
	ConfigOption<bool> fullscreen;
	ConfigOption<float> anisotropylvl;
};

namespace keys
{
	struct fullscreen;
	struct anisotropylvl;
}

#define MAKE_CONFIG_OPTION_FUSION(T)									\
	BOOST_FUSION_ADAPT_STRUCT(ConfigOption<T>,							\
								   (std::string, name)					\
								   (T, value)							\
								   (std::vector<T>, possibleValues))

MAKE_CONFIG_OPTION_FUSION(bool)
MAKE_CONFIG_OPTION_FUSION(float)

boost::fusion::map<boost::fusion::pair<keys::fullscreen, ConfigOption<bool> >,
				   boost::fusion::pair<keys::anisotropylvl, ConfigOption<float> > > configOptions;

BOOST_FUSION_ADAPT_ASSOC_STRUCT(
								Opts,
								(ConfigOption<bool>, fullscreen, keys::fullscreen)
								(ConfigOption<float>, anisotropylvl, keys::anisotropylvl)
								)

struct print_xml
{
	template <typename T>
	void operator()(T const& x) const
	{
		std::cout
			<< '<' << typeid(x).name() << '>'
			<< x
			<< "</" << typeid(x).name() << '>'
			;
	}
};

void try_it()
{
	Opts opt;
	opt.fullscreen.name = "Fullscreen";
	opt.fullscreen.possibleValues.push_back(true);
	opt.fullscreen.possibleValues.push_back(false);
	opt.fullscreen.value = true;

	opt.anisotropylvl.name = "Anisotropy level";
	opt.anisotropylvl.possibleValues.push_back(0.0f);
	opt.anisotropylvl.possibleValues.push_back(4.0f);
	opt.anisotropylvl.possibleValues.push_back(8.0f);
	opt.anisotropylvl.possibleValues.push_back(16.0f);
	opt.anisotropylvl.value = 16.0f;
}

using namespace Phoenix;

ConsoleGL cgl;

void errorFunction(const std::vector<std::string> & args)
{
	cgl.print("Unknown command " + args[0]);
}

void quit(const std::vector<std::string> & args)
{
	exit(0);
}

void setHistory(const std::vector<std::string> & args)
{
	cgl.setOutputBufferSize(atoi(args[1].c_str()));
}

void removeCommand(const std::vector<std::string> & args)
{
	cgl.deleteItem(args[1]);
}

BOOST_AUTO_TEST_SUITE( Console_test_suite )
BOOST_AUTO_TEST_CASE( Console_test )
{
	std::string command;
	static std::string username;
	static float health;
	cgl.addItem("username", &username, ConsoleGL::STRING_T);
	cgl.addItem("health", &health, ConsoleGL::FLOAT_T);
	cgl.addItem("/quit", quit, ConsoleGL::FUNCTION_T);
	cgl.addItem("/history", setHistory, ConsoleGL::FUNCTION_T);
	cgl.addItem("/remove", removeCommand, ConsoleGL::FUNCTION_T);
	cgl.setDefaultCommand(errorFunction);

	//while(1)
	{
		std::getline(std::cin, command);
		cgl.sendImmediateMessage(command);
		cgl.render();
	};

}

using namespace boost::phoenix;
using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::ascii;
using namespace boost::spirit::arg_names;

///////////////////////////////////////////////////////////////////////////////
//  Our number list parser
///////////////////////////////////////////////////////////////////////////////
//[tutorial_numlist1
template <typename Iterator>
struct calculator : grammar<Iterator, space_type>
{
	calculator() : calculator::base_type(expression)
	{
		expression =
			term
			>> *(   ('+' >> term)
			|   ('-' >> term)
			)
			;

		term =
			factor
			>> *(   ('*' >> factor)
			|   ('/' >> factor)
			)
			;

		factor =
			double_
			|	uint_
			|   '(' >> expression >> ')'
			|   ('-' >> factor)
			|   ('+' >> factor)
			;
	}

	rule<Iterator, space_type> expression, term, factor;
};

template <typename Iterator>
struct sample_functor
{
	typedef boost::fusion::vector<int, double> argument_t;

	sample_functor()
	{
		args_rule = (int_ >> double_)[operator()(boost::fusion::make_vector(_1, _2))];
	}

	void operator()(argument_t args)
	{
		std::cout << boost::fusion::at_c<0> (args) << std::endl;
	}

	rule<Iterator, space_type> args_rule;
};

template <typename Iterator, class RegisteredTypes>
struct console_parser : grammar<Iterator, space_type>
{
	console_parser()
	: console_parser::base_type(start)
	{
		//start = 
	}

	rule<Iterator, space_type> start;
	RegisteredTypes store;
};

struct temp
{
	Atomic<int, false> id;
};

BOOST_AUTO_TEST_CASE( test )
{
	temp t;

	t.id = 10;
	BOOST_ASSERT(t.id.get() == 10);

	boost::fusion::vector<int> v = boost::fusion::make_vector(15, 10, -1);
	try_it();

	typedef std::string::const_iterator iterator_type;
	typedef calculator<iterator_type> calculator;

	console_parser<iterator_type, boost::fusion::list<sample_functor<iterator_type>::argument_t> > cp;

	calculator calc; // Our grammar

	std::string str;
	while (std::getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			break;

		std::string::const_iterator iter = str.begin();
		std::string::const_iterator end = str.end();
		bool r = phrase_parse(iter, end, calc, space);

		if (r && iter == end)
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded\n";
			std::cout << "-------------------------\n";
		}
		else
		{
			std::string rest(iter, end);
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "stopped at: \": " << rest << "\"\n";
			std::cout << "-------------------------\n";
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()