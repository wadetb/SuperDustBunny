//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef GAMESCORE_H
#define GAMESCORE_H

struct SScore
{
    int CounterSingle;
    int CounterTen;
    int CounterHundred;
    int CounterThousand;
    int CounterTenThousand;
    
    int Type;
    
    int ToRaise;
    int Left;
    
    bool Raise;
    bool Single;
    bool Ten;
    bool Hundred;
    bool Thousand;
    bool TenThousand;  
    
    int PageTime;
};

extern SScore Score;

void InitScore();
void DisplayScore();
void UpdateScore();

void ResetScore();

#endif