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
#include "Settings.h"


SVacuum Vacuum;

const int VACUUM_INITIAL_TIME = 5*60;
const int VACUUM_RETREAT_TIME = 3*60;
const int VACUUM_UNJAM_TIME   = 1*60;

const float VacuumYOffset = 100;


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
	// If the vacuum is disabled for this page, don't display at all.
	if (Chapter.PageProps.VacuumOff || Settings.DisableVacuum)
		return;

	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
        gxSprite* Sprite;
        switch (Vacuum.BlinkTimer/6)
        {
            case 0: Sprite = &Vacuum1Sprite; break;
            case 1: Sprite = &Vacuum2Sprite; break;
            case 2: Sprite = &Vacuum3Sprite; break;
            case 3: Sprite = &Vacuum2Sprite; break;
            case 4: Sprite = &Vacuum1Sprite; break;
            default: Sprite = &Vacuum1Sprite; break;
        }
        
		if (Vacuum.Dir == VACUUMDIR_UP)
			AddLitSpriteScaled(LIGHTLIST_VACUUM, Sprite, 0, Vacuum.Y + ScrollY - VacuumYOffset, 1.0f, 1.0f);
		else
			AddLitSpriteScaled(LIGHTLIST_VACUUM, Sprite, 0, Vacuum.Y + ScrollY + VacuumYOffset, 1.0f, -1.0f);

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

	// If the vacuum is disabled for this page, don't update at all.
	if (Chapter.PageProps.VacuumOff || Settings.DisableVacuum)
		return;

	if (Vacuum.State == VACUUMSTATE_FAR)
	{
		// Place the vacuum far off screen for dust purposes.
		if (Vacuum.Dir == VACUUMDIR_UP)
			Vacuum.Y = (float)-ScrollY + (float)LitScreenHeight + 1500;
		else
			Vacuum.Y = (float)-ScrollY - 1500;

		Vacuum.Timer--;
		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_ONSCREEN;
			
			int LightsOffset = Chapter.PageProps.LightsOff ? 512 : 0;

			// Vacuum.Y is the leading edge of the vacuum.
			if (Vacuum.Dir == VACUUMDIR_UP)
				Vacuum.Y = (float)-ScrollY + (float)LitScreenHeight + VacuumYOffset + LightsOffset;
			else
				Vacuum.Y = (float)-ScrollY - VacuumYOffset - LightsOffset;

			Vacuum.Timer = 0;
		}
	}
	else if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		if (Dusty.State != DUSTYSTATE_DIE)
		{
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
    
    if (Vacuum.State == VACUUMSTATE_ONSCREEN || Vacuum.State == VACUUMSTATE_RETREAT)
    {
        Vacuum.BlinkTimer--;
        if (Vacuum.BlinkTimer < 0)
            Vacuum.BlinkTimer = 120 + rand() % 120;
    }
    
    // Zoom the screen when the vacuum is on screen.
    float TargetZoom;
    if (Vacuum.State == VACUUMSTATE_ONSCREEN && Dusty.State != DUSTYSTATE_DIE)
    {
        LitSceneOffsetX = Clamp(LitSceneOffsetX + (rand() % 13) - 6, -10, 10);
        LitSceneOffsetY = Clamp(LitSceneOffsetY + (rand() % 13) - 6, -10, 10);
        TargetZoom = 1.1f;
    }
    else
    {
        LitSceneOffsetX = 0.0f;
        LitSceneOffsetY = 0.0f;
        TargetZoom = 1.0f;
    }
    LitSceneZoom = LitSceneZoom * 0.9f + TargetZoom * 0.1f;
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
    
    LitSceneOffsetX = 0.0f;
    LitSceneOffsetY = 0.0f;
    LitSceneZoom = 1.0f;
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
	// If the vacuum is disabled for this page, no vacuum forces.
	if (Chapter.PageProps.VacuumOff || Settings.DisableVacuum)
	{
		*VX = 0;
		*VY = 0;
		return;
	}

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
