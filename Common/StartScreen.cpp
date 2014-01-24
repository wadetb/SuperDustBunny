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
#include "Recorder.h"
#include "LeaderboardScreen.h"
#include "Music.h"

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


#define PLAY_Y      (StartScreen.ButtonYOffset + (LitScreenHeight/5.0)*0.85)
#define TRAIN_Y     (StartScreen.ButtonYOffset + (LitScreenHeight/5.0)*2.1)
#define STORE_Y     (StartScreen.ButtonYOffset + (LitScreenHeight/5.0)*3.35)

#define BUTTON_HEIGHT (LitScreenHeight/5.0)


float MEDAL_SCALE = 0.7f;



struct SStartScreen
{
    float BackgroundFadeAlpha;
    
    bool ReleasedAtLeastOnce;
	int Pressed;
    
    float PressedTime;
    float StartupTime;
    
    float ButtonYOffset;
    
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
 
    StartScreen.PlayChapter = Random(FIRST_STARTING_CHAPTER, LAST_STARTING_CHAPTER+1);

    StartScreen.ReleasedAtLeastOnce = false;
    StartScreen.BackgroundFadeAlpha = 0.0f;
    StartScreen.Pressed = 0;
    StartScreen.PressedTime = 0.0f;
    
    StartScreen.WiggleTime = 0;
    
//    if (ScreenAspect == ASPECT_IPHONE_5)
//        StartScreen.ButtonYOffset = 100;
//    else
        StartScreen.ButtonYOffset = 0;
    
    ResetLightState();
    
    StopMusicTrack(MUSIC_CHAPTER);
    QueueMusicTrack(MUSIC_CHAPTER, &TitleScreenMusic);
}

static void StartScreen_Advance()
{
    if (StartScreen.TitleVisible)
    {
        if (StartScreen.StartupTime < 3)
            StartScreen.StartupTime = 3;
        else
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
            
            StartRecordingSession();

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
	
            SetGameState_Transition(GAMETRANSITION_FIRST_PAGE);
        }
        else if (StartScreen.Pressed == 3)
        {
#ifdef PLATFORM_IPHONE
            if (StartScreen.PressedTime >= 2.0f)
            {
                [TestFlight passCheckpoint:[NSString stringWithFormat:@"Opened Settings"]];
                
                [theViewController showSettings];
            }
            else
#endif
            {
                SetGameState_StoreScreen();
            }
        }
    }

    StartScreen.ReleasedAtLeastOnce = false;
    StartScreen.Pressed = 0;
    StartScreen.PressedTime = 0.0f;
}

