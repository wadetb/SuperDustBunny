//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Debug.h"


#define MAX_DEBUG_LINES 100


struct SDebugLine
{
    int EndFrame;
    float X1, Y1;
    float X2, Y2;
    unsigned int Color;
};


SDebugLine DebugLines[MAX_DEBUG_LINES];
int CurFrame;

void AddDebugLine(float X1, float Y1, float X2, float Y2, unsigned int Color, float Time)
{    
    for (int i = 0; i < MAX_DEBUG_LINES; i++)
    {
        if (DebugLines[i].EndFrame <= CurFrame)
        {
            SDebugLine* DebugLine = &DebugLines[i];
            DebugLine->X1 = X1;
            DebugLine->Y1 = Y1;
            DebugLine->X2 = X2;
            DebugLine->Y2 = Y2;
            DebugLine->Color = Color;
            DebugLine->EndFrame = CurFrame + int(Time*60.0f);
            return;
        }
    }
}

void DisplayDebugLine(float X1, float Y1, float X2, float Y2, float Width, unsigned int Color)
{
    float PerpX = Y2 - Y1;
    float PerpY = -(X2 - X1);
    float L = Length(PerpX, PerpY);
    if (Length > 0)
    {
        PerpX *= Width / L;
        PerpY *= Width / L;
    }
    else
    {
        PerpX = 0;
        PerpY = 0;
    }
    
    AddLitQuad(LIGHTLIST_WIPE, &WhiteSprite, Color,
               X1 - PerpX, Y1 - PerpY, 0, 0,
               X1 + PerpX, Y1 + PerpY, 0, 0,
               X2 + PerpX, Y2 + PerpY, 0, 0,
               X2 - PerpX, Y2 - PerpY, 0, 0);    
}

void DisplayDebug()
{
    for (int i = 0; i < MAX_DEBUG_LINES; i++)
    {
        if (DebugLines[i].EndFrame > CurFrame)
        {
            SDebugLine* DebugLine = &DebugLines[i];
            
            float Width = 4.0f;
            
            DisplayDebugLine(DebugLine->X1, DebugLine->Y1, DebugLine->X2, DebugLine->Y2, Width, DebugLine->Color);
        }
    }
    
    CurFrame++;
}
