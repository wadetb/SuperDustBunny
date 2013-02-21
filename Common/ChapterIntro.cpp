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
#include "Dust.h"
#include "Settings.h"
#include "Input.h"
#include "Ghost.h"
#include "Text.h"


struct SChapterIntro
{
	float Y;
	float VelocityY;
	bool Tapped;
};


SChapterIntro ChapterIntro;


void InitChapterIntro()
{
	ChapterIntro.Tapped = false;
	ChapterIntro.Y = -100;
	ChapterIntro.VelocityY = 0;

	Dusty.NoCollision = true;
	Dusty.FloatX = Chapter.StartX - 350;
	RemoteControl.MoveRight = true;
	RemoteControl.Enabled = true;
	SetDustyState_IntroStand();
	Dusty.SpriteTransition = 10;
}

void HideChapterIntro()
{
	ChapterIntro.Y = -1000;
}

void DisplayChapterIntro()
{
	AddLitSprite(LIGHTLIST_VACUUM, &ChapterTitleSprite, 0, ChapterIntro.Y);
    if (Chapter.Title)
        DisplayMultilineStringAlpha(LIGHTLIST_VACUUM, Chapter.Title, FORMAT_CENTER_X, 400, ChapterIntro.Y+560, 1.25f, 0.75f);
}

static void ChapterIntro_Advance()
{
	Dusty.NoCollision = false;
	RemoteControl.Enabled = false;

	SetGameState_Playing();
}

void UpdateChapterIntro()
{
	if (msButton1 && !msOldButton1)
	{
		ChapterIntro.Tapped = true;
	}

	if (Dusty.FloatX >= Chapter.StartX)
    {
		RemoteControl.MoveRight = false;
        ChapterIntro.Tapped = true;
    }

	UpdateDusty();
	UpdateDust();
    UpdateGhost();

	if ( ChapterIntro.Tapped )
	{
		ChapterIntro.Y += ChapterIntro.VelocityY;
		ChapterIntro.VelocityY -= 1.0f;

		// Advance to playing state when Timer Expires.
		if (ChapterIntro.Y < -ChapterTitleSprite.height)
		{
			ChapterIntro_Advance();
			return;
		}
	}
}
