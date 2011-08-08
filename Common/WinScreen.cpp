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


gxSprite MedalFrames[MEDAL_FRAMES];

const char* BronzeMedalNames[MEDAL_FRAMES] =
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

const char* SilverMedalNames[MEDAL_FRAMES] =
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

const char* GoldMedalNames[MEDAL_FRAMES] =
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


void DisplayLetter(ELightList List, char Letter, float X, float Y, float Scale)
{
    switch (Letter)
    {
        case 'a': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  0*48,   0,  1*48, 60, Scale, Scale); break;
        case 'b': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  1*48,   0,  2*48, 60, Scale, Scale); break;
        case 'c': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  2*48,   0,  3*48, 60, Scale, Scale); break;
        case 'd': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  3*48,   0,  4*48, 60, Scale, Scale); break;
        case 'e': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  4*48,   0,  5*48, 60, Scale, Scale); break;
        case 'f': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  5*48,   0,  6*48, 60, Scale, Scale); break;
        case 'g': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  6*48,   0,  7*48, 60, Scale, Scale); break;
        case 'h': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  7*48,   0,  8*48, 60, Scale, Scale); break;
        case 'i': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  8*48,   0,  9*48, 60, Scale, Scale); break;
        case 'j': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  9*48,   0, 10*48, 60, Scale, Scale); break;
        case 'k': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y, 10*48,   0, 11*48, 60, Scale, Scale); break;
        case 'l': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y, 11*48,   0, 12*48, 60, Scale, Scale); break;
        case 'm': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y, 12*48,   0, 13*48, 60, Scale, Scale); break;
        case 'n': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  0*48,  64,  1*48, 124, Scale, Scale); break;
        case 'o': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  1*48,  64,  2*48, 124, Scale, Scale); break;
        case 'p': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  2*48,  64,  3*48, 124, Scale, Scale); break;
        case 'q': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  3*48,  64,  4*48, 124, Scale, Scale); break;
        case 'r': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  4*48,  64,  5*48, 124, Scale, Scale); break;
        case 's': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  5*48,  64,  6*48, 124, Scale, Scale); break;
        case 't': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  6*48,  64,  7*48, 124, Scale, Scale); break;
        case 'u': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  7*48,  64,  8*48, 124, Scale, Scale); break;
        case 'v': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  8*48,  64,  9*48, 124, Scale, Scale); break;
        case 'w': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  9*48,  64, 10*48, 124, Scale, Scale); break;
        case 'x': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y, 10*48,  64, 11*48, 124, Scale, Scale); break;
        case 'y': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y, 11*48,  64, 12*48, 124, Scale, Scale); break;
        case 'z': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y, 12*48,  64, 13*48, 124, Scale, Scale); break;
        case '1': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  0*48, 128,  1*48, 188, Scale, Scale); break;
        case '2': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  1*48, 128,  2*48, 188, Scale, Scale); break;
        case '3': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  2*48, 128,  3*48, 188, Scale, Scale); break;
        case '4': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  3*48, 128,  4*48, 188, Scale, Scale); break;
        case '5': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  4*48, 128,  5*48, 188, Scale, Scale); break;
        case '6': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  5*48, 128,  6*48, 188, Scale, Scale); break;
        case '7': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  6*48, 128,  7*48, 188, Scale, Scale); break;
        case '8': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  7*48, 128,  8*48, 188, Scale, Scale); break;
        case '9': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  8*48, 128,  9*48, 188, Scale, Scale); break;
        case '0': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y,  9*48, 128, 10*48, 188, Scale, Scale); break;
        case '!': AddLitSubSpriteScaled(List, &AlphabetSprite, X, Y, 10*48, 128, 11*48, 188, Scale, Scale); break;
        default: break;
    }
}

int GetLetterWidth(char Letter)
{
    switch (Letter)
    {
        case 'a': return 44;
        case 'b': return 44;
        case 'c': return 44;
        case 'd': return 46;
        case 'e': return 44;
        case 'f': return 44;
        case 'g': return 46;
        case 'h': return 44;
        case 'i': return 44;
        case 'j': return 46;
        case 'k': return 44;
        case 'l': return 40;
        case 'm': return 44;
        case 'n': return 44;
        case 'o': return 44;
        case 'p': return 44;
        case 'q': return 44;
        case 'r': return 44;
        case 's': return 44;
        case 't': return 46;
        case 'u': return 44;
        case 'v': return 44;
        case 'w': return 46;
        case 'x': return 44;
        case 'y': return 40;
        case 'z': return 46;
        case '1': return 44;
        case '2': return 44;
        case '3': return 44;
        case '4': return 44;
        case '5': return 44;
        case '6': return 44;
        case '7': return 44;
        case '8': return 44;
        case '9': return 44;
        case '0': return 44;
        case '!': return 30;
        case ' ': return 30;
        default: return 46;
    }
}

float GetStringWidth(const char* String, float Scale)
{
    float CurWidth = 0;
    
    const char* P = String;
    while (*P)
    {
        char C = *P;
        CurWidth += GetLetterWidth(C) * Scale;
        P++;
    }
    
    return CurWidth;
}

enum EStringFormatFlag
{
    FORMAT_CENTER_X = 1,
};

