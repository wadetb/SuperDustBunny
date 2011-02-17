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
#include "Dusty.h"


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
	&IconCredits1Sprite,
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
    
    float PressedTime;
    float StartupTime;
};

SStartScreen StartScreen;


extern "C" void ShowSettings();


void InitStartScreen()
{
	StartScreen.CurItem = STARTSCREEN_ITEM_START;
	StartScreen.X = StartScreen.CurItem * 600.0f;
	StartScreen.PrevX = StartScreen.X;
    StartScreen.PressedTime = 0.0f;
    StartScreen.StartupTime = 0.0f;
}

void StartScreen_Advance()
{
	if (StartScreen.CurItem == STARTSCREEN_ITEM_START)
	{
		Dusty.Lives = 0;

		SetGameState_Transition(GAMETRANSITION_FIRST_PAGE);
	}
	else if (StartScreen.CurItem == STARTSCREEN_ITEM_HELP)
	{
		SetGameState_Help();
	}
	else if (StartScreen.CurItem == STARTSCREEN_ITEM_CREDITS)
	{
        ShowSettings();
//		SetGameState_Credits();
    }
}

void DisplayStartScreen()
{
	AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenStart2Sprite, 0, 0 );

	for (int i = 0; i < STARTSCREEN_ITEM_COUNT; i++)
	{
        float X = 384 + i*600 - (int)StartScreen.X;
        float Y = 720;

        if (i == StartScreen.CurItem)
        {
            float OverlayAlpha = SinWave(StartScreen.PressedTime, 1.0f);
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, StartScreenIcons[i], X, Y, 1.0, 1.0f);
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, StartScreenPressedIcons[i], X, Y, 1.0, OverlayAlpha);
        }
        else
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, StartScreenIcons[i], X, Y, 1.0, 1.0f);
        }
	}

    if (StartScreen.StartupTime < 1.0f)
    {
        float Alpha = Max(0, 1.0f - StartScreen.StartupTime);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, 0, Alpha);
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

            StartScreen.CurItem = (int)Round(StartScreen.X / 600.0f);
        }
		else 
		{
			if (abs(msX - StartScreen.DragX) > 10)
				StartScreen.Pressed = false;

			StartScreen.X += StartScreen.DragX - msX;
			StartScreen.DragX = (float)msX;
		}
        
	}
	else
	{
		if (msButton1)
		{
			StartScreen.Pressed = true;
			StartScreen.Dragging = true;
			StartScreen.DragX = (float)msX;
            StartScreen.PressedTime = 0.0f;
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

	StartScreen.PrevX = StartScreen.X;
    
    if (StartScreen.Pressed)
    {
        if (StartScreen.PressedTime < 0.5f)
            StartScreen.PressedTime += 1.0f/60.0f;        
    }
    else
    {
        if (StartScreen.PressedTime > 0.0f)
            StartScreen.PressedTime -= 1.0f/60.0f;
    }
    
    StartScreen.StartupTime += 1.0f/60.0f;
}
