/*
*	Timer.hpp
*
*	A timer class that uses performance counters on Windows and time of the day on Unix.
*
*  Created by Julien Lebot on 05/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Timer_hpp__
#define __Timer_hpp__

#include <Phoenix/Base.h>

namespace Phoenix
{
	class _PhoenixExport Timer
	{
	public:
		/**
		 * @brief Default constructor.
		 * @param runContinuousy If true the timer will be unstoppable until it is destroyed.
		 */
		Timer(const bool runContinuously = false);

		/// Starts the timer. This will also reset it.
		void start();

		/// Stops the timer. It does not reset the timer so you can still read it.
		void stop();

		/**
		 * @brief Returns the elapsed time since the timer was started in seconds.
		 * @return A double represent time how much time elapsed since the timer was started.
		 */
		double getElapsedTime();

		/**
		* @brief Returns the elapsed time since the timer was started in seconds.
		* @return A double represent time how much time elapsed since the timer was started.
		*/
		double getElapsedTimeInSec();

		/**
		* @brief Returns the elapsed time since the timer was started in milliseconds.
		* @return A double represent time how much time elapsed since the timer was started.
		*/
		double getElapsedTimeInMilliSec();

		/**
		* @brief Returns the elapsed time since the timer was started in microseconds.
		* @return A double represent time how much time elapsed since the timer was started.
		*/
		double getElapsedTimeInMicroSec();

	private:
		double mStartTimeInMicroSec;	// starting time in micro-second
		double mEndTimeInMicroSec;		// ending time in micro-second
		bool mStopped;					// stop flag 
		bool mStoppable;
#if _PLATFORM_ == _PLATFORM_WIN32_
		LARGE_INTEGER mFrequency;		// ticks per second
		LARGE_INTEGER mStartCount;
		LARGE_INTEGER mEndCount;
#else
		timeval mStartCount;
		timeval mEndCount;
#endif
	};
}

#endif // __Timer_hpp__