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
	gxDrawRectangleFilled(0, 0, 768, 1024, 0xffffffff);

	float t = DieScreen.Timer / 10.0f;
	float dx = cos(t/5)*10 + cos(1+t/7)*10 + cos(1-t/9)*10 + 100;
	float dy = sin(t/5)*10 + sin(1+t/7)*10 + sin(1-t/9)*10 - Min(t*2, 175.0f);

	float alpha = Min(t, 1.0f);

	gxDrawSpriteAlpha(30, 600, alpha, DieScreen.Pressed ? &ScreenLoseGrave2Sprite : &ScreenLoseGrave1Sprite);
	gxDrawSpriteAlpha(768/2-ScreenLoseGhostSprite.width/2 + (int)dx, 200+(int)dy, alpha*alpha*0.5f, &ScreenLoseGhostSprite);
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
