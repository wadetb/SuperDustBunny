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
	int CurItem;
    int PrevItem;
	
    float DragTimer;
	float DragX;
	float DragStartX;
    float DisplayX;

    float BackgroundFadeAlpha;
    float LeaderboardButtonAlpha;
    
    bool LeaderboardsUnlocked;
    bool LeaderboardsToolTipVisible;//Tooltip for Leaderboards
    
	bool Dragging;
	bool Pressed;
    
    bool ReleasedAtLeastOnce;
    
    float PressedTime;
    float StartupTime;
    
    bool WelcomeDisplayed;
    
    float WiggleTime;
    
    bool LeaderboardVisible;
};

SStartScreen StartScreen;

const float StartScreenSpacing = 470.0f;
const float StartScreenDragSpacing = 300.0f;


static gxSprite* GetStartScreenMedal(int Item)
{
    if (Item >= STARTSCREEN_ITEM_FIRST_CHAPTER)
    {
        SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
        
        if (Chapter->BestTime <= Chapter->GoldTime)
            return &GoldMedalSprite;
        if (Chapter->BestTime <= Chapter->SilverTime)
            return &SilverMedalSprite;
        if (Chapter->BestTime <= Chapter->SilverTime)
            return &BronzeMedalSprite;
    }
    
    return NULL;
}

static gxSprite* GetStartScreenBadge(int Item)
{
    if (Item >= STARTSCREEN_ITEM_FIRST_CHAPTER)
    {
        SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
        
        if (Chapter->Unlocked && !Chapter->Played)
            return &NewUnlockSprite;
    }
    
    return NULL;
}

static gxSprite* GetStartScreenIcon(int Item)
{
    if (Item == STARTSCREEN_ITEM_HELP)
        return &IconHelp1Sprite;
    
    if (Item == STARTSCREEN_ITEM_CREDITS)
        return &IconCredits1Sprite;

    SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
    if (Chapter->Unlocked)
    {
        if (Chapter->HasIcon)
            return &Chapter->IconSprite;
        else
            return &IconStart1Sprite;
    }
    else
        return &IconLockedSprite;
}

static gxSprite* GetStartScreenPressedIcon(int Item)
{
    if (Item == STARTSCREEN_ITEM_HELP)
        return &IconHelp2Sprite;
    
    if (Item == STARTSCREEN_ITEM_CREDITS)
        return &IconCredits2Sprite;
    
    SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
    if (Chapter->Unlocked)
    {
        if (Chapter->HasIcon)
            return &Chapter->IconSprite;
        else
            return &IconStart2Sprite;
    }
    else
        return &IconLockedSprite;
}

static gxSprite* GetStartScreenBackground(int Item)
{
    if (Item == STARTSCREEN_ITEM_HELP)
        return &BackgroundFridgeSprite;
    
    if (Item == STARTSCREEN_ITEM_CREDITS)
        return &BackgroundFridgeSprite;
    
    SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
    if (Chapter->HasBackground)
        return &Chapter->BackgroundSprite;
    else
        return &BackgroundFridgeSprite;
}

static bool GetStartScreenLocked(int Item)
{
    if (Item == STARTSCREEN_ITEM_HELP)
        return false;
    
    if (Item == STARTSCREEN_ITEM_CREDITS)
        return false;
    
    SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
    return !Chapter->Unlocked;
}

static int GetStartScreenItemCount()
{
    return STARTSCREEN_ITEM_NON_CHAPTER_COUNT + NChapters;
}

void InitStartScreen()
{
	StartScreen.CurItem = STARTSCREEN_ITEM_FIRST_CHAPTER + CurrentChapter;
    StartScreen.PrevItem = StartScreen.CurItem;
    StartScreen.BackgroundFadeAlpha = 0.0f;
    StartScreen.LeaderboardButtonAlpha = 0.0f;
	StartScreen.DragX = StartScreen.CurItem * StartScreenDragSpacing;
	StartScreen.DisplayX = StartScreen.CurItem * StartScreenSpacing;
    StartScreen.PressedTime = 0.0f;
    StartScreen.ReleasedAtLeastOnce = false;
    
    StartScreen.LeaderboardsToolTipVisible = false;  
    
    StartScreen.LeaderboardsUnlocked = Chapters[CurrentChapter].Completed;
}

