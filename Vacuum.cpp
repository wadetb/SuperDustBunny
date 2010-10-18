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
#include "Chapter.h"


SVacuum Vacuum;


void InitVacuum()
{
	Vacuum.State = VACUUMSTATE_OFF;
	Vacuum.Y = (float)Chapter.StitchedHeight;
	Vacuum.Timer = 10*60;
	Vacuum.Volume = 0.5f;

	// These two sounds loop continuously.
	sxSetSoundVolume(&VacuumOnSound, 0);
	sxPlaySoundLooping(&VacuumOnSound);
	sxSetSoundVolume(&VacuumJamSound, 0);
	sxPlaySoundLooping(&VacuumJamSound);
}

void DisplayVacuum_BeforeDusty()
{
	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		gxDrawSprite(0, (int)Vacuum.Y + ScrollY, &VacuumBackSprite);
	}
}

void DisplayVacuum_AfterDusty()
{
	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		gxDrawSprite(0, (int)Vacuum.Y + ScrollY, &VacuumFrontSprite);
	}

	if (DevMode)
	{
		gxDrawString(5, 64, 16, 0xffffffff, "Vacuum State=%d Timer=%d y=%d\n      [On%02d Jam%02d TOn%02d TOff%02d]", 
			Vacuum.State, Vacuum.Timer, (int)Vacuum.Y, (int)(VacuumOnSound.volume*99), (int)(VacuumJamSound.volume*99), (int)(VacuumTurnOnSound.volume*99), (int)(VacuumTurnOffSound.volume*99));
	}
}

void UpdateVacuumSound()
{
	float TargetVolume;
	if (Vacuum.State == VACUUMSTATE_FAR)
	{
		TargetVolume = 0.5f;
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		TargetVolume = 0.8f;
	}
	else
	{
		TargetVolume = 1.0f;
	}

	if (Vacuum.Volume != TargetVolume)
	{
		if (Vacuum.Volume < TargetVolume)
			Vacuum.Volume += 0.01f;
		else
			Vacuum.Volume -= 0.01f;
	}

	if (Vacuum.State == VACUUMSTATE_FAR || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		if (VacuumOnSound.volume < 1.0f)
			sxSetSoundVolume(&VacuumOnSound, VacuumOnSound.volume + 0.1f);
		if (VacuumJamSound.volume > 0.0f)
			sxSetSoundVolume(&VacuumJamSound, VacuumJamSound.volume - 0.1f);
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		if (VacuumJamSound.volume < 1.0f)
			sxSetSoundVolume(&VacuumJamSound, VacuumJamSound.volume + 0.1f);
		if (VacuumOnSound.volume > 0.0f)
			sxSetSoundVolume(&VacuumOnSound, VacuumOnSound.volume - 0.1f);
	}
	else if (Vacuum.State == VACUUMSTATE_OFF)
	{
		if (VacuumTurnOffSound.volume < 1.0f)
			sxSetSoundVolume(&VacuumTurnOffSound, VacuumTurnOffSound.volume + 0.1f);
		if (VacuumOnSound.volume > 0.0f)
			sxSetSoundVolume(&VacuumOnSound, VacuumOnSound.volume - 0.1f);
	}
}

void UpdateVacuum()
{
#ifdef PLATFORM_WINDOWS
	// V key to summon the vacuum.
	if (kbIsKeyDown(KB_V) && !kbWasKeyDown(KB_V))
		Vacuum.Timer = 0;
	// Shift-V key to banish the vacuum.
	if (kbIsKeyDown(KB_LSHIFT) && kbIsKeyDown(KB_V) && !kbWasKeyDown(KB_V))
		Vacuum.Timer = 1000000;
#endif

	if (Vacuum.State == VACUUMSTATE_FAR)
	{
		Vacuum.Timer--;
		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_ONSCREEN;
			Vacuum.Y = -ScrollY + (float)gxScreenHeight;
			Vacuum.Timer = 0;
		}
	}
	else if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		if (Dusty.State != DUSTYSTATE_DIE)
		{
			// Shake the screen by adjusting ScrollY randomly.
			int ShakeY = (rand() % 13) - 6;
			ScrollY += ShakeY;

			// The vacuum gets faster the longer it stays unclogged.
			float VacuumSpeed;
			if (Vacuum.Timer < 4*60)
				VacuumSpeed = 4;
			else if (Vacuum.Timer < 8*60)
				VacuumSpeed = 6;
			else
				VacuumSpeed = 8;

			Vacuum.Timer++;

			// Move the vacuum gradually up the screen.
			Vacuum.Y -= VacuumSpeed;

			if ((float)Vacuum.Y + 150 <= Dusty.FloatY)
			{
				SetDustyState_Die();
			}
		}
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		Vacuum.Timer--;

		// Move the vacuum back down the screen.
		Vacuum.Y += 5;

		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_FAR;
			Vacuum.Timer = 3*60;
		}
	}
}

void JamVacuum()
{
	Vacuum.State = VACUUMSTATE_RETREAT;
	Vacuum.Timer = 6*60;
}

void TurnOffVacuum()
{
	if (Vacuum.State != VACUUMSTATE_OFF)
	{
		Vacuum.State = VACUUMSTATE_OFF;
		sxSetSoundVolume(&VacuumTurnOffSound, 0);
		sxPlaySound(&VacuumTurnOffSound);
	}
}

void TurnOnVacuum()
{
	if (Vacuum.State == VACUUMSTATE_OFF)
	{
		Vacuum.State = VACUUMSTATE_FAR;
		sxSetSoundVolume(&VacuumTurnOnSound, 0);
		sxPlaySound(&VacuumTurnOnSound);
	}
}
