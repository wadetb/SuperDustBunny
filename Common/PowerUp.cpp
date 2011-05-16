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
#include "Tutorial.h"
#include "GameScore.h"

#define MAX_POWERUP 100

int NPowerUps = 0;
SPowerUp PowerUps[MAX_POWERUP];

void InitPowerUp()
{
    SPowerUp* PowerUp = &PowerUps[NPowerUps++];
    
    PowerUpToggle.Jump = false;
    
    PowerUp->IncreaseJump = 10.0f;
        
    PowerUp->Duration = 500;
    
    PowerUp->Value = 0;
    
    PowerUp->State = POWERUPSTATE_INACTIVE;
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

    PowerUp->State = POWERUPSTATE_INACTIVE;
    
    PowerUp->X = (float)X + 32;
    PowerUp->Y = (float)Y + 32;
    PowerUp->FloatVelocityY = 0.0f;
    PowerUp->FloatVelocityX = 0.0f;
}

void DisplayPowerUp()
{
    for (int i = 0; i < NPowerUps; i++)
    {
        SPowerUp* PowerUp = &PowerUps[i];
        
        if (PowerUp->State == POWERUPSTATE_ACTIVE)
            continue;
        
        AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerUpSprite, PowerUp->X, PowerUp->Y + ScrollY - 30, 1.0f, 0.0f);
    }
}

void UpdatePowerUp()
{
    for (int i = 0; i < NPowerUps; i++)
    {
        SPowerUp* PowerUp = &PowerUps[i];

        if (PowerUp->State == POWERUPSTATE_INACTIVE)
        {
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, PowerUp->X, PowerUp->Y);

            if (Dist < 100)
            {
                PowerUp->Duration = 750;
                PowerUpToggle.Jump = true;

                PowerUp->State = POWERUPSTATE_ACTIVE;             
            } 	                       	    
        }
        
        //Boost Dusty's overall performance
        if (PowerUp->State == POWERUPSTATE_ACTIVE)
        {
           
                                          
            if (PowerUp->Duration == 0)
            {
                PowerUp->X = -100;
                PowerUpToggle.Jump = false;
                PowerUp->State = POWERUPSTATE_INACTIVE;
            }
            else
            {
                PowerUp->Duration --;
            }     
        }
    }
}
