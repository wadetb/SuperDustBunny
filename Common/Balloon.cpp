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

#define BALLOON_TAIL_COUNT  10
#define BALLOON_TAIL_LENGTH 250.0f
#define BALLOON_TAIL_OFFSET 80

struct SBalloon
{
    EBalloonState State;
    EBalloonColor Color;
    float X, Y;
    float VX, VY;
    float Timer;
    bool DustyOnBoard;
    float TailX[BALLOON_TAIL_COUNT];
    float TailY[BALLOON_TAIL_COUNT];
    float TailPrevX[BALLOON_TAIL_COUNT];
    float TailPrevY[BALLOON_TAIL_COUNT];
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
    
    for (int i = 0; i < BALLOON_TAIL_COUNT; i++)
    {
        Balloon->TailX[i] = Balloon->TailPrevX[i] = Balloon->X;
        Balloon->TailY[i] = Balloon->TailPrevY[i] = Balloon->Y + BALLOON_TAIL_OFFSET + i * (BALLOON_TAIL_LENGTH / BALLOON_TAIL_COUNT);
    }
    
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

            
            SLitVertex* Verts = AddLitQuad(LIGHTLIST_FOREGROUND, &WhiteSprite, (BALLOON_TAIL_COUNT-1)*2);
            
            float X0 = Balloon->TailX[0];
            float Y0 = Balloon->TailY[0];

            for (int i = 1; i < BALLOON_TAIL_COUNT; i++)
            {
                float X1 = Balloon->TailX[i];
                float Y1 = Balloon->TailY[i];
                
                float Width = 2.0f;
                
                float PerpX = Y0 - Y1;
                float PerpY = -(X0 - X1);
                float Length = sqrtf(PerpX*PerpX + PerpY*PerpY);
                if (Length > 0)
                {
                    PerpX *= Width / Length;
                    PerpY *= Width / Length;
                }
                
                float U = (float)i / (float)(BALLOON_TAIL_COUNT-1);
                
                Verts->X = X0 - PerpX + ScrollX;
                Verts->Y = Y0 - PerpY + ScrollY;
                Verts->X = LitSceneOffsetX + (Verts->X - (gxScreenWidth/2)) * LitSceneZoom + (gxScreenWidth/2);
                Verts->Y = LitSceneOffsetY + (Verts->Y - (LitScreenHeight/2)) * LitSceneZoom + (LitScreenHeight/2);
                Verts->U = U;
                Verts->V = 0.0f;
                Verts->Color = gxRGBA32(0,0,0,255);
                Verts++;
                
                Verts->X = X0 + PerpX + ScrollX;
                Verts->Y = Y0 + PerpY + ScrollY;
                Verts->X = LitSceneOffsetX + (Verts->X - (gxScreenWidth/2)) * LitSceneZoom + (gxScreenWidth/2);
                Verts->Y = LitSceneOffsetY + (Verts->Y - (LitScreenHeight/2)) * LitSceneZoom + (LitScreenHeight/2);
                Verts->U = U;
                Verts->V = 1.0f;
                Verts->Color = gxRGBA32(0,0,0,255);
                Verts++;
                
                X0 = X1;
                Y0 = Y1;
            }
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
                if (fabsf(Dusty.FloatX - Balloon->X) < 30 && 
                    Dusty.FloatY >= Balloon->Y + BALLOON_TAIL_OFFSET*0.5f && 
                    Dusty.FloatY <= Balloon->Y + BALLOON_TAIL_LENGTH*1.5f)
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
                
                Balloon->TailPrevX[0] += Random(-30, 30);
                
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
        
//        for (int i = 0; i < BALLOON_TAIL_COUNT; i++)
//        {
//            Balloon->TailPrevX[i] = Balloon->TailX[i];
//            Balloon->TailPrevY[i] = Balloon->TailY[i];
//        }

        float dx = cosf(Balloon->Timer*2.0f) * 5.0f + cosf(Balloon->Timer*1.0f/6.0f) * 5.0f;
        float dy = sinf(Balloon->Timer*2.0f) * 5.0f + sinf(Balloon->Timer*1.0f/6.0f) * 5.0f;
        
//        if (Balloon->State != BALLOONSTATE_POP)
//        {
            Balloon->TailX[0] = Balloon->X + dx;
            Balloon->TailY[0] = Balloon->Y + 80 + dy;
//        }

        for (int i = 1; i < BALLOON_TAIL_COUNT; i++)
            Balloon->TailY[i] += 5.0f;

        for (int i = 1; i < BALLOON_TAIL_COUNT; i++)
        {
            float X0 = Balloon->TailX[i-1];
            float Y0 = Balloon->TailY[i-1];
            float X1 = Balloon->TailX[i];
            float Y1 = Balloon->TailY[i];
            
            float D = Distance(X0, Y0, X1, Y1);
            if (D>0)
            {
                X1 = X0 + (X1-X0)/D * (BALLOON_TAIL_LENGTH / BALLOON_TAIL_COUNT);
                Y1 = Y0 + (Y1-Y0)/D * (BALLOON_TAIL_LENGTH / BALLOON_TAIL_COUNT);
            }
            
            Balloon->TailX[i] = X1;
            Balloon->TailY[i] = Y1;
        }

//        if (Balloon->State != BALLOONSTATE_POP)
//        {
//            Balloon->TailX[0] = Balloon->X + dx;
//            Balloon->TailY[0] = Balloon->Y + BALLOON_TAIL_OFFSET + dy;
//        }

//        for (int i = 0; i < BALLOON_TAIL_COUNT; i++)
//        {
//            Balloon->TailX[i] += (Balloon->TailX[i] - Balloon->TailPrevX[i])*0.75f;
//            Balloon->TailY[i] += (Balloon->TailY[i] - Balloon->TailPrevY[i])*0.75f;
//        }
    }
}
