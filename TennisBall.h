//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef Ball_H
#define Ball_H


enum EBallState
{
    BALLSTATE_ACTIVE,
    BALLSTATE_INACTIVE,
};

struct SBall
{
    EBallState State;

    int Sprite;
    int Transition;
    
    int X, Y;

    bool Collided;

};

void CreateBall(int X, int Y, const char* Desc);

void UpdateBall();
void DisplayBall();

#endif