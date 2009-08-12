// SoundTime.h: interface for the CSoundTime class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SOUND_TIME_H
#define SOUND_TIME_H

#include <SDL.h>

// perfer to have all define start with ES_ as easy sound similar to GL_ for openGL
/**
 * -1: the value is play the sound looping
 */
#define ES_LOOP	-1
/**
 * 0: the value is play the sound once
 */
#define ES_ONCE	0

/**
 * class CSoundTime: The is the time convertion from min:sec:ms into int value for SDL library
 *
 * @author Shannon, Grahan and Shay
 */
class CSoundTime  
{
public:
	/**
	 * CSoundTime:
	 *
	 * @param time 
	 * @return  
	 */
	CSoundTime(Uint32 time = (Uint32)0);
	/**
	 * CSoundTime:
	 *
	 * @param min 
	 * @param sec 
	 * @param ms 
	 * @return  
	 */
	CSoundTime(int min, int sec, int ms);
	/**
	 * ~CSoundTime:
	 *
	 * @return 
	 */
	virtual ~CSoundTime();

	/**
	 * GetSDLTime:
	 *
	 * @return int 
	 */
	Uint32	GetSDLTime();

	/**
	 * operator+=:
	 *
	 * @param  
	 * @return CSoundTime& 
	 */
	CSoundTime & operator+= (CSoundTime *);
	/**
	 * operator-=:
	 *
	 * @param  
	 * @return CSoundTime& 
	 */
	CSoundTime & operator-= (CSoundTime *);
	/**
	 * operator+=:
	 *
	 * @param  
	 * @return CSoundTime& 
	 */
	CSoundTime & operator+= (int); // add by seconds
	/**
	 * operator-=:
	 *
	 * @param  
	 * @return CSoundTime& 
	 */
	CSoundTime & operator-= (int); // sub by seconds
	/**
	 * operator++:
	 *
	 * @return CSoundTime& 
	 */
	CSoundTime & operator++ (); // add count of seconds
	/**
	 * operator--:
	 *
	 * @return CSoundTime& 
	 */
	CSoundTime & operator-- (); // sub count of seconds
	/**
	 * friend operator==:
	 *
	 * @param cst1
	 * @param cst2
	 * @return int 
	 */
	friend int operator== (const CSoundTime& cst1, const CSoundTime& cst2);					
	/**
	 * operator==:
	 *
	 * @param cst 
	 * @return int 
	 */
	int operator== (const CSoundTime& cst);					
	
	/**
	 * addMin:
	 *
	 * @param mins 
	 * @return void 
	 */
	void addMin(int mins);
	/**
	 * addSec:
	 *
	 * @param seconds 
	 * @return void 
	 */
	void addSec(int seconds);
	/**
	 * addMS:
	 *
	 * @param milliseconds 
	 * @return void 
	 */
	void addMS(int milliseconds);

private:
	/**
	 * m_minutes:
	 */
	int		m_minutes;
	/**
	 * m_seconds:
	 */
	int		m_seconds;
	/**
	 * m_milliseconds:
	 */
	int		m_milliseconds;

	/**
	 * m_time:
	 */
	Uint32		m_time; // the time need for SDL library
};

#endif
