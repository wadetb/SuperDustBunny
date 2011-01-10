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
}

void DisplayScore()
{
    AddLitSprite( LIGHTLIST_VACUUM, &Score1Sprite, -40, -30);
    gxDrawString(500, 100, 32, gxRGB32(92, 51, 23), "%d", Score.Counter);
}

void UpdateScore()
{
   if (Gear.State == GEARSTATE_FALLING)
   {
        Score.Type = 1;
        RaiseScore();
   }
   
   if (Ball.State == BALLSTATE_FALLING)
   {
        Score.Type = 1;
        RaiseScore();
   }
   
   if (Barrel.State == BARRELSTATE_LAUNCH)
   {
        Score.Type = 2;
        RaiseScore();
   }
   
   if (FireWork.State == FIREWORKSTATE_EXPLODE)
   {
        Score.Type = 3;
        RaiseScore();
   }
         
}

void RaiseScore()
{
    switch (Score.Type)
    {
    case 0:    break;
    case 1:    Score.Counter += 1; break;
    case 2:    Score.Counter += 5; break;
    case 3:    Score.Counter += 10; break;
    case 4:    Score.Counter += 15; break;
    case 5:    Score.Counter += 20; break;
    default:   break;   
    }
    
    Score.Type = 0;
}