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

#define GOLD_PAGE_COUNT     20
#define SILVER_PAGE_COUNT   10

void InitDieScreen()
{
    DieScreen.ButtonEverReleased = false;
    DieScreen.ScoreVisible = true;
	DieScreen.Timer = 0;
    DieScreen.TearTime = 0;
    DieScreen.SlideIn = 1.0f;
    DieScreen.MedalFrame = 0;

    ResetLightState();
    
    const char** MedalNames;
    if (Portfolio.PageCount > GOLD_PAGE_COUNT)
    {
        MedalNames = GoldMedalNames;
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Gold medal"]];
#endif
    }
    else if (Portfolio.PageCount > SILVER_PAGE_COUNT)
    {
        MedalNames = SilverMedalNames;
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Silver medal"]];
#endif
    }
    else
    {
        MedalNames = BronzeMedalNames;
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Got a Bronze medal"]];
#endif
    }
    
    for (int i = 0; i < MEDAL_FRAMES; i++)
        LoadSpriteAsset(MedalNames[i], &MedalFrames[i]);
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
    if (!DieScreen.ScoreVisible && msButton1)
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
    
    AddLitSprite(LIGHTLIST_VACUUM, &LeaderboardBackgroundSprite, 0 - 600*DieScreen.SlideIn, 0);

    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &MedalFrames[(DieScreen.MedalFrame/8)%MEDAL_FRAMES], 384, 300 + 300*DieScreen.SlideIn, 0.5f, 1.0f);

    DisplayString(LIGHTLIST_WIPE, "you survived", FORMAT_CENTER_X, 384 - 600*DieScreen.SlideIn, 540, 1.0f);
    char Work[40];
    snprintf(Work, sizeof(Work), "%d", Portfolio.PageCount);
    DisplayString(LIGHTLIST_WIPE, Work, FORMAT_CENTER_X, 384 - 600*DieScreen.SlideIn, 610, 1.75f);
    DisplayString(LIGHTLIST_WIPE, "pages!", FORMAT_CENTER_X, 384 - 600*DieScreen.SlideIn, 740, 1.0f);

    AddLitSpriteScaled(LIGHTLIST_WIPE, &CoinSpin1Sprite, 220 - 600*DieScreen.SlideIn, 800, 1.5f, 1.5f);
    snprintf(Work, sizeof(Work), "x%d", Score.TotalLives);
    DisplayString(LIGHTLIST_WIPE, Work, 0, 380 - 600*DieScreen.SlideIn, 860, 1.75f);
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
            }
            else
            {
                DieScreen_Advance();
                return;
            }
        }
    }
}
