//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef TRIGGER_H
#define TRIGGER_H


enum ETriggerState
{
    TRIGGERSTATE_ACTIVE 				,
    TRIGGERSTATE_INACTIVE				,
};

struct STrigger
{
    ETriggerState State;

    int X, Y;

    int Transition;
    int Dist;
};

void CreateTrigger(int X, int Y, const char* Desc);
void ClearTriggers();

void UpdateCrumbTriggers();
void DisplayCrumbTriggers();

#endif