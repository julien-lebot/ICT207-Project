/**
* @file Log.cpp
*
* @author Joshua Wilding
* modified on 07/10/2009 by Julien Lebot: modified order of namespace inclusion (after includes, not before), added namespace Phoenix.
* @date 06/10/2009
*/
#include <iostream>
#include <string>
#include <ctime>

#include <Phoenix/Log.hpp>
#include <Phoenix/Exception.hpp>

using namespace std;
using namespace Phoenix;

Log::Log(const std::string fileName,
		 const bool override,
		 const bool debugVerbose,
		 const bool printSeverity,
		 const LoggingLevel minLoggingLevel)
: mFileName(fileName),
  mDebugVerbose(debugVerbose),
  mPrintSeverity(printSeverity),
  mLoggingLevel(minLoggingLevel),
  mOverride(override),
  mOutput(mFileName.c_str(), (mOverride ? ios::trunc : ios::ate) | ios::out)
{
}

Log::~Log()
{
	mOutput.close();
}

std::string Log::getLiteralSeverity(const LoggingLevel ll)
{
	switch (ll)
	{
	case LL_CRITICAL:
		return "[CRITICAL]";
		break;
	case LL_ERROR:
		return "[ERROR]";
		break;
	case LL_WARNING:
		return "[WARNING]";
		break;
	case LL_INFO:
		return "[INFO]";
		break;
	case LL_NOTICE:
		return "[NOTICE]";
		break;
	default:
		PHOENIX_EXCEPT("Invalid switch case");
		break;
	}
}

void Log::write(const std::string& message, const LoggingLevel loggingLevel)
{
	char timeStamp[12];
	time_t rawtime = time(NULL);

	if (loggingLevel >= mLoggingLevel)
	{
		// If debug verbose, spew out to cerr too !
		if (mDebugVerbose)
			std::cerr << message << std::endl;

		// Dunno why is so important to people ?
		strftime(timeStamp, 12, "[%I:%M:%S]", localtime(&rawtime));
		mOutput << timeStamp;

		if (mPrintSeverity)
			mOutput << "::" << getLiteralSeverity(loggingLevel);

		mOutput << message << std::endl;

		// Make sure our garbage is out
		mOutput.flush();
	}
}