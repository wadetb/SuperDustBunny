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
#include "win/graphics.h"
#include "win/mouse.h"
#include "win/keyboard.h"
#include "win/sound.h"
#elif defined(PLATFORM_IPHONE)
#include "iPhone/graphics.h"
#include "iPhone/mouse.h"
#include "iPhone/sound.h"
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

#include "Assets.h"

// Constants

static const float PI = acosf(-1.0f);

// Macros

extern bool DevMode;
extern int ScrollY;

#define White gxRGB32(255, 255, 255)
#define Black gxRGB32(0, 0, 0)

inline float Max(float a, float b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

inline float Min(float a, float b)
{
	if ( a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
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

inline float Random(float Min, float Max)
{
#ifdef PLATFORM_WINDOWS
	unsigned int i;
	rand_s( &i );
	return (float)( Min + ( (double)i / (double)UINT_MAX ) * (Max-Min) );
#endif
#ifdef PLATFORM_IPHONE
	u_int32_t i = arc4random();
	return (float)( Min + ( (double)i / (double)((2<<31)-1) ) * (Max-Min) );
#endif
}

#ifdef PLATFORM_WINDOWS
#define strcasecmp _stricmp
#define strdup _strdup
#endif

enum ETutorialState
{
    TUTORIALSTATE_NONE,
    TUTORIALSTATE_INITIAL,
    TUTORIALSTATE_COIN,
    TUTORIALSTATE_BALL,
    TUTORIALSTATE_FIREWORK,
    TUTORIALSTATE_BARREL,
    TUTORIALSTATE_WALLJUMP,
    TUTORIALSTATE_JUMP,
    TUTORIALSTATE_GEAR,
};

struct STutorial
{
    ETutorialState State;
    
    bool CoinDisplayed;
    bool FireworkDisplayed;
    bool BallDisplayed;
    bool BarrelDisplayed;
    bool WallJumpDisplayed;
    bool InitialDisplayed;
    bool JumpDisplayed;
    bool GearDisplayed;

	// Causes the game to ignore the jump key until it's released.
	bool JumpInhibit;
};

extern STutorial Tutorial;


bool GetInput_MoveLeft();
bool GetInput_MoveRight();
bool GetInput_Jump();

void SetGameState_DieScreen();
void SetGameState_WinScreen();
void SetGameState_PauseScreen();
void SetGameState_Crumb(ETutorialState State);

void InitTutorial();
void SkipTutorials();


#endif