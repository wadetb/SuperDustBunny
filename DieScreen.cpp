//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "DieScreen.h"

struct SDieScreen
{
	bool Pressed;
	int Timer;
};

SDieScreen DieScreen;

void InitDieScreen()
{
	DieScreen.Pressed = false;
	DieScreen.Timer = 0;
}

void DisplayDieScreen()
{
	ResetLighting();

	AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundCardboardSprite, 0, 0);

	float t = DieScreen.Timer / 10.0f;
	float dx = cos(t/5)*10 + cos(1+t/7)*10 + cos(1-t/9)*10 + 100;
	float dy = sin(t/5)*10 + sin(1+t/7)*10 + sin(1-t/9)*10 - Min(t*2, 175.0f);

	float Alpha = 1.0f; //Min(t, 1.0f);

	AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, DieScreen.Pressed ? &ScreenLoseGrave2Sprite : &ScreenLoseGrave1Sprite, 384, 800, 1.0f, Alpha);
	AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &ScreenLoseGhostSprite, 768/2 + dx, 200+ScreenLoseGhostSprite.height/2+dy, 1.0f, Alpha*Alpha*0.5f);

	RenderLighting();
}

void DieScreen_Advance()
{
	SetGameState_StartScreen();
}

void UpdateDieScreen()
{
#ifdef PLATFORM_WINDOWS
	bool Pressed = kbIsKeyDown(KB_RETURN) || msButton1;
#endif
#ifdef PLATFORM_IPHONE
	bool Pressed = msButton1;
#endif

	// Advance to playing state when button is released.
	if (!Pressed && DieScreen.Pressed)
	{
		DieScreen_Advance();
		return;
	}

	DieScreen.Pressed = Pressed;

	DieScreen.Timer++;
}
