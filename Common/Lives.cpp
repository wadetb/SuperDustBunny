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

void DisplayLives()
{
	for (int i = 0; i < Dusty.Lives; i++)
	{
		AddLitSpriteScaled(LIGHTLIST_WIPE, &CoinSpin1Sprite, (float)gxScreenWidth - 100 - 45*i, -20, 0.65f, 0.65f);
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
