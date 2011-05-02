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
    
    PowerUp->IncreaseJump = 18;
    
    PowerUp->Duration = 500;
    
    PowerUp->Value = 0;
    
    PowerUp->State = POWERUPSTATE_INACTIVE;
}

void SetPowerUp(int DefinedCounter)
{
    SPowerUp* PowerUp = &PowerUps[NPowerUps++];
    
    PowerUp->Duration = 500;
    
    PowerUp->Value = DefinedCounter;
    
    PowerUpToggle.Jump = true;
        
    PowerUp->State = POWERUPSTATE_ACTIVE;
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
}

void DisplayPowerUp()
{

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
                SetPowerUp(PowerUp->IncreaseJump);
            } 	                       	    
        }
        
        if (PowerUp->State == POWERUPSTATE_ACTIVE)
        {  
            //Boost Dusty's overall performance     
            Dusty.FloatVelocityX += PowerUp->Value;
            Dusty.FloatVelocityY += PowerUp->Value;
           
            if (PowerUp->Duration == 0)
            {
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
