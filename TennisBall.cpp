//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "TennisBall.h"
#include "Dusty.h"
#include "Tutorial.h"
#include "Vacuum.h"

#define MAX_BALLS 100

int NBalls = 0;

SBall Balls[MAX_BALLS];

void CreateBall(int X, int Y)
{
    SBall* Ball = &Balls[NBalls++];

    Ball->X = (float)X + 32;
    Ball->Y = (float)Y + 32;
    
    Ball->FloatVelocityY = 0.0f;
    Ball->FloatGravity = 0.5f;
    
    Ball->Transition = 40;
    Ball->Sprite = 1;

    Ball->Collided = false;

	Ball->State = BALLSTATE_ACTIVE;
}

void ClearBalls()
{
	NBalls = 0;
}

extern int ScrollY;

void DisplayBall()
{
    for (int i = 0; i < NBalls; i++)
    {
        SBall* Ball = &Balls[i];

		if (Ball->State == BALLSTATE_INACTIVE)
			continue;

        if (Ball->Sprite == 1) 
        {
           gxDrawSpriteCenteredRotated( (int)Ball->X, (int)(Ball->Y + ScrollY), 0, &TennisBallSpin1Sprite );
        } 

        if (Ball->Sprite == 2)
        {
            gxDrawSpriteCenteredRotated( (int)Ball->X, (int)(Ball->Y + ScrollY), 0, &TennisBallSpin2Sprite );
        } 

        if (Ball->Sprite == 3)
        {
            gxDrawSpriteCenteredRotated( (int)Ball->X, (int)(Ball->Y + ScrollY), 0, &TennisBallSpin3Sprite );
        }

        if (Ball->Sprite == 4)
        {
            gxDrawSpriteCenteredRotated( (int)Ball->X, (int)(Ball->Y + ScrollY), 0, &TennisBallSpin4Sprite );
        }   
    }    
}

void UpdateBall()
{
    for (int i = 0; i < NBalls; i++)
    {
        SBall* Ball = &Balls[i];

		if (Ball->State == BALLSTATE_INACTIVE)
			continue;
       
        if (Ball->State == BALLSTATE_ACTIVE)
        {
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, Ball->X, Ball->Y);

            if (Dist < 125)
            {
                Ball->State = BALLSTATE_FALLING;        

                sxPlaySound( &VacuumClogSound );  

                if (Tutorial.BallDisplayed == false)
                {
                    SetGameState_Tutorial(TUTORIALSTATE_BALL);
                    return;
                }
            }      
        }
        else if (Ball->State == BALLSTATE_FALLING)
        {
            Ball->Y += Ball->FloatVelocityY;
            Ball->FloatVelocityY += 1.0f;

            if (Ball->Y >= Vacuum.Y + 175)
            {
                Ball->State = BALLSTATE_INACTIVE;
                sxPlaySound (&VacuumClogSound);
                JamVacuum();
            }
        }
        
        Ball->Transition -= 1;
        if (Ball->Transition == 0)
        {
            Ball->Transition = 5;

            Ball->Sprite += 1;
            if (Ball->Sprite == 5)
            {
                Ball->Sprite = 1;
            }  
        }              
    }    
}