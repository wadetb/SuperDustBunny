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
#include "Crumb.h"
#include "Gear.h"
#include "Wipe.h"
#include "Flashlight.h"
#include "Debris.h"
#include "Lives.h"
#include "Stapler.h"
#include "Recorder.h"
#include "Tutorial.h"
#include "GameScore.h"
#include "Settings.h"

#include "StartScreen.h"
#include "HelpScreen.h"
#include "CreditsScreen.h"
#include "DieScreen.h"
#include "WinScreen.h"
#include "ChapterIntro.h"


enum EGameState
{
	GAMESTATE_PLAYING,
	GAMESTATE_TRANSITION,
	GAMESTATE_START_SCREEN,
	GAMESTATE_DIE_SCREEN,
	GAMESTATE_WIN_SCREEN,
	GAMESTATE_HELP_SCREEN,
	GAMESTATE_CREDITS_SCREEN,
	GAMESTATE_TUTORIAL,
	GAMESTATE_CHAPTER_INTRO,
};

EGameState GameState = GAMESTATE_START_SCREEN;

EGameTransition GameTransition;

#define MAX_CHAPTERS 10

const char* ChapterNames[MAX_CHAPTERS] =
{
	"Chapters/01-Study",
	"Chapters/02-Fridge",
	"Chapters/Wade1",
	"Chapters/Thomas1",
	"",
	"",
	"",
	"",
	"",
	""
};

int CurrentChapter = 0;

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
	
    InitSettings();
	InitLighting();

	LoadAssets();
	
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
	printf("SuperDustBunny Error: %s\n", Work);
    DisplayAlert("SuperDustBunny Error", Work);
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


void DisplayAlert(const char* Title, const char* AlertMessage, ...)
{
	char Work[1024];
    
	va_list args;
	va_start(args, AlertMessage);
	vsnprintf(Work, sizeof(Work), AlertMessage, args);
	va_end(args);
    
#ifdef PLATFORM_IPHONE
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:Title]
                                                    message:[NSString stringWithUTF8String:Work]
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                             Input functions
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

SRemoteControl RemoteControl;


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                         Tilt controls                                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //


float AccelThreshold[3] = { 0.20f, 0.15f, 0.10f };


bool GetInput_MoveLeft()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried when a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.MoveLeft;

	if (RemoteControl.Enabled)
		return RemoteControl.MoveLeft;

#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_A);
#endif
#ifdef PLATFORM_IPHONE
	return msAccelX < -AccelThreshold[Settings.TiltSensitivity];
#endif
}

bool GetInput_MoveRight()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried while a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.MoveRight;

	if (RemoteControl.Enabled)
		return RemoteControl.MoveRight;

#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_D);
#endif
#ifdef PLATFORM_IPHONE
	return msAccelX > AccelThreshold[Settings.TiltSensitivity];
#endif
}

bool GetInput_Jump()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried while a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.Jump;

	if (RemoteControl.Enabled)
		return RemoteControl.Jump;

	// Jump inhibitor is used to make the game ignore the jump key until it's released.
	if (Tutorial.JumpInhibit)
	{
#ifdef PLATFORM_WINDOWS
		if (!kbIsKeyDown(KB_SPACE))
			Tutorial.JumpInhibit = false;
#endif
#ifdef PLATFORM_IPHONE
		if (!msButton1)
			Tutorial.JumpInhibit = false;
#endif
	}
	if (Tutorial.JumpInhibit)
		return false;

    if (Settings.ContinuousJump)
    {
#ifdef PLATFORM_WINDOWS
        return kbIsKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
        return msButton1;
#endif	
    }
    else
    {
#ifdef PLATFORM_WINDOWS
        return kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
        return msButton1 && !msOldButton1;
#endif	
    }
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                        Debug lines                                                                      //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //

#define MAX_DEBUG_LINES 100


struct SDebugLine
{
    int EndFrame;
    float X1, Y1;
    float X2, Y2;
    unsigned int Color;
};


SDebugLine DebugLines[MAX_DEBUG_LINES];
int CurFrame;

