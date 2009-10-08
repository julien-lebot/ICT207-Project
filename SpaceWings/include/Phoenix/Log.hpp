/**
 * @file Log.hpp
 * @class Log
 *
 * @brief This class is a logging class, it is used for debugging.
 *
 * @author Joshua Wilding
 * modified on 07/10/2009 by Julien Lebot: modified order of namespace inclusion (after includes, not before), added namespace Phoenix.
 * @date 06/10/2009
 */

#ifndef __LOG_HPP
#define __LOG_HPP

#include <Phoenix/Base.h>
#include <fstream>
#include <string>

namespace Phoenix
{
	/**
	 * A class to log information out to a file and optionally to a debug screen (std::cerr).
	 * @remarks Do not construct this class directly but use the LogManager instead.
	 */
	class Log
	{
	public:
		/// Enum describing the logging level for each write call.
		enum LoggingLevel
		{
			LL_CRITICAL = 5,	// Use for logging critical errors
			LL_ERROR = 4,	 	// Use for logging non-critical errors
			LL_WARNING = 3,	// Use for logging warnings
			LL_INFO = 2,		// Use for logging key informational events
			LL_NOTICE = 1		// Slightly less than Info, but not debug spew
		};

		/**
		* write This function saves a given string into the log and optionally to a debug screen (std::cerr).
		* @remarks It will print out the message only if the severity of this message is > to the severity of the log.
		*
		* @param message This parameter contains the string to be saved into a file.
		* @param loggingLevel The severity of this message.
		*/
		void write(const std::string& message, const LoggingLevel loggingLevel = LL_INFO);

		/**
		 * Named constructor.
		 * @remarks Do not construct this class directly but use the LogManager instead.
		 * @param fileName The log name.
		 * @param override Set to true if this log will override any existing previous log.
		 * @param debugVerbose Set to true if you want to see debug information printed out in the log. Defaults to true in DEBUG builds, false otherwise.
		 */
		Log(const std::string fileName,
			const bool override = true,
#ifdef _DEBUG
			const bool debugVerbose = true,
#else
			const bool debugVerbose = false,
#endif
			const bool printSeverity = true,
			const LoggingLevel minLoggingLevel = LL_INFO);

		/// Concrete destructor. Don't subclass this class just yet. Maybe in the future if need arises.
		~Log();
	protected:
		/// The name of the log
		std::string mFileName;

		/// Whether to print out debug info
		bool mDebugVerbose;

		/// Whether to log the severity too.
		bool mPrintSeverity;

		/// The logging level
		LoggingLevel mLoggingLevel;

		/// Override file on disk or append ?
		bool mOverride;

		/// The output stream
		std::ofstream mOutput;

		/**
		* Default constructor: blank constructor.
		*/ 
		Log();

		/**
		 * Get the severity as a string to be printed out on screen.
		 * @return A string representing the severity.
		 */
		std::string getLiteralSeverity(const LoggingLevel ll);
	};

	typedef tr1::shared_ptr<Log> LogPtr;
}
#endif // __LOG_HPP