void DisplayStartScreen()
{
    if (StartScreen.StartupTime < 3.0f)
    {
        float IconAlpha = 1.0f;
        IconAlpha *= Remap(StartScreen.StartupTime, 2.0f, 3.0f, 1, 0, true);
        
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2, IconAlpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2-ScreenStart1Sprite.height, IconAlpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2+ScreenStart1Sprite.height, IconAlpha);
        
        float TitleAlpha = 1.0f;
        TitleAlpha *= Remap(StartScreen.StartupTime, 0.5f, 1, 0, 1, true);
        TitleAlpha *= Remap(StartScreen.StartupTime, 2.5f, 3, 1, 0, true);
        
        AddLitSpriteSizedColor(LIGHTLIST_WIPE, &WhiteSprite, 0, 0, 768, LitScreenHeight, gxRGBA32(0, 0, 0, (int)(255*TitleAlpha)));
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &LogoSprite, 768/2 - LogoSprite.width/2, LitScreenHeight/2 - LogoSprite.height/2, TitleAlpha);
    }
    
    if (StartScreen.TitleVisible)
    {
        if (ScreenAspect == ASPECT_IPHONE_5)
        {
            if (StartScreen.Pressed)
                AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenStartPressediPhone5Sprite, 0, 0, 768, LitScreenHeight, 1.0f);
            else
                AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenStartiPhone5Sprite, 0, 0, 768, LitScreenHeight, 1.0f);
        }
        else
        {
            if (StartScreen.Pressed)
                AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenStartPressedSprite, 0, 0, 768, LitScreenHeight, 1.0f);
            else
                AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenStartSprite, 0, 0, 768, LitScreenHeight, 1.0f);            
        }
        return;
    }

    if (Chapters[StartScreen.PlayChapter].HasBackground)
        AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &Chapters[StartScreen.PlayChapter].BackgroundSprite, 0, 0, 768, LitScreenHeight, 1.0f);

    // Medals
    float MedalYOffset = -20;//Remap(StartScreen.StartupTime, 10, 10.5f, -50, 0, true);
    if (Settings.PageCount >= BRONZE_PAGE_COUNT)
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &BronzeMedalSprite, 384+120, MedalYOffset+75, MEDAL_SCALE*2.5f, 1.0f);
        if (Settings.PageCount < SILVER_PAGE_COUNT)
        {
            char Work[20];
            snprintf(Work, sizeof(Work), "%d", Settings.PageCount);
            DisplayMultilineStringAlpha(LIGHTLIST_VACUUM, Work, FORMAT_CENTER_X|FORMAT_CENTER_Y, 384+120, MedalYOffset+75+50, MEDAL_SCALE*1.4f, 0.25f);
        }
    }
    if (Settings.PageCount >= SILVER_PAGE_COUNT)
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &SilverMedalSprite, 384-120, MedalYOffset+60, MEDAL_SCALE*2.5f, 1.0f);
        if (Settings.PageCount < GOLD_PAGE_COUNT)
        {
            char Work[20];
            snprintf(Work, sizeof(Work), "%d", Settings.PageCount);
            DisplayMultilineStringAlpha(LIGHTLIST_VACUUM, Work, FORMAT_CENTER_X|FORMAT_CENTER_Y, 384-120, MedalYOffset+60+50, MEDAL_SCALE*1.4f, 0.25f);
        }
    }
    if (Settings.PageCount >= GOLD_PAGE_COUNT)
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &GoldMedalSprite, 384, MedalYOffset+80, MEDAL_SCALE*2.75f, 1.0f);
        char Work[20];
        snprintf(Work, sizeof(Work), "%d", Settings.PageCount);
        DisplayMultilineStringAlpha(LIGHTLIST_VACUUM, Work, FORMAT_CENTER_X|FORMAT_CENTER_Y, 384, MedalYOffset+80+50, MEDAL_SCALE*1.4f, 0.25f);
    }
    
    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &ScreenStartButtonsSprite, 180 + sinf(StartScreen.WiggleTime)*4.0f, PLAY_Y, 0,  10*64, 512, 14*64, StartScreen.Pressed == 1 ? 0.5f : 1.0f);
    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &ScreenStartButtonsSprite, 100 + sinf(StartScreen.WiggleTime+3)*4.0f, TRAIN_Y, 0,  3*64, 512,  7*64, StartScreen.Pressed == 2 ? 0.5f : 1.0f);
    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &ScreenStartButtonsSprite, 190 + sinf(StartScreen.WiggleTime+5)*4.0f, STORE_Y, 0,  7*64, 512, 10*64, StartScreen.Pressed == 3 ? 0.5f : 1.0f);
    
    // Lives
    AddLitSprite(LIGHTLIST_VACUUM, &ScreenCoinBackgroundSprite, 0, LitScreenHeight - 120);
    AddLitSpriteScaled(LIGHTLIST_VACUUM, &CoinIconSprite, 240, LitScreenHeight - 110, 0.65f*1.0f, 0.65f*1.0f);
    char Work[20];
    snprintf(Work, sizeof(Work), "x%d", Settings.Lives);
    DisplayString(LIGHTLIST_VACUUM, Work, 0, 350, LitScreenHeight - 80, 1.1f);

    // Leaderboard button.
    if (StartScreen.LeaderboardVisible)
    {
        DisplayLeaderboardScreen();
    }
    else
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenGoBackSprite, 70, 70, 0.5f, 1.0f);
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonLeaderboardSprite, 768-70, 70, 0.5f, 1.0f);
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
        if (msY < 200 && msX > 768-200 && msButton1 && !msOldButton1)
        {
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:@"Closed Leaderboards"];
#endif
            StartScreen.LeaderboardVisible = false;
            StartScreen.ReleasedAtLeastOnce = false;
            return;
        }
        
/*
        if (msY > 700 && msX > 300 && msX < 500 && msButton1 && !msOldButton1)
        {
            Settings.GhostActive = !Settings.GhostActive;
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Toggled Ghost mode to %s", Settings.GhostActive ? "on" : "off"]];
#endif
            SaveSettings();
        }
*/
        
        if (msY > 800 && msX > 500 && msButton1 && !msOldButton1)
        {
            SwitchToNextLeaderboard();
            return;
        }
        
        if (msY > 800 && msX < 300 && msButton1 && !msOldButton1)
        {
            SwitchToPreviousLeaderboard();
            return;
        }
        
        UpdateLeaderboardScreen();
        return;
    }
    
    if (msX < 100 && msY <= 100 && msButton1 && !msOldButton1)
    {
        StartScreen.TitleVisible = true;
        StartScreen.PlayChapter = Random(FIRST_NORMAL_CHAPTER, LAST_NORMAL_CHAPTER+1);
        return;
    }

    if (msY < PLAY_Y && msX > 384 && msButton1 && !msOldButton1)
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
        if (!msButton1)
            StartScreen.PressedTime = 0;
    }

    if (StartScreen.TitleVisible)
    {
        if (msY >= 256 && msButton1)
        {
            StartScreen.Pressed = true;
        }
    }
    else
    {
        if (msY >= PLAY_Y && msY < PLAY_Y+BUTTON_HEIGHT && msButton1)
        {
            StartScreen.Pressed = 1;
        }
        else if (msY >= TRAIN_Y && msY < TRAIN_Y+BUTTON_HEIGHT && msButton1)
        {
            StartScreen.Pressed = 2;
        }
        else if (msY >= STORE_Y && msY < STORE_Y+BUTTON_HEIGHT && msButton1)
        {
            StartScreen.Pressed = 3;
        }
        else
        {
            StartScreen.Pressed = false;
        }
    }
    
    if (StartScreen.StartupTime >= 1.0f && !StartScreen.WelcomeDisplayed)
    {
        StartScreen.WelcomeDisplayed = true;
        //DisplayWelcomeAlert();
    }
}
