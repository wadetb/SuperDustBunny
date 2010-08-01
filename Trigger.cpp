//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Trigger.h"
#include "Common.h"
#include "Dusty.h"
#include "chapter.h"

#define MAX_TRIGGERS 100

int NTriggers = 0;
STrigger Triggers[MAX_TRIGGERS];

void CreateTrigger(int X, int Y, const char* Desc)
{
    STrigger* Trigger = &Triggers[NTriggers++];

    Trigger->X = X + 32;
    Trigger->Y = Y + 32;
}

void ClearTriggers()
{
    NTriggers = 0;
}

extern int ScrollY;

void DisplayCrumbTriggers()
{
   
}

void UpdateCrumbTriggers()
{
    for(int i = 0; i < NTriggers; i++)
    {
        STrigger* Trigger = &Triggers[i];

        float XDist = (float)(Dusty.FloatX - Trigger->X);
        float YDist = (float)((Dusty.FloatY-50) - (Trigger->Y));
        float Dist = sqrtf(XDist*XDist + YDist*YDist);

        if (Dist < 100)
        {
            SetGameState_Crumb();
            return;           
        }
    }                 
}      
