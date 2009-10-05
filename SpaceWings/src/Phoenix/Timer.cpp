/*
*	Timer.cpp
*
*	A timer class that uses performance counters on Windows and time of the day on Unix.
*
*  Created by Julien Lebot on 05/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Timer.hpp>

using namespace Phoenix;

Timer::Timer(const bool runContinuously)
: mStoppable(!runContinuously)
{
#if _PLATFORM_ == _PLATFORM_WIN32_
	QueryPerformanceFrequency(&mFrequency);
#endif
}

void Timer::start()
{
	mStopped = false; // reset stop flag
#if _PLATFORM_ == _PLATFORM_WIN32_
	QueryPerformanceCounter(&mStartCount);
#else
	gettimeofday(&mStartCount, NULL);
#endif
}

void Timer::stop()
{
	if (mStoppable)
	{
		mStopped = true; // set timer stopped flag
#if _PLATFORM_ == _PLATFORM_WIN32_
		QueryPerformanceCounter(&mEndCount);
#else
		gettimeofday(&mEndCount, NULL);
#endif
	}
}

double Timer::getElapsedTime()
{
	return getElapsedTimeInSec();
}

double Timer::getElapsedTimeInSec()
{
	return getElapsedTimeInMicroSec() * 0.000001;
}

double Timer::getElapsedTimeInMilliSec()
{
	return getElapsedTimeInMicroSec() * 0.001;
}

double Timer::getElapsedTimeInMicroSec()
{
#if _PLATFORM_ == _PLATFORM_WIN32_
	if(!mStopped)
		QueryPerformanceCounter(&mEndCount);

	mStartTimeInMicroSec = mStartCount.QuadPart * (1000000.0 / mFrequency.QuadPart);
	mEndTimeInMicroSec = mEndCount.QuadPart * (1000000.0 / mFrequency.QuadPart);
#else
	if(!mStopped)
		gettimeofday(&mEndCount, NULL);

	mStartTimeInMicroSec = (mStartCount.tv_sec *1000000.0) + mStartCount.tv_usec;
	mEndTimeInMicroSec = (mEndCount.tv_sec * 1000000.0) + mEndCount.tv_usec;
#endif

	return mEndTimeInMicroSec - mStartTimeInMicroSec;
}