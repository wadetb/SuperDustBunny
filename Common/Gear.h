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

struct SBlock;

struct SGearProperties
{
    gxSprite Sprite;
};

enum EGearState
{
    GEARSTATE_ACTIVE,
    GEARSTATE_FALLING,
    GEARSTATE_INACTIVE,
};

struct SGear
{
    EGearState State;

    SGearProperties* Props;
    
    float X, Y;
    float FloatVelocityX, FloatVelocityY;
    float FloatGravity;
	
	float AngularVelocity;
	float Angle;
    
    float Timer;
};


extern int NGears;
extern SGear Gears[];

void ParseGearProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);
void FreeGearProperties(SBlock* Block);

void CreateGear(int X, int Y, SGearProperties* Props);
void ClearGears();

void UpdateGear();
void DisplayGear();

#endif