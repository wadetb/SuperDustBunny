//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Lives.h"
#include "Settings.h"
#include "Dusty.h"
#include "Chapter.h"
#include "Text.h"


float LivesScaleFactor = 0.85f;
float LivesTextScaleFactor = 1.0f;


void DisplayLives()
{
    if (Chapter.PageProps.VacuumOff)
        return;

    AddLitSpriteScaled(LIGHTLIST_WIPE, &CoinSpin1Sprite, (float)gxScreenWidth - 80 - 70*LivesScaleFactor*4, -20, LivesScaleFactor, LivesScaleFactor);
    
    char Work[20];
    snprintf(Work, sizeof(Work), "x%d", Settings.Lives);
    DisplayString(LIGHTLIST_WIPE, Work, 0, (float)gxScreenWidth - 60 - 70*LivesScaleFactor*3, 10, LivesTextScaleFactor);
}

void UpdateLives()
{
	if (Dusty.GainLife)
	{
 		Dusty.GainLife = false;
		Settings.Lives += 1;
        SaveSettings();
	}
}
