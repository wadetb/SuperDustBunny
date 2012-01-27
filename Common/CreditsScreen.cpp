//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "CreditsScreen.h"

struct SCreditsScreen
{
    bool CreditsPressed;
};

SCreditsScreen CreditsScreen;


void InitCreditsScreen()
{
    CreditsScreen.CreditsPressed = false;
}

void DisplayCreditsScreen()
{
    if (CreditsScreen.CreditsPressed)
    {
        AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenCredits2Sprite, 0, 0);
    }
    else
    {
        AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenCredits1Sprite, 0, 0);
    }   
}

void UpdateCreditsScreen()
{   
    bool CreditsPressed = msButton1;

    // Advance to playing state when button is released.
    if (!CreditsPressed && CreditsScreen.CreditsPressed)
    {
        SetGameState_StartScreen();;
        return;
    }

    CreditsScreen.CreditsPressed = CreditsPressed;
}