//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//


#include "Common.h"
#include "Dusty.h"
#include "Lives.h"

float LivesScaleFactor = 0.85f;

void DisplayLives()
{
	for (int i = 0; i < Dusty.Lives; i++)
	{
		AddLitSpriteScaled(LIGHTLIST_WIPE, &CoinSpin1Sprite, (float)gxScreenWidth - 100 - 70*LivesScaleFactor*i, -20, LivesScaleFactor, LivesScaleFactor);
	}
}

void UpdateLives()
{
	if (Dusty.GainLife)
	{
 		Dusty.GainLife = false;
		Dusty.Lives += 1;
		if (Dusty.Lives > 5)
		{
			Dusty.Lives = 5;
		}
	}
}
