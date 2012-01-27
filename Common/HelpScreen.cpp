//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "HelpScreen.h"

struct SHelpScreen
{
    bool HelpPressed;
};

SHelpScreen HelpScreen;


void InitHelpScreen()
{
    HelpScreen.HelpPressed = false;
}

void DisplayHelpScreen()
{
    if (HelpScreen.HelpPressed)
    {
        AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenHelp2Sprite, 0, 0);
    }
    else
    {
        AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenHelp1Sprite, 0, 0);
    }
}

void UpdateHelpScreen()
{
    bool HelpPressed = msButton1;

    // Advance to playing state when button is released.
    if (!HelpPressed && HelpScreen.HelpPressed)
    {
        SetGameState_StartScreen();
        return;
    }

    HelpScreen.HelpPressed = HelpPressed;
}