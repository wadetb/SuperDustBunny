//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef INPUT_H
#define INPUT_H

void GetInput_BeginSwipe(float X, float Y, double Time);
void GetInput_AddToSwipe(float X, float Y, double Time);
void GetInput_EndSwipe(float X, float Y, double Time);

float GetInput_GetSwipeTimeLeft();
float GetInput_GetSwipeCurrent();
void GetInput_GetSwipePosAtTime(float* X, float* Y, double Time);

void GetInput_ConsumeSwipe(float Dist);
void GetInput_ConsumeAllSwipe();

void DisplaySwipe();

#endif
