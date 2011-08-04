//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Tutorial.h"
#include "Chapter.h"
#include "Dusty.h"

STutorial Tutorial;

void InitTutorial()
{
    Tutorial.Timer = 0.0f;
    Tutorial.WiggleTimer = 0.0f;
}

void DisplayTutorial()
{
    if (CurrentChapter == 0 && Chapter.PageNum == 0)
    {
        float X = Tutorial.X;
        float Y = Tutorial.Y;

        X += cosf(Tutorial.WiggleTimer*4.0f) * 2.5f + cosf(Tutorial.WiggleTimer*1.0f/3.0f) * 2.5f;
        Y += sinf(Tutorial.WiggleTimer*4.0f) * 2.5f + sinf(Tutorial.WiggleTimer*1.0f/3.0f) * 2.5f;

        float Alpha = Remap(Tutorial.WiggleTimer, 0.5f, 1.0f, 0.0f, 1.0f, true);
        Alpha *= Remap(Tutorial.Timer, 0.0f, 1.0f, 1.0f, 0.0f, true);
        
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &FlickToJumpSprite, X + ScrollX, Y + ScrollY, 1.0f, Alpha);
    }
}

void UpdateTutorial()
{   
    if (CurrentChapter == 0 && Chapter.PageNum == 0)
    {
        if (Dusty.HasJumped)
            Tutorial.Timer += 1.0f/60.0f;
        else
        {
            Tutorial.X = Dusty.FloatX + 250;
            Tutorial.Y = Dusty.FloatY - 250;
        }
        Tutorial.WiggleTimer += 1.0f/60.0f;
    }
}

