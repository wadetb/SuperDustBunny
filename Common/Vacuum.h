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
	VACUUMSTATE_FAR,
	VACUUMSTATE_ONSCREEN,
	VACUUMSTATE_RETREAT,
};

enum EVacuumDir
{
	VACUUMDIR_UP,
	VACUUMDIR_DOWN
};

struct SVacuum
{
	EVacuumDir Dir;

	EVacuumState State;

	int Timer;
	float Y;

	float Volume;
};

extern SVacuum Vacuum;

void InitVacuum();
void DisplayVacuum();
void UpdateVacuum();
void UpdateVacuumSound();

void TurnOnVacuum();
void TurnOffVacuum();
void JamVacuum();

bool IsInVacuum(float Y);

void GetVacuumForce(float X, float Y, float* VX, float* VY, float Strength);

#endif
