//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef COMMON_H
#define COMMON_H

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_RAND_S
#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define PLATFORM_IPHONE

#elif TARGET_OS_MAC
#define PLATFORM_MAC

#elif defined(_MSC_VER)
#define PLATFORM_WINDOWS
#endif


#if defined(PLATFORM_WINDOWS)
#include "../win/graphics.h"
#include "../win/mouse.h"
#include "../win/keyboard.h"
#include "../win/sound.h"
#include <shlwapi.h>
#include <windowsx.h>
#elif defined(PLATFORM_IPHONE)
#include "../iOS/graphics.h"
#include "../iOS/mouse.h"
#include "../iOS/sound.h"
#include <mach/mach.h>
#include <mach/mach_time.h>
#elif defined(PLATFORM_MAC)
#include "Mac/graphics.h"
#include "Mac/mouse.h"
#include "Mac/keyboard.h"
#include "Mac/sound.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "rapidxml/rapidxml.hpp"

#include "Assets.h"
#include "Lighting.h"

//#define SWIPE_DEBUG



static const float PI = acosf(-1.0f);

inline float DegreesToRadians(float a)
{
	return a * PI/180.0f;
}

inline float RadiansToDegrees(float a)
{
	return a * 180.0f/PI;
}

inline float DirectionToAngle(float a)
{
	return DegreesToRadians(90.0f-a);
}

inline float Max(float a, float b)
{
	if (a > b)
		return a;
	else
		return b;
}

inline float Min(float a, float b)
{
	if ( a < b)
		return a;
	else
		return b;
}

inline float Clamp(float a, float mn, float mx)
{
    if (a < mn)
        return mn;
    if (a > mx)
        return mx;
    return a;
}

inline float Length(float dx, float dy)
{
	return sqrtf(dx*dx + dy*dy);
}

inline float Distance(float x1, float y1, float x2, float y2)
{
	float dx = x2-x1;
	float dy = y2-y1;
	return sqrtf(dx*dx + dy*dy);
}

inline float AngleBetween(float x1, float y1, float x2, float y2)
{
	return atan2f(x2-x1, -(y2-y1));
}

inline float Remap(float Value, float FromMin, float FromMax, float ToMin, float ToMax, bool Clamp)
{
	float RelativeValue = (Value - FromMin) / (FromMax - FromMin);
	if (Clamp)
	{
		if (RelativeValue < 0.0f) RelativeValue = 0.0f;
		if (RelativeValue > 1.0f) RelativeValue = 1.0f;
	}
	return ToMin + RelativeValue * (ToMax - ToMin);
}

inline float Lerp(float Value, float FromMin, float FromMax, float ToMin=0.0f, float ToMax=1.0f, bool Clamp=true)
{
	float RelativeValue = (Value - FromMin) / (FromMax - FromMin);
	if (Clamp)
	{
		if (RelativeValue < 0.0f) RelativeValue = 0.0f;
		if (RelativeValue > 1.0f) RelativeValue = 1.0f;
	}
	return ToMin + RelativeValue * (ToMax - ToMin);
}

inline float Sign(float Value)
{
	if (Value < 0)
		return -1.0f;
	else if (Value > 0)
		return 1.0f;
	return 0;
}

inline float Random(float Min, float Max)
{
#ifdef PLATFORM_WINDOWS
	unsigned int i;
	rand_s( &i );
	return (float)( Min + ( (double)i / (double)UINT_MAX ) * (Max-Min) );
#endif
#ifdef PLATFORM_IPHONE
	u_int32_t i = arc4random();
	return (float)( Min + ( (double)i / (double)((2ULL<<31)-1) ) * (Max-Min) );
#endif
}

inline int Random(int Min, int Max)
{
#ifdef PLATFORM_WINDOWS
	unsigned int i;
	rand_s( &i );
	return Min + ( i % (Max-Min) );
#endif
#ifdef PLATFORM_IPHONE
	u_int32_t i = arc4random();
	return (float)( Min + ( (double)i / (double)((2ULL<<31)-1) ) * (Max-Min) );
#endif
}

inline float Round(float f)
{
	return (f > 0.0f) ? floor(f + 0.5f) : ceil(f - 0.5f);
}

inline float SinWave(float Time, float Period, float Amplitude=1.0f)
{
    return Amplitude * (1.0f-(cosf((Time * 2*PI) / Period)*0.5f+0.5f));
}

#ifdef PLATFORM_WINDOWS
#define strcasecmp _stricmp
#define strdup _strdup
#define snprintf _snprintf
#endif

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                            Temporary landing zone for global functions                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

// Stack of "error contexts" which provide extra information about the state of the program when an error occurs.
void PushErrorContext(const char* ErrorContext, ...);
void PopErrorContext();

// Reports a fatal error and immediately exits the program.
void ReportError(const char* ErrorMessage, ...);

// Print something to the application log.
void LogMessage(const char* LogMessage, ...);

// Misc ObjC function exports.
void DisplayWelcomeAlert();
void HTTPPost(const char* URL, const void* Data, int DataSize);
void ShowSettings();

// Returns current time in seconds.  ONLY to be used for logging prints and non-final profiling code.
#undef GetCurrentTime
double GetCurrentTime();

void AddDebugLine(float X1, float Y1, float X2, float Y2, unsigned int Color, float Time);
void DisplayDebugLine(float X1, float Y1, float X2, float Y2, float Width, unsigned int Color);


struct SRemoteControl
{
	bool Enabled;
	bool MoveLeft;
	bool MoveRight;
	bool Jump;
};

extern SRemoteControl RemoteControl;



bool GetInput_MoveLeft();
bool GetInput_MoveRight();
bool GetInput_Jump();

void SetGameState_StartScreen();
void SetGameState_DieScreen();
void SetGameState_WinScreen();
void SetGameState_Tutorial(int State);
void SetGameState_Playing();
void SetGameState_Help();
void SetGameState_Credits();
void SetGameState_ChapterIntro();

enum EGameTransition
{
	GAMETRANSITION_FIRST_PAGE,
	GAMETRANSITION_NEXT_PAGE,
	GAMETRANSITION_RESTART_PAGE,
	GAMETRANSITION_DIE_SCREEN,
};

void SetGameState_Transition(EGameTransition Type);

void LoadCurrentChapter();
void AdvanceToNextPage();
void DisplayGame_Playing();

extern bool DevMode;
extern int ScrollY;

extern bool ChapterIntroDisplayed;
extern bool GamePause;

#endif
