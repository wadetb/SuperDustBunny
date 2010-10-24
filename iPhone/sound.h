#ifndef SOUND_H
#define SOUND_H

struct sxSound
{
	float volume;
	unsigned int buffer;
	unsigned int source;
};

void sxInit();
void sxExit();
#define sxDeinit sxExit

void sxLoadSound(const char* filename, sxSound* sound);
#define sxLoadWav sxLoadSound
void sxDestroySound(sxSound* sound);

void sxPlaySound(sxSound* sound);
void sxPlaySoundLooping(sxSound* sound);

void sxSetSoundVolume(sxSound* sound, float volume);

#endif
