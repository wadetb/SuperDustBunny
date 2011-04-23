//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef POWERUP_H
#define POWERUP_H

enum EPowerUpState
{
    POWERUPSTATE_INACTIVE,
    POWERUPSTATE_ACTIVE,
};

struct SPowerUp
{
    bool Jump;
    
    int Duration;
    int Value;
    
    EPowerUpState State;
};

void InitPowerUp();

void SetPowerUp(int DefinedCounter);
void UpdatePowerUp();
void DisplayPowerUp();

#endif