//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Chapter.h"
#include "Lighting.h"
#include "Barrel.h"
#include "Fan.h"
#include "Dusty.h"
#include "Coin.h"
#include "Vacuum.h"
#include "TennisBall.h"
#include "FireWorks.h"
#include "Dust.h"
#include "Gear.h"
#include "Wipe.h"
#include "Flame.h"
#include "Flashlight.h"
#include "Debris.h"
#include "Lives.h"
#include "Stapler.h"
#include "Recorder.h"
#include "GameScore.h"
#include "Settings.h"
#include "PowerUp.h"
#include "Input.h"
#include "Smoke.h"
#include "Tutorial.h"
#include "Ghost.h"
#include "Hanger.h"
#include "Baby.h"
#include "Tweak.h"
#include "Balloon.h"

#include "StartScreen.h"
#include "StoreScreen.h"
#include "HelpScreen.h"
#include "CreditsScreen.h"
#include "LeaderboardScreen.h"
#include "DieScreen.h"
#include "WinScreen.h"
#include "ChapterIntro.h"
#import "Text.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif
#ifdef PLATFORM_MAC
#import "SDBAppDelegate.h"
#endif

enum EGameState
{
	GAMESTATE_PLAYING,
	GAMESTATE_TRANSITION,
	GAMESTATE_START_SCREEN,
	GAMESTATE_STORE_SCREEN,
	GAMESTATE_DIE_SCREEN,
	GAMESTATE_WIN_SCREEN,
	GAMESTATE_HELP_SCREEN,
	GAMESTATE_CREDITS_SCREEN,
	GAMESTATE_LEADERBOARD_SCREEN,
	GAMESTATE_CHAPTER_INTRO,
};

EGameState GameState = GAMESTATE_START_SCREEN;

EGameTransition GameTransition;

bool DisplayHelp = false;
bool SlowMotionMode = false;
float PauseTimer = 0.0f;
float PauseSlideIn = 1.0f;
bool GamePause = false;
bool GameMuted = false;

float FPS;

//int PlaybackID = 4396;
int PlaybackID = -1;


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Initialization functions                                                           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void Init()
{
	// Use iPad "portrait mode" screen dimensions.
	gxInit();

#ifdef PLATFORM_WINDOWS
	gxEmulateDisplaySize(GXDISPLAY_IPAD_PORTRAIT);
#endif
    
	sxInit();

#ifdef PLATFORM_WINDOWS
	kbInit();
#endif
	
	msInit();
    
    InitSettings();
	InitLighting();
    
    LoadInventory();
	
    LoadBundleAssetList();

    SetGameState_StartScreen();
}

void Exit()
{
#ifdef PLATFORM_WINDOWS
	kbDeinit();
#endif

	msDeinit();
	
	gxDeinit();
	
	sxDeinit();
}

