//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef GEAR_H
#define GEAR_H


enum EGearState
{
    GEARSTATE_ACTIVE,
    GEARSTATE_FALLING,
    GEARSTATE_INACTIVE,
};

struct SGear
{
    EGearState State;

    float X, Y;
    float FloatVelocityX, FloatVelocityY;
    float FloatGravity;
	
	float AngularVelocity;
	float Angle;
};

extern SGear Gear;

void CreateGear(int X, int Y);
void ClearGears();

void UpdateGear();
void DisplayGear();

extern int NGears;
extern SGear Gears[];

#endif