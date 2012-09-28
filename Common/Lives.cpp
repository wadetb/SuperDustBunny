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
#include "GameScore.h"


float LivesScaleFactor = 0.85f;
float LivesTextScaleFactor = 1.0f;

int LivesTimer;
int PageTimer;

void ShowPageNumber()
{
    LivesTimer = 0;
    PageTimer = 8*60;
}

void DisplayLives()
{
    if (Chapter.PageProps.VacuumOff)
        return;

    if (PageTimer > 0)
    {
        float Alpha = 1.0f;
        Alpha *= Remap(PageTimer, 7.5f*60, 8*60, 1.0f, 0.0f, true);
        Alpha *= Remap(PageTimer, 0*60, 0.5f*60, 0.0f, 1.0f, true);

        char Work[20];
        snprintf(Work, sizeof(Work), "%d", Portfolio.PageCount+1);
        DisplayStringAlpha(LIGHTLIST_WIPE, Work, FORMAT_ALIGN_RIGHT, (float)gxScreenWidth - 60, 10, LivesTextScaleFactor, Alpha);
    }
    if (LivesTimer > 0)
    {
        float Alpha = 1.0f;
        Alpha *= Remap(LivesTimer, 7.5f*60, 8*60, 1.0f, 0.0f, true);
        Alpha *= Remap(LivesTimer, 0*60, 0.5f*60, 0.0f, 1.0f, true);
        AddLitSpriteScaledAlpha(LIGHTLIST_WIPE, &CoinIconSprite, (float)gxScreenWidth - 90 - 70*LivesScaleFactor*3, -10, 0.65f*LivesScaleFactor, 0.65f*LivesScaleFactor, Alpha);
        
        char Work[20];
        snprintf(Work, sizeof(Work), "x%d", Settings.Lives);
        DisplayStringAlpha(LIGHTLIST_WIPE, Work, 0, (float)gxScreenWidth - 60 - 70*LivesScaleFactor*2, 10, LivesTextScaleFactor, Alpha);
    }
}

void UpdateLives()
{
	if (Dusty.GainLife)
	{
 		Dusty.GainLife = false;
		Settings.Lives += 1;
        Score.TotalLives++;
        PageTimer = 0;
        LivesTimer = 8*60;
        SaveSettings();
	}
    
    LivesTimer--;
    PageTimer--;
}
