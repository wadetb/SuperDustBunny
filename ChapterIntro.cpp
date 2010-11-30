//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Chapter.h"
#include "ChapterIntro.h"
#include "Vacuum.h"
#include "Dusty.h"

#include "Lighting.h"
#include "Barrel.h"
#include "Fan.h"
#include "Dusty.h"
#include "Coin.h"
#include "Vacuum.h"
#include "TennisBall.h"
#include "FireWorks.h"
#include "Dust.h"
#include "Crumb.h"
#include "Gear.h"
#include "Wipe.h"
#include "Flashlight.h"

struct SChapterIntro
{
	bool Pressed;
	int Timer;
};

SChapterIntro ChapterIntro;

void InitChapterIntro()
{
	ChapterIntro.Pressed = false;
	ChapterIntro.Timer = 2;
}

void DisplayChapterIntro()
{
	switch (ChapterIntro.Timer)
	{
	case 2:		gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 4:		gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 6:		gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 8:		gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 10:	gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 12:	gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 14:	gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 16:	gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 18:	gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 20:	gxDrawSprite(90, 5800, &ChapterTitle); break;
	default:    break;
	}

	DisplayGame_Playing(); 
}

void ChapterIntro_Advance()
{
	LoadCurrentChapter();
	TurnOnVacuum();
	SetGameState_Playing();
}

void UpdateChapterIntro()
{
	// Advance to playing state when Timer Expires.
	if (ChapterIntro.Timer >= 30)
	{
		ChapterIntro.Timer = 0;
		#ifdef PLATFORM_WINDOWS
			bool Pressed = kbIsKeyDown(KB_RETURN) || msButton1;
		#endif
		#ifdef PLATFORM_IPHONE
			bool Pressed = msButton1;
		#endif
		// Advance to playing state when button is released.
		if (!Pressed && ChapterIntro.Pressed)
		{
			ChapterIntro_Advance();
			return;
		}
		ChapterIntro.Pressed = Pressed;
	}
	ChapterIntro.Timer += 1;
}