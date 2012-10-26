//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "PowerUp.h"
#include "Chapter.h"
#include "Dusty.h"
#include "GameScore.h"

int NPowerUps = 0;
SPowerUp PowerUps[MAX_POWERUP];

void InitPowerUp()
{
}

void ClearPowerUps()
{
    NPowerUps = 0;
}

void CreatePowerUp(int X, int Y)
{
    if (NPowerUps >= MAX_POWERUP)
        ReportError("Exceeded the maximum of %d total PowerUps.", MAX_POWERUP);

    SPowerUp* PowerUp = &PowerUps[NPowerUps++];

    PowerUp->State = POWERUPSTATE_ACTIVE;
    
    PowerUp->X = (float)X + 32;
    PowerUp->Y = (float)Y + 32;

    PowerUp->PulseTimer = 0;
}

void DisplayPowerUp()
{
    for (int i = 0; i < NPowerUps; i++)
    {
        SPowerUp* PowerUp = &PowerUps[i];
        
        if (PowerUp->State == POWERUPSTATE_ACTIVE)
        {
            float Scale = SinWave(PowerUp->PulseTimer, 0.75f, 0.25f) + 1.25f;
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerUpSprite, PowerUp->X + ScrollX, PowerUp->Y + ScrollY, Scale, 0.0f);
        }
        
        if (PowerUp->State == POWERUPSTATE_INACTIVE)
        {
            float Scale = 1.5f + (1.0f+PowerUp->PulseTimer)*(1.0f+PowerUp->PulseTimer);
            float Alpha = Remap(PowerUp->PulseTimer, 0.0f, 0.5f, 1.0f, 0.0f, true);

            if (Alpha > 0.01f)
                AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &PowerUpSprite, PowerUp->X + ScrollX, PowerUp->Y + ScrollY, Scale, Alpha);
        }
    }
}

void ActivatePowerUp(SPowerUp* PowerUp)
{
    Dusty.PowerUpTimer = 60 * 10;
    Dusty.FloatVelocityX *= 1.5f;
    Dusty.FloatVelocityY *= 1.5f;
    
    PowerUp->State = POWERUPSTATE_INACTIVE;
    PowerUp->PulseTimer = 0.0f;
}

void UpdatePowerUp()
{
    for (int i = 0; i < NPowerUps; i++)
    {
        SPowerUp* PowerUp = &PowerUps[i];

        if (PowerUp->State == POWERUPSTATE_ACTIVE)
        {
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, PowerUp->X, PowerUp->Y);

            if (Dist < 100)
            {
                ActivatePowerUp(PowerUp);
            }
        }

        PowerUp->PulseTimer += 1.0f/60.0f;
    }
}
