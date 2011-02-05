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

    EStaplerState State;

    int Timer;
    int PowerJump;
    int PowerJumpCounter;
};

extern SStapler Stapler;

void ParseStaplerProperties(struct SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

void CreateStapler(int X, int Y, SStaplerProperties* Properties);
void ClearStaplers();

void UpdateStaplers();
void DisplayStaplers();

#endif
