#include "sound.h"

#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>

ALCcontext* sxContext;
ALCdevice* sxDevice;

void sxInit()
{
	sxDevice = alcOpenDevice(NULL);
	
	sxContext = alcCreateContext(sxDevice, NULL);
	alcMakeContextCurrent(sxContext);
}

void sxExit()
{
	alcDestroyContext(sxContext);
	alcCloseDevice(sxDevice);
}

void sxGetOpenALAudioData(CFURLRef inFileURL, void** outData, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei* outSampleRate)
{
	ExtAudioFileRef extRef;
	ExtAudioFileOpenURL(inFileURL, &extRef);
	
    if (extRef == NULL)
    {
        *outData = NULL;
        *outDataSize = 0;
        return;
    }
    
	AudioStreamBasicDescription fileFormat;
	UInt32 propSize = sizeof(fileFormat);
	ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &propSize, &fileFormat);
	
	// Set the client format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	AudioStreamBasicDescription outputFormat;
	outputFormat.mSampleRate = fileFormat.mSampleRate;
	outputFormat.mChannelsPerFrame = fileFormat.mChannelsPerFrame;
	outputFormat.mFormatID = kAudioFormatLinearPCM;
	outputFormat.mBytesPerPacket = 2 * outputFormat.mChannelsPerFrame;
	outputFormat.mFramesPerPacket = 1;
	outputFormat.mBytesPerFrame = 2 * outputFormat.mChannelsPerFrame;
	outputFormat.mBitsPerChannel = 16;
	outputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	
	ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(outputFormat), &outputFormat);
	
	SInt64 fileLengthInFrames;
	propSize = sizeof(fileLengthInFrames);
	ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &propSize, &fileLengthInFrames);
	
	void* data = NULL;
	UInt32 dataSize = fileLengthInFrames * outputFormat.mBytesPerFrame;
	data = malloc(dataSize);
	
	AudioBufferList	dataBuffer;
	dataBuffer.mNumberBuffers = 1;
	dataBuffer.mBuffers[0].mDataByteSize = dataSize;
	dataBuffer.mBuffers[0].mNumberChannels = outputFormat.mChannelsPerFrame;
	dataBuffer.mBuffers[0].mData = data;
	
	ExtAudioFileRead(extRef, (UInt32*)&fileLengthInFrames, &dataBuffer);
	
	ExtAudioFileDispose(extRef);
	
	*outData = data;
	*outDataSize = dataSize;
	*outDataFormat = (outputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	*outSampleRate = outputFormat.mSampleRate;
}

void sxLoadSound(const char* filename, sxSound* sound)
{
	CFURLRef url = (CFURLRef)[[NSURL fileURLWithPath:[[NSString alloc] initWithCString:filename]] retain];
	
	ALenum format;
	ALsizei size;
	ALsizei freq;
	ALvoid* data;
	sxGetOpenALAudioData(url, &data, &size, &format, &freq);
	
	CFRelease(url);

    if (data == NULL)
    {
        sound->buffer = 0;
        sound->source = 0;
        return;
    }
    
	alGenBuffers(1, &sound->buffer);
	alBufferData(sound->buffer, format, data, size, freq); 
	
	alGenSources(1, &sound->source); 
	alSourcei(sound->source, AL_BUFFER, sound->buffer);
	alSourcef(sound->source, AL_PITCH, 1.0f);
	alSourcef(sound->source, AL_GAIN, 1.0f);
	
	free(data);
}

void sxDestroySound(sxSound *sound)
{
	alDeleteSources(1, &sound->source);
	alDeleteBuffers(1, &sound->buffer);
	sound->source = 0;
	sound->buffer = 0;
}

void sxPlaySound(sxSound* sound)
{
	alSourcei(sound->source, AL_LOOPING, AL_FALSE);
	alSourcePlay(sound->source);
}

void sxPlaySoundLooping(sxSound* sound)
{
	alSourcei(sound->source, AL_LOOPING, AL_TRUE);
	alSourcePlay(sound->source);
}

void sxSetSoundVolume(sxSound* sound, float volume)
{
	sound->volume = volume;
	alSourcef(sound->source, AL_GAIN, volume);
}


