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
    Score.CounterSingle = 5;
    Score.CounterTen = 4;
    Score.CounterHundred = 3;
    Score.CounterThousand = 2;
    Score.CounterTenThousand = 6;
    
    Score.Type = 0;
    Score.ToRaise = 0;
    Score.Left = 0;
    
    Score.Raise = false;
    Score.Single = true;
    Score.Ten = true;
    Score.Hundred = true;
    Score.Thousand = true;
    Score.TenThousand = true;
}

void DisplayScore()
{
    if (Score.Single)
    {
        switch (Score.CounterSingle)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 84, 100); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 84, 0, 116, 100); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 116, 0, 202, 100); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 202, 0, 290, 100); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 290, 0, 372, 100); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 372, 0, 456, 100); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 456, 0, 542, 100); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 542, 0, 626, 100); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 626, 0, 708, 100); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 708, 0, 796, 100); break;
        default:   break;   
        }
    }

    if (Score.Ten)
    {
        switch (Score.CounterTen)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 84, 100); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 84, 0, 116, 100); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 116, 0, 202, 100); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 202, 0, 290, 100); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 290, 0, 372, 100); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 372, 0, 456, 100); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 456, 0, 542, 100); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 542, 0, 626, 100); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 626, 0, 708, 100); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 708, 0, 796, 100); break;
        default:   break;   
        }
    }

    if (Score.Hundred)
    {
        switch (Score.CounterHundred)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 84, 100); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 84, 0, 116, 100); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 116, 0, 202, 100); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 202, 0, 290, 100); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 290, 0, 372, 100); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 372, 0, 456, 100); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 456, 0, 542, 100); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 542, 0, 626, 100); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 626, 0, 708, 100); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 708, 0, 796, 100); break;
        default:   break;   
        }
    }

    if (Score.Thousand)
    {
        switch (Score.CounterThousand)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 84, 100); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 84, 0, 116, 100); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 116, 0, 202, 100); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 202, 0, 290, 100); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 290, 0, 372, 100); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 372, 0, 456, 100); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 456, 0, 542, 100); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 542, 0, 626, 100); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 626, 0, 708, 100); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 708, 0, 796, 100); break;
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
        if (Score.Left == Score.ToRaise)
        {
            Score.Raise = false;
            Score.Type = 0;
            Score.Left = 0;
        }
        
        Score.CounterSingle += 1;
        Score.Left += 1;
        
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
    }         
}