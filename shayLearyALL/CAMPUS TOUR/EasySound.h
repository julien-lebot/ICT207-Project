// EasySound.h: interface for the CEasySound class.
//
//////////////////////////////////////////////////////////////////////

#ifndef EASY_SOUND_H
#define EASY_SOUND_H

#include <stdlib.h>
#include <SDL.h>
//#include <SDL_mixer.h>
//#include <SDL_audio.h>

#include <list>
#include "Sound.h"
#include "SoundTime.h"

/**
 * class Score: This class displays the score, bonus and time on the screen
 *
 * @author Shay Leary
 */

// make sure to link this librarys or error will occur
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

#define ES_SAMPLE	8192
#define ES_FREQ		88200

extern void mixaudio(void *unused, Uint8 *stream, int len);
/**
 * class CEasySound: This class is singleton design pattern, and it control all object related to sound. (use SDL library)
 *
 * @author Shannon, Graham and Shay
 */
class CEasySound  
{
public:
	/**
  * Instance: Get CEasySound class before using any methods (Singleton Design Pattern)
  *
  * @return static CEasySound* 
  */
	static CEasySound* Instance();
	/**
  * ~CEasySound:
  *
  * @return 
  */
	virtual ~CEasySound();

	/**
  * CallMixAudio: ? no idea eh
  *
  * @param unused 
  * @param stream 
  * @param len 
  * @return void 
  */
	void CallMixAudio(void *unused, Uint8 *stream, int len);

	/**
  * Load: load the sound and store in the m_listSound list
	*
	* @param filename - filename of sound wave file
	* @return int	- get sound's ID for future use
	*/
	int Load(char *filename);
	/**
  * Unload: unload the sound and remove form the m_listSound list
  *
  * @param iSoundID - Sound's ID to find if there match ID in the m_listSound and unload it.
  * @return void 
  */
	void Unload(int iSoundID);
	/**
  * Unload: unload the sound and remove from the m_listSound list
  *
  * @param  sound - CSound class to find if there match ID in the m_listSound and unload it.
  * @return void 
  */
	void Unload(CSound *sound);
	/**
  * GetSound: get the sound from the m_listSound list
  *
  * @param iSoundID - Sound's ID to be given before get Sound Object from the m_listSound
  * @return CSound* - Sound Object to use it (Play, Stop, etc)
  */
	CSound* GetSound(int iSoundID);

	SDL_AudioSpec GetObtained() { return obtained; };

protected:
	/**
  * CEasySound: The construction had to be protected because of Singleton Design Pattern
  *
  * @return  
  */
	CEasySound();
private:
	/**
  * _instance: pointer of CEasySound class as part of the Singleton Design Pattern
  */ 
	static CEasySound* _instance;

	/**
  * m_listSound: list of Sound's object to store it. (STL's list object)
  */ 
	std::list<CSound*>m_listSound;

	SDL_AudioSpec desired;
	SDL_AudioSpec obtained;

//	int m_currentID;
};

#endif
