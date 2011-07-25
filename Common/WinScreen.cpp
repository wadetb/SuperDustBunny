//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "WinScreen.h"
#include "Chapter.h"
#include "GameScore.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


struct SWinScreen
{
	bool Pressed;
    int Timer;
};


SWinScreen WinScreen;


void InitWinScreen()
{
	WinScreen.Pressed = false;
    WinScreen.Timer = 0;
}

void DisplayWinScreen()
{    
/*
	if (WinScreen.Pressed)
	{
		AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenWin2Sprite, 0, 0);
        AddLitSpriteSizedColor(LIGHTLIST_BACKGROUND, &WhiteSprite, 0, ScreenWin2Sprite.height, gxScreenWidth, 1000.0f, gxRGB32(0, 0, 0));
	}
	else
	{
		AddLitSprite(LIGHTLIST_BACKGROUND, &ScreenWin1Sprite, 0, 0);
        AddLitSpriteSized(LIGHTLIST_BACKGROUND, &WhiteSprite, 0, ScreenWin2Sprite.height, gxScreenWidth, 1000.0f);
	}
    
	AddLitSprite(LIGHTLIST_VACUUM, &ChapterTitleSprite, 0, -400);
	AddLitSprite(LIGHTLIST_VACUUM, &ChapterTextSprite, 220, 200);
*/
    
    int CurrentY = 300;
    
    for (int i = 0; i < Chapter.NPages; i++)
    {
        DisplayTime(100, CurrentY, 0.75f, Score.PageTime[i]);
        DisplayTimeDigit(Score.PageBonus[i], 490, CurrentY, 0.75f, 0, 0);
        DisplayTimeDigit(Score.NewRecord[i], 620, CurrentY, 0.75f, 0, 0);
        CurrentY += 75;
    }
    
    CurrentY += 200;
    
    DisplayTime(200, CurrentY, 1.0f, Score.ChapterTime);
    CurrentY += 100;
    DisplayTimeDigit(Score.NewChapterRecord, 370, CurrentY, 1.0f, 0, 0);
}

void WinScreen_Advance()
{    
	SetGameState_StartScreen();
}

void UpdateWinScreen()
{
#ifdef PLATFORM_WINDOWS
	bool Pressed = kbIsKeyDown(KB_RETURN) || msButton1;
#endif
#ifdef PLATFORM_IPHONE
	bool Pressed = msButton1;
#endif

    WinScreen.Timer++;

    if (WinScreen.Timer == 120)
    {
#ifdef PLATFORM_IPHONE
        theViewController.paused = TRUE;
        
        NSString *title = @"Thanks for playing Super Dust Bunny!";
        
        NSString *message = 
        @"Congratulations, you beat the game!  (Or at least, the tiny part of the game that's included in this beta build..) "
        "Please let us know how you felt about it by leaving us feedback.\n\n"
        "We'll be sure to mix up the content in each round of beta testing, so you'll have some new challenges before long. "
        "In the meantime, why not play again and try to win as quickly as possible?  We'd love to hear about your best times.\n\n"
        "Thanks again for testing our game!";
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:theViewController
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:@" Send Feedback ", nil];
        [alert show];
        [alert release];
#endif
    }
    
	// Advance to playing state when button is released.
	if (WinScreen.Timer > 130)
    { 
        if (!Pressed && WinScreen.Pressed)
        {
            WinScreen_Advance();
            return;
        }
        
        WinScreen.Pressed = Pressed;
    }
}

