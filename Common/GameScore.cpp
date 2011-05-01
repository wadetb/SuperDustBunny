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
    Score.CounterTen = 0;
    Score.CounterHundred = 0;
    Score.CounterThousand = 0;
    Score.CounterTenThousand = 0;
    
    Score.Type = 0;
    Score.ToRaise = 0;
    Score.Left = 0;
    
    Score.Raise = false;
    Score.Single = true;
    Score.Ten = false;
    Score.Hundred = false;
    Score.Thousand = false;
    Score.TenThousand = false;
    
    Score.PageTime = 0;
}

void DisplayDigit(int Digit, int X, int Y)
{
    switch (Digit)
    {
        case 0:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 1,   0, 54,  63, 0.5f, 0.5f); break;        
        case 1:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X+10, Y, 55,  0, 74,  63, 0.5f, 0.5f); break;
        case 2:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 76,  0, 131, 63, 0.5f, 0.5f); break;
        case 3:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 133, 0, 187, 63, 0.5f, 0.5f); break;
        case 4:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 189, 0, 242, 63, 0.5f, 0.5f); break;
        case 5:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 243, 0, 296, 63, 0.5f, 0.5f); break;
        case 6:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 298, 0, 353, 63, 0.5f, 0.5f); break;
        case 7:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 354, 0, 406, 63, 0.5f, 0.5f); break;
        case 8:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 408, 0, 462, 63, 0.5f, 0.5f); break;
        case 9:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 463, 0, 520, 63, 0.5f, 0.5f); break;
        default:   break;   
    }    
}

void DisplayPeriod(int X, int Y)
{
	AddLitSubSpriteScaled( LIGHTLIST_WIPE, &FullScoreSprite, X, Y, 55,  0, 74,  63, 0.5f, 0.5f/4.0f);
}

void DisplayScore()
{
    int Seconds = Score.PageTime / 60;
    int Hundredths = ( Score.PageTime % 60 ) * 60 / 100;
    
	DisplayDigit(Seconds/60, 42, 10);
	DisplayPeriod(74, 15);
	DisplayPeriod(74, 27);
	DisplayDigit((Seconds%60)/10, 90, 10);
	DisplayDigit((Seconds%60)%10, 122, 10);
	DisplayPeriod(154, 30);
    DisplayDigit(Hundredths/10, 170, 10);
    DisplayDigit(Hundredths%10, 206, 10);
    
#if 0
    if (Score.Single)
    {
        switch (Score.CounterSingle)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 54, 63); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 54, 0, 74, 63); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 74, 0, 131, 63); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 131, 0, 187, 63); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 187, 0, 242, 63); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 242, 0, 296, 63); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 296, 0, 353, 63); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 353, 0, 406, 63); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 406, 0, 462, 63); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 462, 0, 520, 63); break;
        default:   break;   
        }
    }

    if (Score.Ten)
    {
        switch (Score.CounterTen)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 54, 63); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 54, 0, 74, 63); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 74, 0, 131, 63); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 131, 0, 187, 63); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 187, 0, 242, 63); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 242, 0, 296, 63); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 296, 0, 353, 63); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 353, 0, 406, 63); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 406, 0, 462, 63); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 462, 0, 520, 63); break;
        default:   break;   
        }
    }

    if (Score.Hundred)
    {
        switch (Score.CounterHundred)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 54, 63); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 54, 0, 74, 63); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 74, 0, 131, 63); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 131, 0, 187, 63); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 187, 0, 242, 63); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 242, 0, 296, 63); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 296, 0, 353, 63); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 353, 0, 406, 63); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 406, 0, 462, 63); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 462, 0, 520, 63); break;
        default:   break;   
        }
    }

    if (Score.Thousand)
    {
        switch (Score.CounterThousand)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 54, 63); break;        
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 54, 0, 74, 63); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 74, 0, 131, 63); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 131, 0, 187, 63); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 187, 0, 242, 63); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 242, 0, 296, 63); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 296, 0, 353, 63); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 353, 0, 406, 63); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 406, 0, 462, 63); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 462, 0, 520, 63); break;
        default:   break;   
        }
    }

    if (Score.TenThousand)
    {
        switch (Score.CounterTenThousand)
        {
        case 0:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 0, 0, 54, 63); break;   
        case 1:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 54, 0, 74, 63); break;
        case 2:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 74, 0, 131, 63); break;
        case 3:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 131, 0, 187, 63); break;
        case 4:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 187, 0, 242, 63); break;
        case 5:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 242, 0, 296, 63); break;
        case 6:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 296, 0, 353, 63); break;
        case 7:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 353, 0, 406, 63); break;
        case 8:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 406, 0, 462, 63); break;
        case 9:    AddLitSubSprite( LIGHTLIST_FOREGROUND, &FullScoreSprite, 280, 5, 462, 0, 520, 63); break;
        default:   break;   
        }
    }                      
#endif
}

void ResetScore()
{    
    Score.PageTime = 0;

#if 0
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
#endif
}

void UpdateScore()
{    
    Score.PageTime++;
    
#if 0
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
#endif
}