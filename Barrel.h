//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef BARREL_H
#define BARREL_H

enum EBarrelState
{
	BARRELSTATE_WAIT,
	BARRELSTATE_TURN,
	BARRELSTATE_LAUNCH,
	BARRELSTATE_RESET,
};

struct SBarrel
{
	float X, Y;

	EBarrelState State;

	float FromDir;
	float ToDir;
	float Dir;

	int Timer;
};

void CreateBarrel(int X, int Y, const char* Desc);
void ClearBarrels();

void UpdateBarrels();
void DisplayBarrels_BeforeDusty();
void DisplayBarrels_AfterDusty();

#endif
