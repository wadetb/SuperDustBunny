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

SPowerUp PowerUp;

void InitPowerUp()
{
    PowerUp.Jump = false;

    PowerUp.Duration = 0;
    PowerUp.Value = 0;
    
    PowerUp.State = POWERUPSTATE_INACTIVE;
}

void SetPowerUp(int DefinedCounter)
{
    PowerUp.Duration = 500;
    
    PowerUp.Value = DefinedCounter;
    
    
    
    PowerUp.State = POWERUPSTATE_ACTIVE;

}

void DisplayPowerUp()
{

}

void UpdatePowerUp()
{
    if(PowerUp.State == POWERUPSTATE_INACTIVE)
    {
        
    }
    
    if (PowerUp.State == POWERUPSTATE_ACTIVE)
    {       
        if (PowerUp.Duration == 0)
        {
            PowerUp.Jump = false;
        }
        else
        {
            PowerUp.Duration --;
        }     
    
    
    }
    

}
