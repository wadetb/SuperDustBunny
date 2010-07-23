#ifndef COMMON_H
#define COMMON_H

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
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

// Macros

extern bool DevMode;
extern int ScrollY;

#define White gxRGB32(255, 255, 255)
#define Black gxRGB32(0, 0, 0)

inline int Max(int a, int b)
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

inline int Min(int a, int b)
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


extern gxSprite DustyHop01;
extern gxSprite DustyHop02;
extern gxSprite DustyHop03;
extern gxSprite DustyHop04;
extern gxSprite DustyHop05;
extern gxSprite LeftFaceStanding01;
extern gxSprite LeftFaceStanding02;
extern gxSprite RightFaceStanding01;
extern gxSprite RightFaceStanding02;
extern gxSprite LeftFaceWallJump01;
extern gxSprite RightFaceWallJump01;
extern gxSprite DustyDeath01;
extern gxSprite DustyHopLeft01;
extern gxSprite DustyHopLeft02;
extern gxSprite DustyHopLeft03;
extern gxSprite DustyHopLeft04;
extern gxSprite DustyHopLeft05;
extern gxSprite DustyHopLeft06;
extern gxSprite WoodBox_Platform01;
extern gxSprite Background01;
extern gxSprite UnknownBlock;
extern gxSprite BarrelSprite;

extern gxSprite Coin01;
extern gxSprite Coin02;
extern gxSprite Coin03;
extern gxSprite Coin04;
extern gxSprite Coin05;
extern gxSprite Coin06;
extern gxSprite Coin07;
extern gxSprite Coin08;
extern gxSprite Coin09;
extern gxSprite Coin010;
extern gxSprite Coin011;
extern gxSprite Coin012;

extern sxSound DustyToJump;
extern sxSound DustyJumps;
extern sxSound WallJump;
extern sxSound LaunchSound01;
extern sxSound BackgroundSong01;
extern sxSound BackgroundSong02;
extern sxSound BackgroundSong03;

bool GetInput_MoveLeft();
bool GetInput_MoveRight();
bool GetInput_Jump();

void SetGameState_DieScreen();

#endif