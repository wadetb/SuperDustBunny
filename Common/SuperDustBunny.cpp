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

#include "StartScreen.h"
#include "HelpScreen.h"
#include "CreditsScreen.h"
#include "DieScreen.h"
#include "WinScreen.h"
#include "ChapterIntro.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif

enum EGameState
{
	GAMESTATE_PLAYING,
	GAMESTATE_TRANSITION,
	GAMESTATE_START_SCREEN,
	GAMESTATE_DIE_SCREEN,
	GAMESTATE_WIN_SCREEN,
	GAMESTATE_HELP_SCREEN,
	GAMESTATE_CREDITS_SCREEN,
	GAMESTATE_CHAPTER_INTRO,
};

EGameState GameState = GAMESTATE_START_SCREEN;

EGameTransition GameTransition;

bool NextPageButtonPressed = false;

bool DisplayHelp = false;
bool DevMode = false;
bool SlowMotionMode = false;
bool ChapterIntroDisplayed = false;
bool GamePause = false;
bool GameMuted = false;

int BackgroundX = 0;
int BackgroundY = 0;

int SongCounter = 1;
int SongDuration = 1500;
int BackgroundMusic = 1;

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
	// Default to iPhone size.
	gxEmulateDisplaySize(GXDISPLAY_IPHONE_PORTRAIT);
#endif
    
	sxInit();

#ifdef PLATFORM_WINDOWS
	kbInit();
#endif
	
#ifdef PLATFORM_IPHONE
	msInit();
#endif	
	
    LoadBundleAssetList();

    InitSettings();
	InitLighting();

    SetGameState_StartScreen();
}

