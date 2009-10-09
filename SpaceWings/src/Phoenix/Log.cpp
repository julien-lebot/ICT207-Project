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
			mOutput << "::" << getLiteralSeverity(loggingLevel) << " ";

		mOutput << message << std::endl;

		// Make sure our garbage is out
		mOutput.flush();
	}
}

Log::Stream Log::prepareForStreaming(const Log::LoggingLevel loggingLevel)
{
	Stream stream(this,loggingLevel);
	return stream;
}

Log::Stream::Stream(Log* log, const Log::LoggingLevel loggingLevel)
: mLog(log), mLoggingLevel(loggingLevel) {}

Log::Stream::~Stream()
{
	if (mStringStream.tellp() > 0)
		flush();
}

Log::Stream* Log::Stream::flush()
{
	mLog->write(mStringStream.str(), mLoggingLevel);
	mStringStream.str("");
	return this;
}

Log::Stream::Stream(const Stream& rhs)
: mLog(rhs.mLog),
  mLoggingLevel(rhs.mLoggingLevel),
  mStringStream(rhs.mStringStream.str())
{
}

Log::Stream& Log::Stream::operator=(const Stream& rhs)
{
	mLog = rhs.mLog;
	mLoggingLevel = rhs.mLoggingLevel;
	mStringStream.str(rhs.mStringStream.str());
	return *this;
}