double GetCurrentTime()
{
#ifdef PLATFORM_IPHONE_OR_MAC
	mach_timebase_info_data_t timerInfo;
	mach_timebase_info(&timerInfo);
	
	const double TIMER_RATIO = ((double)timerInfo.numer / (double)timerInfo.denom);

	return (double)mach_absolute_time() * TIMER_RATIO / 1000000000.0;
#endif

#ifdef PLATFORM_WINDOWS
    return timeGetTime();
#endif
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Frames per second tracking                                                            //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //

static void UpdateFPS()
{
#ifdef PLATFORM_WINDOWS
	static DWORD LastTime = 0;
	DWORD Time = timeGetTime();
	FPS = (1000.0f / ((float)Time - (float)LastTime) + 0.5f);
	LastTime = Time;
#endif
#ifdef PLATFORM_IPHONE_OR_MAC
	mach_timebase_info_data_t timerInfo;
	mach_timebase_info(&timerInfo);
    
	const double TIMER_RATIO = 1e-9 * ((float)timerInfo.numer / (float)timerInfo.denom);
    
	static double LastTime = mach_absolute_time() * TIMER_RATIO - 1.0f / 60.0f;
	double Time = mach_absolute_time() * TIMER_RATIO;
	FPS = 1.0f / (Time - LastTime);
	LastTime = Time;
#endif
    
	//FPS = (double)(int(FPS / 10.0f + 0.5f) * 10);
    
    Recorder.SumFPS += (int)FPS;
    Recorder.FPSCount++;
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   LoadCurrentChapter Implementation                                                       //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

void LoadCurrentChapter()
{
	ClearChapter();

	if (CurrentChapter < 0 || CurrentChapter >= NChapters)
		CurrentChapter = 0;

    LoadTweaks();
    
	LoadChapter(Chapters[CurrentChapter].Name);
}

static void SetupNextPage()
{
    if (CurrentChapter == 0)
    {
        SetCurrentPage(0);
    }
    else
    {
        AdvancePortfolio();
    }
}

void AdvanceToNextPage()
{
    Portfolio.VacuumDamage = Vacuum.Damage;
    if (Portfolio.DustBuster)
    {
        Portfolio.VacuumDistance = Portfolio.InitialVacuumDistance;
        Portfolio.VacuumSide = Vacuum.Side;
        Portfolio.VacuumDir = Vacuum.Dir;
    }
    else
    {
        Portfolio.VacuumDistance = fabsf(Dusty.FloatY - Vacuum.Y);
    }
    
    if (CurrentChapter == 0)
    {
        sxPlaySound(&NextPageSound);

        if (IsRecordingActive())
    		StopRecording(RESULT_NEXT_PAGE);

        SetGameState_StartScreen();
    }
    else
	{
        sxPlaySound(&NextPageSound);

        if (IsRecordingActive())
    		StopRecording(RESULT_NEXT_PAGE);
        
		SetGameState_Transition(GAMETRANSITION_NEXT_PAGE);
	}
}

void AdvanceToDieScreen()
{
    if (CurrentChapter == 0)
    {
        sxPlaySound(&NextPageSound);
        
        if (IsRecordingActive())
    		StopRecording(RESULT_NEXT_PAGE);
        
		SetGameState_Transition(GAMETRANSITION_NEXT_PAGE);
    }
    else
    {        
        if (IsRecordingActive())
            StopRecording(RESULT_DIE);
        
        SetGameState_Transition(GAMETRANSITION_DIE_SCREEN);
        
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Game over"]];
#endif
    }
}

static void SkipToNextPage()
{
#ifdef PLATFORM_IPHONE
    [TestFlight passCheckpoint:@"Skipped to the next page"];
#endif
    if (IsRecordingActive())
        StopRecording(RESULT_NONE);
    SetCurrentPage((Chapter.PageNum+1) % Chapter.NPages);
}

static void SkipToPreviousPage()
{
#ifdef PLATFORM_IPHONE
    [TestFlight passCheckpoint:@"Skipped to the previous page"];
#endif
    if (IsRecordingActive())
        StopRecording(RESULT_NONE);
    SetCurrentPage((Chapter.PageNum+Chapter.NPages-1) % Chapter.NPages);
}

static void LoadRecordingChapterAndPage()
{
    char* ChapterName = GetRecordingChapterName();
    
    CurrentChapter = -1;
    for (int i = 0; i < NChapters; i++)
    {
        if (strcmp(ChapterName, Chapters[i].Name) == 0)
        {
            CurrentChapter = i;
            break;
        }
    }
    if (CurrentChapter == -1)
        ReportError("Chapter '%s' required by recording %d does not exist.", ChapterName, PlaybackID);
    
    LoadCurrentChapter();
    
    char* PageName = GetRecordingPageName();
    
    int PageNum = -1;
    for (int i = 0; i < Chapter.NPages; i++)
    {
        if (strcmp(PageName, Chapter.Pages[i].Name) == 0)
        {
            PageNum = i;
            break;
        }
    }
    if (PageNum == -1)
        ReportError("Page '%s' required by recording %d does not exist.", PageName, PlaybackID);
    
    SetCurrentPage(PageNum);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   SetGameState functions for screens                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

bool AssetsLoaded = false;

void SetGameState_StartScreen()
{
	GameState = GAMESTATE_START_SCREEN;

    if (!AssetsLoaded)
    {
        LoadAssets();
        AssetsLoaded = true;
    }

    ClearChapterList();
    LoadChapterList();
    
    CurrentChapter = 0;

	InitStartScreen();
}

void SetGameState_StoreScreen()
{
	GameState = GAMESTATE_STORE_SCREEN;
    
	InitStoreScreen();
}

void SetGameState_Help()
{
    GameState = GAMESTATE_HELP_SCREEN;

    InitHelpScreen();
}

void SetGameState_Credits()
{
    GameState = GAMESTATE_CREDITS_SCREEN;
    
    InitCreditsScreen();
}

void SetGameState_Leaderboard()
{
    GameState = GAMESTATE_LEADERBOARD_SCREEN;
    
    InitLeaderboardScreen();
}

void SetGameState_DieScreen()
{
	GameState = GAMESTATE_DIE_SCREEN;

    LitSceneZoom = 1.0f;
}

void SetGameState_WinScreen()
{
	GameState = GAMESTATE_WIN_SCREEN;

    LitSceneZoom = 1.0f;

	TurnOffVacuum();

#if 0
	InitWinScreen();
#endif
}

void SetGameState_ChapterIntro()
{
    GameState = GAMESTATE_CHAPTER_INTRO;
}

void SetGameState_Playing()
{
	GameState = GAMESTATE_PLAYING;

    GamePause = false;
    
	InitPowerUp();

    GetInput_ConsumeAllSwipe();
}

static void DisplayPauseScreen()
{
    if (GameState == GAMESTATE_CHAPTER_INTRO || GameState == GAMESTATE_TRANSITION)
        return;

    if (GamePause)
    {
        float Lerp = Remap(PauseTimer, 0.0f, 0.175f, 0.0f, 1.0f, true);
        
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonPauseSprite, 384, 64, 1.0f+Lerp, 1.0f-Lerp);
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonPlaySprite, 384, 160 - 160*PauseSlideIn, 2.25f, 1.0f);
        
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonHomeSprite, 192 - 600*PauseSlideIn, 500, 1.2f, 1.0f);
        
        if (GameMuted)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonUnmuteSprite, 768-192 + 600*PauseSlideIn, 500, 1.2f, 1.0f);
        else
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonMuteSprite, 768-192 + 600*PauseSlideIn, 500, 1.2f, 1.0f);        
        
        if (Settings.DeveloperMode)
        {
            char Work[1024];
            snprintf(Work, sizeof(Work), "%s", Chapter.Pages[Chapter.PageNum].Name);
            for (int i = 0; i < strlen(Work); i++)
                Work[i] = tolower(Work[i]);
            DisplayString(LIGHTLIST_WIPE, Work, FORMAT_CENTER_X, 384, LitScreenHeight-60, 0.75f);
        }
        
        if (Settings.LiveAssets)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_WIPE, &ButtonPlaySprite, 384 - 600*PauseSlideIn, 860, 2.25f, DegreesToRadians(90.0f));
        }
        
        if (!Settings.DeveloperMode && !Settings.LiveAssets)
        {
            char Work[40];
            AddLitSpriteScaled(LIGHTLIST_WIPE, &CoinIconSprite, 170 - 600*PauseSlideIn, 720, 0.65f*1.5f, 0.65f*1.5f);
            snprintf(Work, sizeof(Work), "x%d", Settings.Lives);
            DisplayString(LIGHTLIST_WIPE, Work, 0, 330 - 600*PauseSlideIn, 760, 1.75f);
        }
    }
    else
    {
        // TODO ADD FLYOUT
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonPauseSprite, 384, 64, 1.0f, 1.0f);
    }
}

