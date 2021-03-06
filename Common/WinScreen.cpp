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
#include "Text.h"
#include "Music.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif

#if 0

#define MEDAL_FRAMES 8


enum EWinScreenState
{
    WINSCREEN_MEDAL,
    WINSCREEN_TIMES
};

struct SWinScreen
{
    EWinScreenState State;
    
	bool Pressed;
    float Timer;
    float SlideIn;
    
    int MedalFrame;
};


SWinScreen WinScreen;


static gxSprite MedalFrames[MEDAL_FRAMES];

static const char* BronzeMedalNames[MEDAL_FRAMES] =
{
    "Assets/METAL_BRONZE_1.png",
    "Assets/METAL_BRONZE_2.png",
    "Assets/METAL_BRONZE_3.png",
    "Assets/METAL_BRONZE_4.png",
    "Assets/METAL_BRONZE_5.png",
    "Assets/METAL_BRONZE_6.png",
    "Assets/METAL_BRONZE_7.png",
    "Assets/METAL_BRONZE_8.png",
};

static const char* SilverMedalNames[MEDAL_FRAMES] =
{
    "Assets/METAL_SILVER_1.png",
    "Assets/METAL_SILVER_2.png",
    "Assets/METAL_SILVER_3.png",
    "Assets/METAL_SILVER_4.png",
    "Assets/METAL_SILVER_5.png",
    "Assets/METAL_SILVER_6.png",
    "Assets/METAL_SILVER_7.png",
    "Assets/METAL_SILVER_8.png",
};

static const char* GoldMedalNames[MEDAL_FRAMES] =
{
    "Assets/METAL_GOLD_1.png",
    "Assets/METAL_GOLD_2.png",
    "Assets/METAL_GOLD_3.png",
    "Assets/METAL_GOLD_4.png",
    "Assets/METAL_GOLD_5.png",
    "Assets/METAL_GOLD_6.png",
    "Assets/METAL_GOLD_7.png",
    "Assets/METAL_GOLD_8.png",
};


void InitWinScreen()
{
#ifdef PLATFORM_IPHONE
    [TestFlight passCheckpoint:[NSString stringWithFormat:@"Beat the chapter"]];
#endif

    if (Score.Medal != MEDAL_NONE)
    {
        WinScreen.State = WINSCREEN_MEDAL;
        
        const char** MedalNames;
        if (Score.Medal == MEDAL_BRONZE)
        {
            MedalNames = BronzeMedalNames;
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Bronze medal"]];
#endif
        }
        else if (Score.Medal == MEDAL_SILVER)
        {
            MedalNames = SilverMedalNames;
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Silver medal"]];
#endif
        }
        else 
        {
            MedalNames = GoldMedalNames;
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Gold medal"]];
#endif
        }
        
        for (int i = 0; i < MEDAL_FRAMES; i++)
            LoadSpriteAsset(MedalNames[i], &MedalFrames[i]);
    }
    else
        WinScreen.State = WINSCREEN_TIMES;
    
	WinScreen.Pressed = false;
    WinScreen.Timer = 0;
    WinScreen.SlideIn = 1.0f;
    
    ResetLightState();
    
    StopMusicTrack(MUSIC_CHAPTER);
    QueueMusicTrack(MUSIC_CHAPTER, &WinMusic);
}

