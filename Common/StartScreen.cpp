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
#include "Dusty.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


enum 
{
	STARTSCREEN_ITEM_HELP,
	STARTSCREEN_ITEM_START,
	STARTSCREEN_ITEM_CREDITS,
	STARTSCREEN_ITEM_COUNT
};

gxSprite* StartScreenIcons[STARTSCREEN_ITEM_COUNT] =
{
	&IconHelp1Sprite,
	&IconStart1Sprite,
	&IconCredits1Sprite,
};

gxSprite* StartScreenPressedIcons[STARTSCREEN_ITEM_COUNT] =
{
	&IconHelp2Sprite,
	&IconStart2Sprite,
	&IconCredits2Sprite
};


struct SStartScreen
{
	int CurItem;
	
	float X;
	float PrevX;
	float DragX;

	bool Dragging;
	bool Pressed;
    
    bool ReleasedAtLeastOnce;
    
    float PressedTime;
    float StartupTime;
    
    bool WelcomeDisplayed;
};

SStartScreen StartScreen;


void InitStartScreen()
{
	StartScreen.CurItem = STARTSCREEN_ITEM_START;
	StartScreen.X = StartScreen.CurItem * 600.0f;
	StartScreen.PrevX = StartScreen.X;
    StartScreen.PressedTime = 0.0f;
    StartScreen.ReleasedAtLeastOnce = false;
}

void StartScreen_Advance()
{
    StartScreen.Dragging = false;
    StartScreen.Pressed = false;
    StartScreen.PressedTime = 0.0f;
    StartScreen.ReleasedAtLeastOnce = false;
    
	if (StartScreen.CurItem == STARTSCREEN_ITEM_START)
	{
		Dusty.Lives = 3;

		SetGameState_Transition(GAMETRANSITION_FIRST_PAGE);
	}
	else if (StartScreen.CurItem == STARTSCREEN_ITEM_HELP)
	{
		SetGameState_Help();
	}
	else if (StartScreen.CurItem == STARTSCREEN_ITEM_CREDITS)
	{
#ifdef PLATFORM_IPHONE
        ShowSettings();
#endif
//		SetGameState_Credits();
    }
}

void DisplayStartScreen()
{
	AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 0);

    float Scale = Lerp(StartScreen.StartupTime, 0.0f, 0.8f, 2.0f, 1.0f);
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &LogoSprite, 384, 220, Scale, 1.0f);

	for (int i = 0; i < STARTSCREEN_ITEM_COUNT; i++)
	{
        float X = 384 + i*600 - (int)StartScreen.X;
        float Y = 730;

        if (i == StartScreen.CurItem)
        {
            float OverlayAlpha = SinWave(StartScreen.PressedTime, 1.0f);
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, StartScreenIcons[i], X, Y, 1.0, 1.0f);
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, StartScreenPressedIcons[i], X, Y, 1.0, OverlayAlpha);
        }
        else
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, StartScreenIcons[i], X, Y, 1.0, 1.0f);
        }
	}

    if (StartScreen.StartupTime < 1.0f)
    {
        float Alpha = Max(0, 1.0f - StartScreen.StartupTime);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, 0, Alpha);
    }
}

void DisplayWelcomeAlert()
{
#ifdef PLATFORM_IPHONE
    theViewController.paused = TRUE;
    
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
    
    NSString *title = [NSString stringWithFormat:@"Welcome to SuperDustBunny Beta Build %@", version];
    
    NSString *message = 
    @"We're thrilled that you're helping us to test and improve our first iPhone game.\n\n"
    "The goal of this beta is to see how the game runs on different models of iPhone and iTouch.  Please let us know how well it works.\n\n"
    "We would also like feedback about how the game looks, and how easy it is to control the bunny.\n\n"
    "Thanks for your efforts!";
    
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:message
                                                   delegate:theViewController
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:@" Send Feedback ", nil];
    [alert show];
    [alert release];
#endif
}

void UpdateStartScreen()
{
#ifdef PLATFORM_WINDOWS
	// Keyboard shortcuts for Windows
	StartScreen.Pressed = kbIsKeyDown(KB_RETURN);

	if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
	{
		StartScreen_Advance();
		return;
	}

	if (kbIsKeyDown(KB_A))
	{
		StartScreen.X -= 100;
	}
	if (kbIsKeyDown(KB_D))
	{
		StartScreen.X += 100;
	}
#endif

    if (!msButton1)
        StartScreen.ReleasedAtLeastOnce = true;
    if (!StartScreen.ReleasedAtLeastOnce)
        return;
    
	if (StartScreen.Dragging)
	{
		if (!msButton1)
		{
			if (StartScreen.Pressed)
			{
				StartScreen_Advance();
			}

			StartScreen.Dragging = false;
			StartScreen.Pressed = false;

            StartScreen.CurItem = (int)Round(StartScreen.X / 600.0f);
        }
		else 
		{
			if (abs(msX - StartScreen.DragX) > 10)
				StartScreen.Pressed = false;

			StartScreen.X += StartScreen.DragX - msX;
			StartScreen.DragX = (float)msX;
		}        
	}
	else
	{
		if (msButton1)
		{
			StartScreen.Pressed = true;
			StartScreen.Dragging = true;
			StartScreen.DragX = (float)msX;
            StartScreen.PressedTime = 0.0f;
		}
		else 
		{
			StartScreen.X = StartScreen.X * 0.8f + StartScreen.CurItem * 600.0f * 0.2f;
			StartScreen.X = StartScreen.X + (StartScreen.X-StartScreen.PrevX) * 0.25f;
		}
	}

    if (StartScreen.X < 0)
		StartScreen.X = 0;
	if (StartScreen.X >= (STARTSCREEN_ITEM_COUNT-1)*600)
		StartScreen.X = (STARTSCREEN_ITEM_COUNT-1)*600;

	StartScreen.PrevX = StartScreen.X;
    
    if (StartScreen.Pressed)
    {
        if (StartScreen.PressedTime < 0.5f)
            StartScreen.PressedTime += 1.0f/60.0f;        
    }
    else
    {
        if (StartScreen.PressedTime > 0.0f)
            StartScreen.PressedTime -= 1.0f/60.0f;
    }
    
    StartScreen.StartupTime += 1.0f/60.0f;
    
    if (StartScreen.StartupTime >= 1.0f && !StartScreen.WelcomeDisplayed)
    {
        StartScreen.WelcomeDisplayed = true;
        DisplayWelcomeAlert();
    }
}