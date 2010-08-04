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
    GEARSTATE_INACTIVE,
};

struct SGear
{
    EGearState State;

    int Sprite;
    int Transition;

    int X, Y;
    float FloatVelocityY;
    float FloatGravity;

    bool Collided;

};

extern SGear Gear;
void CreateGear(int X, int Y, const char* Desc);
void ClearGears();

void UpdateGear();
void DisplayGear();

#endif