static void StartScreen_Advance()
{
    StartScreen.Dragging = false;
    StartScreen.Pressed = false;
    StartScreen.PressedTime = 0.0f;
    StartScreen.ReleasedAtLeastOnce = false;
    

    
	if (StartScreen.CurItem >= STARTSCREEN_ITEM_FIRST_CHAPTER)
	{
#ifdef PLATFORM_IPHONE
        if (Settings.LiveAssets)
            UpdateLiveAssetCache();
#endif

        CurrentChapter = StartScreen.CurItem - STARTSCREEN_ITEM_FIRST_CHAPTER;

#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Entered chapter %s", Chapters[CurrentChapter].Name]];
#endif

        extern int PlaybackID;
        if (PlaybackID >= 0)
            SetGameState_Transition(GAMETRANSITION_PLAY_RECORDING);
        else
            SetGameState_Transition(GAMETRANSITION_FIRST_PAGE);
    }
	else if (StartScreen.CurItem == STARTSCREEN_ITEM_HELP)
	{
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Opened Help"]];
#endif

		SetGameState_Help();
	}
	else if (StartScreen.CurItem == STARTSCREEN_ITEM_CREDITS)
	{
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Opened Settings"]];

        [theViewController showSettings];
#endif
//		SetGameState_Credits();
    }
}

