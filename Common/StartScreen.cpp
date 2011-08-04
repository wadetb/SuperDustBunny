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
	
	float DragX;
	float DragStartX;
    float DisplayX;

    float BackgroundFadeAlpha;
    
	bool Dragging;
	bool Pressed;
    
    bool ReleasedAtLeastOnce;
    
    float PressedTime;
    float StartupTime;
    
    bool WelcomeDisplayed;
    
    float WiggleTime;
};

SStartScreen StartScreen;

const float StartScreenSpacing = 470.0f;
const float StartScreenDragSpacing = 235.0f;


gxSprite* GetStartScreenBadge(int Item)
{
    if (Item >= STARTSCREEN_ITEM_FIRST_CHAPTER)
    {
        SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
        
        if (Chapter->Unlocked && !Chapter->Played)
            return &NewUnlockSprite;
    }
    
    return NULL;
}


gxSprite* GetStartScreenIcon(int Item)
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

gxSprite* GetStartScreenPressedIcon(int Item)
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

gxSprite* GetStartScreenBackground(int Item)
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

bool GetStartScreenLocked(int Item)
{
    if (Item == STARTSCREEN_ITEM_HELP)
        return false;
    
    if (Item == STARTSCREEN_ITEM_CREDITS)
        return false;
    
    SChapterListEntry* Chapter = &Chapters[Item - STARTSCREEN_ITEM_FIRST_CHAPTER];
    return !Chapter->Unlocked;
}

int GetStartScreenItemCount()
{
    return STARTSCREEN_ITEM_NON_CHAPTER_COUNT + NChapters;
}

void InitStartScreen()
{
	StartScreen.CurItem = STARTSCREEN_ITEM_FIRST_CHAPTER + CurrentChapter;
    StartScreen.PrevItem = StartScreen.CurItem;
    StartScreen.BackgroundFadeAlpha = 0.0f;
	StartScreen.DragX = StartScreen.CurItem * StartScreenDragSpacing;
	StartScreen.DisplayX = StartScreen.CurItem * StartScreenSpacing;
    StartScreen.PressedTime = 0.0f;
    StartScreen.ReleasedAtLeastOnce = false;
}

void StartScreen_Advance()
{
    StartScreen.Dragging = false;
    StartScreen.Pressed = false;
    StartScreen.PressedTime = 0.0f;
    StartScreen.ReleasedAtLeastOnce = false;
    
	if (StartScreen.CurItem >= STARTSCREEN_ITEM_FIRST_CHAPTER)
	{
        CurrentChapter = StartScreen.CurItem - STARTSCREEN_ITEM_FIRST_CHAPTER;
        extern int PlaybackID;
        if (PlaybackID >= 0)
            SetGameState_Transition(GAMETRANSITION_PLAY_RECORDING);
        else
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
	AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, GetStartScreenBackground(StartScreen.CurItem), -384, -256, 1152, 1536, 1.0f-StartScreen.BackgroundFadeAlpha);
	AddLitSpriteSizedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, GetStartScreenBackground(StartScreen.PrevItem), -384, -256 , 1152, 1536, StartScreen.BackgroundFadeAlpha);

    float Scale = Lerp(StartScreen.StartupTime, 0.0f, 0.8f, 2.0f, 1.0f);
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &LogoSprite, 384, 280, Scale, 1.0f);

	for (int i = 0; i < GetStartScreenItemCount(); i++)
	{
        float X = 384 + i*StartScreenSpacing - StartScreen.DisplayX;
        float Y = LitScreenHeight * 0.7f;

        float Scale = Remap(fabsf(384-X), 0.0f, 500.0f, 0.9f, 0.6f, true);
        
        if (i == StartScreen.CurItem)
        {
            X += cosf(StartScreen.WiggleTime*4.0f) * 2.5f + cosf(StartScreen.WiggleTime*1.0f/3.0f) * 2.5f;
            Y += sinf(StartScreen.WiggleTime*4.0f) * 2.5f + sinf(StartScreen.WiggleTime*1.0f/3.0f) * 2.5f;
            
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
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, BadgeSprite, X+150, Y-150, Scale, 1.0f);                
    }

    if (StartScreen.StartupTime < 1.0f)
    {
        float Alpha = Max(0, 1.0f - StartScreen.StartupTime);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2, Alpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2-ScreenStart1Sprite.height, Alpha);
        AddLitSpriteAlpha(LIGHTLIST_WIPE, &ScreenStart1Sprite, 0, LitScreenHeight/2 - ScreenStart1Sprite.height/2+ScreenStart1Sprite.height, Alpha);
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

	if (kbIsKeyDown(KB_A) && !kbWasKeyDown(KB_A))
	{
		StartScreen.DragX -= StartScreenDragSpacing;
	}
	if (kbIsKeyDown(KB_D) && !kbWasKeyDown(KB_D))
	{
		StartScreen.DragX += StartScreenDragSpacing;
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
                if ( !GetStartScreenLocked(StartScreen.CurItem) )
                {
                    StartScreen_Advance();
                }
			}

			StartScreen.Dragging = false;
			StartScreen.Pressed = false;
        }
		else 
		{
			if (abs(msX - StartScreen.DragStartX) > 10)
				StartScreen.Pressed = false;

			StartScreen.DragX += StartScreen.DragStartX - msX;
			StartScreen.DragStartX = (float)msX;
		}        
	}
	else
	{
		if (msButton1)
		{
            StartScreen.Pressed = true;
            StartScreen.Dragging = true;
            StartScreen.DragStartX = (float)msX;
            StartScreen.PressedTime = 0.0f;
		}
	}

    if (StartScreen.DragX < 0)
		StartScreen.DragX = 0;
	if (StartScreen.DragX >= (GetStartScreenItemCount()-1)*StartScreenDragSpacing)
		StartScreen.DragX = (GetStartScreenItemCount()-1)*StartScreenDragSpacing;

    int NewItem = (int)Round(StartScreen.DragX / StartScreenDragSpacing);
    if (NewItem != StartScreen.CurItem)
    {
        StartScreen.PrevItem = StartScreen.CurItem;
        StartScreen.CurItem = NewItem;
        StartScreen.BackgroundFadeAlpha = 1.0f;
    }
    
    StartScreen.BackgroundFadeAlpha *= 0.9f;
    
    StartScreen.DisplayX = ( StartScreen.CurItem * StartScreenSpacing ) * 0.1f + StartScreen.DisplayX * 0.9f;
    
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
        //DisplayWelcomeAlert();
    }
    
    StartScreen.WiggleTime += 1.0f/60.0f;
}
