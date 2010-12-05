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

struct SChapterIntro
{
	int Timer;
};

SChapterIntro ChapterIntro;

void InitChapterIntro()
{
	ChapterIntro.Timer = 0;

	TurnOffVacuum();

	SetDustyPosition(Chapter.StartX - 300, Chapter.StartY);
	Dusty.NoCollision = true;
	Dusty.CollideWithBottomSide = true;

	RemoteControl.Enabled = true;
	RemoteControl.MoveRight = true;

	SetDustyState_Hop(DIRECTION_RIGHT);
}

void DisplayChapterIntro()
{
	DisplayGame_Playing(); 

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
}

void ChapterIntro_Advance()
{
	TurnOnVacuum();
	
	Dusty.NoCollision = false;
	SetDustyState_Stand();

	RemoteControl.Enabled = false;

	SetGameState_Playing();
}

void UpdateChapterIntro()
{
	UpdateDusty();

	// Advance to playing state when Timer Expires.
	if (ChapterIntro.Timer >= 40)
	{
		ChapterIntro_Advance();
		return;
	}
	ChapterIntro.Timer += 1;
}