//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef __VACUUM_H__
#define __VACUUM_H__

enum EVacuumState
{
	VACUUMSTATE_OFF,
	VACUUMSTATE_ONSCREEN,
	VACUUMSTATE_RETREAT,
};

enum EVacuumType
{
	VACUUM_NORMAL,
	VACUUM_DUSTBUSTER
};

enum EVacuumDir
{
	VACUUMDIR_UP,
	VACUUMDIR_DOWN
};

enum EVacuumSide
{
	VACUUMSIDE_LEFT,
	VACUUMSIDE_RIGHT
};

struct SVacuum
{
	EVacuumDir Dir;
    EVacuumType Type;
    EVacuumSide Side;
    
	EVacuumState State;

    bool Charging;
    
	int Timer;
    
    float X;
	float Y;

	float Volume;
    
    int BlinkTimer;

    int* ForceMap;
    int* NextForceMap;
    int ForceMapWidth;
    int ForceMapHeight;
};

struct SBlock;
struct SVacuumTriggerProperties;

extern SVacuum Vacuum;

void InitVacuum();
void DisplayVacuum();
void UpdateVacuum();
void UpdateVacuumSound();

void TurnOnVacuum(float InitialDistance, float DelayBeforeMoving, bool Charging);
void TurnOffVacuum();
void JamVacuum();

bool IsInVacuum(float X, float Y);

void GetVacuumForce(float X, float Y, float* VX, float* VY, float Strength, bool FollowLevel);

void ParseVacuumTriggerProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

void CreateVacuumTrigger(int X, int Y, SVacuumTriggerProperties* Props);
void ClearVacuumTriggers();
void UpdateVacuumTriggers();

#endif
