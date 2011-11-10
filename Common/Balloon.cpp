//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Balloon.h"
#include "chapter.h"
#include "Dusty.h"
#include "Vacuum.h"
#include "Tweak.h"
#include "Dust.h"
#include "GameScore.h"
#include "Tutorial.h"


#define MAX_BALLOONS 50


enum EBalloonState
{
    BALLOONSTATE_FLOAT,
    BALLOONSTATE_RISE,
    BALLOONSTATE_POP,
};

enum EBalloonColor
{
    BALLOONCOLOR_BLUE,
    BALLOONCOLOR_YELLOW,
    BALLOONCOLOR_RED,
    BALLOONCOLOR_PURPLE,
    BALLOONCOLOR_GREEN,
    BALLOONCOLOR_COUNT
};

struct SBalloon
{
    EBalloonState State;
    EBalloonColor Color;
    float X, Y;
    float VX, VY;
    float Timer;
    bool DustyOnBoard;
};


int NBalloons;
SBalloon Balloons[MAX_BALLOONS];

gxSprite* BalloonSprites[BALLOONCOLOR_COUNT] =
{
    &BalloonBlueSprite,
    &BalloonYellowSprite,
    &BalloonRedSprite,
    &BalloonPurpleSprite,
    &BalloonGreenSprite,
};

void CreateBalloon(float X, float Y)
{
	if (NBalloons >= MAX_BALLOONS)
		ReportError("Exceeded the maximum of %d total balloons.", MAX_BALLOONS);
    
	SBalloon* Balloon = &Balloons[NBalloons++];
    
    Balloon->State = BALLOONSTATE_FLOAT;
    
    Balloon->Timer = Random(0.0f, 10.0f);
    
    Balloon->Color = (EBalloonColor)Random(0, BALLOONCOLOR_COUNT);
    
	Balloon->X = (float)X*64 + 32;
	Balloon->Y = (float)Y*64 + 32;
    
    Balloon->VX = 0;
    Balloon->VY = 0;
    
    Balloon->DustyOnBoard = false;
    
    EraseBlock((int)X, (int)Y);
}

void ClearBalloons()
{
	NBalloons = 0;
}

void DisplayBalloons()
{
    for (int i = 0; i < NBalloons; i++)
    {
        SBalloon* Balloon = &Balloons[i];
        
        if (Balloon->State == BALLOONSTATE_FLOAT || Balloon->State == BALLOONSTATE_RISE)
        {
            float dx = cosf(Balloon->Timer*2.0f) * 5.0f + cosf(Balloon->Timer*1.0f/6.0f) * 5.0f;
            float dy = sinf(Balloon->Timer*2.0f) * 5.0f + sinf(Balloon->Timer*1.0f/6.0f) * 5.0f;

            AddLitSprite(LIGHTLIST_FOREGROUND, BalloonSprites[Balloon->Color], 
                                    Balloon->X - 64 + ScrollX + dx, 
                                    Balloon->Y - 64 + ScrollY + dy);            
        }
        else if (Balloon->State == BALLOONSTATE_POP)
        {
        }
    }
}

void UpdateBalloons()
{
    for (int i = 0; i < NBalloons; i++)
    {
        SBalloon* Balloon = &Balloons[i];
        
        Balloon->Timer += 1.0f / 60.0f;
        
        if (Balloon->State == BALLOONSTATE_FLOAT || Balloon->State == BALLOONSTATE_RISE)
        {
            if (!Balloon->DustyOnBoard)
            {
                if (fabsf(Dusty.FloatX - Balloon->X) < 30 && Dusty.FloatY >= Balloon->Y + 128 && Dusty.FloatY <= Balloon->Y + 384)
                {
                    Balloon->VX += Dusty.FloatVelocityX*0.25f;
                    Balloon->VY += Dusty.FloatVelocityY*0.25f;
                    Balloon->VY += 3;

                    SetDustyState_BalloonRide();
                    
                    if (Dusty.FloatX > Balloon->X)
                        Dusty.Direction = DIRECTION_RIGHT;
                    else
                        Dusty.Direction = DIRECTION_LEFT;
                    
                    Balloon->DustyOnBoard = true;
                }
            }
        }
        if (Balloon->State == BALLOONSTATE_FLOAT)
        {
            if (Balloon->DustyOnBoard)
            {
                Balloon->Timer = 0;
                
                Balloon->State = BALLOONSTATE_RISE;
                continue;
            }
        }
        
        if (Balloon->State == BALLOONSTATE_RISE)
        {
            if (Balloon->Timer >= 3.0f)
            {
                Balloon->State = BALLOONSTATE_POP;
                
                if (Balloon->DustyOnBoard)
                    SetDustyState_Launch(0, Balloon->VY);
                
                continue;
            }
            
            Balloon->X += Balloon->VX;
            Balloon->Y += Balloon->VY;
            
            if (Balloon->DustyOnBoard)
            {
                Dusty.FloatX += Balloon->VX;
                Dusty.FloatY += Balloon->VY;
                
                if (Dusty.State != DUSTYSTATE_BALLOONRIDE)
                    Balloon->DustyOnBoard = false;

                if (Balloon->VY >= -20)
                    Balloon->VY -= 0.2f;
                else
                    Balloon->VY += 0.2f;
            }
            else
            {
                if (Balloon->VY >= -30)
                    Balloon->VY -= 0.3f;
            }
            
            Balloon->VX *= 0.98f;
            Balloon->VY *= 0.98f;
        }
    }
}
