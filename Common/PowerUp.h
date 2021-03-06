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

#define MAX_POWERUP 100

enum EPowerUpState
{
    POWERUPSTATE_INACTIVE,
    POWERUPSTATE_ACTIVE,
};

struct SPowerUp
{  
    float X, Y;
    float PulseTimer;
    EPowerUpState State;
};

extern int NPowerUps;
extern SPowerUp PowerUps[MAX_POWERUP];

void CreatePowerUp(int X, int Y);

void ClearPowerUps();

void InitPowerUp();

void UpdatePowerUp();
void DisplayPowerUp();

void ActivatePowerUp(SPowerUp* PowerUp);

#endif