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

SLives Lives;

void InitLives()
{
    Lives.Transition = 5;
	Lives.Sprite = 1;
}

void DisplayLives()
{
	gxSprite* LivesSprites[6] =
		{
			&CoinSpin1Sprite,
			&CoinSpin2Sprite,
			&CoinSpin3Sprite,
			&CoinSpin4Sprite,
			&CoinSpin5Sprite,
			&CoinSpin6Sprite,
		};
 
	AddLitSprite( LIGHTLIST_VACUUM, LivesSprites[Lives.Sprite], 640, -30);
	gxDrawString(550, 0, 16, gxRGB32(255, 255, 255), "Lives: %d", Dusty.Lives);
}

void UpdateLives()
{
	if (Dusty.GainLife)
	{
		Dusty.GainLife = false;
		Dusty.Lives += 1;
	}

	Lives.Transition -= 1;
	if (Lives.Transition == 0)
	{
		Lives.Transition = 5;

		Lives.Sprite += 1;
		if (Lives.Sprite == 6)
		{
			Lives.Sprite = 0;
		}  
	}
}