void AddDebugLine(float X1, float Y1, float X2, float Y2, unsigned int Color, float Time)
{    
    for (int i = 0; i < MAX_DEBUG_LINES; i++)
    {
        if (DebugLines[i].EndFrame <= CurFrame)
        {
            SDebugLine* DebugLine = &DebugLines[i];
            DebugLine->X1 = X1;
            DebugLine->Y1 = Y1;
            DebugLine->X2 = X2;
            DebugLine->Y2 = Y2;
            DebugLine->Color = Color;
            DebugLine->EndFrame = CurFrame + int(Time*60.0f);
            return;
        }
    }
}

void DisplayDebugLine(float X1, float Y1, float X2, float Y2, float Width, unsigned int Color)
{
    float PerpX = Y2 - Y1;
    float PerpY = -(X2 - X1);
    float L = Length(PerpX, PerpY);
    if (Length > 0)
    {
        PerpX *= Width / L;
        PerpY *= Width / L;
    }
    else
    {
        PerpX = 0;
        PerpY = 0;
    }
    
    AddLitQuad(LIGHTLIST_WIPE, &WhiteSprite, Color,
               X1 - PerpX, Y1 - PerpY, 0, 0,
               X1 + PerpX, Y1 + PerpY, 0, 0,
               X2 + PerpX, Y2 + PerpY, 0, 0,
               X2 - PerpX, Y2 - PerpY, 0, 0);    
}

void DisplayDebug()
{
    for (int i = 0; i < MAX_DEBUG_LINES; i++)
    {
        if (DebugLines[i].EndFrame > CurFrame)
        {
            SDebugLine* DebugLine = &DebugLines[i];

            float Width = 4.0f;
            
            DisplayDebugLine(DebugLine->X1, DebugLine->Y1, DebugLine->X2, DebugLine->Y2, Width, DebugLine->Color);
        }
    }
    
    CurFrame++;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                        Swipe controls                                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //

#define MAX_SWIPE_POINTS 128


struct SSwipe
{
    bool Active;
    bool Valid;
    int Count;
};

struct SSwipePoint
{
    float X, Y;
};


SSwipe Swipe;

SSwipePoint SwipePoints[MAX_SWIPE_POINTS];


void UpdateSwipe()
{
    if (Settings.ControlStyle != CONTROL_SWIPE)
        return;

    // Consume old points.
    if (Swipe.Valid)
    {
        for (int i = 0; i < Swipe.Count-1; i++)
            SwipePoints[i] = SwipePoints[i+1];
        
        Swipe.Count--;
        
        if (Swipe.Count <= 1)
        {
            Swipe.Count = 0;
            Swipe.Valid = false;
        }
    }
    
    // Add new ones.
    if (!Swipe.Active)
    {
        if (msButton1)
        {
            Swipe.Active = true;
        }
    }
    else
    {
        if (Swipe.Count < MAX_SWIPE_POINTS)
        {
            if (Swipe.Count > 0)
                AddDebugLine(SwipePoints[Swipe.Count-1].X, SwipePoints[Swipe.Count-1].Y, msX, msY, gxRGBA32(192, 128, 128, 255), 1.0f/60.0f);
        
            SwipePoints[Swipe.Count].X = msX;
            SwipePoints[Swipe.Count].Y = msY;
            
            Swipe.Count++;
            
            if (!Swipe.Valid && Swipe.Count >= 3)
                Swipe.Valid = true;
        }
        
        if (!msButton1)
        {
            Swipe.Active = false;
        }
    }
}

void GetInput_NextSwipeDir(float* dX, float* dY)
{
    if (Settings.ControlStyle != CONTROL_SWIPE)
        ReportError("Swipe controls queried while a different control style was in use.");
    
    if (!Swipe.Valid)
    {
        *dX = 0;
        *dY = 0;
        return;
    }
    
    *dX = SwipePoints[1].X - SwipePoints[0].X;
    *dY = SwipePoints[1].Y - SwipePoints[0].Y;
}

bool GetInput_CheckSwipeDir(float Angle, float Range)
{
    if (Settings.ControlStyle != CONTROL_SWIPE)
        ReportError("Swipe controls queried while a different control style was in use.");
    
    if (!Swipe.Valid)
        return false;
    
    float dX, dY;
    GetInput_NextSwipeDir(&dX, &dY);
    
    float L = sqrtf(dX*dX + dY*dY);
    if (L < 10.0f)
        return false;
    
    dX /= L;
    dY /= L;
    
    float aX = cosf(DegreesToRadians(Angle));
    float aY = -sinf(DegreesToRadians(Angle));
    
    float Dot = aX*dX + aY*dY;
    float cR = cosf(Range);
    
    return Dot >= cR;
}

void GetInput_UseSwipe()
{
    if (Settings.ControlStyle != CONTROL_SWIPE)
        ReportError("Swipe controls queried while a different control style was in use.");
}

void DisplaySwipe()
{
    unsigned int color = Swipe.Valid ? gxRGBA32(192, 128, 128, 255) : gxRGBA32(128, 192, 128, 255);
    
    for (int i = 1; i < Swipe.Count; i++)
        DisplayDebugLine(SwipePoints[i-1].X, SwipePoints[i-1].Y, SwipePoints[i].X, SwipePoints[i].Y, color, 1.0f/60.0f);
    
	//gxDrawString(5, 37, 16, gxRGB32(255, 255, 255), "Active:%d Valid:%d Count:%d", Swipe.Active, Swipe.Valid, Swipe.Count);
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
	ResetScore();

	ClearChapter();

	if (CurrentChapter < 0 || CurrentChapter >= MAX_CHAPTERS)
		CurrentChapter = 0;

	LoadChapter(ChapterNames[CurrentChapter]);
}

void AdvanceToNextPage()
{
	if (Chapter.PageNum < Chapter.NPages-1)
	{
		StopRecording(RESULT_NEXT_PAGE);
		SetGameState_Transition(GAMETRANSITION_NEXT_PAGE);
	}
	else
	{
		StopRecording(RESULT_CHAPTER_END);
		SetGameState_WinScreen();
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   SetGameState functions for screens                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_StartScreen()
{
	GameState = GAMESTATE_START_SCREEN;

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
}

void SetGameState_WinScreen()
{
	GameState = GAMESTATE_WIN_SCREEN;

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
    
	InitTutorial();
	InitScore();

	// Remove this to re-enable the tutorials when we are closer to a release.
	// For now they just slow down development.
	SkipTutorials();

	if (Tutorial.InitialDisplayed == false)
	{    
		SetGameState_Tutorial(TUTORIALSTATE_INITIAL);
	}    
}

void DisplayPauseScreen()
{
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonHomeSprite, 192, 500, 1.0f, 1.0f);
    
    if (GameMuted)
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonUnmuteSprite, 768-192, 500, 1.0f, 1.0f);
    else
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonMuteSprite, 768-192, 500, 1.0f, 1.0f);        
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
			StopRecording(RESULT_RESTART_PAGE);
            SetGameState_StartScreen();
        }
    }
}

