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
#define PLATFORM_IPHONE_OR_MAC

#elif TARGET_OS_MAC
#define PLATFORM_MAC
#define PLATFORM_IPHONE_OR_MAC
#define PLATFORM_WINDOWS_OR_MAC

#elif defined(_MSC_VER)
#define PLATFORM_WINDOWS
#define PLATFORM_WINDOWS_OR_MAC
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
#include "../iOS/graphics.h"
#include "../iOS/mouse.h"
#include "../iOS/sound.h"
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "rapidxml/rapidxml.hpp"

#include "Assets.h"
#include "Lighting.h"
#include "Debug.h"


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

inline float NormalizeAngle(float Angle)
{
    while (Angle < 0)
        Angle += 360.0f;
    while (Angle >= 360.0f)
        Angle -= 360.0f;
    return Angle;
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
    if (mn > mx)
    {
        float tmp = mn;
        mn = mx;
        mx = tmp;
    }
    if (a < mn)
        return mn;
    if (a > mx)
        return mx;
    return a;
}

inline float AngleDifference(float a, float b) 
{
	float mn = a - b;
	while (mn < -180) mn += 360;
	while (mn > 180) mn -= 360;
	return mn;
}

inline float ClampAngle(float a, float mn, float mx)
{
    if (mn > mx)
    {
        float tmp = mn;
        mn = mx;
        mx = tmp;
    }
    if (AngleDifference(a, mn) < 0)
        return mn;
    if (AngleDifference(a, mx) > 0)
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

inline void Swap(float* A, float* B)
{
    float Temp = *A;
    *A = *B;
    *B = Temp;
}

inline float Random(float Min, float Max)
{
#ifdef PLATFORM_WINDOWS
	unsigned int i;
	rand_s( &i );
	return (float)( Min + ( (double)i / (double)UINT_MAX ) * (Max-Min) );
#endif
#ifdef PLATFORM_IPHONE_OR_MAC
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
#ifdef PLATFORM_IPHONE_OR_MAC
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
inline char* strtok_r(char* p, char* t, char**) { return strtok(p, t); }
inline float strtof(char* p, char** pp) { return (float)strtod(p, pp); }
#endif

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                            Temporary landing zone for global functions                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

// Returns current time in seconds.  ONLY to be used for logging prints and non-final profiling code.
#undef GetCurrentTime
double GetCurrentTime();

void SetGameState_StartScreen();
void SetGameState_DieScreen();
void SetGameState_WinScreen();
void SetGameState_Playing();
void SetGameState_Help();
void SetGameState_Credits();
void SetGameState_Leaderboard();
void SetGameState_ChapterIntro();

enum EGameTransition
{
	GAMETRANSITION_FIRST_PAGE,
	GAMETRANSITION_NEXT_PAGE,
	GAMETRANSITION_RESTART_PAGE,
	GAMETRANSITION_DIE_SCREEN,
    GAMETRANSITION_PLAY_RECORDING,
};

void SetGameState_Transition(EGameTransition Type);

void LoadCurrentChapter();
void AdvanceToNextPage();
void DisplayGame_Playing();

extern bool DevMode;

extern float ScrollX;
extern float ScrollY;

extern bool ChapterIntroDisplayed;
extern bool GamePause;

#endif
