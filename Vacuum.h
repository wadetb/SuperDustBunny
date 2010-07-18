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

struct SVacuum
{
	int Distance;
	int Y;
};

void Vacuum_Init(SVacuum* Vacuum);
void Vacuum_Display(SVacuum* Vacuum);
void Vacuum_Update(SVacuum* Vacuum);

#endif
