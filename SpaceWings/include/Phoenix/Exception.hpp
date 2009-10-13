/*
*	Exception.hpp
*
*	A base Exception class derived from the std::exception. Makes it easier to throw exceptions.
*
*  Created by Julien Lebot on 08/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Exception_hpp__
#define __Exception_hpp__

#include <Phoenix/Base.h>
#include <string>
#include <exception>

namespace Phoenix
{
	/**
	 * An exception class derived from the std::exception to make it easier to throw exceptions and to obtain information about the exception that occured.
	 */
	class Exception
	: public std::exception
	{
	protected:
		/// The line number where this exception occured
		long mLine;

		/// The function name where this exception occured
		std::string mFunction;

		/// The compilation unit where this exception occured
		std::string mSourceFile;

		/// The description of this exception.
		std::string mDescription;
	public:

		/**
		* Constructs an exception.
		* @param description A description of the exception that occured.
		* @param source The source file where this exception occured
		* @param function The function name where this exception occured.
		* @param line The line number where this exception occured.
		*/
		Exception(const std::string& description,
				  const std::string& source,
				  const std::string& function,
				  const long line);

		/**
		* Copy constructor.
		* @param rhs The exception to copy.
		*/
		Exception(const Exception& rhs);

		/// Needed for  compatibility with std::exception
		~Exception() throw() {}

		/** Assignment operator.
		* @param rhs The source exception.
		*/
		void operator =(const Exception& rhs);

		/** Gets the source function.
		*/
		virtual const std::string& getFunction() const { return mFunction; }

		/** Gets source file name.
		*/
		virtual const std::string &getSourceFile() const { return mSourceFile; }

		/** Gets line number.
		*/
		virtual long getLine() const { return mLine; }

		/** 
		*/
		virtual const std::string& getDescription(void) const { return mDescription; }

		/// Override std::exception::what
		const char* what() const throw() { return getDescription().c_str(); }
	};
}

#ifndef PHOENIX_EXCEPT
#define PHOENIX_EXCEPT(desc) throw Exception(desc, __FILE__, __FUNCTION__, __LINE__);
#endif

#endif // __Exception_hpp__