void Exit()
{
#ifdef PLATFORM_WINDOWS
	kbDeinit();
#endif

#ifdef PLATFORM_IPHONE
	msDeinit();
#endif	
	
	gxDeinit();
	
	sxDeinit();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Error handling (platform specific)                                                 //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

#define MAX_ERROR_CONTEXT 10

struct SErrorContext
{
	char Text[1024];
};

int NErrorContexts = 0;
SErrorContext ErrorContexts[MAX_ERROR_CONTEXT];

void PushErrorContext(const char* ErrorContext, ...)
{
	if (NErrorContexts >= MAX_ERROR_CONTEXT)
		ReportError("Exceeded the maximum of %d error contexts.", MAX_ERROR_CONTEXT);

	SErrorContext* Context = &ErrorContexts[NErrorContexts];

	va_list args;
	va_start(args, ErrorContext);
	vsnprintf(Context->Text, sizeof(Context->Text), ErrorContext, args);
	va_end(args);

	NErrorContexts++;
}

void PopErrorContext()
{
	if (NErrorContexts <= 0)
		ReportError("Error context underflow.");

	NErrorContexts--;
}

void ReportError(const char* ErrorMessage, ...)
{
	char TotalMessage[8192];
	int MessageSize = 0;

	// Append the error contexts.
	strcpy(TotalMessage, "");
	for (int i = 0; i < NErrorContexts; i++)
	{
		MessageSize += snprintf(TotalMessage + MessageSize, sizeof(TotalMessage) - MessageSize, "%s", ErrorContexts[i].Text);
	}

	// Append the error message.
	char Work[1024];

	va_list args;
	va_start(args, ErrorMessage);
	vsnprintf(Work, sizeof(Work), ErrorMessage, args);
	va_end(args);

	snprintf(TotalMessage + MessageSize, sizeof(TotalMessage) - MessageSize, "%s", Work);

#ifdef PLATFORM_WINDOWS
	MessageBox(NULL, TotalMessage, "SuperDustBunny Error", MB_OK | MB_ICONSTOP);
	exit(1);
#endif

#ifdef PLATFORM_IPHONE
	printf("SuperDustBunny Error: %s\n", TotalMessage);
    
    theViewController.paused = TRUE;

    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"SuperDustBunny Error"
                                                    message:[NSString stringWithUTF8String:TotalMessage]
                                                   delegate:theViewController
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
    
    while (theViewController.paused)
    {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
    
    exit(255);
#endif
}

void LogMessage(const char* LogMessage, ...)
{
#ifdef PLATFORM_IPHONE
	va_list args;
	va_start(args, LogMessage);
	vprintf(LogMessage, args);
	va_end(args);
#endif
}

double GetCurrentTime()
{
#ifdef PLATFORM_IPHONE
	mach_timebase_info_data_t timerInfo;
	mach_timebase_info(&timerInfo);
	
	const double TIMER_RATIO = ((double)timerInfo.numer / (double)timerInfo.denom);

	return (double)mach_absolute_time() * TIMER_RATIO / 1000000000.0;
#endif

#ifdef PLATFORM_WINDOWS
    return 0;
#endif
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Frames per second tracking                                                            //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //

void UpdateFPS()
{
#ifdef PLATFORM_WINDOWS
	static DWORD LastTime = 0;
	DWORD Time = timeGetTime();
	FPS = (1000.0f / ((float)Time - (float)LastTime) + 0.5f);
	LastTime = Time;
#endif
#ifdef PLATFORM_IPHONE
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

	LoadChapter(Chapters[CurrentChapter].Name);

    LoadChapterScores(Chapters[CurrentChapter].Name);
}

void AdvanceToNextPage()
{
    RecordPageScore(Chapter.PageNum);

	if (Chapter.PageNum < Chapter.NPages-1)
	{
        if (IsRecordingActive())
    		StopRecording(RESULT_NEXT_PAGE);
        
		SetGameState_Transition(GAMETRANSITION_NEXT_PAGE);
	}
	else
	{
        if (IsRecordingActive())
    		StopRecording(RESULT_CHAPTER_END);
        
        SaveChapterScores(Chapters[CurrentChapter].Name);
        SaveChapterUnlocks();
        
		SetGameState_WinScreen();
	}
}

void SkipToNextPage()
{
    if (IsRecordingActive())
        StopRecording(RESULT_NONE);
    SetCurrentPage((Chapter.PageNum+1) % Chapter.NPages);
    StartRecording();
}

void SkipToPreviousPage()
{
    if (IsRecordingActive())
        StopRecording(RESULT_NONE);
    SetCurrentPage((Chapter.PageNum+Chapter.NPages-1) % Chapter.NPages);
    StartRecording();    
}

void LoadRecordingChapterAndPage()
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

    LitSceneZoom = 1.0f;

#ifdef PLATFORM_IPHONE
    if (Settings.LiveAssets)
    {
        UpdateLiveAssetCache();
        LoadAssets();
        AssetsLoaded = true;
    }
#endif
    
    if (!AssetsLoaded)
    {
        LoadAssets();
        AssetsLoaded = true;
    }

    ClearChapterList();
    LoadChapterList();
    LoadChapterUnlocks();
    
    // Advance to first unlocked chapter.
    if (CurrentChapter < 0)
    {
        CurrentChapter = 0;
        while (Chapters[CurrentChapter].Unlocked)
            CurrentChapter++;
        if (CurrentChapter > 0) 
            CurrentChapter--;
    }

	InitStartScreen();
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

	InitWinScreen();
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

void DisplayPauseScreen()
{
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonHomeSprite, 192, 500, 1.0f, 1.0f);
    
    if (GameMuted)
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonUnmuteSprite, 768-192, 500, 1.0f, 1.0f);
    else
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonMuteSprite, 768-192, 500, 1.0f, 1.0f);        

    if (Settings.ChapterSkip)
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonFastForwardSprite, 192+16, 850, -2.0f, 2.0f);
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonFastForwardSprite, 768-192-16, 850, 2.0f, 1.0f);        
    }
}

