//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "ChapterIntro.h"
#include "Vacuum.h"
#include "Dusty.h"

struct SChapterIntro
{
	bool Pressed;
};

SChapterIntro ChapterIntro;

void InitChapterIntro()
{
	ChapterIntro.Pressed = false;
}

void DisplayChapterIntro()
{
	switch (Dusty.ChapterTimer)
	{
	case 100:  gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 90:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 80:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 70:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 60:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 50:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 40:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 30:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 20:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	case 10:   gxDrawSprite(90, 5800, &ChapterTitle); break;
	default:    break;
	}
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
	if (Dusty.ChapterTimer == 0)
	{
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
}