void DisplayStartScreen()
{
    // Fading backgrounds.
	AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, GetStartScreenBackground(StartScreen.CurItem), 0, 0, 768, LitScreenHeight, 1.0f-StartScreen.BackgroundFadeAlpha);
	AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, GetStartScreenBackground(StartScreen.PrevItem), 0, 0 , 768, LitScreenHeight, StartScreen.BackgroundFadeAlpha);

    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &LogoSprite, 384, 280, 1.0f, 1.0f);

    // Leaderboard button.
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonLeaderboardSprite, 768-90 + sinf(StartScreen.WiggleTime)*4.0f, 90, 1.0f, StartScreen.LeaderboardButtonAlpha);
    
    //Display ToolTip next to Leaderboard Button
    if(StartScreen.LeaderboardsToolTipVisible)
    {
        AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, 658-90 + sinf(StartScreen.WiggleTime)*4.0f, 110, 0, 40, 40, 74, StartScreen.LeaderboardButtonAlpha);
        
        AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, 558-90 + sinf(StartScreen.WiggleTime)*4.0f, 150, 0, 0, 45, 42, StartScreen.LeaderboardButtonAlpha);
        
        AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, 208-90 + sinf(StartScreen.WiggleTime)*4.0f, 200, 85, 0, 736, 236, StartScreen.LeaderboardButtonAlpha);
    
        DisplayMultilineStringAlpha(LIGHTLIST_VACUUM, "tap for\n leaderboards", FORMAT_CENTER_X | FORMAT_CENTER_Y, 488-90 + sinf(StartScreen.WiggleTime)*4.0f, 320, .7f, StartScreen.LeaderboardButtonAlpha);
    }
    
    // Sliding main buttons.
	for (int i = 0; i < GetStartScreenItemCount(); i++)
	{
        float X = 384 + i*StartScreenSpacing - StartScreen.DisplayX;
        float Y = LitScreenHeight * 0.75f;

        float Scale = Remap(fabsf(384-X), 0.0f, 500.0f, 0.85f, 0.6f, true);
        
        if (i == StartScreen.CurItem)
        {
            X += cosf(StartScreen.WiggleTime*4.0f) * 5.0f + cosf(StartScreen.WiggleTime*1.0f/3.0f) * 5.0f;
            Y += sinf(StartScreen.WiggleTime*4.0f) * 5.0f + sinf(StartScreen.WiggleTime*1.0f/3.0f) * 5.0f;
            
            float OverlayAlpha = SinWave(StartScreen.PressedTime, 1.0f);
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, GetStartScreenIcon(i), X, Y, Scale, 1.0f);
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, GetStartScreenPressedIcon(i), X, Y, Scale, OverlayAlpha);
        }
        else
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, GetStartScreenIcon(i), X, Y, Scale, 1.0f);
        }

        gxSprite* BadgeSprite = GetStartScreenBadge(i);
        if (BadgeSprite)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, BadgeSprite, X+150*Scale, Y-150*Scale, Scale, 1.0f);                
        
        gxSprite* MedalSprite = GetStartScreenMedal(i);
        if (MedalSprite)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, MedalSprite, X+10*Scale, Y-150*Scale, Scale*2, 1.0f);                
    }

    if (StartScreen.StartupTime < 1.0f)
    {
        float Alpha = Max(0, 1.0f - StartScreen.StartupTime);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2, Alpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2-ScreenStart1Sprite.height, Alpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2+ScreenStart1Sprite.height, Alpha);
    }
    
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
    if (StartScreen.LeaderboardVisible)
    {
        StartScreen.LeaderboardButtonAlpha = Clamp(StartScreen.LeaderboardButtonAlpha - 0.05f, 0.0f, 1.0f);    

        if (msY < 200 && msX < 200 && msButton1 && !msOldButton1)
        {
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Closed Leaderboards", Chapters[CurrentChapter].Name]];
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
    
    if (!msButton1)
        StartScreen.ReleasedAtLeastOnce = true;
    if (!StartScreen.ReleasedAtLeastOnce)
        return;
    
	if (StartScreen.Dragging)
	{
		if (!msButton1)
		{
			StartScreen.Dragging = false;
        }
		else 
		{
			StartScreen.DragX += StartScreen.DragStartX - msX;
			StartScreen.DragStartX = (float)msX;
		}        
	}
	else
	{
        StartScreen.DragX = StartScreen.CurItem * StartScreenDragSpacing;

        if (StartScreen.Pressed)
        {
            StartScreen.PressedTime += 1.0f/60.0f;
//            if (StartScreen.PressedTime < 0.25f)
            {
                if (fabsf((float)msX - StartScreen.DragStartX) > 10)
                {
                    StartScreen.Pressed = false;
                    StartScreen.Dragging = true;
                }
            }
            
            if (!msButton1)
            {
                StartScreen.Pressed = false;
                if ( !GetStartScreenLocked(StartScreen.CurItem) )
                {
                    StartScreen_Advance();
                }
			}
        }
        else
        {
            if (msY >= 256 && msButton1)
            {
                StartScreen.Pressed = true;
                StartScreen.DragStartX = (float)msX;
                StartScreen.PressedTime = 0.0f;
            }
        }
	}

    if (StartScreen.DragX < 0)
		StartScreen.DragX = 0;
	if (StartScreen.DragX >= (GetStartScreenItemCount()-1)*StartScreenDragSpacing)
		StartScreen.DragX = (GetStartScreenItemCount()-1)*StartScreenDragSpacing;

    int NewItem = (int)Round(StartScreen.DragX / StartScreenDragSpacing);
    if (NewItem > STARTSCREEN_ITEM_FIRST_CHAPTER + NChapters - 1)
        NewItem = STARTSCREEN_ITEM_FIRST_CHAPTER + NChapters - 1;
        
    if (NewItem != StartScreen.CurItem)
    {
        StartScreen.PrevItem = StartScreen.CurItem;
        StartScreen.CurItem = NewItem;
        StartScreen.BackgroundFadeAlpha = 1.0f;
    }
    
    if ( StartScreen.Dragging )
        StartScreen.DisplayX = StartScreen.DragX / StartScreenDragSpacing * StartScreenSpacing;
    else
        StartScreen.DisplayX = ( StartScreen.CurItem * StartScreenSpacing ) * 0.1f + StartScreen.DisplayX * 0.9f;
    
    StartScreen.BackgroundFadeAlpha *= 0.9f;
    
    if (StartScreen.CurItem >= STARTSCREEN_ITEM_FIRST_CHAPTER)
    {
        StartScreen.LeaderboardsUnlocked = Chapters[StartScreen.CurItem - STARTSCREEN_ITEM_FIRST_CHAPTER].Completed;
                
        if (StartScreen.LeaderboardsUnlocked)
        {                  
            if(StartScreen.CurItem == STARTSCREEN_ITEM_FIRST_CHAPTER && Settings.LeaderboardToolTipTap == false)
            {
                StartScreen.LeaderboardsToolTipVisible = true;
            }
            
            if (msY < 256 && msX > 384 && msButton1 && !msOldButton1)
            {
                
                CurrentChapter = StartScreen.CurItem - STARTSCREEN_ITEM_FIRST_CHAPTER;
                
#ifdef PLATFORM_IPHONE
                [TestFlight passCheckpoint:[NSString stringWithFormat:@"Opened Leaderboards for %s", Chapters[CurrentChapter].Name]];
#endif
                StartScreen.LeaderboardVisible = true;
                
                
                //Needs to equal saved component. Which we can do upon initialization.
                if(Settings.LeaderboardToolTipTap == false)
                {
                    StartScreen.LeaderboardsToolTipVisible = false;
                    Settings.LeaderboardToolTipTap = !Settings.LeaderboardToolTipTap;
                    
                    SaveSettings();
                }
                
                InitLeaderboardScreen();
                return;
            }
        }
    }
    else
    {
        StartScreen.LeaderboardsUnlocked = false;
    }

    if (StartScreen.LeaderboardsUnlocked)
        StartScreen.LeaderboardButtonAlpha = Clamp(StartScreen.LeaderboardButtonAlpha + 0.05f, 0.0f, 1.0f);
    else
        StartScreen.LeaderboardButtonAlpha = Clamp(StartScreen.LeaderboardButtonAlpha - 0.05f, 0.0f, 1.0f);    

    StartScreen.StartupTime += 1.0f/60.0f;
    if (StartScreen.StartupTime >= 1.0f && !StartScreen.WelcomeDisplayed)
    {
        StartScreen.WelcomeDisplayed = true;
        //DisplayWelcomeAlert();
    }
    
    StartScreen.WiggleTime += 1.0f/60.0f;
}
