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
#include "Chapter.h"
#include "Text.h"
#include "Settings.h"
#include "LeaderboardScreen.h"
#import "Music.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


enum 
{
	STARTSCREEN_ITEM_HELP,
	STARTSCREEN_ITEM_CREDITS,
    STARTSCREEN_ITEM_NON_CHAPTER_COUNT,
    
	STARTSCREEN_ITEM_FIRST_CHAPTER = STARTSCREEN_ITEM_NON_CHAPTER_COUNT,
};


struct SStartScreen
{
    float BackgroundFadeAlpha;
    
    bool ReleasedAtLeastOnce;
	int Pressed;
    
    float PressedTime;
    float StartupTime;
    
    bool WelcomeDisplayed;
    
    float WiggleTime;
    
    bool TitleVisible;
    bool LeaderboardVisible;
    
    int PlayChapter;
};

SStartScreen StartScreen;

const float StartScreenSpacing = 470.0f;
const float StartScreenDragSpacing = 300.0f;

bool InitialStartScreen = true;

void InitStartScreen()
{
    StartScreen.TitleVisible = InitialStartScreen;
    InitialStartScreen = false;
 
    StartScreen.PlayChapter = Random(1, NChapters);

    StartScreen.ReleasedAtLeastOnce = false;
    StartScreen.BackgroundFadeAlpha = 0.0f;
    StartScreen.Pressed = 0;
    StartScreen.PressedTime = 0.0f;
    
    ResetLightState();
    
    StopMusicTrack(MUSIC_CHAPTER);
    QueueMusicTrack(MUSIC_CHAPTER, &TitleScreenMusic);
}

static void StartScreen_Advance()
{
    if (StartScreen.TitleVisible)
    {
        StartScreen.TitleVisible = false;
    }
    else
    {
        if (StartScreen.Pressed == 1 || StartScreen.Pressed == 2)
        {
#ifdef PLATFORM_IPHONE
            if (Settings.LiveAssets)
                UpdateLiveAssetCache();
#endif

            ResetPortfolio();            
            if (StartScreen.Pressed == 2)
                SetupTutorialPortfolio();
            else
            {
                CurrentChapter = StartScreen.PlayChapter;
                LoadPortfolio();
            }

#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Entered chapter %s", Chapters[CurrentChapter].Name]];
#endif

            extern int PlaybackID;
            if (PlaybackID >= 0)
                SetGameState_Transition(GAMETRANSITION_PLAY_RECORDING);
            else
                SetGameState_Transition(GAMETRANSITION_FIRST_PAGE);
        }
        else if (StartScreen.Pressed == 3)
        {
            SetGameState_StoreScreen();
        }
    }

    StartScreen.ReleasedAtLeastOnce = false;
    StartScreen.Pressed = 0;
    StartScreen.PressedTime = 0.0f;
}

