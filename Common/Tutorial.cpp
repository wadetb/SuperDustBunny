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
#include "Coin.h"
#include "Vacuum.h"
#include "Text.h"


enum ETutorialState
{
    TUTORIAL_INACTIVE,
    TUTORIAL_FLICK_TO_JUMP,
    TUTORIAL_VACUUM_INTRO_1,
    TUTORIAL_VACUUM_INTRO_2,
    TUTORIAL_VACUUM_INTRO_3,
    TUTORIAL_BONUS,
    TUTORIAL_EXTRA_LIFE,
};


struct STutorial
{
    ETutorialState State;
    
    float X, Y;
    
    float WaitTimer;
    float StateTimer;
    float FadeOutTimer;
    float FadeInTimer;
    float WiggleTimer;
    
    bool DustyHasJumped;
};


STutorial Tutorial;


void InitTutorial()
{
    if (CurrentChapter == 0 && Chapter.PageNum == 0)
        Tutorial.State = TUTORIAL_FLICK_TO_JUMP;
    else if (CurrentChapter == 0 && Chapter.PageNum == 1 && NCoins > 0)
        Tutorial.State = TUTORIAL_EXTRA_LIFE;
//    else if (CurrentChapter == 0 && Chapter.PageNum == 2 && NGears > 0)
//        Tutorial.State = TUTORIAL_BONUS;
    else
        Tutorial.State = TUTORIAL_INACTIVE;

    Tutorial.WaitTimer = 0.0f;
    Tutorial.StateTimer = 0.0f;
    Tutorial.FadeOutTimer = 0.0f;
    Tutorial.FadeInTimer = 0.0f;
    Tutorial.WiggleTimer = 0.0f;
    
    Tutorial.DustyHasJumped = false;
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
    Alpha *= Remap(Tutorial.FadeInTimer, 0.0f, 0.5f, 0.0f, 1.0f, true);
    Alpha *= Remap(Tutorial.FadeOutTimer, 0.0f, 0.25f, 1.0f, 0.0f, true);
    
    if (Alpha > 0)
    {
        if (Tutorial.State == TUTORIAL_FLICK_TO_JUMP)
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &TextFlickToJumpSprite, X + ScrollX, Y + ScrollY, 1.0f, Alpha);
        }
        else if (Tutorial.State == TUTORIAL_VACUUM_INTRO_1)
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &TextBubbleSprite, 384, 230, 1.0f, Alpha);
            DisplayStringAlpha(LIGHTLIST_VACUUM, "mom!! lets play", FORMAT_CENTER_X, 384, 230, 1.0f, Alpha);
        }
        else if (Tutorial.State == TUTORIAL_VACUUM_INTRO_2)
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &TextBubbleSprite, 384, 300, -1.0f, Alpha);
            DisplayStringAlpha(LIGHTLIST_VACUUM, "i have to vacuum", FORMAT_CENTER_X, 384, 230, 1.0f, Alpha);
        }
        else if (Tutorial.State == TUTORIAL_VACUUM_INTRO_3)
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &TextBubbleSprite, 384, 230, 1.0f, Alpha);
            DisplayStringAlpha(LIGHTLIST_VACUUM, "look out dusty!", FORMAT_CENTER_X, 384, 230, 1.0f, Alpha);
        }
        else if (Tutorial.State == TUTORIAL_EXTRA_LIFE)
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &TextExtraLifeSprite, X + ScrollX, Y + ScrollY, 1.0f, Alpha);            
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
    
    Tutorial.WaitTimer -= 1.0f/60.0f;
    if (Tutorial.WaitTimer > 0.0f)
        return;
        
    Tutorial.WiggleTimer += 1.0f/60.0f;

    if (Tutorial.State == TUTORIAL_FLICK_TO_JUMP)
    {
        Tutorial.FadeInTimer += 1.0f/60.0f;

        if (Dusty.State == DUSTYSTATE_JUMP)
        {
            Tutorial.DustyHasJumped = true;
            Tutorial.StateTimer = 5.0f;
        }

        if (Tutorial.DustyHasJumped)
        {
            Tutorial.FadeOutTimer += 1.0f/60.0f;
            if (Tutorial.FadeOutTimer >= 0.25f)
            {
                Tutorial.State = TUTORIAL_VACUUM_INTRO_3;
                Tutorial.FadeInTimer = 0;
                Tutorial.FadeOutTimer = 0;
                Tutorial.WaitTimer = 3;
                Tutorial.StateTimer = 5.0f;
                return;
            }            
        }
        else
        {
            Tutorial.X = Dusty.FloatX + 175;
            Tutorial.Y = Dusty.FloatY - 350;            
        }
    }
    else if (Tutorial.State == TUTORIAL_VACUUM_INTRO_1)
    {
        if (Dusty.FloatY <= 86*64)
        {
            Tutorial.FadeInTimer += 1.0f/60.0f;
            Tutorial.StateTimer -= 1.0f/60.0f;
        }
        
        if (Tutorial.StateTimer <= 0.0f)
        {
            Tutorial.FadeOutTimer += 1.0f/60.0f;
            if (Tutorial.FadeOutTimer >= 0.25f)
            {
                Tutorial.State = TUTORIAL_VACUUM_INTRO_2;
                Tutorial.FadeInTimer = 0;
                Tutorial.FadeOutTimer = 0;
                Tutorial.WaitTimer = 1;
                Tutorial.StateTimer = 3.0f;
            }
        }
    }
    else if (Tutorial.State == TUTORIAL_VACUUM_INTRO_2)
    {
        Tutorial.FadeInTimer += 1.0f/60.0f;
        Tutorial.StateTimer -= 1.0f/60.0f;
        
        if (Tutorial.StateTimer <= 0.0f)
        {
            Tutorial.FadeOutTimer += 1.0f/60.0f;
            if (Tutorial.FadeOutTimer >= 0.25f)
            {
                Tutorial.State = TUTORIAL_VACUUM_INTRO_3;
                Tutorial.FadeInTimer = 0;
                Tutorial.FadeOutTimer = 0;
                Tutorial.WaitTimer = 1;
                Tutorial.StateTimer = 3.0f;
            }
        }
    }
    else if (Tutorial.State == TUTORIAL_VACUUM_INTRO_3)
    {
        Tutorial.FadeInTimer += 1.0f/60.0f;
        Tutorial.StateTimer -= 1.0f/60.0f;
        
        if (Tutorial.StateTimer <= 1.0f && Tutorial.StateTimer+1.0f/60.0f >= 1.0f)
        {
            Chapter.PageProps.VacuumOff = false;
            TurnOnVacuum();
            Vacuum.Timer = 0;            
        }
        
        if (Tutorial.StateTimer <= 0.0f)
        {
            Tutorial.FadeOutTimer += 1.0f/60.0f;
            if (Tutorial.FadeOutTimer >= 0.25f)
            {
                Tutorial.State = TUTORIAL_INACTIVE;
            }
        }
    }
    else if (Tutorial.State == TUTORIAL_EXTRA_LIFE)
    {
        Tutorial.FadeInTimer += 1.0f/60.0f;

        SCoin* LastCoin = &Coins[NCoins-1];
        
        if (LastCoin->State != COINSTATE_ACTIVE)
        {
            Tutorial.FadeOutTimer += 1.0f/60.0f;
        }
        else
        {
            Tutorial.X = LastCoin->X - 160;
            Tutorial.Y = LastCoin->Y - 200;
        }
    }
    else if (Tutorial.State == TUTORIAL_BONUS)
    {
        Tutorial.FadeInTimer += 1.0f/60.0f;

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
}

