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

struct SPowerUpToggle
{
    bool Jump;
    
};

struct SPowerUp
{  
    int Duration;
    int Value;
    float IncreaseJump;

    float X, Y;
    float FloatVelocityY;
    float FloatVelocityX;
    
    EPowerUpState State;
};

extern SPowerUp PowerUp;
extern SPowerUpToggle PowerUpToggle;

void CreatePowerUp(int X, int Y);

void ClearPowerUps();

void InitPowerUp();

void UpdatePowerUp();
void DisplayPowerUp();

#endif