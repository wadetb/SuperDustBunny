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


void SetVacuumSoundState(EVacuumSoundState NewSoundState);


void InitVacuum()
{
	Vacuum.State = VACUUMSTATE_FAR;
	Vacuum.SoundState = VACUUMSOUNDSTATE_OFF;
	Vacuum.Timer = 10*60;
	Vacuum.Volume = 0.0f;

	// These two sounds loop continuously.
	sxSetSoundVolume(&VacuumOnSound, 0);
	sxPlaySoundLooping(&VacuumOnSound);
	//sxSetSoundVolume(&VacuumJamSound, 0);
	//sxPlaySoundLooping(&VacuumJamSound);

	SetVacuumSoundState(VACUUMSOUNDSTATE_TURN_ON);
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
}

void SetVacuumSoundState(EVacuumSoundState NewSoundState)
{
	Vacuum.TargetSoundState = NewSoundState;
}

void UpdateVacuumSound()
{
	if (Vacuum.SoundState == VACUUMSOUNDSTATE_OFF && Vacuum.TargetSoundState == VACUUMSOUNDSTATE_TURN_ON)
	{
		sxSetSoundVolume(&VacuumTurnOnSound, Vacuum.Volume);
		sxPlaySound(&VacuumTurnOnSound);
		Vacuum.SoundTimerUp = 0;
		Vacuum.SoundTimerDown = (int)( 1.34f * 60 );
		Vacuum.SoundState = VACUUMSOUNDSTATE_TURN_ON;
	}
	if (Vacuum.SoundState == VACUUMSOUNDSTATE_ON && Vacuum.TargetSoundState == VACUUMSOUNDSTATE_JAM)
	{
		Vacuum.SoundState = VACUUMSOUNDSTATE_JAM;
		Vacuum.SoundTimerUp = 0;
		Vacuum.SoundTimerDown = (int)( 1.0f * 60 );
	}
	if (Vacuum.SoundState == VACUUMSOUNDSTATE_JAM && (Vacuum.TargetSoundState == VACUUMSOUNDSTATE_UNJAM || Vacuum.TargetSoundState == VACUUMSOUNDSTATE_TURN_OFF))
	{
		Vacuum.SoundState = VACUUMSOUNDSTATE_UNJAM;
		Vacuum.SoundTimerUp = 0;
		Vacuum.SoundTimerDown = (int)( 1.0f * 60 );
	}
	if (Vacuum.SoundState == VACUUMSOUNDSTATE_ON && Vacuum.TargetSoundState == VACUUMSOUNDSTATE_TURN_OFF)
	{
		sxSetSoundVolume(&VacuumTurnOffSound, 0);
		sxPlaySound(&VacuumTurnOffSound);
		Vacuum.SoundState = VACUUMSOUNDSTATE_TURN_OFF;
		Vacuum.SoundTimerUp = 0;
		Vacuum.SoundTimerDown = (int)( 4.25f * 60 );
	}

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

	if (Vacuum.SoundState == VACUUMSOUNDSTATE_TURN_ON)
	{
		Vacuum.SoundTimerDown--;
		Vacuum.SoundTimerUp++;

		// Before the end of the sound, ramp down the "turn on" sound and ramp up the "on" sound.
		if (Vacuum.SoundTimerDown <= 15)
		{
			sxSetSoundVolume(&VacuumTurnOnSound, Vacuum.Volume * ( (float)Vacuum.SoundTimerDown / 15.0f ));
			sxSetSoundVolume(&VacuumOnSound, Vacuum.Volume * ( 1.0f - ( (float)Vacuum.SoundTimerDown / 15.0f ) ));
		}

		if (Vacuum.SoundTimerDown == 0)
		{
			Vacuum.SoundState = VACUUMSOUNDSTATE_ON;
			return;
		}
	}
	else if (Vacuum.SoundState == VACUUMSOUNDSTATE_TURN_OFF)
	{
		Vacuum.SoundTimerDown--;
		Vacuum.SoundTimerUp++;

		// At the beginning of the sound, ramp down the "turn on" sound and ramp up the "on" sound.
		if (Vacuum.SoundTimerUp <= 15)
		{
			sxSetSoundVolume(&VacuumTurnOffSound, Vacuum.Volume * ( (float)Vacuum.SoundTimerUp / 15.0f ) );
			sxSetSoundVolume(&VacuumOnSound, Vacuum.Volume * ( 1.0f - ( (float)Vacuum.SoundTimerUp / 15.0f ) ));
		}

		if (Vacuum.SoundTimerDown == 0)
		{
			Vacuum.SoundState = VACUUMSOUNDSTATE_OFF;
			return;
		}
	}
	else if (Vacuum.SoundState == VACUUMSOUNDSTATE_JAM)
	{
		Vacuum.SoundTimerUp++;

		// At the beginning, ramp down the "on" sound and ramp up the "jam" sound.
		if (Vacuum.SoundTimerUp <= 15)
		{
			sxSetSoundVolume(&VacuumJamSound, Vacuum.Volume * ( (float)Vacuum.SoundTimerUp / 15.0f ));
			sxSetSoundVolume(&VacuumOnSound, Vacuum.Volume * ( 1.0f - ( (float)Vacuum.SoundTimerUp / 15.0f ) ));
		}
	}
	else if (Vacuum.SoundState == VACUUMSOUNDSTATE_UNJAM)
	{
		Vacuum.SoundTimerDown--;

		// At the end, ramp up the "on" sound and ramp down the "jam" sound.
		if (Vacuum.SoundTimerDown <= 15)
		{
			sxSetSoundVolume(&VacuumJamSound, Vacuum.Volume * ( (float)Vacuum.SoundTimerDown / 15.0f ));
			sxSetSoundVolume(&VacuumOnSound, Vacuum.Volume * ( 1.0f - ( (float)Vacuum.SoundTimerDown / 15.0f ) ));
		}

		if (Vacuum.SoundTimerDown == 0)
		{
			Vacuum.SoundState = VACUUMSOUNDSTATE_ON;
			return;
		}
	}
	else if (Vacuum.SoundState == VACUUMSOUNDSTATE_ON)
	{
		sxSetSoundVolume(&VacuumOnSound, Vacuum.Volume);
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
		if (Dusty.State != DUSTYSTATE_DIE)
		{
			// Move the vacuum back down the screen.
			Vacuum.Y += 5;
			if (Vacuum.Y + ScrollY >= gxScreenHeight)
			{
				Vacuum.State = VACUUMSTATE_FAR;
				Vacuum.Timer = 3*60;
				SetVacuumSoundState(VACUUMSOUNDSTATE_UNJAM);
			}
		}
	}
}

void JamVacuum()
{
	SetVacuumSoundState(VACUUMSOUNDSTATE_JAM);

	if (Vacuum.State == VACUUMSTATE_NEAR || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		Vacuum.State = VACUUMSTATE_RETREAT;
	}
	else
	{
		Vacuum.Timer = 6*60;
	}
}

void TurnOffVacuum()
{
	if (Vacuum.SoundState != VACUUMSOUNDSTATE_OFF)
		SetVacuumSoundState(VACUUMSOUNDSTATE_TURN_OFF);
}
