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
    BALLSTATE_FALLING,
};

struct SBall
{
    EBallState State;

    int Sprite;
    int Transition;
    
    float X, Y;
    float FloatVelocityY;

    bool Collided;
};

extern SBall Ball;
extern int NBalls;
extern SBall Balls[];

void CreateBall(int X, int Y);
void ClearBalls();

void UpdateBall();
void DisplayBall();

#endif
