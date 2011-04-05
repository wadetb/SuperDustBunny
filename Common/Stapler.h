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

enum EStaplerState
{
    STAPLERSTATE_WAIT,
    STAPLERSTATE_PRELAUNCH,
    STAPLERSTATE_LAUNCH,
    STAPLERSTATE_RESET,
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

    EStaplerState State;

    int Timer;
    float PowerJump;
    int PowerJumpCounter;

	bool CollideWithLeftSide;
	bool CollideWithRightSide;
	bool CollideWithTopSide;

	bool CanLaunch;
	
	int TimerWait;
};

void CreateStapler(int X, int Y);
void ClearStaplers();

void UpdateStaplers();
void DisplayStaplers();
void UpdateStapler_Collision();

void ParseStaplerProperties(struct SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

#endif
