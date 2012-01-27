//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef BABY_H
#define BABY_H

void CreateBaby(float X, int Y, unsigned int Flags, int Hat, bool StartFollowing);
void ClearBabies();

void UpdateBabies();
void DisplayBabies();

void SendBabyToGather(float X, float Y);

#endif
