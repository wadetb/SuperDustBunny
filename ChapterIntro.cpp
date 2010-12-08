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
	if (ChapterIntro.Timer <= 200)
	{
		AddLitSprite( LIGHTLIST_VACUUM, &ChapterTitle, 50, -450 + (2 * ChapterIntro.Timer));
	}
	else
	{
		AddLitSprite( LIGHTLIST_VACUUM, &ChapterTitle, 50, -50);
	}
}

void ChapterIntro_Advance()
{
	TurnOnVacuum();

	Dusty.NoCollision = false;

	SetDustyState_Stand();
	
	SetGameState_Playing();
}

void UpdateChapterIntro()
{
	ChapterIntro.Timer += 1;
	
	UpdateDusty();

	// Advance to playing state when Timer Expires.
	if (ChapterIntro.Timer >= 40)
	{
		RemoteControl.Enabled = false;
	}

	if (ChapterIntro.Timer >= 300)
	{
		ChapterIntro_Advance();
		return;
	}
	
}