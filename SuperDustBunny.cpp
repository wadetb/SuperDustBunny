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
#include "Barrel.h"
#include "Dusty.h"
#include "Coin.h"
#include "Vacuum.h"
#include "TennisBall.h"
#include "FireWorks.h"
#include "Dust.h"
#include "Crumb.h"
#include "Gear.h"

void InitTutorial()
{
    Tutorial.State = TUTORIALSTATE_NONE;
    
    Tutorial.CoinDisplayed = false;
    Tutorial.FireworkDisplayed = false;
    Tutorial.BallDisplayed = false;
    Tutorial.BarrelDisplayed = false;
    Tutorial.WallJumpDisplayed = false;
    Tutorial.InitialDisplayed = false;
    Tutorial.JumpDisplayed = false;
    Tutorial.GearDisplayed = false;

	Tutorial.JumpInhibit = false;
}

STutorial Tutorial;

enum EGameState
{
	GAMESTATE_START_SCREEN,
	GAMESTATE_PLAYING,
	GAMESTATE_DIE_SCREEN,
	GAMESTATE_WIN_SCREEN,
	GAMESTATE_CRUMB,
};

extern EGameState GameState;
EGameState GameState = GAMESTATE_START_SCREEN;

bool TitleScreenButtonPressed = false;
bool RetryScreenButtonPressed = false;
bool NextPageButtonPressed = false;

bool DisplayHelp = false;
bool DevMode = false;
bool SlowMotionMode = false;

int BackgroundX = 0;
int BackgroundY = 0;

int SongCounter = 1;
int SongDuration = 1500;
int BackgroundMusic = 0;

