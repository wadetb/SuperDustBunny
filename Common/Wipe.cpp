//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Wipe.h"


SWipe Wipe;


void StartWipe(EWipeType Type, float Time)
{
	Wipe.Type = Type;
	Wipe.Time = Time;
	Wipe.T = 0.0f;
	Wipe.Middle = false;
	Wipe.Finished = false;
}

void UpdateWipe()
{
	if (!Wipe.Finished)
	{
		float OldT = Wipe.T;
		Wipe.T += (1.0f/60.0f) / Wipe.Time;

		if (OldT < 0.5f && Wipe.T >= 0.5f)
		{
			Wipe.Middle = true;
		}
		if (Wipe.T >= 1.0f)
		{
			Wipe.Finished = true;
		}
	}
}

void DisplayWipe()
{
	if (Wipe.Type == WIPE_DIAGONAL)
	{
		float TotalWidth = gxScreenWidth*2 + 1000.0f;

		float TopX1 = gxScreenWidth - TotalWidth*Wipe.T;
		float TopX2 = TopX1 + gxScreenWidth + 800.0f;
		float BotX1 = gxScreenWidth + 200.0f - TotalWidth*Wipe.T;
		float BotX2 = BotX1 + gxScreenWidth + 800.0f;

        AddLitQuad(LIGHTLIST_WIPE, &WipeDiagonalSprite, gxRGBA32(0, 0, 0, 255),
           TopX1, 0,                       0.0f, 0.0f, 
           TopX2, 0,                       1.0f, 0.0f,
           BotX2, (float)LitScreenHeight,  1.0f, 1.0f, 
           BotX1, (float)LitScreenHeight,  0.0f, 1.0f);
	}
	else
	if (Wipe.Type == WIPE_FADE_TO_BLACK)
	{
		int Alpha = (int)(sinf(Wipe.T*PI)*255);

        AddLitQuad(LIGHTLIST_WIPE, &WhiteSprite, gxRGBA32(0, 0, 0, Alpha),
			0,                    0,                       0.0f, 0.0f, 
			(float)gxScreenWidth, 0,                       1.0f, 0.0f,
			(float)gxScreenWidth, (float)LitScreenHeight,  1.0f, 1.0f, 
			0,                    (float)LitScreenHeight,  0.0f, 1.0f);
	}
	else
	if (Wipe.Type == WIPE_FADE_TO_WHITE)
	{
		int Alpha;
		if (Wipe.T < 0.5f)
			Alpha = (int)Lerp(Wipe.T, 0.0f, 0.5f, 0.0f, 255.0f);
		else
			Alpha = (int)Lerp(Wipe.T, 0.5f, 1.0f, 255.0f, 0.0f);

        AddLitQuad(LIGHTLIST_WIPE, &WhiteSprite, gxRGBA32(255, 255, 255, Alpha),
			0,                    0,                       0.0f, 0.0f, 
			(float)gxScreenWidth, 0,                       1.0f, 0.0f,
			(float)gxScreenWidth, (float)LitScreenHeight,  1.0f, 1.0f, 
			0,                    (float)LitScreenHeight,  0.0f, 1.0f);
	}
}

