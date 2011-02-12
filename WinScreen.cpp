//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "WinScreen.h"

struct SWinScreen
{
	bool Pressed;
};

SWinScreen WinScreen;

void InitWinScreen()
{
	WinScreen.Pressed = false;
}

void DisplayWinScreen()
{
	if (WinScreen.Pressed)
	{
		AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenWin2Sprite, 0, 0);
	}
	else
	{
		AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenWin1Sprite, 0, 0);
	}
}

void WinScreen_Advance()
{
	SetGameState_StartScreen();
}

void UpdateWinScreen()
{
#ifdef PLATFORM_WINDOWS
	bool Pressed = kbIsKeyDown(KB_RETURN) || msButton1;
#endif
#ifdef PLATFORM_IPHONE
	bool Pressed = msButton1;
#endif

	// Advance to playing state when button is released.
	if (!Pressed && WinScreen.Pressed)
	{
		WinScreen_Advance();
		return;
	}

	WinScreen.Pressed = Pressed;
}

