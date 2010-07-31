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
	FIREWORKSTATE_LAUNCH,
	FIREWORKSTATE_EXPLODE,
	FIREWORKSTATE_DONE,
};

struct SFireWork
{
	float X, Y;

	EFireWorkState State;

	int Dir;

	int Timer;
};

void CreateFireWork(int X, int Y, const char* Desc);
void ClearFireWorks();

void DisplayFireWorks();
void UpdateFireWorks();

#endif
