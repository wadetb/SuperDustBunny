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
#include "Gear.h"


enum ETutorialState
{
    TUTORIAL_INACTIVE,
    TUTORIAL_FLICK_TO_JUMP,
    TUTORIAL_BONUS,
};


struct STutorial
{
    ETutorialState State;
    float X, Y;
    float FadeOutTimer;
    float WiggleTimer;
};


STutorial Tutorial;


void InitTutorial()
{
    if (CurrentChapter == 0 && Chapter.PageNum == 0)
        Tutorial.State = TUTORIAL_FLICK_TO_JUMP;
    else if (CurrentChapter == 0 && Chapter.PageNum == 1 && NGears > 0)
        Tutorial.State = TUTORIAL_BONUS;
    else
        Tutorial.State = TUTORIAL_INACTIVE;

    Tutorial.FadeOutTimer = 0.0f;
    Tutorial.WiggleTimer = 0.0f;
}

void DisplayTutorial()
{
    if (Tutorial.State == TUTORIAL_INACTIVE)
        return;

    float X = Tutorial.X;
    float Y = Tutorial.Y;

    X += cosf(Tutorial.WiggleTimer*4.0f) * 2.5f + cosf(Tutorial.WiggleTimer*1.0f/3.0f) * 2.5f;
    Y += sinf(Tutorial.WiggleTimer*4.0f) * 2.5f + sinf(Tutorial.WiggleTimer*1.0f/3.0f) * 2.5f;

    float Alpha = 1.0f;
    Alpha *= Remap(Tutorial.WiggleTimer, 0.0f, 0.5f, 0.0f, 1.0f, true);
    Alpha *= Remap(Tutorial.FadeOutTimer, 0.0f, 0.25f, 1.0f, 0.0f, true);
    
    if (Alpha > 0)
    {
        if (Tutorial.State == TUTORIAL_FLICK_TO_JUMP)
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &TextFlickToJumpSprite, X + ScrollX, Y + ScrollY, 1.0f, Alpha);
        }
        else if (Tutorial.State == TUTORIAL_BONUS)
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &TextBonusSprite, X + ScrollX, Y + ScrollY, 1.0f, Alpha);            
        }
    }
}

void UpdateTutorial()
{   
    if (Tutorial.State == TUTORIAL_INACTIVE)
        return;
    
    if (Tutorial.State == TUTORIAL_FLICK_TO_JUMP)
    {
        if (Dusty.HasJumped)
        {
            Tutorial.FadeOutTimer += 1.0f/60.0f;
        }
        else
        {
            Tutorial.X = Dusty.FloatX + 175;
            Tutorial.Y = Dusty.FloatY - 350;
        }
    }
    else if (Tutorial.State == TUTORIAL_BONUS)
    {
        SGear* LastGear = &Gears[NGears-1];
        
        if (LastGear->State != GEARSTATE_ACTIVE)
        {
            Tutorial.FadeOutTimer += 1.0f/60.0f;
        }
        else
        {
            Tutorial.X = LastGear->X - 190;
            Tutorial.Y = LastGear->Y - 160;
        }
    }

    Tutorial.WiggleTimer += 1.0f/60.0f;
}

