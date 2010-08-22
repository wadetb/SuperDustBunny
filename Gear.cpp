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
SGear Gears[MAX_GEARS];

void CreateGear(int X, int Y, const char* Desc)
{
    SGear* Gear = &Gears[NGears++];

    Gear->X = X + 32;
    Gear->Y = Y + 32;

	Gear->FloatVelocityX = 0;
    Gear->FloatVelocityY = 0;

	Gear->Angle = 0;
	Gear->AngularVelocity = 0;

    Gear->State = GEARSTATE_ACTIVE;
}

void ClearGears()
{
    NGears = 0;
}

void DisplayGear()
{
    for (int i = 0; i < NGears; i++)
    {
        SGear* Gear = &Gears[i];

        if (Gear->State == GEARSTATE_INACTIVE)
            continue;

		gxDrawSpriteCenteredRotated( Gear->X, Gear->Y + ScrollY, Gear->Angle, &GearSprite );
    }    
}

void UpdateGear()
{
    for (int i = 0; i < NGears; i++)
    {
        SGear* Gear = &Gears[i];

        if (Gear->State == GEARSTATE_ACTIVE)
		{
			float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, Gear->X, Gear->Y);

			if (Dist < 100)
			{
				Gear->State = GEARSTATE_FALLING;
				sxPlaySound( &GearGrindSound );
				
				if (Dusty.FloatVelocityX < 0)
				{
					Gear->AngularVelocity = -5 * PI / 180.0f;
					Gear->FloatVelocityX = -3;
				}
				else
				{
					Gear->AngularVelocity = 5 * PI / 180.0f;
					Gear->FloatVelocityX = 3;
				}
				
				Gear->FloatVelocityY = -2;

				if (Tutorial.GearDisplayed == false)
				{
					SetGameState_Crumb(TUTORIALSTATE_GEAR);
					JamVacuum();
					return;
				}
			}
		}
		else if (Gear->State == GEARSTATE_FALLING)
        {
            Gear->X += Gear->FloatVelocityX;
            Gear->Y += Gear->FloatVelocityY;
			
            Gear->FloatVelocityY += 1.0f;

			Gear->Angle += Gear->AngularVelocity;
			
			if (Gear->X + 50 > gxScreenWidth)
			{
				Gear->X = gxScreenWidth - 50;
				Gear->FloatVelocityX = -Gear->FloatVelocityX;
			}
			if (Gear->X - 50 < 0)
			{
				Gear->X = 50;
				Gear->FloatVelocityX = -Gear->FloatVelocityX;
			}
			
            if (Gear->Y + ScrollY >= gxScreenHeight + 200)
            {
                Gear->State = GEARSTATE_INACTIVE;
                sxPlaySound (&VacuumClogSound);
                JamVacuum();
            }       
        }
    }    
}