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
	VACUUMSTATE_FAR,
	VACUUMSTATE_NEAR,
	VACUUMSTATE_ONSCREEN,
	VACUUMSTATE_RETREAT,
};

enum EVacuumSoundState
{
	VACUUMSOUNDSTATE_OFF,
	VACUUMSOUNDSTATE_TURN_ON,
	VACUUMSOUNDSTATE_TURN_OFF,
	VACUUMSOUNDSTATE_ON,
	VACUUMSOUNDSTATE_JAM,
	VACUUMSOUNDSTATE_UNJAM,
};

struct SVacuum
{
	EVacuumState State;

	int Timer;
	float Y;

	EVacuumSoundState SoundState;
	EVacuumSoundState TargetSoundState;
	int SoundTimerUp;
	int SoundTimerDown;

	float Volume;
};

extern int Timer;
extern SVacuum Vacuum;

void InitVacuum();
void DisplayVacuum_BeforeDusty();
void DisplayVacuum_AfterDusty();
void UpdateVacuum();
void UpdateVacuumSound();

void JamVacuum();
void TurnOffVacuum();

#endif
