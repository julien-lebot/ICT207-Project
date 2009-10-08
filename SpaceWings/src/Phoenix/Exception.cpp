/*
*	Exception.hpp
*
*	A base Exception class derived from the std::exception. Makes it easier to throw exceptions.
*
*  Created by Julien Lebot on 08/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Exception.hpp>

using namespace Phoenix;

Exception::Exception(const std::string& description,
					 const std::string& source,
					 const std::string& function,
					 const long line)
: mDescription(description),
  mSourceFile(source),
  mFunction(function),
  mLine(line)
{
}

Exception::Exception(const Exception& rhs)
: mDescription(rhs.mDescription),
  mSourceFile(rhs.mSourceFile),
  mFunction(rhs.mFunction),
  mLine(rhs.mLine)
{
}

void Exception::operator =(const Exception& rhs)
{
	mDescription = rhs.mDescription;
	mSourceFile = rhs.mSourceFile;
	mFunction = rhs.mFunction;
	mLine = rhs.mLine;
}