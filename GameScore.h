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
    int Counter;
    int Type;
    int Place;
    int Number;  
};

extern SScore Score;

void InitScore();
void DisplayScore();
void UpdateScore();
void RaiseScore();

#endif