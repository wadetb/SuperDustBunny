//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Gear.h"
#include "Dusty.h"
#include "Vacuum.h"
#include "GameScore.h"
#include "Chapter.h"

#define MAX_GEARS 100

int NGears = 0;
SGear Gears[MAX_GEARS];

void CreateGear(int X, int Y)
{
	if (NGears >= MAX_GEARS)
		ReportError("Exceeded the maximum of %d total gears.", MAX_GEARS);

	SGear* Gear = &Gears[NGears++];

    Gear->X = (float)X + 32;
    Gear->Y = (float)Y + 32;

	Gear->FloatVelocityX = 0;
    Gear->FloatVelocityY = 0;

	Gear->Angle = 0;
	Gear->AngularVelocity = 0;
    
    Gear->Timer = 0;

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

        float X = Gear->X;
        float Y = Gear->Y; 

        if (Gear->State == GEARSTATE_ACTIVE)
        {
            X += cosf(Gear->Timer*4.0f) * 2.5f + cosf(Gear->Timer*1.0f/3.0f) * 2.5f; 
            Y += sinf(Gear->Timer*4.0f) * 2.5f + sinf(Gear->Timer*1.0f/3.0f) * 2.5f;
        }
        
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_VACUUM, &GearSprite, X + ScrollX, Y + ScrollY, 1.0f, Gear->Angle);
    }    
}

void UpdateGear()
{
    for (int i = 0; i < NGears; i++)
    {
        SGear* Gear = &Gears[i];

        if (Gear->State == GEARSTATE_ACTIVE)
		{
            Gear->Timer += 1.0f/60.0f;
            
			float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, Gear->X, Gear->Y);

			if (Dist < 100)
			{
				Gear->State = GEARSTATE_FALLING;
				sxPlaySound( &GearGrindSound );
				
                AwardBonus(Gear->X, Gear->Y - 100);
				
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
			}
		}
		else if (Gear->State == GEARSTATE_FALLING)
        {
            Gear->X += Gear->FloatVelocityX;
            Gear->Y += Gear->FloatVelocityY;
			
            Gear->FloatVelocityY += 1.0f;

			Gear->Angle += Gear->AngularVelocity;
			
			if (Gear->X + 50 > Chapter.PageWidth*64)
			{
				Gear->X = (float)Chapter.PageWidth*64 - 50;
				Gear->FloatVelocityX = -Gear->FloatVelocityX;
			}
			if (Gear->X - 50 < 0)
			{
				Gear->X = 50;
				Gear->FloatVelocityX = -Gear->FloatVelocityX;
			}
			
			if (IsInVacuum(Gear->Y))
            {
                Gear->State = GEARSTATE_INACTIVE;
                sxPlaySound (&VacuumClogSound);
                JamVacuum();
            }           
        }
    }    
}