void DisplayGame_Playing()
{	
	// Calculate scrolling.
	CalculateScrollY();

	// Repeating background.
	int y = 0;
	while (y < Chapter.PageHeight*64)
	{
		AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundPaperSprite, 0, (float)y+ScrollY);
		y += BackgroundPaperSprite.height;
	}

	// Chapter Drawing - Everything here is behind Dusty
	DisplayChapter();
	DisplayBarrels_BeforeDusty();
	DisplayCoins();
    DisplayGear();
    DisplayBall();
	DisplayFireWorks();
	DisplayDebris();
	DisplayStaplers();

	// Dusty Drawing
	DisplayDusty();
	
	// More Chapter Drawing - Everything here is on top of Dusty
	DisplayFans();
	DisplayBarrels_AfterDusty();
	DisplayDust();
	DisplayVacuum();
	DisplayLives();
	
    // HUD Drawing - Score, etc.
    DisplayScore();
    
    DisplayDebug();
    
	// Display buttons
    if (GamePause)
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonPlaySprite, gxScreenWidth/2, 64, 1.0f, 1.0f);
        DisplayPauseScreen();
    }
    else
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonPauseSprite, gxScreenWidth/2, 64, 1.0f, 1.0f);
        
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
            GamePause = !GamePause;
    }
#endif
    
    // TODO: GamePause should actually stop the update and display loop, to reduce battery use.
	if (GamePause)
    {
        UpdatePauseScreen();
        return;
    }
    
    UpdateDusty();
        
    if (Dusty.State != DUSTYSTATE_DIE)
    {
        UpdateFans();
        UpdateBarrels();
        UpdateCoins();   
        UpdateBall();  
        UpdateGear();  
        UpdateFireWorks();
        UpdateFlashlight();
        UpdateScore();		 
        UpdateLives();
        UpdateStaplers();
    }

    UpdateDust();
    UpdateDebris();
    UpdateVacuum(); 
}

