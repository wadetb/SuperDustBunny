//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef __WIPE_H__
#define __WIPE_H__

enum EWipeType
{
	WIPE_NONE,
	WIPE_DIAGONAL,
	WIPE_FADE_TO_BLACK,
	WIPE_FADE_TO_WHITE,
};

struct SWipe
{
	EWipeType Type;
	float T;
	float Time;
	bool Middle;
	bool Finished;
};

extern SWipe Wipe;

void StartWipe(EWipeType Type, float Time);

void UpdateWipe();
void DisplayWipe();

#endif