static void UpdatePauseScreen()
{
    PauseTimer += 1.0f/60.0f;
    PauseSlideIn *= 0.8f;
    
    if (msButton1 && !msOldButton1)
    {
        if (msX > 384+64 && msY >= 300 && msY <= 600)
        {
            GameMuted = !GameMuted;
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"%s the game", GameMuted ? "Muted" : "Unmuted"]];
#endif
        }

        if (msX < 384-64 && msY >= 300 && msY <= 600)
        {
            GamePause = false;
            if (IsRecordingActive())
			    StopRecording(RESULT_RESTART_PAGE);
#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:@"Escaped to the home screen"];
#endif
            TurnOffVacuum();
            SetGameState_StartScreen();
        }
        
        if (Settings.LiveAssets)
        {
            if (abs(msX - 384) < 300 && msY >= 700 && msY <= 1000)
            {
#ifdef PLATFORM_IPHONE
                [TestFlight passCheckpoint:@"Replayed the current page"];
#endif
                UpdateLiveAssetCache();
                TurnOffVacuum();
                if (IsRecordingActive())
                    StopRecording(RESULT_NONE);
                float OldX = Dusty.FloatX;
                float OldY = Dusty.FloatY;
                LoadCurrentChapter();
                SetCurrentPage(Chapter.PageNum);
                Dusty.FloatX = OldX;
                Dusty.FloatY = OldY;
            }
        }
    }
}

