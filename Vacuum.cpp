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
	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_NEAR || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		gxDrawSprite(0, (int)Vacuum.Y, &VacuumBackSprite);
	}
}

void DisplayVacuum_AfterDusty()
{
	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_NEAR || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		gxDrawSprite(0, (int)Vacuum.Y, &VacuumFrontSprite);
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
#ifdef PLATFORM_WINDOWS
	// V key to summon the vacuum.
	if (kbIsKeyDown(KB_V) && !kbWasKeyDown(KB_V))
		Vacuum.Timer = 0;
#endif

	if (Vacuum.State == VACUUMSTATE_FAR)
	{
		UpdateVacuum_Volume(0.5f);

		Vacuum.Timer--;
		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_NEAR;
			Vacuum.Y = (float)gxScreenHeight;
			Vacuum.Timer = 6*60;
		}
	}
	else if (Vacuum.State == VACUUMSTATE_NEAR)
	{
		UpdateVacuum_Volume(0.8f);

		if (Dusty.State != DUSTYSTATE_DIE)
		{
			// Shake the screen by adjusting ScrollY randomly.
			int ShakeY = (rand() % 3) - 1;
			ScrollY += ShakeY;

			// Move the vacuum gradually up the screen.
			if (Vacuum.Y > gxScreenHeight - 200)
				Vacuum.Y -= 5;

			if ((float)Vacuum.Y + 150 <= Dusty.FloatY + ScrollY)
			{
				SetDustyState_Die();
			}
		}

		Vacuum.Timer--;
		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_ONSCREEN;
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

			float VacuumSpeed;
			if (Vacuum.Y - Dusty.FloatY >= 1000)
				VacuumSpeed = 5;
			else if (Vacuum.Y - Dusty.FloatY >= 500)
				VacuumSpeed = 4;
			else
				VacuumSpeed = 2;

			// Move the vacuum gradually up the screen.
			Vacuum.Y -= VacuumSpeed;

			if ((float)Vacuum.Y + 150 <= Dusty.FloatY + ScrollY)
			{
				SetDustyState_Die();
			}
		}
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		if (Dusty.State != DUSTYSTATE_DIE)
		{
			// Shake the screen by adjusting ScrollY randomly.
			int ShakeY = (rand() % 3) - 1;
			ScrollY += ShakeY;

			// Move the vacuum back down the screen.
			Vacuum.Y += 5;
			if (Vacuum.Y >= gxScreenHeight)
			{
				Vacuum.State = VACUUMSTATE_FAR;
				Vacuum.Timer = 6*60;
			}
		}
	}
}

void JamVacuum()
{
	if (Vacuum.State == VACUUMSTATE_NEAR || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		Vacuum.State = VACUUMSTATE_RETREAT;
	}
	else
	{
		Vacuum.Timer = 10*60;
	}
}
