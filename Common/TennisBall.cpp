//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "TennisBall.h"
#include "Dusty.h"
#include "Vacuum.h"
#include "GameScore.h"

#define MAX_BALLS 100

int NBalls = 0;

SBall Balls[MAX_BALLS];

void CreateBall(int X, int Y)
{
/*
	if (NBalls >= MAX_BALLS)
		ReportError("Exceeded the maximum of %d total balls.", MAX_BALLS);

	SBall* Ball = &Balls[NBalls++];

    Ball->X = (float)X + 32;
    Ball->Y = (float)Y + 32;
    
    Ball->FloatVelocityY = 0.0f;
    
    Ball->Transition = 40;
    Ball->Sprite = 1;

    Ball->Collided = false;

	Ball->State = BALLSTATE_ACTIVE;
*/
}

void ClearBalls()
{
/*
	NBalls = 0;
*/
}

void DisplayBall()
{
/*
    for (int i = 0; i < NBalls; i++)
    {
        SBall* Ball = &Balls[i];

		if (Ball->State == BALLSTATE_INACTIVE)
			continue;

        if (Ball->Sprite == 1) 
        {
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_VACUUM, &TennisBallSpin1Sprite, Ball->X + ScrollX, Ball->Y + ScrollY, 1.0f, 0.0f);
        } 

        if (Ball->Sprite == 2)
        {
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TennisBallSpin2Sprite, Ball->X + ScrollX, Ball->Y + ScrollY, 1.0f, 0.0f);
        } 

        if (Ball->Sprite == 3)
        {
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TennisBallSpin3Sprite, Ball->X + ScrollX, Ball->Y + ScrollY, 1.0f, 0.0f);
        }

        if (Ball->Sprite == 4)
        {
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TennisBallSpin4Sprite, Ball->X + ScrollX, Ball->Y + ScrollY, 1.0f, 0.0f);
        }   
    }    
*/
}

void UpdateBall()
{
/*
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
            }      
        }
        else if (Ball->State == BALLSTATE_FALLING)
        {
            Ball->Y += Ball->FloatVelocityY;
            Ball->FloatVelocityY += 1.0f;

            if (IsInVacuum(Ball->X, Ball->Y))
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
 */
}