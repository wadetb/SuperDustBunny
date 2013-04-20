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
#include "Text.h"
#include "GameScore.h"
#include "Settings.h"
#include "LeaderboardScreen.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


#define MEDAL_FRAMES 8


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

struct SDieScreen
{
    bool ScoreVisible;
    bool LeaderboardVisible;
    
	int Timer;
    
    int TearTime;
    float TearDropX;
    float TearDropY;
    float TearDropVY;
    
    float SlideIn;
    int MedalFrame;
    
    bool ButtonEverReleased;
};

SDieScreen DieScreen;

void InitDieScreen()
{
    DieScreen.ButtonEverReleased = false;
    DieScreen.ScoreVisible = true;
    DieScreen.LeaderboardVisible = false;
	DieScreen.Timer = 0;
    DieScreen.TearTime = 0;
    DieScreen.SlideIn = 1.0f;
    DieScreen.MedalFrame = 0;

    ResetLightState();
    
    if (Portfolio.PageCount >= BRONZE_PAGE_COUNT)
    {
        const char** MedalNames;
        if (Portfolio.PageCount >= GOLD_PAGE_COUNT)
        {
            MedalNames = GoldMedalNames;
    #ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Gold medal"]];
    #endif
        }
        else if (Portfolio.PageCount >= SILVER_PAGE_COUNT)
        {
            MedalNames = SilverMedalNames;
    #ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Silver medal"]];
    #endif
        }
        else if (Portfolio.PageCount >= BRONZE_PAGE_COUNT)
        {
            MedalNames = BronzeMedalNames;
    #ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Bronze medal"]];
    #endif
        }
        
        for (int i = 0; i < MEDAL_FRAMES; i++)
            LoadSpriteAsset(MedalNames[i], &MedalFrames[i]);
    }
    
    if (Portfolio.PageCount > Settings.PageCount)
    {
        Settings.PageCount = Portfolio.PageCount;
        SaveSettings();
    }

    UploadScore();
    
    InitLeaderboardScreen();
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
    AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenLoseGrave1Sprite, 0, 0, 768, LitScreenHeight, 1.0f);
    
    float PulseAlpha;
    if (!DieScreen.ScoreVisible && !DieScreen.LeaderboardVisible && msButton1)
        PulseAlpha = 1.0f;
    else
        PulseAlpha = SinWave(DieScreen.Timer/60.0f, 2.0f)*0.5f;
    
    AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &ScreenLoseGrave2Sprite, 0, 0, 768, LitScreenHeight, PulseAlpha);

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
    
    // Score
    AddLitSpriteScaled(LIGHTLIST_VACUUM, &LeaderboardBackgroundSprite, LeaderboardBackgroundSprite.width - 600*DieScreen.SlideIn, 0, -1, 1);

    float TextY;
    
    if (Portfolio.PageCount >= BRONZE_PAGE_COUNT)
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &MedalFrames[(DieScreen.MedalFrame/8)%MEDAL_FRAMES], 384, 300 + 300*DieScreen.SlideIn, 0.5f, 1.0f);
        TextY = 500;
    }
    else
    {
        TextY = 300;
    }

    DisplayString(LIGHTLIST_WIPE, "you survived", FORMAT_CENTER_X, 384 - 600*DieScreen.SlideIn, TextY+40, 1.0f);
    char Work[40];
    snprintf(Work, sizeof(Work), "%d", Portfolio.PageCount);
    DisplayString(LIGHTLIST_WIPE, Work, FORMAT_CENTER_X, 384 - 600*DieScreen.SlideIn, TextY+110, 1.75f);
    DisplayString(LIGHTLIST_WIPE, "pages!", FORMAT_CENTER_X, 384 - 600*DieScreen.SlideIn, TextY+240, 1.0f);

    AddLitSpriteScaled(LIGHTLIST_WIPE, &CoinIconSprite, 220 - 600*DieScreen.SlideIn, 820, 0.65f*1.5f, 0.65f*1.5f);
    snprintf(Work, sizeof(Work), "x%d", Portfolio.TotalLives);
    DisplayString(LIGHTLIST_WIPE, Work, 0, 380 - 600*DieScreen.SlideIn, 860, 1.75f);

    if (DieScreen.LeaderboardVisible)
        DisplayLeaderboardScreen();
}

static void DieScreen_Advance()
{
    for (int i = 0; i < MEDAL_FRAMES; i++)
        gxDestroySprite(&MedalFrames[i]);

	SetGameState_StartScreen();
}

void UpdateDieScreen()
{
    if (DieScreen.Timer == 60)
        DieScreen.ScoreVisible = true;

    if (DieScreen.ScoreVisible)
        DieScreen.SlideIn *= 0.9f;
    else
        DieScreen.SlideIn *= 1.5f;
    
    if (DieScreen.LeaderboardVisible)
        UpdateLeaderboardScreen();
    
    DieScreen.MedalFrame++;

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
    
    if (!msButton1)
        DieScreen.ButtonEverReleased = true;
    if (!DieScreen.ButtonEverReleased)
        return;
    
    if (DieScreen.Timer >= 130)
    {
        if (msButton1 && !msOldButton1)
        {
            if (DieScreen.ScoreVisible)
            {
                DieScreen.ScoreVisible = false;
                DieScreen.ButtonEverReleased = false;
                DieScreen.SlideIn = 0.01f;
                DieScreen.LeaderboardVisible = true;
            }
            else if (DieScreen.LeaderboardVisible)
            {
                if (msX > 768-200 && msY < 200)
                {
                    DieScreen.LeaderboardVisible = false;
                }
                
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
            }
            else
            {
                DieScreen_Advance();
                return;
            }
        }
    }
}
