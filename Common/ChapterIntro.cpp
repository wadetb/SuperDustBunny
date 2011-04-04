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
#include "Settings.h"


struct SChapterIntro
{
	int Timer;
};


SChapterIntro ChapterIntro;


void ChapterIntro_Advance();

void InitChapterIntro()
{
    ChapterIntro.Timer = 0;

	TurnOffVacuum();

    if (Settings.ControlStyle == CONTROL_TILT)
    {
        SetDustyPosition(Chapter.StartX - 400, Chapter.StartY);
        Dusty.NoCollision = true;
        Dusty.CollideWithBottomSide = true;

        RemoteControl.Enabled = true;
        RemoteControl.MoveRight = true;

        SetDustyState_Hop(DIRECTION_RIGHT);
    }
}

void DisplayChapterIntro()
{
	if (ChapterIntro.Timer <= 40)
	{
		AddLitSprite( LIGHTLIST_VACUUM, &ChapterTitle, 0, -100);
	}
	else
	{
		AddLitSprite( LIGHTLIST_VACUUM, &ChapterTitle, 0, -100 - (20 * (float)(ChapterIntro.Timer - 40)));
	}
}

void ChapterIntro_Advance()
{
	TurnOnVacuum();

    if (Settings.ControlStyle == CONTROL_TILT)
    {
        RemoteControl.Enabled = false;
        Dusty.NoCollision = false;
    }

	SetGameState_Playing();
}

void UpdateChapterIntro()
{
	ChapterIntro.Timer += 1;
	
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        UpdateDusty();

        if (ChapterIntro.Timer == 40)
        {
            RemoteControl.MoveRight = false;
        }
    }

	// Advance to playing state when Timer Expires.
	if (ChapterIntro.Timer >= 100)
	{
		ChapterIntro_Advance();
		return;
	}
	
}