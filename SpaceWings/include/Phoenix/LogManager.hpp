/*
*	LogManager.hpp
*
*	A singleton class that manages logs
*
*  Created by Julien Lebot on 08/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __LogManager_hpp__
#define __LogManager_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Log.hpp>
#include <Phoenix/Singleton.hpp>
#include <map>
#include <string>

namespace Phoenix
{
	class LogManager
	: public Singleton<LogManager>
	{
		friend struct CreateUsingNew<LogManager>;

		/// The log map
		typedef std::map<std::string, LogPtr> LogMap;
	public:
		LogPtr createLog(const std::string& logName);
		void destroyLog(const std::string& logName);
		LogPtr getDefaultLog() { return mDefaultLog; }
		void setDefaultLog(const std::string& logName);

		// Protected methods
	protected:
		/// Default constructor.
		LogManager();

		/// Concrete destructor.
		~LogManager();

		// Protected data members
	protected:
		/// The list of logs
		LogMap mLogs;

		/// The default log
		LogPtr mDefaultLog;
	};
}

#endif // __LogManager_hpp__