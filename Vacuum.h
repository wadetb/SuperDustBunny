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

struct SVacuum
{
	EVacuumState State;

	int Timer;
	float Y;

	float Volume;
};

extern int Timer;
extern SVacuum Vacuum;

void InitVacuum();
void DisplayVacuum_BeforeDusty();
void DisplayVacuum_AfterDusty();
void UpdateVacuum();

void JamVacuum();

#endif
