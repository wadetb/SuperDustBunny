//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "DieScreen.h"
#include "Chapter.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


struct SDieScreen
{
	bool Pressed;
	int Timer;
};

SDieScreen DieScreen;

void InitDieScreen()
{
	DieScreen.Pressed = false;
	DieScreen.Timer = 0;

	Chapter.PageProps.LightsOff = false;
}

void DisplayDieScreen()
{
//	AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundCardboardSprite, 0, 0);
//	AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundCardboardSprite, 0, 1024);

    AddLitSprite(LIGHTLIST_BACKGROUND, DieScreen.Pressed ? &ScreenLoseGrave2Sprite : &ScreenLoseGrave1Sprite, 0, 0);

	float t = DieScreen.Timer / 10.0f;
	float dx = cos(t/5)*10 + cos(1+t/7)*10 + cos(1-t/9)*10 + 100;
	float dy = sin(t/5)*10 + sin(1+t/7)*10 + sin(1-t/9)*10 - Min(t*2, 125.0f);

	float Alpha = 1.0f; //Min(t, 1.0f);

	AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &ScreenLoseGhostSprite, 768/2 + dx, 150+ScreenLoseGhostSprite.height/2+dy, 1.0f, Alpha*Alpha*0.5f);
}

static void DieScreen_Advance()
{
	SetGameState_StartScreen();
}

void UpdateDieScreen()
{
#ifdef PLATFORM_WINDOWS
	bool Pressed = kbIsKeyDown(KB_RETURN) || msButton1;
#endif
#ifdef PLATFORM_IPHONE_OR_MAC
	bool Pressed = msButton1;
#endif

	DieScreen.Timer++;
    
    if (DieScreen.Timer == 120)
    {
#ifdef PLATFORM_IPHONE
        theViewController.paused = TRUE;
        
        NSString *title = @"Thanks for playing Super Dust Bunny!";
        
        NSString *message = 
        @"Looks like you ran out of lives, sorry about that.  To get more lives, pick up any gold coins you see.\n\n"
        "If part of the game seems too hard, please send us feedback about it!\n\n"
        "Also, if you just want to see everything there is to see, check out the cheats available in the Settings page. "
        "To get to the settings page, swipe to the right once at the main menu.\n\n"
        "Thanks for testing our game!";
        
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:theViewController
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:@" Send Feedback ", nil];
        [alert show];
        [alert release];
#endif
    }
    
    if (DieScreen.Timer >= 130)
    {
        if (!Pressed && DieScreen.Pressed)
        {
            DieScreen_Advance();
            return;
        }
        
        DieScreen.Pressed = Pressed;
    }
}
