/*--------------------------------------------------------------------------------------------------------
 
  Sound Library

  Library Overview:

  Structures:
    sxSound                 Sound structure, contains an audio clip that can be played.

  Functions:
	sxInit					Initializes the sound system.
	sxDeinit				Cleans up the sound system.

    sxLoadWav               Load a .WAV file from disk into a sxSound variable.
    sxPlaySound             Start a sound playing.

--------------------------------------------------------------------------------------------------------*/
#ifndef SOUND_H
#define SOUND_H

#include <dsound.h>

/*--------------------------------------------------------------------------------------------------------

  sxSound Structure

  Stores a sound clip which can be played back using sxPlaySound.

--------------------------------------------------------------------------------------------------------*/
struct sxSound
{
	IDirectSoundBuffer8* buffer;
};

/*--------------------------------------------------------------------------------------------------------

  sxInit, sxDeinit

  Call these two functions at the beginning and end of your program.  Caution: sxInit must be called 
  *after* gxInit.

  Example:

  // Initialize the sound system.
  sxInit();

--------------------------------------------------------------------------------------------------------*/
void sxInit();
void sxDeinit();

/*--------------------------------------------------------------------------------------------------------

  sxLoadSound

  Loads a *.wav file into a sound object. 

  Example:

  // Create a new sound from a 'bang.wav'.
  sxSound bangSound;
  sxLoadSound( "bang.wav", &bangSound );

--------------------------------------------------------------------------------------------------------*/
void sxLoadSound( const char* name, sxSound* snd );

/*--------------------------------------------------------------------------------------------------------

  sxDestroySound

  Destroys a loaded sound.  Don't forget to call this or you'll leak memory!

--------------------------------------------------------------------------------------------------------*/
void sxDestroySound( sxSound* snd );

/*--------------------------------------------------------------------------------------------------------

  sxPlaySound

  Starts a sound clip playing.  Note that while this function returns immediately, the sound will keep
  playing until it is done.

  Example:

  sxPlaySound( &bangSound );

--------------------------------------------------------------------------------------------------------*/
void sxPlaySound( sxSound* snd );

// Continuously loops the sound.  Good for background music.
void sxPlaySoundLooping( sxSound* snd );

// Sets the volume of the sound.  Can be used while playing.
void sxSetSoundVolume( sxSound* snd, float volume );

/*--------------------------------------------------------------------------------------------------------

	DirectSound access

	Use this pointer only if you know exactly what you're doing.  It allows you to access the DirectSound
	subsystem for advanced things that aren't covered by this library.

--------------------------------------------------------------------------------------------------------*/
extern IDirectSound8* sxDev; 


#endif
