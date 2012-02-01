//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef SMOKE_H
#define SMOKE_H

void InitSmoke();
void DisplaySmoke();
void UpdateSmoke();

void CreateSmoke(float X, float Y, int Count);
void CreateSmallSmoke(float X, float Y);
void CreateWhiteSmoke(float X, float Y);
void CreateBonus(float X, float Y);
void Create1UpBubble(float X, float Y);
void CreateBalloonPop(float X, float Y);

#endif