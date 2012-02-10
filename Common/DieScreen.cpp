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
    int TearTime;
    float TearDropX;
    float TearDropY;
    float TearDropVY;
};

SDieScreen DieScreen;

void InitDieScreen()
{
	DieScreen.Pressed = false;
	DieScreen.Timer = 0;
    DieScreen.TearTime = 0;

	Chapter.PageProps.LightsOff = false;
}

gxSprite* TearSprites[4] =
{
    &TearEye1Sprite,
    &TearEye2Sprite,
    &TearEye3Sprite,
    &TearEye4Sprite,
};

void DisplayDieScreen()
{
    AddLitSprite(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenLoseGrave1Sprite, 0, 0);
    
    float PulseAlpha;
    if (DieScreen.Pressed)
        PulseAlpha = 1.0f;
    else
        PulseAlpha = SinWave(DieScreen.Timer/60.0f, 2.0f)*0.5f;
    
    AddLitSpriteAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenLoseGrave2Sprite, 0, 0, PulseAlpha);

	float t = DieScreen.Timer / 10.0f;
	float dx = cos(t/5)*10 + cos(1+t/7)*10 + cos(1-t/9)*10 + 100;
	float dy = sin(t/5)*10 + sin(1+t/7)*10 + sin(1-t/9)*10 - Min(t*2, 125.0f);

	float Alpha = 0.5f; //Min(t, 1.0f);

    float GhostX = 768/2 + dx;
    float GhostY = 150+ScreenLoseGhostSprite.height/2+dy;
	AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ScreenLoseGhostSprite, GhostX, GhostY, 1.0f, Alpha);
    
    if (DieScreen.TearTime < 40)
    {
        int TearFrame = (int)( DieScreen.TearTime / 10 );
        AddLitSpriteAlpha(LIGHTLIST_VACUUM, TearSprites[TearFrame], GhostX-32, GhostY-12, Alpha);
    }
    if (DieScreen.TearTime >= 35)
    {
        AddLitSpriteAlpha(LIGHTLIST_VACUUM, &TearDropSprite, DieScreen.TearDropX, DieScreen.TearDropY, Alpha);
    }
}

static void DieScreen_Advance()
{
	SetGameState_StartScreen();
}

void UpdateDieScreen()
{
	bool Pressed = msButton1;

	DieScreen.Timer++;

    DieScreen.TearTime++;
    if (DieScreen.TearTime == 35)
    {
        float t = DieScreen.Timer / 10.0f;
        float dx = cos(t/5)*10 + cos(1+t/7)*10 + cos(1-t/9)*10 + 100;
        float dy = sin(t/5)*10 + sin(1+t/7)*10 + sin(1-t/9)*10 - Min(t*2, 125.0f);
        float GhostX = 768/2 + dx;
        float GhostY = 150+ScreenLoseGhostSprite.height/2+dy;
        DieScreen.TearDropX = GhostX;
        DieScreen.TearDropY = GhostY;
        DieScreen.TearDropVY = 0;
    }
    if (DieScreen.TearTime > 35)
    {
        DieScreen.TearDropY += DieScreen.TearDropVY;
        DieScreen.TearDropVY += 1.0f;
    }
    if (DieScreen.TearTime > 100)
        DieScreen.TearTime = 0;
    
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
