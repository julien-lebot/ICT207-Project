// EasySound.cpp: implementation of the CEasySound class.
//
//////////////////////////////////////////////////////////////////////

#if 0
#include "EasySound.h"
extern void mixaudio(void *unused, Uint8 *stream, int len){
	CEasySound::Instance()->CallMixAudio(unused,stream,len);
}

//////////////////////////////////////////////////////////////////////
// Member for Singleton Pattern
//////////////////////////////////////////////////////////////////////

CEasySound* CEasySound::_instance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEasySound::CEasySound()
{

  if ((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_NOPARACHUTE)==-1))  {
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
      exit(1);
  }
  atexit(SDL_Quit);

	/* Open the audio device */

	/* 22050Hz - FM Radio quality */
	desired.freq=ES_FREQ;

	/* 16-bit signed audio */
	desired.format=AUDIO_S16;

	/* Mono */
	desired.channels=2;

	/* Large audio buffer reduces risk of dropouts but increases response time */
	desired.samples=ES_SAMPLE;

	desired.callback = mixaudio;

	desired.userdata=NULL;
	/* Open the audio device */
	if ( SDL_OpenAudio(&desired, &obtained) < 0 ){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

}

CEasySound::~CEasySound()
{
	SDL_Quit();
	delete _instance;
	_instance = NULL;
}

CEasySound* CEasySound::Instance()
{
	if (_instance == NULL)
		_instance = new CEasySound();
	return _instance;
}

void CEasySound::CallMixAudio(void *unused, Uint8 *stream, int len)
{

	//int i;
    Uint32 amount, data_len, data_pos;

	std::list<CSound*>::iterator cs;
	for (cs = m_listSound.begin(); cs != m_listSound.end(); ++cs) {
		//data_len = (*cs)->GetLength().GetSDLTime();
		data_len = (*cs)->m_stop.GetSDLTime();
		data_pos = (*cs)->m_pos.GetSDLTime();
		amount = (data_len - data_pos);
		if ( amount > len) {
			amount = len;
		}
		Uint8 *data = &(*cs)->m_data[data_pos];
		SDL_MixAudio(stream, data, amount, SDL_MIX_MAXVOLUME);
		(*cs)->m_pos = CSoundTime(data_pos + amount);
	}

}


int CEasySound::Load(char *filename)
{
	// find ID for sound	
	std::list<CSound*>::iterator cs;
	int iSoundID = 0;
	bool bFoundID = true;
	while( bFoundID ) { // while loop until if there no ID match and set that ID for new sound's ID
		bFoundID = false;
		for (cs = m_listSound.begin(); cs != m_listSound.end(); ++cs) {
			if ((*cs)->GetSoundID() == iSoundID) {
				bFoundID = true;
				iSoundID++;
				break;
			}
		}
	}

	// load the sound with new ID
	CSound* sound = new CSound(filename,iSoundID);
	if (sound->GetLength() == CSoundTime(0,0,0)) {
		return -1; // fail to load
	}
	m_listSound.insert(m_listSound.end(),sound);
	return iSoundID; // fail to load
}

void CEasySound::Unload(int iSoundID)
{
	std::list<CSound*>::iterator cs;
	for (cs = m_listSound.begin(); cs != m_listSound.end(); ++cs) {
		if ((*cs)->GetSoundID() == iSoundID) {
			m_listSound.erase(cs);
			delete (*cs);
			return;
		}
	}
}

void CEasySound::Unload(CSound *sound)
{
	std::list<CSound*>::iterator cs;
	for (cs = m_listSound.begin(); cs != m_listSound.end(); ++cs) {
		if ((*cs)->GetSoundID() == sound->GetSoundID()) {
			m_listSound.erase(cs);
			delete (*cs);
			return;
		}
	}
}

CSound* CEasySound::GetSound(int iSoundID)
{
	std::list<CSound*>::iterator cs;
	for (cs = m_listSound.begin(); cs != m_listSound.end(); ++cs) {
		if ((*cs)->GetSoundID() == iSoundID) {
			return (*cs);
		}
	}

	return NULL; // empty sound
}

#endif