//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef STAPLER_H
#define STAPLER_H

enum EStaplerType
{
    STAPLER_STAPLER,
    STAPLER_JELLO,
    STAPLER_CLOTHESPIN,
};

enum EStaplerState
{
    STAPLERSTATE_WAIT,
    STAPLERSTATE_CHARGE,
    STAPLERSTATE_LAUNCH,
};

struct SStaplerProperties
{
    int From;
    int To;
};

struct SStapler
{
    float X, Y;

	float Left;
	float Right;
	float Bottom;
	float Top;
    
    EStaplerType Type;
    EStaplerState State;

    int Timer;

    float ChargeTime;
    
	bool CollideWithLeftSide;
	bool CollideWithRightSide;
	bool CollideWithTopSide;
    
    bool NoShadow;
};


SStapler* CreateStapler(int X, int Y, EStaplerType Type);
void ClearStaplers();

void UpdateStaplers();
void DisplayStaplers();
void UpdateStapler_Collision();

void ParseStaplerProperties(struct SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

#endif