void DisplayString(ELightList List, const char* String, float X, float Y, float Scale, unsigned int FormatFlags)
{
    float CurX = X;
    float CurY = Y;
    
    if (FormatFlags & FORMAT_CENTER_X)
        CurX -= GetStringWidth(String, Scale)/2;
    
    const char* P = String;
    while (*P)
    {
        char C = *P;
        DisplayLetter(List, C, CurX, CurY, Scale);
        CurX += GetLetterWidth(C) * Scale;
        P++;
    }
}

void InitWinScreen()
{
    if (Score.Medal != MEDAL_NONE)
    {
        WinScreen.State = WINSCREEN_MEDAL;
        
        const char** MedalNames;
        if (Score.Medal == MEDAL_BRONZE)
            MedalNames = BronzeMedalNames;
        else if (Score.Medal == MEDAL_SILVER)
            MedalNames = SilverMedalNames;
        else 
            MedalNames = GoldMedalNames;
        
        for (int i = 0; i < MEDAL_FRAMES; i++)
            LoadSpriteAsset(MedalNames[i], &MedalFrames[i]);
    }
    else
        WinScreen.State = WINSCREEN_TIMES;
    
	WinScreen.Pressed = false;
    WinScreen.Timer = 0;
    WinScreen.SlideIn = 1.0f;
}

void DisplayWinScreen()
{    
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 0);
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 1024);
    
    if (WinScreen.State == WINSCREEN_MEDAL)
    {
        if (Score.Medal != MEDAL_NONE)
        {
            float MedalScale = 1.5f + SinWave(WinScreen.Timer, 0.75f) * 0.25f;

            const char* MedalString;
            if (Score.Medal == MEDAL_BRONZE) MedalString = "bronze";
            if (Score.Medal == MEDAL_SILVER) MedalString = "silver";
            if (Score.Medal == MEDAL_GOLD) MedalString = "gold";
            
            DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, MedalString, 384 + 600*WinScreen.SlideIn, 75, MedalScale, FORMAT_CENTER_X);
            DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, "medal!", 384 - 600*WinScreen.SlideIn, 175, MedalScale, FORMAT_CENTER_X);
            
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &MedalFrames[(WinScreen.MedalFrame/8)%MEDAL_FRAMES], 384, 665 + 300*WinScreen.SlideIn, 0.8f, 1.0f);                
        }
    }
    
    if (WinScreen.State == WINSCREEN_TIMES)
    {
        if (Score.Medal != MEDAL_NONE)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &MedalFrames[0], 384, 675, 0.8f, 1.0f);        
        
        int CurrentY = 75;
        
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterCompleteSprite, 384, CurrentY, 1.0f, 1.0f);
        CurrentY += 100;
        
        bool AnyNewRecords = false;
        for (int i = 0; i < Chapter.NPages; i++)
        {
            if (Score.NewRecord[i])
            {
                AnyNewRecords = true;
                break;
            }
        }

        int XOffset = AnyNewRecords ? 0 : 90;
        
        AddLitSprite(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterTimeSprite, XOffset + 60, CurrentY);
        AddLitSprite(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterBonusSprite, XOffset + 350, CurrentY);
        CurrentY += 100;
        
        for (int i = 0; i < Chapter.NPages; i++)
        {
            DisplayTime(XOffset + 80 - (i+1)*300*WinScreen.SlideIn, CurrentY, 0.75f, Score.PageTime[i]);
            DisplayTimeDigit(Score.PageBonus[i], XOffset + 450 - (i+1)*300*WinScreen.SlideIn, CurrentY, 0.75f, 0, 0);
            
            if (Score.NewRecord[i])
                AddLitSprite(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterNewSprite, XOffset + 550 + i*WinScreen.SlideIn*300, CurrentY);
            
            CurrentY += 75;
        }
        
        CurrentY += 75;
        
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterTotalTimeSprite, 384 + 2000*WinScreen.SlideIn, CurrentY, 1.0f, 1.0f);
        CurrentY += 75;

        DisplayTime(200, CurrentY, 1.0f, Score.ChapterTime);
        CurrentY += 150;
        
        if (Score.NewChapterRecord)
        {
            float NewRecordScale = 1.0f + SinWave(WinScreen.Timer, 0.75f) * 0.30f;
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ChapterNewRecordSprite, 384, CurrentY, NewRecordScale, 1.0f);
        }
    }
    
    float TapScape = 1.0f + SinWave(WinScreen.Timer, 0.5f) * 0.05f;
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &TapToContinueSprite, 384, LitScreenHeight - 75 + 300*WinScreen.SlideIn, TapScape, 1.0f);
}

void WinScreen_Advance()
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
#ifdef PLATFORM_WINDOWS
	bool Pressed = kbIsKeyDown(KB_RETURN) || msButton1;
#endif
#ifdef PLATFORM_IPHONE
	bool Pressed = msButton1;
#endif

    WinScreen.Timer += 1.0f/60.0f;
    
    WinScreen.SlideIn *= 0.9f;

    WinScreen.MedalFrame++;
    
	// Advance to playing state when button is released.
	if (WinScreen.Timer > 2.0f)
    { 
        if (!Pressed && WinScreen.Pressed)
        {
            if (WinScreen.State == WINSCREEN_MEDAL)
            {
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

