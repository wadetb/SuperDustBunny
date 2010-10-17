//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "StartScreen.h"

enum 
{
	STARTSCREEN_ITEM_HELP,
	STARTSCREEN_ITEM_START,
	STARTSCREEN_ITEM_CREDITS,
	STARTSCREEN_ITEM_COUNT
};

gxSprite* StartScreenIcons[STARTSCREEN_ITEM_COUNT] =
{
	&IconHelp1Sprite,
	&IconStart1Sprite,
	&IconCredits1Sprite
};

gxSprite* StartScreenPressedIcons[STARTSCREEN_ITEM_COUNT] =
{
	&IconHelp2Sprite,
	&IconStart2Sprite,
	&IconCredits2Sprite
};

struct SStartScreen
{
	int CurItem;
	
	float X;
	float PrevX;
	float DragX;

	bool Dragging;
	bool Pressed;
};

SStartScreen StartScreen;

void InitStartScreen()
{
	StartScreen.CurItem = STARTSCREEN_ITEM_START;
	StartScreen.X = StartScreen.CurItem * 600.0f;
	StartScreen.PrevX = StartScreen.X;
}

void StartScreen_Advance()
{
	if (StartScreen.CurItem == STARTSCREEN_ITEM_START)
	{
		LoadCurrentChapter();
		SetGameState_Playing();
	}
	//else if (StartScreen.CurItem == STARTSCREEN_ITEM_HELP)
	//	SetGameState_Help();
	//else if (StartScreen.CurItem == STARTSCREEN_ITEM_CREDITS)
	//	SetGameState_Credits();
}

void DisplayStartScreen()
{
	gxDrawSprite( 0, 0, &ScreenStart2Sprite );

	for (int i = 0; i < STARTSCREEN_ITEM_COUNT; i++)
	{
		float Alpha = 1.0f;
		if (i != StartScreen.CurItem)
			Alpha = 0.5f;

		bool Pressed = false;
		if (StartScreen.Pressed && i == StartScreen.CurItem)
			Pressed = true;

		gxDrawSpriteAlpha(768/2-600/2 + i*600 - (int)StartScreen.X, 420, Alpha, (Pressed ? StartScreenPressedIcons : StartScreenIcons)[i]);
	}
}

void UpdateStartScreen()
{
#ifdef PLATFORM_WINDOWS
	// Keyboard shortcuts for Windows
	StartScreen.Pressed = kbIsKeyDown(KB_RETURN);

	if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
	{
		StartScreen_Advance();
		return;
	}

	if (kbIsKeyDown(KB_A))
	{
		StartScreen.X -= 100;
	}
	if (kbIsKeyDown(KB_D))
	{
		StartScreen.X += 100;
	}
#endif

	if (StartScreen.Dragging)
	{
		if (!msButton1)
		{
			if (StartScreen.Pressed)
			{
				StartScreen_Advance();
			}

			StartScreen.Dragging = false;
			StartScreen.Pressed = false;
		}
		else 
		{
			if (abs(msY - StartScreen.DragX) > 10)
				StartScreen.Pressed = false;

			StartScreen.X += msY - StartScreen.DragX;
			StartScreen.DragX = (float)msY;
		}
	}
	else
	{
		if (msButton1)
		{
			StartScreen.Pressed = true;
			StartScreen.Dragging = true;
			StartScreen.DragX = (float)msY;
		}
		else 
		{
			StartScreen.X = StartScreen.X * 0.8f + StartScreen.CurItem * 600.0f * 0.2f;
			StartScreen.X = StartScreen.X + (StartScreen.X-StartScreen.PrevX) * 0.25f;
		}
	}

	if (StartScreen.X < 0)
		StartScreen.X = 0;
	if (StartScreen.X >= (STARTSCREEN_ITEM_COUNT-1)*600)
		StartScreen.X = (STARTSCREEN_ITEM_COUNT-1)*600;

	StartScreen.CurItem = (int)Round(StartScreen.X / 600.0f);

	StartScreen.PrevX = StartScreen.X;
}

