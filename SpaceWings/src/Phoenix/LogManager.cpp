/*
*	LogManager.cpp
*
*	A singleton class that manages logs
*
*  Created by Julien Lebot on 08/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/LogManager.hpp>

using namespace Phoenix;

template<> LogManager* Singleton<Phoenix::LogManager>::mInstance = NULL;
template<> bool Singleton<Phoenix::LogManager>::mDestroyed = false;

#if defined(_THREAD_SUPPORT)
template<> boost::recursive_mutex Singleton<Phoenix::LogManager>::singletonMutex;
#endif

LogPtr LogManager::createLog(const std::string& logName)
{
	LogPtr newLog;
	LogMap::const_iterator cit = mLogs.find(logName);

	if (cit != mLogs.end())
	{
		newLog = (*cit).second;
	}
	else
	{
		newLog = LogPtr(new Log(logName));
		mLogs[logName] = newLog;
	}
	
	return newLog;
}

void LogManager::setDefaultLog(const std::string& logName)
{
	mDefaultLog = createLog(logName);
}

void LogManager::destroyLog(const std::string& logName)
{
	LogMap::iterator it = mLogs.find(logName);

	if (it != mLogs.end())
	{
		mLogs.erase(it);
	}
}

LogManager::LogManager()
{

}

LogManager::~LogManager()
{
	mLogs.clear();
}