void Init()
{
	// Use iPad "portrait mode" screen dimensions.
	gxInit(GXDISPLAY_IPAD_PORTRAIT);

	sxInit();

#ifdef PLATFORM_WINDOWS
	kbInit();
#endif
	
#ifdef PLATFORM_IPHONE
	msInit();
#endif	
	
	LoadAssets();
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
//                                                             Input functions
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

bool GetInput_MoveLeft()
{
#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_A);
#endif
#ifdef PLATFORM_IPHONE
	return msAccelX < -0.25f;
#endif
}

bool GetInput_MoveRight()
{
#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_D);
#endif
#ifdef PLATFORM_IPHONE
	return msAccelX > 0.25f;
#endif
}

bool GetInput_Jump()
{
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

#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
	return msButton1;
#endif	
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   LoadLevel Declaration                                                                 //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
void LoadLevel(const char* Name)
{
	Score.ScoreCounter = 0;

	ClearBarrels();
	ClearCoins();
	ClearFireWorks();
	ClearBalls();

	InitDusty();
	InitVacuum();
	InitDust();

	LoadChapter(Name);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  Tutorial State switching function declarations                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_Crumb(ETutorialState State);

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   GAMESTATE_START_SCREEN                                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_Playing();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   GAMESTATE_START_SCREEN                                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_StartScreen()
{
	GameState = GAMESTATE_START_SCREEN;
}

void DisplayGame_StartScreen()
{
	if (TitleScreenButtonPressed)
	{
		gxDrawSprite( 0, 0, &ScreenStart1Sprite );
	}
	else
	{
		gxDrawSprite( 0, 0, &ScreenStart2Sprite );
	}
}

void UpdateGame_StartScreen()
{
#ifdef PLATFORM_WINDOWS
	TitleScreenButtonPressed = kbIsKeyDown(KB_RETURN);

	// Advance to playing state when return key is released.
	if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
	{
		LoadLevel("Docs/TBook01_Chapter01_Page01.txt");
		SetGameState_Playing();
		return;
	}
#endif
#ifdef PLATFORM_IPHONE
	// TODO: iPhone needs a rectangle check.
	TitleScreenButtonPressed = msButton1;
	
	// Advance to playing state when return key is released.
	if (!msButton1 && msOldButton1)
	{
		LoadLevel("Docs/Easy.txt");
		SetGameState_Playing();
		return;
	}
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  SET GAMESTATE_DIE_SCREEN                                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_DieScreen();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   GAMESTATE_DIE_SCREEN                                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_DieScreen()
{
	GameState = GAMESTATE_DIE_SCREEN;
}

void DisplayGame_DieScreen()
{
	if (RetryScreenButtonPressed)
	{
		gxDrawSprite(0, 0, &ScreenLose1Sprite);
	}
	else
	{
		gxDrawSprite(0, 0, &ScreenLose2Sprite);
	}
}

void UpdateGame_DieScreen()
{
#ifdef PLATFORM_WINDOWS
	RetryScreenButtonPressed = kbIsKeyDown(KB_RETURN);

	// Advance to playing state when return key is released.
	if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
	{
		LoadLevel("Docs/TBook01_Chapter01_Page01.txt");
		SetGameState_Playing();
		return;
	}
#endif
#ifdef PLATFORM_IPHONE
	// TODO: iPhone needs a rectangle check.
	RetryScreenButtonPressed = msButton1;
	
	// Advance to playing state when return key is released.
	if (!msButton1 && msOldButton1)
	{
		LoadLevel("Docs/Easy.txt");
		SetGameState_Playing();
		return;
	}
#endif
}

void SkipTutorials()
{
    Tutorial.CoinDisplayed = true;
    Tutorial.FireworkDisplayed = true;
    Tutorial.BallDisplayed = true;
    Tutorial.BarrelDisplayed = true;
    Tutorial.WallJumpDisplayed = true;
    Tutorial.InitialDisplayed = true;
    Tutorial.JumpDisplayed = true;
    Tutorial.GearDisplayed = true;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  SET GAMESTATE_WIN_SCREEN                                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
void SetGameState_WinScreen();
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  GAMESTATE_WIN_SCREEN                                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_WinScreen()
{
    GameState = GAMESTATE_WIN_SCREEN;
}

void DisplayGame_WinScreen()
{
    if (NextPageButtonPressed)
    {
        gxDrawSprite( 0, 0, &ScreenWin1Sprite);
    }
    else
    {
        gxDrawSprite( 0, 0, &ScreenWin2Sprite);
    }
}

void UpdateGame_WinScreen()
{
#ifdef PLATFORM_WINDOWS
    NextPageButtonPressed = kbIsKeyDown(KB_RETURN);

    // Advance to playing state when return key is released.
    if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
    {
		LoadLevel("Docs/TBook01_Chapter01_Page01.txt");
        SetGameState_Playing();
        return;
    }
#endif
#ifdef PLATFORM_IPHONE
	// TODO: iPhone needs a rectangle check.
	NextPageButtonPressed = msButton1;
	
	// Advance to playing state when return key is released.
	if (!msButton1 && msOldButton1)
	{
		LoadLevel("Docs/TBook01_Chapter01_Page01.txt");
		SetGameState_Playing();
		return;
	}
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   GAMESTATE_PLAYING                                                                     //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_Playing()
{
	GameState = GAMESTATE_PLAYING;

	if (Tutorial.InitialDisplayed == false)
	{    
		SetGameState_Crumb(TUTORIALSTATE_INITIAL);
	}    
}

void DisplayGame_Playing()
{	
	// Calculate scrolling.
	CalculateScrollY();
	
	// Non-scrolling background Drawing
	//gxDrawSprite( BackgroundX, BackgroundY, &BackgroundCardboardSprite );
	
	// Chapter Drawing - Everything here is behind Dusty
	DisplayChapter();
	DisplayVacuum_BeforeDusty();
	DisplayBarrels_BeforeDusty();
	DisplayCoins();
    DisplayGear();
    DisplayBall();
	DisplayFireWorks();	
	
	// Dusty Drawing
	DisplayDusty();
	
	// More Chapter Drawing - Everything here is on top of Dusty
	DisplayBarrels_AfterDusty();
	DisplayDust();
	DisplayVacuum_AfterDusty();
	
    // HUD Drawing - Score, etc.
    DisplayScore();   
    	
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Debugging aids                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	float FPS = 0;
	
#ifdef PLATFORM_WINDOWS
	static DWORD LastTime = 0;
	DWORD Time = timeGetTime();
	FPS = (1000.0f / ((float)Time - (float)LastTime) + 0.5f);
	LastTime = Time;
	
	FPS = (float)(int(FPS / 10.0f + 0.5f) * 10);
#endif
#ifdef PLATFORM_IPHONE
	//Get the resolution of the iPhone timer.
	mach_timebase_info_data_t timerInfo;
	mach_timebase_info(&timerInfo);
	
	const double TIMER_RATIO = 1e-9 * ((float)timerInfo.numer / (float)timerInfo.denom);
	
	static double LastTime = mach_absolute_time() * TIMER_RATIO - 1.0f / 60.0f;
	double Time = mach_absolute_time() * TIMER_RATIO;
	FPS = 1.0f / (Time - LastTime);
	LastTime = Time;
	
	FPS = (double)(int(FPS / 10.0f + 0.5f) * 10);
#endif
	
	if (DevMode)
	{
		// Status of common variables
		gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "FPS: %.0f ( %.1f %.1f ) State: %d Col: %d%d%d%d\nLastWall: %d Direction: %d", 
			FPS, Dusty.FloatX, Dusty.FloatY, Dusty.State, Dusty.CollideWithLeftSide, Dusty.CollideWithRightSide, Dusty.CollideWithTopSide, Dusty.CollideWithBottomSide,
			Dusty.LastWall, Dusty.Direction);
	}
	else 
	{
		// FPS only
		gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "FPS: %.0f", FPS);
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
			return;
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
#endif

	UpdateDusty();

	if (Dusty.State != DUSTYSTATE_DIE)
	{
		UpdateBarrels();
		UpdateCoins();   
        UpdateBall();  
        UpdateGear();  
		UpdateFireWorks();
		UpdateScore();
	}

	UpdateDust();
    UpdateVacuum();  
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   SetGameState_Crumb Implementation                                                     //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetGameState_Crumb(ETutorialState State)
{
    Tutorial.State = State;
    
    GameState = GAMESTATE_CRUMB;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Tutorial State Display Crumb Implementation                                           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//  

void Display_Crumb()
{    
	// Calculate scrolling.
	CalculateScrollY();
	
	// Non-scrolling background Drawing
	//gxDrawSprite( BackgroundX, BackgroundY, &BackgroundCardboardSprite );
	
	// Chapter Drawing - Everything here is behind Dusty
	DisplayChapter();
	DisplayVacuum_BeforeDusty();
	DisplayBarrels_BeforeDusty();
	DisplayCoins();
    DisplayGear();
    DisplayBall();
	DisplayFireWorks();	
	
	// Dusty Drawing
	DisplayDusty();
	
	// More Chapter Drawing - Everything here is on top of Dusty
	DisplayBarrels_AfterDusty();
	DisplayDust();
	DisplayVacuum_AfterDusty();
	
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Tutorial State Display                                                                //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//  
    if (Tutorial.State == TUTORIALSTATE_INITIAL)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialInitialSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite );  
    }

    if (Tutorial.State == TUTORIALSTATE_BALL)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialBallSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite );
    }

    if (Tutorial.State == TUTORIALSTATE_BARREL)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialBarrelSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite );
    }

    if (Tutorial.State == TUTORIALSTATE_COIN)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialCoinSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite ); 
    }

    if (Tutorial.State == TUTORIALSTATE_FIREWORK)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialFireWorkSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite );
    }

    if (Tutorial.State == TUTORIALSTATE_JUMP)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialJumpSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite );
    }

    if (Tutorial.State == TUTORIALSTATE_WALLJUMP)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialWallJumpSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite );\
    }
    
    if (Tutorial.State == TUTORIALSTATE_GEAR)
    {
        gxDrawSpriteCenteredRotated (380, 600, 0, &TutorialGearSprite );  
        gxDrawSpriteCenteredRotated (700, 910, 0, &CrumbStandSprite );\
    }

