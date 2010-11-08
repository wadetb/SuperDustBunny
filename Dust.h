//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef DUST_H
#define DUST_H

struct SDustMote
{
	float X, Y;
	float VX, VY;
	float Size;
	float Depth;
	float Life;
	float Time;
	float Alpha;
};

void InitDust();
void DisplayDust();
void UpdateDust();

void MakeDustMote(float X, float Y);

#endif
