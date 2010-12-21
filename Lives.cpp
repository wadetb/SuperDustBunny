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
	switch(Dusty.Lives)
	{
	case 1:     AddLitSprite( LIGHTLIST_VACUUM, &CoinLife1Sprite, 640, -30); break;
    case 2:     AddLitSprite( LIGHTLIST_VACUUM, &CoinLife2Sprite, 640, -30); break;
    case 3:     AddLitSprite( LIGHTLIST_VACUUM, &CoinLife3Sprite, 640, -30); break;
    case 4:     AddLitSprite( LIGHTLIST_VACUUM, &CoinLife4Sprite, 640, -30); break;
    case 5:     AddLitSprite( LIGHTLIST_VACUUM, &CoinLife5Sprite, 640, -30); break;
    case 6:     AddLitSprite( LIGHTLIST_VACUUM, &CoinLife6Sprite, 640, -30); break;
    default:    break;	
	}
	gxDrawString(550, 0, 16, gxRGB32(255, 255, 255), "Lives: %d", Dusty.Lives);
}

void UpdateLives()
{
	if (Dusty.GainLife)
	{
		Dusty.GainLife = false;
		Dusty.Lives += 1;
		if (Dusty.Lives > 6)
		{
			Dusty.Lives = 6;
		}
	}


}