void UpdatePauseScreen()
{
    if (msButton1 && !msOldButton1)
    {
        if (msX > 384+64 && msY >= 300 && msY <= 600)
        {
            GameMuted = !GameMuted;
        }

        if (msX < 384-64 && msY >= 300 && msY <= 600)
        {
            GamePause = false;
            if (IsRecordingActive())
			    StopRecording(RESULT_RESTART_PAGE);
            SetGameState_StartScreen();
        }

        if (Settings.ChapterSkip)
        {
            if (msX < 384-64 && msY >= 700 && msY <= 1000)
            {
                SkipToPreviousPage();
            }
            if (msX > 384+64 && msY >= 700 && msY <= 1000)
            {
                SkipToNextPage();
            }
        }
    }
}

void DisplayBackground()
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
            x += BackgroundSprite->width;
        }
        
		y += BackgroundSprite->height;
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
	
    // HUD Drawing - Score, etc.
    DisplayScore();
    
    DisplayDebug();
    
	// Display buttons
    if (GamePause)
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonPlaySprite, (float)gxScreenWidth/2, 64, 1.0f, 1.0f);
        DisplayPauseScreen();
    }
    else
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonPauseSprite, (float)gxScreenWidth/2, 64, 1.0f, 1.0f);
        
	// Lighting effects.
	DisplayFlashlight();

	// Overlay
	DisplayChapterIntro();
      	
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Debugging aids                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	if (DevMode)
	{
		// Status of common variables
		gxDrawString(5, 5+16, 16, gxRGB32(255, 255, 255), "( %.1f %.1f ) ( %.1f %.1f ) %s\n State: %d Col: %d%d%d%d Direction: %d Material: %d", 
			Dusty.FloatX, Dusty.FloatY, Dusty.FloatVelocityX, Dusty.FloatVelocityY, Chapter.Pages[Chapter.PageNum].Name,
			Dusty.State, Dusty.CollideWithLeftSide, Dusty.CollideWithRightSide, Dusty.CollideWithTopSide, Dusty.CollideWithBottomSide, Dusty.Direction, Dusty.CollideMaterial);
		
#ifdef PLATFORM_IPHONE
		// Draw the accelerometer data directly.
		gxDrawRectangleFilled(768/2, 1024-32, msAccelX*300, 16, 0xff00ff00);
#endif
	}

	// Indicator for when slow motion is activated.
	if (SlowMotionMode)
	{
		gxDrawString(gxScreenWidth-101, 5, 16, gxRGB32(255, 255, 0), "[SLOW]");
	}
}