void DisplayStartScreen()
{
    if (StartScreen.StartupTime < 1.0f)
    {
        float Alpha = Max(0, 1.0f - StartScreen.StartupTime);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2, Alpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2-ScreenStart1Sprite.height, Alpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2+ScreenStart1Sprite.height, Alpha);
    }

    if (StartScreen.TitleVisible)
    {
        if (StartScreen.Pressed)
            AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenStartPressedSprite, 0, 0, 768, LitScreenHeight, 1.0f);
        else
            AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenStartSprite, 0, 0, 768, LitScreenHeight, 1.0f);
        return;
    }

    if (Chapters[StartScreen.PlayChapter].HasBackground)
        AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &Chapters[StartScreen.PlayChapter].BackgroundSprite, 0, 0, 768, LitScreenHeight, 1.0f);

    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &ScreenStartButtonsSprite, 180, 190, 0, 10*64, 512, 14*64, StartScreen.Pressed == 1 ? 0.5f : 1.0f);
    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &ScreenStartButtonsSprite, 100, 440, 0,  3*64, 512,  7*64, StartScreen.Pressed == 2 ? 0.5f : 1.0f);
    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &ScreenStartButtonsSprite, 190, 670, 0,  7*64, 512, 10*64, StartScreen.Pressed == 3 ? 0.5f : 1.0f);
    
    // Lives
    AddLitSprite(LIGHTLIST_VACUUM, &ScreenCoinBackgroundSprite, 0, LitScreenHeight - 120);

    AddLitSpriteScaled(LIGHTLIST_VACUUM, &CoinIconSprite, 240, 910, 0.65f*1.0f, 0.65f*1.0f);
    
    char Work[20];
    snprintf(Work, sizeof(Work), "x%d", Settings.Lives);
    DisplayString(LIGHTLIST_VACUUM, Work, 0, 350, 940, 1.1f);

    // Leaderboard button.
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonLeaderboardSprite, 768-90 + sinf(StartScreen.WiggleTime)*4.0f, 90, 0.65f*1.0f, 1.0f);

    if (StartScreen.LeaderboardVisible)
    {
        DisplayLeaderboardScreen();
    }
}

static void DisplayWelcomeAlert()
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
    StartScreen.BackgroundFadeAlpha *= 0.9f;
    StartScreen.WiggleTime += 1.0f/60.0f;
    StartScreen.StartupTime += 1.0f/60.0f;

    if (!msButton1)
        StartScreen.ReleasedAtLeastOnce = true;
    
    if (!StartScreen.ReleasedAtLeastOnce)
        return;
    
    if (StartScreen.LeaderboardVisible)
    {
        if (msY < 200 && msX < 200 && msButton1 && !msOldButton1)
        {
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:@"Closed Leaderboards"];
#endif
            StartScreen.LeaderboardVisible = false;            
        }
        
        if (msY > 700 && msX > 300 && msX < 500 && msButton1 && !msOldButton1)
        {
            Settings.GhostActive = !Settings.GhostActive;
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Toggled Ghost mode to %s", Settings.GhostActive ? "on" : "off"]];
#endif
            SaveSettings();
        }
        
        if (msY > 700 && msX > 500 && msButton1 && !msOldButton1)
        {
            SwitchToNextLeaderboard();
            return;
        }
        
        if (msY > 700 && msX < 300 && msButton1 && !msOldButton1)
        {
            SwitchToPreviousLeaderboard();
            return;
        }
        
        UpdateLeaderboardScreen();
        return;
    }
    
    if (msY < 190 && msX > 384 && msButton1 && !msOldButton1)
    {
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Opened Leaderboards for %s", Chapters[CurrentChapter].Name]];
#endif
        StartScreen.LeaderboardVisible = true;
        
        InitLeaderboardScreen();
        return;
    }
    
    if (StartScreen.Pressed)
    {
        StartScreen.PressedTime += 1.0f/60.0f;
        
        if (!msButton1)
            StartScreen_Advance();
    }
    else
    {
        if (StartScreen.TitleVisible)
        {
            if (msY >= 256 && msButton1)
            {
                StartScreen.Pressed = true;
                StartScreen.PressedTime = 0.0f;
            }
        }
        else
        {
            if (msY >= 190 && msY < 440 && msButton1)
            {
                StartScreen.Pressed = 1;
                StartScreen.PressedTime = 0.0f;
            }
            else if (msY >= 440 && msY < 670 && msButton1)
            {
                StartScreen.Pressed = 2;
                StartScreen.PressedTime = 0.0f;
            }
            else if (msY >= 670 && msY < 670 + 3*64 && msButton1)
            {
                StartScreen.Pressed = 3;
                StartScreen.PressedTime = 0.0f;
            }
        }
    }
    
    if (StartScreen.StartupTime >= 1.0f && !StartScreen.WelcomeDisplayed)
    {
        StartScreen.WelcomeDisplayed = true;
        //DisplayWelcomeAlert();
    }
}
