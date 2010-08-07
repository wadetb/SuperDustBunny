//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Vacuum.h"
#include "Dusty.h"

SVacuum Vacuum;

void InitVacuum()
{
	Vacuum.State = VACUUMSTATE_FAR;
	Vacuum.Timer = 1000;

	sxSetSoundVolume(&VacuumRunSound, 0.0f);
	sxPlaySoundLooping(&VacuumRunSound);
}

void DisplayVacuum_BeforeDusty()
{
	if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		gxDrawSprite(0, Vacuum.Y + ScrollY, &VacuumBackSprite);
	}
}

void DisplayVacuum_AfterDusty()
{
	if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		gxDrawSprite(0, Vacuum.Y + ScrollY, &VacuumFrontSprite);
	}
}

void UpdateVacuum_Volume(float Target)
{
	if (Vacuum.Volume != Target)
	{
		if (Vacuum.Volume < Target)
			Vacuum.Volume += 0.1f;
		else
			Vacuum.Volume -= 0.1f;

		sxSetSoundVolume(&VacuumRunSound, Vacuum.Volume);
	}
}

void UpdateVacuum()
{
	if (Vacuum.State == VACUUMSTATE_FAR)
	{
		UpdateVacuum_Volume(0.5f);

		Vacuum.Timer--;
		if (Vacuum.Timer == 0)
		{
			Vacuum.State = VACUUMSTATE_NEAR;
			Vacuum.Timer = 500;
		}
	}
	else if (Vacuum.State == VACUUMSTATE_NEAR)
	{
		UpdateVacuum_Volume(0.8f);

		// Shake the screen by adjusting ScrollY randomly.
		ScrollY += (rand() % 3) - 1;

		Vacuum.Timer--;
		if (Vacuum.Timer == 0)
		{
			Vacuum.State = VACUUMSTATE_ONSCREEN;
			Vacuum.Y = gxScreenHeight - ScrollY;
		}
	}
	else if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		UpdateVacuum_Volume(1.0f);

		if (Dusty.State != DUSTYSTATE_DIE)
		{
			// Shake the screen by adjusting ScrollY randomly.
			int ShakeY = (rand() % 13) - 6;
			ScrollY += ShakeY;
			Vacuum.Y -= ShakeY;

			if (Vacuum.Y + ScrollY > gxScreenHeight - 768 + 10)
				Vacuum.Y -= 2;
		}

		if (Vacuum.Y + ScrollY < gxScreenHeight - 768)
			Vacuum.Y = gxScreenHeight - ScrollY - 768;

		if (Dusty.State != DUSTYSTATE_DIE && (float)Vacuum.Y <= Dusty.FloatY)
		{
			SetDustyState_Die();
		}
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		if (Dusty.State != DUSTYSTATE_DIE)
		{
			// Shake the screen by adjusting ScrollY randomly.
			int ShakeY = (rand() % 13) - 6;
			ScrollY += ShakeY;
			Vacuum.Y -= ShakeY;

			Vacuum.Y += 2;
			if (Vacuum.Y + ScrollY >= gxScreenHeight)
				Vacuum.State = VACUUMSTATE_NEAR;
		}
	}
}

void JamVacuum()
{
	if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		Vacuum.State = VACUUMSTATE_RETREAT;
	}
	else if (Vacuum.State == VACUUMSTATE_NEAR)
	{
		Vacuum.State = VACUUMSTATE_FAR;
	}
	else
	{
		Vacuum.Timer = 1000;
	}
}
