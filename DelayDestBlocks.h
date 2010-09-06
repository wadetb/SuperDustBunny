//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef DBLOCK_H
#define DBLOCK_H


enum EDBlockState
{
    DBLOCKSTATE_ACTIVE,
    DBLOCKSTATE_INACTIVE,
};

struct SDBlock
{
    EDBlockState State;

    float X, Y;
    float FloatVelocityY;
    
    int Sprite;
    int Transition;
};

void CreateDBlock(int X, int Y, const char* Desc);
void ClearDBlocks();

void UpdateDBlocks();
void DisplayDBlocks();

#endif