void UpdateGame_Playing()
{ 
#ifdef PLATFORM_WINDOWS
    //Pause Update
    if (kbIsKeyDown(KB_P) && !kbWasKeyDown(KB_P))
    {
        GamePause = !GamePause;
    }
#endif
#ifdef PLATFORM_IPHONE
    if (msButton1 && !msOldButton1)
    {
        if (msX >= 384-150 && msX <= 384+150 && msY < 200)
        {
            GamePause = !GamePause;
        }
    }
#endif
    
    // TODO: GamePause should actually stop the update and display loop, to reduce battery use.
	if (GamePause)
    {
        UpdatePauseScreen();
        return;
    }

    if (Recorder.RecordingActive && Settings.ControlStyle == CONTROL_TILT)
        RecordTiltEvents();
    
    UpdateRecorder();
    
    UpdateDusty();
        
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
        UpdateScore();		 
        UpdateLives();
        UpdateStaplers();
        UpdatePowerUp();
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

void UpdateGame_Transition()
{
	if (GameTransition == GAMETRANSITION_FIRST_PAGE)
	{
		UpdateWipe();

		if (Wipe.Middle)
		{
			LoadCurrentChapter();
			InitChapterIntro();
            Dusty.Lives = 3;            
			Wipe.Middle = false;
		}

		if (Wipe.Finished)
		{
			SetGameState_ChapterIntro();
			StartRecording();
		}
	}
	else if (GameTransition == GAMETRANSITION_NEXT_PAGE)
	{
		UpdateWipe();

		if (Wipe.Middle)
		{
			SetCurrentPage(Chapter.PageNum+1);
			Wipe.Middle = false;
		}

		if (Wipe.Finished)
		{
			SetGameState_Playing();
			StartRecording();
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
			StartRecording();
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

void DisplayGame_Transition()
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
	else if (GameState == GAMESTATE_HELP_SCREEN)
	{
	    DisplayHelpScreen();
	}
	else if (GameState == GAMESTATE_CREDITS_SCREEN)
	{
	    DisplayCreditsScreen();
	}
	else if (GameState == GAMESTATE_DIE_SCREEN)
	{
		DisplayDieScreen();
	}
    else if (GameState == GAMESTATE_WIN_SCREEN)
    {
        DisplayWinScreen();
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
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "     Super Dust Bunny Keyboard Commands      ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Enter  - Dismiss start / win / lose screen  ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F1     - Toggle help                        ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F2     - Emulate iPhone display size        ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F3     - Emulate iPad display size          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Home   - Toggle development mode            ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " \\      - Hold for slow motion               ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " ]      - Step one frame (in slow motion)    ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " A      - Hop left                           ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " D      - Hop right                          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Space  - Jump                               ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Chapters:                                   ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 1-9    - Jump to chapter by number          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " PgUp   - Previous page                      ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " PgDn   - Next page                          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Ctrl+E - Edit current page                  ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
	}
	else
	{
        gxDrawString(20, gxScreenHeight - 32, 16, gxRGB32(255, 255, 255), "Press F1 for help");
	}
#endif	

    UpdateFPS();
    
	gxDrawString(5, gxScreenHeight-16, 16, gxRGB32(255, 255, 255), "FPS: %.0f", FPS);

	//gxDrawString(msX-8, msY-8, 16, gxRGB32(255, 255, 255), "X");
}

bool Update()
{
	/*
	//Background Music
    if (BackgroundMusic == 1)
    {	        
        if (SongDuration == 0 && SongCounter == 1)
        {
            SongDuration = 2000;
            SongCounter = 2;   
        }

        if (SongDuration == 0 && SongCounter == 2)
        {
            SongDuration = 1500;
            SongCounter = 1;
        } 
        
        if (SongDuration == 1500 && SongCounter == 1)
        {   
            sxPlaySound(&Song1Sound);
        }
        
        if (SongDuration == 2000 && SongCounter == 2)
        {
            sxPlaySound(&Song3Sound);
        }
        
        if (SongDuration >= 1)
        {
            SongDuration -= 1;     
        }
    } 
*/

#ifdef PLATFORM_WINDOWS
	kbUpdateKeys();
#endif
	
#ifdef PLATFORM_IPHONE
	msUpdateMouse();
#endif

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
		snprintf(Work, sizeof(Work), "%s%s.tmx", Chapters[CurrentChapter].Name, Chapter.Pages[Chapter.PageNum].Name);
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
                CurrentChapter = i-1;
                LoadCurrentChapter();
                SetGameState_Playing();
            }
        }
    }
#endif
    
#ifdef PLATFORM_IPHONE
    // TODO: Consider holding a button down while hitting the rocker to engage debug features.
#endif

	// Vacuum sounds always need to be updated no matter what state the game is in.
	UpdateVacuumSound();
	
	if (GameState == GAMESTATE_START_SCREEN)	
	{
		UpdateStartScreen();
	}
	else if (GameState == GAMESTATE_HELP_SCREEN)
	{
	    UpdateHelpScreen();
	}
	else if (GameState == GAMESTATE_CREDITS_SCREEN)
	{
	    UpdateCreditsScreen();
	}    
	else if (GameState == GAMESTATE_DIE_SCREEN)
	{
		UpdateDieScreen();
	}
	else if (GameState == GAMESTATE_WIN_SCREEN)
	{
	    UpdateWinScreen();
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
