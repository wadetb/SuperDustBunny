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

	TurnOffVacuum();
}

void DisplayChapterIntro()
{
	AddLitSprite(LIGHTLIST_VACUUM, &ChapterTitleSprite, 0, ChapterIntro.Y);
	AddLitSprite(LIGHTLIST_VACUUM, &ChapterTextSprite, 220, ChapterIntro.Y+600);
	int Chapter = 1;
	switch (Chapter)
	{
	case 1: AddLitSubSprite(LIGHTLIST_VACUUM, &ChapterNumbersSprite, 220+305, ChapterIntro.Y+600, 0, 0, 64, 64); break;
	}
}

void ChapterIntro_Advance()
{
    if (!Chapter.PageProps.VacuumOff)
        TurnOnVacuum();

	Dusty.NoCollision = false;
	RemoteControl.Enabled = false;

	SetGameState_Playing();
}

void UpdateChapterIntro()
{
#ifdef PLATFORM_WINDOWS
	if (kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE))
	{
		ChapterIntro.Tapped = true;
	}
#endif
#ifdef PLATFORM_IPHONE
	if (msButton1 && !msOldButton1)
	{
		ChapterIntro.Tapped = true;
	}
#endif

	if (Dusty.FloatX >= Chapter.StartX)
    {
		RemoteControl.MoveRight = false;
        ChapterIntro.Tapped = true;
    }

	UpdateDusty();
	UpdateDust();

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
