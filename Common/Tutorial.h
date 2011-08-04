//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef TUTORIAL_H
#define TUTORIAL_H

struct STutorial
{
    float X, Y;
    float Timer;
    float WiggleTimer;
};

extern STutorial Tutorial;

void InitTutorial();
void UpdateTutorial();
void DisplayTutorial();

#endif
