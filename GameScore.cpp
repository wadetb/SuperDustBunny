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
   
    //If Dusty hits Gear or Tennis Ball
    // Set Score.Type to 1
    // RaiseScore();
    
    //If Dusty Launches from Barrel
    // Set Score.Type to 2
    // RaiseScore();
    
    //If Dusty Jumps with the fan
    // Set Score.Type to 3
    // RaiseScore();
    
    //If Dusty Sets off Fireworks
    // Set Score.Type to 4
    // RaiseScore();
    
    //If Dusty Knocks down debris
    //Set Score.type to 5
    //RaiseScore();
      
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
}