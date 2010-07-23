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
};

struct SVacuum
{
	EVacuumState State;

	int Timer;
	int Y;

	float Volume;
};

void InitVacuum();
void DisplayVacuum_BeforeDusty();
void DisplayVacuum_AfterDusty();
void UpdateVacuum();

#endif