#ifdef PLATFORM_WINDOWS
	gxDrawString(0, 0, 32, gxRGB32(255, 255, 255), "Press SPACE to continue ");
#endif
#ifdef PLATFORM_IPHONE
	gxDrawString(0, 0, 32, gxRGB32(255, 255, 255),  "    Tap to continue     ");
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Tutorial State Update Crumb Implementation                                            //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//  
void UpdateGame_Crumb()
{   
    if (Tutorial.State == TUTORIALSTATE_INITIAL)
    {
        Tutorial.InitialDisplayed = true;
    }

    if (Tutorial.State == TUTORIALSTATE_BALL)
    {
        Tutorial.BallDisplayed = true;
    }

    if (Tutorial.State == TUTORIALSTATE_BARREL)
    {
        Tutorial.BarrelDisplayed = true;
    }

    if (Tutorial.State == TUTORIALSTATE_COIN)
    {
        Tutorial.CoinDisplayed = true;
    }

    if (Tutorial.State == TUTORIALSTATE_FIREWORK)
    {
        Tutorial.FireworkDisplayed = true;
    }

    if (Tutorial.State == TUTORIALSTATE_JUMP)
    {
        Tutorial.JumpDisplayed = true;
    }

    if (Tutorial.State == TUTORIALSTATE_WALLJUMP)
    {
        Tutorial.WallJumpDisplayed = true;
    }
    
    if (Tutorial.State == TUTORIALSTATE_GEAR)
    {
        Tutorial.GearDisplayed = true;
    }

	// Use the jump key to skip the tutorial.  Note that GetInput_Jump is not used here because
	// I need to wait for the key to be released and pressed again.  GetInput_Jump continues to
	// return true as long as the button is held down, which would be bad.
	bool JumpPressed = false;
#ifdef PLATFORM_WINDOWS
	JumpPressed = kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
	JumpPressed = msButton1 && !msOldButton1;
#endif
	if (JumpPressed)
	{
		// Prevent this from being read by the game as a jump command until the key is released.
		Tutorial.JumpInhibit = true;

		SetGameState_Playing();
        return;
    }
    Dusty.CrumbTimer--;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Central Display and Update functions                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void Display()
{
	if (GameState == GAMESTATE_START_SCREEN)
	{
		DisplayGame_StartScreen();
	}
	else if (GameState == GAMESTATE_DIE_SCREEN)
	{
		DisplayGame_DieScreen();
	}
    else if (GameState == GAMESTATE_WIN_SCREEN)
    {
        DisplayGame_WinScreen();
    }
	else if (GameState == GAMESTATE_PLAYING)
	{
		DisplayGame_Playing();
	}
	else if (GameState == GAMESTATE_CRUMB)
	{
	    Display_Crumb();
	}

#ifdef PLATFORM_WINDOWS
	if (DisplayHelp)
	{
		int line = 2;
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "     Super Dust Bunny Keyboard Commands      ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Enter - Dismiss start / win / lose screen   ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " S     - Skip all tutorials                  ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F1    - Toggle help                         ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F2    - Emulate iPhone display size         ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " F3    - Emulate iPad display size           ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Home  - Toggle development mode             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " \\     - Hold for slow motion                ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " ]     - Step one frame (in slow motion)     ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " A     - Hop left                            ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " D     - Hop right                           ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Space - Jump                                ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " Levels:                                     ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 1     - Easy                                ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 2     - Medium                              ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 3     - Hard                                ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 4     - TBook01_Chapter01_Page01            ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 5     - TBook01_Chapter01_Page02            ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 6     - TBook01_Chapter01_Page03            ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " 0     - Test                                ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, (line++)*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
	}
	else
	{
		gxDrawString(20, gxScreenHeight - 52, 16, gxRGB32(255, 255, 255), "Press F1 for help");
	}
#endif
}

bool Update()
{    
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

#ifdef PLATFORM_WINDOWS
	kbUpdateKeys();
#endif
	
#ifdef PLATFORM_IPHONE
	msUpdateMouse();
#endif
	
#ifdef PLATFORM_WINDOWS
	// Pressing S skips all tutorials.
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

	// Number keys switch levels
	if (kbIsKeyDown(KB_1) && !kbWasKeyDown(KB_1))
	{
		LoadLevel("Docs/Easy.txt");
		SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_2) && !kbWasKeyDown(KB_2))
	{
		LoadLevel("Docs/Medium.txt");
		SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_3) && !kbWasKeyDown(KB_3))
	{
		LoadLevel("Docs/Hard.txt");
		SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_4) && !kbWasKeyDown(KB_4))
	{
	    LoadLevel("Docs/TBook01_Chapter01_Page01.txt");
	    SetGameState_Playing();
	}
    if (kbIsKeyDown(KB_5) && !kbWasKeyDown(KB_5))
	{
	    LoadLevel("Docs/TBook01_Chapter01_Page02.txt");
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_6) && !kbWasKeyDown(KB_6))
	{
	    LoadLevel("Docs/TBook01_Chapter01_Page03.txt");
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_7) && !kbWasKeyDown(KB_7))
	{
	    LoadLevel("Docs/TBook01_Chapter01_Page04.txt");
	    SetGameState_Playing();
	}
	if (kbIsKeyDown(KB_0) && !kbWasKeyDown(KB_0))
	{
		LoadLevel("Docs/Test.txt");
		SetGameState_Playing();
	}
	
#endif

	if (GameState == GAMESTATE_START_SCREEN)	
	{
		UpdateGame_StartScreen();
	}
	else if (GameState == GAMESTATE_DIE_SCREEN)
	{
		UpdateGame_DieScreen();
	}
	else if (GameState == GAMESTATE_WIN_SCREEN)
	{
	    UpdateGame_WinScreen();
	}
	else if (GameState == GAMESTATE_PLAYING)
	{
		UpdateGame_Playing();
	}
	else if (GameState == GAMESTATE_CRUMB)
	{
	    UpdateGame_Crumb();
	}
	
	return true;
}