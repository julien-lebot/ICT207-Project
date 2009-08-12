// Sound.cpp: implementation of the CSound class.
//
//////////////////////////////////////////////////////////////////////

#include <memory.h>
#include <malloc.h>

#include "Sound.h"
#include "EasySound.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSound::CSound(char *filename, int iSoundID)
{
	m_iSoundID = iSoundID;
	Uint32 len;
	if (SDL_LoadWAV(filename, &m_spec, &m_data, &len) == NULL ) {
		fprintf(stderr, "Couldn't load %s: %s\n",filename, SDL_GetError());
		m_len = CSoundTime(0);
		return;
	}
	m_len = CSoundTime(len);
	m_pos = CSoundTime(0);

	CEasySound *es = CEasySound::Instance();
	SDL_AudioSpec obtained = es->GetObtained();
	SDL_AudioCVT cvt;

	SDL_BuildAudioCVT(&cvt, m_spec.format, m_spec.channels, m_spec.freq, obtained.format, obtained.channels, obtained.freq);
	cvt.buf = (Uint8*)malloc(m_len.GetSDLTime() * cvt.len_mult);
	memcpy(cvt.buf, m_data, m_len.GetSDLTime());
	cvt.len = m_len.GetSDLTime();
	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(m_data);
	
	SDL_LockAudio();
	m_data = cvt.buf;
	// the new length of sound after convert from 8bit to 16bit
	m_len = CSoundTime( cvt.len_cvt );
	m_pos = CSoundTime( 0 );
	SDL_UnlockAudio();
	
}

CSound::~CSound()
{
	SDL_FreeWAV(m_data);
}


bool CSound::Play(CSoundTime start, CSoundTime length)
{
	m_pos = start;
	m_stop = length;
	SDL_PauseAudio(0);
	return 0;
}

bool CSound::Play()
{
	m_pos = CSoundTime(0);
	m_stop = m_len;
	SDL_PauseAudio(0);
	
    return 0;
}

CSoundTime CSound::Stop()
{
	SDL_PauseAudio(1);
	return CSoundTime(0,0,0);
}

CSoundTime CSound::GetLength()
{
	return CSoundTime(m_len);
}

