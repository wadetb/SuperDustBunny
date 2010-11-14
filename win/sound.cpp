#include "sound.h"
#include <math.h>

#pragma comment(lib,"dsound.lib")

IDirectSound8* sxDev = NULL; 

extern HWND hWnd;

void sxInit()
{
	DirectSoundCreate8(NULL, &sxDev, NULL);
	if (!sxDev)
		return;

	sxDev->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
}

void sxDeinit()
{
	if (!sxDev)
		return;

	sxDev->Release();

	sxDev = NULL;
}

void sxLoadSound( const char* name, sxSound* snd )
{
	if (!sxDev)
		return;

	HMMIO hmmio = mmioOpen((LPSTR)name, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (!hmmio)
		return;

    MMCKINFO ckRiff;
	mmioDescend(hmmio, &ckRiff, NULL, 0);
    if (ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E'))
	{
		mmioClose(hmmio, 0);
        return;
	}

    MMCKINFO ckIn;
    ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend(hmmio, &ckIn, &ckRiff, MMIO_FINDCHUNK);

    WAVEFORMATEX* m_pwfx = (WAVEFORMATEX*)new BYTE[ckIn.cksize];
    mmioRead(hmmio, (HPSTR)m_pwfx, ckIn.cksize);

    mmioAscend(hmmio, &ckIn, 0);

    ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmioDescend(hmmio, &ckIn, &ckRiff, MMIO_FINDCHUNK);

	DSBUFFERDESC dsbdesc; 
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE; 
	dsbdesc.dwBufferBytes = ckIn.cksize; 
	dsbdesc.lpwfxFormat = m_pwfx; 

	LPDIRECTSOUNDBUFFER dsb = NULL;
	if (SUCCEEDED(sxDev->CreateSoundBuffer(&dsbdesc, &dsb, NULL))) 
	{ 
		dsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&snd->buffer);
		dsb->Release();
	}

	void* write;
	DWORD length;
	snd->buffer->Lock(0, 0, &write, &length, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	mmioRead(hmmio, (HPSTR)write, length);
	snd->buffer->Unlock(write, length, NULL, 0);

    mmioClose(hmmio, 0);

	snd->volume = 1.0f;
}

void sxDestroySound( sxSound* snd )
{
	if (snd->buffer)
	{
		snd->buffer->Release();
		snd->buffer = NULL;
	}
}

void sxPlaySound( sxSound* snd )
{
	if (snd->buffer)
	{
		//snd->buffer->Play(0, 0, 0);
	}
}

void sxPlaySoundLooping( sxSound* snd )
{
	if (snd->buffer)
	{
		//snd->buffer->Play(0, 0, DSBPLAY_LOOPING);
	}
}

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

void sxSetSoundVolume( sxSound* snd, float volume )
{
	snd->volume = volume;

	if (snd->buffer)
	{
		if (volume < 0.0f) volume = 0.0f;
		if (volume > 100.0f) volume = 100.0f;
		snd->buffer->SetVolume(DSVOLUME_TO_DB((int)(volume*100)));
	}
}

