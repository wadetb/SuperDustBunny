//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Gear.h"
#include "Common.h"
#include "Dusty.h"
#include "Vacuum.h"

#define MAX_GEARS 100

int NGears = 0;

SGear Gear;

SGear Gears[MAX_GEARS];

void CreateGear(int X, int Y, const char* Desc)
{
    SGear* Gear = &Gears[NGears++];

    Gear->X = X + 32;
    Gear->Y = Y + 32;

    Gear->FloatVelocityY = 0.0f;
    Gear->FloatGravity = 0.5f;

    Gear->Transition = 40;
    Gear->Sprite = 1;

    Gear->Collided = false;

    Gear->State = GEARSTATE_ACTIVE;
}

void ClearGears()
{
    NGears = 0;
}

extern int ScrollY;

void DisplayGear()
{
    for (int i = 0; i < NGears; i++)
    {
        SGear* Gear = &Gears[i];

        if (Gear->State == GEARSTATE_INACTIVE)
            continue;

        if (Gear->Sprite == 1) 
        {
            gxDrawSpriteCenteredRotated( Gear->X, Gear->Y + ScrollY, 0, &Gear01 );
        } 

        if (Gear->Sprite == 2)
        {
            gxDrawSpriteCenteredRotated( Gear->X, Gear->Y + ScrollY, 0, &Gear02 );
        } 

        if (Gear->Sprite == 3)
        {
            gxDrawSpriteCenteredRotated( Gear->X, Gear->Y + ScrollY, 0, &Gear03 );
        }

        if (Gear->Sprite == 4)
        {
            gxDrawSpriteCenteredRotated( Gear->X, Gear->Y + ScrollY, 0, &Gear04 );
        }   
    }    
}

void UpdateGear()
{
    for (int i = 0; i < NGears; i++)
    {
        SGear* Gear = &Gears[i];

        if (Gear->State == GEARSTATE_INACTIVE)
            continue;

        float XDist = (float)(Dusty.FloatX - Gear->X);
        float YDist = (float)((Dusty.FloatY-50) - (Gear->Y));
        float Dist = sqrtf(XDist*XDist + YDist*YDist);

        if (Dist < 100)
        {
            Gear->Collided = true;
        }

        if (Gear->Collided == true)
        {
            Gear->Y += Gear->FloatVelocityY;
            Gear->FloatVelocityY += Gear->FloatGravity;

            if (Tutorial.GearDisplayed == false)
            {
                SetGameState_Crumb(TUTORIALSTATE_GEAR);
                return;
            }

            if (Gear->Y + ScrollY >= gxScreenHeight)
            {
                Gear->State = GEARSTATE_INACTIVE;
                sxPlaySound (&Clogged);
                JamVacuum();
            }       
        }

        if (Gear->Transition == 40)
        {
            Gear->Sprite = 1;
        }  

        if (Gear->Transition == 30)
        {
            Gear->Sprite = 2;
        }  

        if (Gear->Transition == 20)
        {
            Gear->Sprite = 3;
        } 

        if (Gear->Transition == 10)
        {
            Gear->Sprite = 4;
        }  

        if (Gear->Transition <= 0)
        {
            Gear->Transition = 40;
        }  

        Gear->Transition -= 1;          
    }    
}