void DisplayWinScreen()
{
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 0);
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 1024);
    
    if (WinScreen.State == WINSCREEN_MEDAL)
    {
        if (Score.Medal != MEDAL_NONE)
        {
            //float MedalScale = 1.5f + SinWave(WinScreen.Timer, 0.75f) * 0.25f;

            const char* MedalString = "";
            if (Score.Medal == MEDAL_BRONZE) MedalString = "bronze";
            if (Score.Medal == MEDAL_SILVER) MedalString = "silver";
            if (Score.Medal == MEDAL_GOLD) MedalString = "gold";
            
            DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, MedalString, FORMAT_CENTER_X, 384 + 600*WinScreen.SlideIn, 40, 1.5f);
            DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, "medal!", FORMAT_CENTER_X, 384 - 600*WinScreen.SlideIn, 140, 1.5f);
            
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &MedalFrames[(WinScreen.MedalFrame/8)%MEDAL_FRAMES], 384, 665 + 300*WinScreen.SlideIn, 1.0f, 1.0f);                
        }
    }
    
    if (WinScreen.State == WINSCREEN_TIMES)
    {
        if (Score.Medal != MEDAL_NONE)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &MedalFrames[0], 384, 675, 0.8f, 1.0f);        
        
        float CurrentY = 75;
        
        // Chapter complete!
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterCompleteSprite, 384, CurrentY, 1.0f, 1.0f);
        CurrentY += 100;
        
        // Total time
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterTotalTimeSprite, 384 + 2000*WinScreen.SlideIn, CurrentY, 1.0f, 1.0f);
        CurrentY += 75;
        
        DisplayTime(200, CurrentY, 1.0f, Score.ChapterTime);
        CurrentY += 150;
        
        // New record!
        if (Score.NewChapterRecord)
        {
            float NewRecordScale = 1.0f + SinWave(WinScreen.Timer, 0.75f) * 0.30f;
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterNewRecordSprite, 384, CurrentY, NewRecordScale, 1.0f);
            
            CurrentY += 85;
        }

        // List of chapters.
        bool AnyNewRecords = false;
        for (int i = 0; i < Chapter.NPages; i++)
        {
            if (Score.NewRecord[i])
            {
                AnyNewRecords = true;
                break;
            }
        }

        float XOffset = AnyNewRecords ? 0.0f : 90.0f;
        
        AddLitSprite(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterTimeSprite, XOffset + 60, CurrentY);
        AddLitSprite(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterBonusSprite, XOffset + 350, CurrentY);
        CurrentY += 100;
        
        for (int i = 0; i < Chapter.NPages; i++)
        {
            if (Score.PageTime[i] == 0)
                continue;
            
            DisplayTime(XOffset + 80 - (i+1)*300*WinScreen.SlideIn, CurrentY, 0.75f, Score.PageTime[i]);
            DisplayTimeDigit(Score.PageBonus[i], XOffset + 450 - (i+1)*300*WinScreen.SlideIn, CurrentY, 0.75f, 0, 0);
            
            if (Score.NewRecord[i])
                AddLitSprite(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterNewSprite, XOffset + 550 + i*WinScreen.SlideIn*300, CurrentY);
            
            CurrentY += 75;
        }
    }
    
    // tap to continue
    float TapScape = 1.0f + SinWave(WinScreen.Timer, 0.5f) * 0.05f;
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &TapToContinueSprite, 384, LitScreenHeight - 75 + 300*WinScreen.SlideIn, TapScape, 1.0f);
}

static void WinScreen_Advance()
{
    for (int i = 0; i < MEDAL_FRAMES; i++)
        gxDestroySprite(&MedalFrames[i]);
    
    if (Chapters[CurrentChapter].EndOfGame)
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
        
        CurrentChapter = 0;
    }
    else
    {
        if (CurrentChapter < NChapters)
            CurrentChapter++;
    }
    
	SetGameState_StartScreen();
}

void UpdateWinScreen()
{
	bool Pressed = msButton1;

    WinScreen.Timer += 1.0f/60.0f;
    
    WinScreen.SlideIn *= 0.9f;

    WinScreen.MedalFrame++;
    
	// Advance to playing state when button is released.
	if (WinScreen.Timer > 0.1f && WinScreen.SlideIn < 0.01f)
    { 
        if (!Pressed && WinScreen.Pressed)
        {
            if (WinScreen.State == WINSCREEN_MEDAL)
            {
                UploadChapterScore();                

                WinScreen.State = WINSCREEN_TIMES;
                WinScreen.Timer = 0;
                WinScreen.SlideIn = 1.0f;
            }
            else
            {
                WinScreen_Advance();
                return;
            }
        }
        
        WinScreen.Pressed = Pressed;
    }
}

#endif

