// SoundTime.cpp: implementation of the CSoundTime class.
//
//////////////////////////////////////////////////////////////////////

#include "SoundTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSoundTime::CSoundTime(Uint32 time)
{
	m_time = time;
	// convert to mins, sec and ms <- someone need to do this one
	m_minutes = 0;
	m_seconds = 0;
	m_milliseconds = 0;
}

CSoundTime::CSoundTime(int min, int sec, int ms)
{
	m_minutes = min;
	m_seconds = sec;
	m_milliseconds = ms;
	// convert to time from mins, sec and ms
	// convert to time from mins, sec and ms
	double tmpTime = (100.0 / 60.0  * (double)min * 100000.0) + (100.0 / 60.0  * (double)sec * 1000.0) + (double)ms;
	Uint32 m_time = (Uint32) tmpTime; // <- someone need to do this one
}

CSoundTime::~CSoundTime()
{
}

Uint32	CSoundTime::GetSDLTime()
{
	return (Uint32)m_time;
}


CSoundTime & CSoundTime::operator+= (CSoundTime *)
{
	return *this;
}

CSoundTime & CSoundTime::operator-= (CSoundTime *)
{
	return *this;
}

CSoundTime & CSoundTime::operator+= (int)
{
	return *this;
}

CSoundTime & CSoundTime::operator-= (int)
{
	return *this;
}

CSoundTime & CSoundTime::operator++ ()
{
	return *this;
}

CSoundTime & CSoundTime::operator-- ()
{
	return *this;
}

//int CSoundTime::operator== (const CSoundTime& cst1, const CSoundTime& cst2)
//{
//	return 0;
//}
				
int CSoundTime::operator== (const CSoundTime& cst)
{
	return 0;
}
	
void CSoundTime::addMin(int mins)
{
}

void CSoundTime::addSec(int seconds)
{
}

void CSoundTime::addMS(int milliseconds)
{
	
}
