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
    Score.CounterSingle = 0;
    Score.CounterTen = 0;
    Score.CounterHundred = 0;
    Score.CounterThousand = 0;
    Score.CounterTenThousand = 0;
    
    Score.Type = 0;
    Score.ToRaise = 0;
    
    Score.Raise = false;
    Score.Single = true;
    Score.Ten = false;
    Score.Hundred = false;
    Score.Thousand = false;
    Score.TenThousand = false;
}

void DisplayScore()
{
    AddLitSprite( LIGHTLIST_VACUUM, &Score1Sprite, -40, -30);

    if (Score.Single)
    {
        switch (Score.CounterSingle)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 280, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 280, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 280, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 280, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 280, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 280, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 280, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 280, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 280, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 280, 5); break;
        default:   break;   
        }
    }

    if (Score.Ten)
    {
        switch (Score.CounterTen)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 250, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 250, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 250, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 250, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 250, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 250, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 250, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 250, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 250, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 250, 5); break;
        default:   break;   
        }
    }

    if (Score.Hundred)
    {
        switch (Score.CounterHundred)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 220, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 220, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 220, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 220, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 220, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 220, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 220, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 220, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 220, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 220, 5); break;
        default:   break;   
        }
    }

    if (Score.Thousand)
    {
        switch (Score.CounterThousand)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 190, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 190, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 190, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 190, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 190, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 190, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 190, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 190, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 190, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 190, 5); break;
        default:   break;   
        }
    }

    if (Score.TenThousand)
    {
        switch (Score.CounterTenThousand)
        {
        case 0:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber0Sprite, 160, 5); break;
        case 1:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber1Sprite, 160, 5); break;
        case 2:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber2Sprite, 160, 5); break;
        case 3:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber3Sprite, 160, 5); break;
        case 4:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber4Sprite, 160, 5); break;
        case 5:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber5Sprite, 160, 5); break;
        case 6:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber6Sprite, 160, 5); break;
        case 7:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber7Sprite, 160, 5); break;
        case 8:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber8Sprite, 160, 5); break;
        case 9:    AddLitSprite( LIGHTLIST_VACUUM, &ScoreNumber9Sprite, 160, 5); break;
        default:   break;   
        }
    }                      
}

void ResetScore()
{
    Score.Single = true;
    Score.Ten = false;
    Score.Hundred = false;
    Score.Thousand = false;
    Score.TenThousand = false;

    Score.CounterSingle = 0;
    Score.CounterTen = 0;
    Score.CounterHundred = 0;
    Score.CounterThousand = 0;
    Score.CounterTenThousand = 0;    
}

void UpdateScore()
{       
    switch (Score.Type)
    {
    case 0:    return;
    case 1:    Score.ToRaise = 1; break;
    case 2:    Score.ToRaise = 2; break;
    case 3:    Score.ToRaise = 3; break;
    case 4:    Score.ToRaise = 4; break;
    case 5:    Score.ToRaise = 5; break;
    default:   break;   
    }

    if (Score.Raise)
    {
        for (int i = 0; i < Score.ToRaise; i++)
        {
            Score.CounterSingle += 1;    
        }
        
        if (Score.CounterSingle > 9)
        {
            Score.CounterSingle = (Score.CounterSingle - 10);
            Score.CounterTen += 1;        
            Score.Ten = true;
        }
        
        if (Score.CounterTen > 9)
        {
            Score.CounterTen = (Score.CounterTen - 10);
            Score.CounterHundred += 1;
            Score.Hundred = true;   
        }

        if (Score.CounterHundred > 9)
        {
            Score.CounterHundred = (Score.CounterHundred - 10);
            Score.CounterThousand += 1;
            Score.Thousand = true;
        }

        if (Score.CounterThousand > 9)
        {
            Score.CounterThousand = (Score.CounterThousand - 10);
            Score.CounterTenThousand += 1;
            Score.TenThousand = true;
        }

        if (Score.CounterTenThousand > 9)
        {
            Score.CounterTenThousand = (Score.TenThousand - 10);
        }
        
        Score.Raise = false;
        Score.Type = 0;    
    }         
}