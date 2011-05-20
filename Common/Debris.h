//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef __DEBRIS_H__
#define __DEBRIS_H__

void CreateDebris(float X, float Y, float Angle, gxSprite* Sprite);
void CreateDebris(float X, float Y, float Angle, int BlockID);

void InitDebris();
void UpdateDebris();
void DisplayDebris();

#endif
