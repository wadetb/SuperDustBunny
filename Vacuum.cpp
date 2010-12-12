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

const int VACUUM_INITIAL_TIME = 5*60;
const int VACUUM_RETREAT_TIME = 3*60;
const int VACUUM_UNJAM_TIME   = 1*60;

const float VacuumYOffset = 200;


void InitVacuum()
{
	Vacuum.Dir = (EVacuumDir)Chapter.PageProps.VacuumDir;
	Vacuum.State = VACUUMSTATE_OFF;
	Vacuum.Volume = 0.5f;

	// These two sounds loop continuously.
	sxSetSoundVolume(&VacuumOnSound, 0);
	sxPlaySoundLooping(&VacuumOnSound);
	sxSetSoundVolume(&VacuumJamSound, 0);
	sxPlaySoundLooping(&VacuumJamSound);
}

void DisplayVacuum()
{
	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		if (Vacuum.Dir == VACUUMDIR_UP)
			AddLitSpriteScaled(LIGHTLIST_VACUUM, &VacuumFrontSprite, 0, Vacuum.Y + ScrollY - VacuumYOffset, 1.0f, 1.0f);
		else
			AddLitSpriteScaled(LIGHTLIST_VACUUM, &VacuumFrontSprite, 0, Vacuum.Y + ScrollY + VacuumYOffset, 1.0f, -1.0f);

		if (Chapter.PageProps.LightsOff)
		{
			if (Vacuum.Dir == VACUUMDIR_UP)
			{
				AddLitSpriteCenteredScaledAlpha(LIGHTLIST_LIGHTING, &LightVacuumSprite, 384, Vacuum.Y + ScrollY - 384, 1.0f, 1.0f);
			}
		}
	}

	if (DevMode)
	{
		gxDrawString(5, 64, 16, 0xffffffff, "Vacuum State=%d Timer=%d y=%d\n      [On%02d Jam%02d TOn%02d TOff%02d]", 
			Vacuum.State, Vacuum.Timer, (int)Vacuum.Y, (int)(VacuumOnSound.volume*99), (int)(VacuumJamSound.volume*99), (int)(VacuumTurnOnSound.volume*99), (int)(VacuumTurnOffSound.volume*99));

		gxDrawRectangleFilled(0, (int)Vacuum.Y + ScrollY, gxScreenWidth, 2, gxRGBA32(255,0,0,255));
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
	{
		JamVacuum();
		Vacuum.Timer = 1000000;
	}
#endif

	if (Vacuum.State == VACUUMSTATE_FAR)
	{
		// Place the vacuum far off screen for dust purposes.
		if (Vacuum.Dir == VACUUMDIR_UP)
			Vacuum.Y = (float)-ScrollY + (float)gxScreenHeight + 1500;
		else
			Vacuum.Y = (float)-ScrollY - 1500;

		Vacuum.Timer--;
		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_ONSCREEN;
			
			int LightsOffset = Chapter.PageProps.LightsOff ? 512 : 0;

			// Vacuum.Y is the leading edge of the vacuum.
			if (Vacuum.Dir == VACUUMDIR_UP)
				Vacuum.Y = (float)-ScrollY + (float)gxScreenHeight + VacuumYOffset + LightsOffset;
			else
				Vacuum.Y = (float)-ScrollY - VacuumYOffset - LightsOffset;

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
				VacuumSpeed = 3;
			else if (Vacuum.Timer < 8*60)
				VacuumSpeed = 3.5;
			else
				VacuumSpeed = 6;

			Vacuum.Timer++;

			// Move the vacuum gradually up the screen.
			if (Vacuum.Dir == VACUUMDIR_UP)
				Vacuum.Y -= VacuumSpeed;
			else
				Vacuum.Y += VacuumSpeed;

			if (IsInVacuum(Dusty.FloatY))
			{
				SetDustyState_Die();
			}
		}
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		Vacuum.Timer--;

		// Move the vacuum back down the screen.
		if (Vacuum.Dir == VACUUMDIR_UP)
			Vacuum.Y += 5;
		else
			Vacuum.Y -= 5;

		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_FAR;
			Vacuum.Timer = VACUUM_UNJAM_TIME;
		}
	}
}

void JamVacuum()
{
	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		Vacuum.State = VACUUMSTATE_RETREAT;
		Vacuum.Timer = VACUUM_RETREAT_TIME;
	}
	else
		Vacuum.Timer += VACUUM_UNJAM_TIME;
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
	if (Vacuum.Dir == VACUUMDIR_UP)
		Vacuum.Y = (float)Chapter.PageHeight * 64;
	else
		Vacuum.Y = 0;

	Vacuum.Timer = VACUUM_INITIAL_TIME;

	if (Vacuum.State == VACUUMSTATE_OFF)
	{
		Vacuum.State = VACUUMSTATE_FAR;
		sxSetSoundVolume(&VacuumTurnOnSound, 0);
		sxPlaySound(&VacuumTurnOnSound);
	}
}

bool IsInVacuum(float Y)
{
	if (Vacuum.Dir == VACUUMDIR_UP)
		return Y >= Vacuum.Y;
	else
		return Y <= Vacuum.Y;
}

void GetVacuumForce(float X, float Y, float* VX, float* VY, float Strength)
{
	float DirX = (float)gxScreenWidth/2 - X;
	float DirY = Vacuum.Y - Y;
	
	float Length = sqrtf(DirX*DirX + DirY*DirY);

	if (Length >= 1000)
	{
		*VX = 0;
		*VY = 0;
		return;
	}

	DirX /= Length;
	DirY /= Length;

	float AttenuatedStrength = Strength * Lerp(Length, 1000, 200, 1.5f, 20.0f); 

	*VX = DirX * AttenuatedStrength;
	*VY = DirY * AttenuatedStrength;
}