static void PauseGame()
{
    if (GamePause)
    {
        GamePause = false;
        PauseTimer = 0.0f;
        PauseSlideIn = 1.0f;        
    }
    else
    {
        GamePause = true;
        PauseTimer = 0.0f;
        PauseSlideIn = 1.0f;
    }
}

static void DisplayBackground()
{
    gxSprite* BackgroundSprite;
    
    if (Chapter.HasBackground)
        BackgroundSprite = &Chapter.BackgroundSprite;
    else
        BackgroundSprite = &BackgroundPaperSprite;
    
    if (!BackgroundSprite || !BackgroundSprite->width || !BackgroundSprite->height)
        return;
    
	int y = 0;
	while (y < Chapter.PageHeight*64)
	{
        int x = 0;
        while (x < Chapter.PageWidth*64)
        {
            AddLitSprite(LIGHTLIST_BACKGROUND, BackgroundSprite, x + ScrollX, (float)y+ScrollY);
            x += BackgroundSprite->width-1;
        }
        
		y += BackgroundSprite->height-1;
	}
}

void DisplayGame_Playing()
{	
	// Calculate scrolling.
	CalculateScroll();

	// Chapter Drawing - Everything here is behind Dusty
    DisplayBackground();
	DisplayChapterBaseLayer();
	DisplayBarrels_BeforeDusty();
	DisplayCoins();
    DisplayGear();
    DisplayBall();
	DisplayFireWorks();
    DisplayFlames();
	DisplayDebris();
	DisplayStaplers();
	DisplayBabies();
	DisplayBalloons();

	// Dusty Drawing
	DisplayDusty();
	
	// More Chapter Drawing - Everything here is on top of Dusty
	DisplayChapterExtraLayers();
    DisplayPortal();
	DisplayFans();
	DisplayBarrels_AfterDusty();
	DisplayDust();
	DisplayVacuum();
    DisplaySmoke();
	DisplayLives();
	DisplayPowerUp();
    DisplayTutorial();
	DisplayHangers();
    
    // HUD Drawing - Score, etc.
    DisplayDebug();
    
	// Display buttons
    DisplayPauseScreen();
        
	// Lighting effects.
	DisplayFlashlight();

	DisplayChapterIntro();
    DisplayGhost();
      	
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Debugging aids                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//	if (DevMode)
//	{
//		// Status of common variables
//		gxDrawString(5, 5+16, 16, gxRGB32(255, 255, 255), "( %.1f %.1f ) ( %.1f %.1f ) %s\n State: %d Col: %d%d%d%d Direction: %d Material: %d", 
//			Dusty.FloatX, Dusty.FloatY, Dusty.FloatVelocityX, Dusty.FloatVelocityY, Chapter.Pages[Chapter.PageNum].Name,
//			Dusty.State, Dusty.CollideWithLeftSide, Dusty.CollideWithRightSide, Dusty.CollideWithTopSide, Dusty.CollideWithBottomSide, Dusty.Direction, Dusty.CollideMaterial);
//		
//#ifdef PLATFORM_IPHONE_OR_MAC
//		// Draw the accelerometer data directly.
//		gxDrawRectangleFilled(768/2, 1024-32, msAccelX*300, 16, 0xff00ff00);
//#endif
//	}

	// Indicator for when slow motion is activated.
//	if (SlowMotionMode)
//	{
//		gxDrawString(gxScreenWidth-101, 5, 16, gxRGB32(255, 255, 0), "[SLOW]");
//	}
}

