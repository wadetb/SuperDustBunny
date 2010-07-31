﻿//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "TennisBall.h"
#include "Common.h"
#include "Dusty.h"
#include "Vacuum.h"

#define MAX_BALLS 100

int NBalls = 0;

SBall Balls[MAX_BALLS];

void CreateBall(int X, int Y, const char* Desc)
{
    SBall* Ball = &Balls[NBalls++];

    Ball->X = X + 32;
    Ball->Y = Y + 32;
    
    Ball->Transition = 40;
    Ball->Sprite = 1;

    Ball->Collided = false;
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

        if (Ball->Sprite == 1) 
        {
           gxDrawSpriteCenteredRotated( Ball->X, Ball->Y + ScrollY, 0, &Ball01 );
        } 

        if (Ball->Sprite == 2)
        {
            gxDrawSpriteCenteredRotated( Ball->X, Ball->Y + ScrollY, 0, &Ball02 );
        } 

        if (Ball->Sprite == 3)
        {
            gxDrawSpriteCenteredRotated( Ball->X, Ball->Y + ScrollY, 0, &Ball03 );
        }

        if (Ball->Sprite == 4)
        {
            gxDrawSpriteCenteredRotated( Ball->X, Ball->Y + ScrollY, 0, &Ball04 );
        }   
    }    
}

void UpdateBall()
{
    for (int i = 0; i < NBalls; i++)
    {
        SBall* Ball = &Balls[i];
        
        float XDist = (float)(Dusty.FloatX - Ball->X);
        float YDist = (float)((Dusty.FloatY-50) - (Ball->Y));
        float Dist = sqrtf(XDist*XDist + YDist*YDist);
        
        if (Dist < 100)
        {
            Ball->Collided = true;
        }
        
        if (Ball->Collided == true)
        {
            Ball->Y += 7;
        }
        
        if (Dist >= gxScreenHeight)
        {
            sxPlaySound (&Clogged);
            //Vacuum.Timer += 500;
        }       
                          
        if (Ball->Transition == 40)
        {
            Ball->Sprite = 1;
        }  

        if (Ball->Transition == 30)
        {
            Ball->Sprite = 2;
        }  

        if (Ball->Transition == 20)
        {
            Ball->Sprite = 3;
        } 

        if (Ball->Transition == 10)
        {
            Ball->Sprite = 4;
        }  

        if (Ball->Transition <= 0)
        {
            Ball->Transition = 40;
        }  

        Ball->Transition -= 1;          
    }    
}