void SetGameState_Tutorial(int State)
{
    Tutorial.State = (ETutorialState)State;
    
    GameState = GAMESTATE_TUTORIAL;
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
	else if (GameState == GAMESTATE_TUTORIAL)
	{
		DisplayGame_Playing();
	    DisplayTutorial();
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
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " S      - Skip all tutorials                 ");
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
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 1      - Wade1                              ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 2      - Thomas1                            ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " PgUp   - Previous page                      ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " PgDn   - Next page                          ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Ctrl+E - Edit current page                  ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
	}
	else
	{
		if (GameState != GAMESTATE_TUTORIAL)
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

	// Recorder must be updated immediately after the devices are queried, before UpdateSwipe or any GetInput_Xyz calls.
	UpdateRecorder();

    // UpdatSwipe must be called before any GetInput_Xyz calls.
    UpdateSwipe();
    
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

	// Pressing S skips all tutorials.
	// RECORDER FIXME
    if (kbIsKeyDown(KB_S))
    {
		SkipTutorials();
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

	// PgUp and PgDown advance and retreat pages.
	// RECORDER FIXME
	if (kbIsKeyDown(KB_PRIOR) && !kbWasKeyDown(KB_PRIOR))
	{
		if (Chapter.NPages > 0)
		{
			StopRecording(RESULT_NONE);
			SetCurrentPage((Chapter.PageNum+Chapter.NPages-1) % Chapter.NPages);
			StartRecording();
		}
	}
	if (kbIsKeyDown(KB_NEXT) && !kbWasKeyDown(KB_NEXT))
	{
		if (Chapter.NPages > 0)
		{
			StopRecording(RESULT_NONE);
			SetCurrentPage((Chapter.PageNum+1) % Chapter.NPages);
			StartRecording();
		}
	}

	// Ctrl+E key launches Tiled on the current page.
	if (kbIsKeyDown(KB_LCONTROL) && kbIsKeyDown(KB_E) && !kbWasKeyDown(KB_E))
	{
		char Work[1024];
		snprintf(Work, sizeof(Work), "%s%s.tmx", ChapterNames[CurrentChapter], Chapter.Pages[Chapter.PageNum].Name);
		ShellExecute(NULL, NULL, "Tools\\Tiled\\Tiled.exe", Work, NULL, 0);
	}

	// Number keys switch chapters
	// RECORDER FIXME
	if (kbIsKeyDown(KB_1) && !kbWasKeyDown(KB_1))
	{
		CurrentChapter = 0;
		LoadCurrentChapter();
		SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_2) && !kbWasKeyDown(KB_2))
	{
		CurrentChapter = 1;
		LoadCurrentChapter();
		SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_3) && !kbWasKeyDown(KB_3))
	{
		CurrentChapter = 2;
		LoadCurrentChapter();
		SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_4) && !kbWasKeyDown(KB_4))
	{
		CurrentChapter = 3;
		LoadCurrentChapter();
	    SetGameState_Playing();
	}
    if (kbIsKeyDown(KB_5) && !kbWasKeyDown(KB_5))
	{
		CurrentChapter = 4;
		LoadCurrentChapter();
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_6) && !kbWasKeyDown(KB_6))
	{
		CurrentChapter = 5;
		LoadCurrentChapter();
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_7) && !kbWasKeyDown(KB_7))
	{
		CurrentChapter = 6;
		LoadCurrentChapter();
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_8) && !kbWasKeyDown(KB_8))
	{
		CurrentChapter = 7;
		LoadCurrentChapter();
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_9) && !kbWasKeyDown(KB_9))
	{
		CurrentChapter = 8;
		LoadCurrentChapter();
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_0) && !kbWasKeyDown(KB_0))
	{
		CurrentChapter = 9;
		LoadCurrentChapter();
		SetGameState_Playing();
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
	else if (GameState == GAMESTATE_TUTORIAL)
	{
	    UpdateTutorial();
	}
	else if (GameState == GAMESTATE_CHAPTER_INTRO)
	{
	    UpdateChapterIntro();
	}
	
	return true;
}
