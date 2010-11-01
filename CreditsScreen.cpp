//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "StartScreen.h"

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
        gxDrawSprite( 0, 0, &ScreenCredits2Sprite );
    }
    else
    {
        gxDrawSprite( 0, 0, &ScreenCredits1Sprite );
    }   
}

void UpdateCreditsScreen()
{   
#ifdef PLATFORM_WINDOWS
    bool CreditsPressed = kbIsKeyDown(KB_RETURN) || msButton1;
#endif
#ifdef PLATFORM_IPHONE
    bool CreditsPressed = msButton1;
#endif

    // Advance to playing state when button is released.
    if (!CreditsPressed && CreditsScreen.CreditsPressed)
    {
        SetGameState_StartScreen();;
        return;
    }

    CreditsScreen.CreditsPressed = CreditsPressed;
}