//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//


#include "Common.h"
#include "GameScore.h"
#include "Dusty.h"

#include "Barrel.h"
#include "Fan.h"
#include "Coin.h"
#include "TennisBall.h"
#include "FireWorks.h"
#include "Gear.h"
#include "Debris.h"
#include "Lives.h"

SScore Score;

void InitScore()
{
    Score.Counter = 0;
    Score.Type = 0;
    Score.Place = 1;
    Score.Number = 0;
}

void DisplayScore()
{
    AddLitSprite( LIGHTLIST_VACUUM, &Score1Sprite, -40, -30);
    gxDrawString(500, 100, 32, gxRGB32(92, 51, 23), "%d", Score.Counter);
    
    if (Score.Place == 1)
    {
        switch (Score.Number)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 150, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 150, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 150, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 150, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 150, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 150, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 150, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 150, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 150, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 150, 5); break;
        default:   break;   
        }
    }

    if (Score.Place == 2)
    {
        switch (Score.Number)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 150, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 150, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 150, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 150, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 150, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 150, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 150, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 150, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 150, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 150, 5); break;
        default:   break;   
        }
    }

    if (Score.Place == 3)
    {
        switch (Score.Number)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 150, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 150, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 150, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 150, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 150, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 150, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 150, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 150, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 150, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 150, 5); break;
        default:   break;   
        }
    }
    
    if (Score.Place == 4)
    {
        switch (Score.Number)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 150, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 150, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 150, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 150, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 150, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 150, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 150, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 150, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 150, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 150, 5); break;
        default:   break;   
        }
    }

    
    
}

void UpdateScore()
{

    switch (Score.Counter)
    {
    case 0:         Score.Place = 1;
    case 10:        Score.Place = 2;
    case 100:       Score.Place = 3;
    case 1000:      Score.Place = 4;
    case 10000:     Score.Place = 5;
    }
    
    
   //if (Gear.State == GEARSTATE_FALLING)
   //{
   //     Score.Type = 1;
   //     RaiseScore();
   //}
   //
   //if (Ball.State == BALLSTATE_FALLING)
   //{
   //     Score.Type = 1;
   //     RaiseScore();
   //}
   //
   //if (Barrel.State == BARRELSTATE_LAUNCH)
   //{
   //     Score.Type = 2;
   //     RaiseScore();
   //}
   //
   //if (FireWork.State == FIREWORKSTATE_EXPLODE)
   //{
   //     Score.Type = 3;
   //     RaiseScore();
   //}
         
}

void RaiseScore()
{
    switch (Score.Type)
    {
    case 0:    break;
    case 1:    Score.Counter += (1 + Score.Number); break;
    case 2:    Score.Counter += (2 + Score.Number); break;
    case 3:    Score.Counter += (3 + Score.Number); break;
    case 4:    Score.Counter += (4 + Score.Number); break;
    case 5:    Score.Counter += (5 + Score.Number); break;
    default:   break;   
    }
           
    Score.Type = 0;
}