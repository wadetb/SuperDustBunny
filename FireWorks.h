//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef FIREWORKS_H
#define FIREWORKS_H

enum EFireWorkState
{
	FIREWORKSTATE_WAIT,
	FIREWORKSTATE_FUSE,
	FIREWORKSTATE_LAUNCH,
	FIREWORKSTATE_EXPLODE,
	FIREWORKSTATE_DONE,
};

struct SFireWork
{
	EFireWorkState State;

	float X, Y;
	float OriginalX, OriginalY;
	float VelocityX, VelocityY;

	int FlightDistance;
	int FlightDir;
	int ExplosionSize;

	int Timer;
};

extern SFireWork FireWork;
void CreateFireWork(int X, int Y, const char* Desc);
void ClearFireWorks();

void DisplayFireWorks();
void UpdateFireWorks();

#endif