static void UpdateGame_Playing()
{ 
    if (msButton1 && !msOldButton1)
    {
        if (msX >= 384-150 && msX <= 384+150 && msY < 200)
        {
            PauseGame();
        }
    }
    
    // TODO: GamePause should actually stop the update and display loop, to reduce battery use.
	if (GamePause)
    {
        UpdatePauseScreen();
        return;
    }

    if (Recorder.RecordingActive && Settings.ControlStyle == CONTROL_TILT)
        RecordTiltEvents();
    
    UpdateRecorder();

    ResetDustyCollision();
	UpdateStapler_Collision();
    UpdateHangers();
    UpdateDusty();
    UpdateBabies();
    
    if (Dusty.State != DUSTYSTATE_DIE)
    {
        UpdateFans();
        UpdateBarrels();
        UpdateCoins();   
        UpdateBall();  
        UpdateGear();  
        UpdateFireWorks();
        UpdateFlames();
        UpdateFlashlight();
        UpdateLives();
        UpdateStaplers();
        UpdatePowerUp();
        UpdateVacuumTriggers();
        UpdateBalloons();
    }

    UpdateDust();
    UpdateDebris();
    UpdateVacuum(); 
    UpdateSmoke();
    UpdateTutorial();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Transition Game States                                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_Transition(EGameTransition Type)
{
	GameTransition = Type;

    LitSceneOffsetX = 0.0f;
    LitSceneOffsetY = 0.0f;
    LitSceneZoom = 1.0f;
    
	if (GameTransition == GAMETRANSITION_FIRST_PAGE)
	{
		StartWipe(WIPE_FADE_TO_BLACK, 2.0f);
	}
	else if (GameTransition == GAMETRANSITION_NEXT_PAGE)
	{
		StartWipe(WIPE_DIAGONAL, 0.5f);
	}
	else if (GameTransition == GAMETRANSITION_RESTART_PAGE)
	{
		StartWipe(WIPE_FADE_TO_WHITE, 1.0f);
	}
	else if (GameTransition == GAMETRANSITION_DIE_SCREEN)
	{
		StartWipe(WIPE_FADE_TO_WHITE, 3.0f);
	}
    else if (GameTransition == GAMETRANSITION_PLAY_RECORDING)
    {
		StartWipe(WIPE_FADE_TO_BLACK, 2.0f);
    }

	GameState = GAMESTATE_TRANSITION;
}

static void UpdateGame_Transition()
{
	if (GameTransition == GAMETRANSITION_FIRST_PAGE)
	{
		UpdateWipe();

		if (Wipe.Middle)
		{
			InitChapterIntro();
            StartChapterMusic();
            ShowPageNumber();
			Wipe.Middle = false;
		}
 
		if (Wipe.Finished)
		{
			SetGameState_ChapterIntro();
		}
	}
	else if (GameTransition == GAMETRANSITION_NEXT_PAGE)
	{
		UpdateWipe();

		if (Wipe.Middle)
		{
            SetupNextPage();
            if (Portfolio.NewChapter)
            {
                InitChapterIntro();
                StartChapterMusic();
            }
            ShowPageNumber();
			Wipe.Middle = false;
		}

		if (Wipe.Finished)
		{
            if (Portfolio.NewChapter)
            {
                Portfolio.NewChapter = false;
                SetGameState_ChapterIntro();
            }
            else
            {
                SetGameState_Playing();
            }
		}
	}
	else if (GameTransition == GAMETRANSITION_RESTART_PAGE)
	{
		UpdateWipe();

		if (Wipe.Middle)
		{
			SetCurrentPage(Chapter.PageNum);
			Wipe.Middle = false;
		}

		if (Wipe.Finished)
		{
			SetGameState_Playing();
            RestartChapterMusic();
		}
	}
	else if (GameTransition == GAMETRANSITION_DIE_SCREEN)
	{
		UpdateWipe();

		if (Wipe.Middle)
		{
			TurnOffVacuum();
			InitDieScreen();
			Wipe.Middle = false;
		}

        if (Wipe.T >= 0.5f)
        {
            UpdateDieScreen();
        }
        
		if (Wipe.Finished)
		{
			SetGameState_DieScreen();
		}
	}
    else if (GameTransition == GAMETRANSITION_PLAY_RECORDING)
    {
		UpdateWipe();
        
		if (Wipe.Middle)
		{
            DownloadRecording(PlaybackID);

            LoadRecordingChapterAndPage();

			if (Chapter.PageNum == 0)
                InitChapterIntro();
            
			Wipe.Middle = false;
		}
        
		if (Wipe.Finished)
		{
            StartPlayback();
            
            if (Chapter.PageNum == 0)
                SetGameState_ChapterIntro();
            else
                SetGameState_Playing();
		}
    }
}

static void DisplayGame_Transition()
{
	if (GameTransition == GAMETRANSITION_FIRST_PAGE)
	{
		if (Wipe.T < 0.5f)
		{
			DisplayStartScreen();
		}
		else
		{
			DisplayGame_Playing();
		}
		DisplayWipe();
	}
	else if (GameTransition == GAMETRANSITION_NEXT_PAGE)
	{
		DisplayGame_Playing();
		DisplayWipe();
	}
	else if (GameTransition == GAMETRANSITION_RESTART_PAGE)
	{
		DisplayGame_Playing();
		DisplayWipe();
	}
	else if (GameTransition == GAMETRANSITION_DIE_SCREEN)
	{
		if (Wipe.T < 0.5f)
		{
			DisplayGame_Playing();
		}
		else
		{
			DisplayDieScreen();
		}
		DisplayWipe();
	}
    else if (GameTransition == GAMETRANSITION_PLAY_RECORDING)
    {
		if (Wipe.T < 0.5f)
		{
			DisplayStartScreen();
		}
		else
		{
			DisplayGame_Playing();
		}
		DisplayWipe();
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Central Display and Update functions                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void Display()
{	
	ResetLighting();

	if (GameState == GAMESTATE_START_SCREEN)
	{
		DisplayStartScreen();
	}
	if (GameState == GAMESTATE_STORE_SCREEN)
	{
		DisplayStoreScreen();
	}
	else if (GameState == GAMESTATE_HELP_SCREEN)
	{
	    DisplayHelpScreen();
	}
	else if (GameState == GAMESTATE_CREDITS_SCREEN)
	{
	    DisplayCreditsScreen();
	}
	else if (GameState == GAMESTATE_LEADERBOARD_SCREEN)
	{
	    DisplayLeaderboardScreen();
	}
	else if (GameState == GAMESTATE_DIE_SCREEN)
	{
		DisplayDieScreen();
	}
    else if (GameState == GAMESTATE_WIN_SCREEN)
    {
#if 0
        DisplayWinScreen();
#endif
    }
	else if (GameState == GAMESTATE_PLAYING)
	{
		DisplayGame_Playing();
	}
	else if (GameState == GAMESTATE_TRANSITION)
	{
		DisplayGame_Transition();
	}
	else if (GameState == GAMESTATE_CHAPTER_INTRO)
	{
		DisplayGame_Playing();
	}
    
    
    DisplaySwipe();

    RenderLighting();

#ifdef PLATFORM_WINDOWS
	if (DisplayHelp)
	{
		int line = 2;
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "           Super Dust Bunny Help             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "     This game requires a Wacom tablet.      ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "  Draw on the tablet like an iPad to play.   ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Esc    - Quit game                          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F1     - Toggle help                        ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F2     - Emulate iPhone display size        ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F3     - Emulate iPad display size          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Home   - Toggle development mode            ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " \\      - Hold for slow motion              ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " ]      - Step one frame (in slow motion)    ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " V      - Summon the vacuum                  ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Shft V - Banish the vacuum                  ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Chapters:                                   ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 1-9    - Jump to chapter by number          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " PgUp   - Previous page                      ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " PgDn   - Next page                          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Ctrl+E - Edit current page                  ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
	}
	else
	{
        gxDrawString(20, gxScreenHeight - 32, 16, gxRGB32(255, 255, 255), "Press F1 for help");
	}
#endif	

    UpdateFPS();
    
//	gxDrawString(5, gxScreenHeight-16, 16, gxRGB32(255, 255, 255), "FPS: %.0f", FPS);

	//gxDrawString(msX-8, msY-8, 16, gxRGB32(255, 255, 255), "X");
}

bool Update()
{
#ifdef PLATFORM_WINDOWS
	kbUpdateKeys();
#endif
	msUpdateMouse();

#ifdef PLATFORM_WINDOWS
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Slow Motion Update                                                                    //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	// Backslash key held down causes slow motion mode.
	if (kbIsKeyDown(KB_BACKSLASH))
	{
		SlowMotionMode = true;

		bool StepOneFrame;
		if (kbIsKeyDown(KB_RBRACKET) && !kbWasKeyDown(KB_RBRACKET))
		{
			StepOneFrame = true;
		}
		else
		{
			StepOneFrame = false;
		}

		// If Return was not pressed, skip Update() for this frame.
		if (!StepOneFrame)
		{
			return true;
		}
	}
	else
	{
		SlowMotionMode = false;
	}

	// Home key causes devmode.
	if (kbIsKeyDown(KB_HOME) && !kbWasKeyDown(KB_HOME))
	{
		DevMode = !DevMode;
	}

	// Pressing escape quits the program.
	if (kbIsKeyDown(KB_ESCAPE))
	{
		return false;
	}

	// Pressing F1 triggers the help screen.
	if (kbIsKeyDown(KB_F1) && !kbWasKeyDown(KB_F1))
	{
		DisplayHelp = !DisplayHelp;
	}

	// Pressing F2-F3 emulates different screen sizes.
	// F2 - iPhone
	// F3 - iPad
	if (kbIsKeyDown(KB_F2) && !kbWasKeyDown(KB_F2))
	{
		gxEmulateDisplaySize(GXDISPLAY_IPHONE_PORTRAIT);
	}
	if (kbIsKeyDown(KB_F3) && !kbWasKeyDown(KB_F3))
	{
		gxEmulateDisplaySize(GXDISPLAY_IPAD_PORTRAIT);
	}
    
	// Ctrl+E key launches Tiled on the current page.
	if (kbIsKeyDown(KB_LCONTROL) && kbIsKeyDown(KB_E) && !kbWasKeyDown(KB_E))
	{
		char Work[1024];
		snprintf(Work, sizeof(Work), "%s\\%s.tmx", Chapters[CurrentChapter].Name, Chapter.Pages[Chapter.PageNum].Name);
		ShellExecute(NULL, NULL, "Tools\\Tiled\\Tiled.exe", Work, NULL, 0);
	}

	if (!Recorder.PlaybackActive)  // Chapter jumping currently disallowed during playback.
    {
        // PgUp and PgDown advance and retreat pages.
        if (kbIsKeyDown(KB_PRIOR) && !kbWasKeyDown(KB_PRIOR))
        {
            if (Chapter.NPages > 0)
            {
                SkipToNextPage();
            }
        }
        if (kbIsKeyDown(KB_NEXT) && !kbWasKeyDown(KB_NEXT))
        {
            if (Chapter.NPages > 0)
            {
                SkipToPreviousPage();
            }
        }

        // Number keys switch chapters
        for (int i = KB_1; i <= KB_9; i++)
        {
            if (kbIsKeyDown(i) && !kbWasKeyDown(i))
            {
                if (IsRecordingActive())
                    StopRecording(RESULT_NONE);
                CurrentChapter = i-KB_1;
                LoadCurrentChapter();
                SetGameState_Playing();
				HideChapterIntro();
            }
        }
    }
#endif
    
#ifdef PLATFORM_IPHONE_OR_MAC
    // TODO: Consider holding a button down while hitting the rocker to engage debug features.
#endif

	// Vacuum sounds always need to be updated no matter what state the game is in.
	UpdateVacuumSound();
	
	if (GameState == GAMESTATE_START_SCREEN)
	{
		UpdateStartScreen();
	}
	if (GameState == GAMESTATE_STORE_SCREEN)
	{
		UpdateStoreScreen();
	}
	else if (GameState == GAMESTATE_HELP_SCREEN)
	{
	    UpdateHelpScreen();
	}
	else if (GameState == GAMESTATE_CREDITS_SCREEN)
	{
	    UpdateCreditsScreen();
	} 
	else if (GameState == GAMESTATE_LEADERBOARD_SCREEN)
	{
	    UpdateLeaderboardScreen();
	}    
	else if (GameState == GAMESTATE_DIE_SCREEN)
	{
		UpdateDieScreen();
	}
	else if (GameState == GAMESTATE_WIN_SCREEN)
	{
#if 0
	    UpdateWinScreen();
#endif
	}
	else if (GameState == GAMESTATE_PLAYING)
	{
		UpdateGame_Playing();
	}
	else if (GameState == GAMESTATE_TRANSITION)
	{
		UpdateGame_Transition();
	}
	else if (GameState == GAMESTATE_CHAPTER_INTRO)
	{
	    UpdateChapterIntro();
	}
	
	return true;
}
