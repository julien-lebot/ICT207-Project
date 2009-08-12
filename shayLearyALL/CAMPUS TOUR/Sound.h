// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include "SoundTime.h"

/**
 * class CSound: This class is the sound object, and it allow to play, stop, etc
 *
 * @author Shannon, Grahan and Shay
 */
class CSound  
{
public:
	/**
	 * CSound:
	 *
	 * @param filename 
	 * @param iSoundID 
	 * @return  
	 */
	CSound(char *filename, int iSoundID);
	/**
	 * ~CSound:
	 *
	 * @return 
	 */
	virtual ~CSound();

	/**
	 * Play:
	 *
	 * @param start 
	 * @param length 
	 * @return bool 
	 */
	bool		Play(CSoundTime start, CSoundTime length);
	/**
	 * Play:
	 *
	 * @return bool 
	 */
	bool		Play();
	/**
	 * Stop:
	 *
	 * @return CSoundTime 
	 */
	CSoundTime	Stop();
	/**
	 * GetLength:
	 *
	 * @return CSoundTime 
	 */
	CSoundTime	GetLength();
	/**
	 * GetSoundID:
	 *
	 * @return int 
	 */
	int					GetSoundID() { return m_iSoundID; };

public:
	/**
	 * m_data:
	 */
	Uint8			*m_data;
	/**
	 * m_pos:
	 */
	CSoundTime		m_pos;
	/**
	 * m_len:
	 */
	CSoundTime		m_len;
	CSoundTime		m_stop;
	/**
	 * m_spec:
	 */
	SDL_AudioSpec	m_spec;
	/**
	 * m_iSoundID:
	 */
	int						m_iSoundID; // id number for sound

};

#endif
