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

enum EGameState
{
	GAMESTATE_START_SCREEN,
	GAMESTATE_PLAYING,
	GAMESTATE_DIE_SCREEN,
	GAMESTATE_WIN_SCREEN,
};

EGameState GameState = GAMESTATE_START_SCREEN;

bool TitleScreenButtonPressed = false;
bool RetryScreenButtonPressed = false;
bool NextPageButtonPressed = false;

bool DisplayHelp = false;
bool DevMode = false;
bool SlowMotionMode = false;

int BackgroundX = 0;
int BackgroundY = 0;

gxSprite DustyHop01;
gxSprite DustyHop02;
gxSprite DustyHop03;
gxSprite DustyHop04;
gxSprite DustyHop05;
gxSprite LeftFaceStanding01;
gxSprite LeftFaceStanding02;
gxSprite RightFaceStanding01;
gxSprite RightFaceStanding02;
gxSprite LeftFaceWallJump01;
gxSprite RightFaceWallJump01;
gxSprite DustyDeath01;
gxSprite DustyHopLeft01;
gxSprite DustyHopLeft02;
gxSprite DustyHopLeft03;
gxSprite DustyHopLeft04;
gxSprite DustyHopLeft05;
gxSprite DustyHopLeft06;
gxSprite DustyDie;

gxSprite WoodBox_Platform01;
gxSprite Background01;
gxSprite UnknownBlock;
gxSprite BarrelSprite;
gxSprite VacuumSprite;
gxSprite VacuumFrontSprite;
gxSprite ArrowSprite;
gxSprite DustMoteSprite;

gxSprite Coin01;
gxSprite Coin02;
gxSprite Coin03;
gxSprite Coin04;
gxSprite Coin05;
gxSprite Coin06;
gxSprite Coin07;
gxSprite Coin08;
gxSprite Coin09;
gxSprite Coin010;
gxSprite Coin011;
gxSprite Coin012;

gxSprite Ball01;
gxSprite Ball02;
gxSprite Ball03;
gxSprite Ball04;

gxSprite FireWork01;
gxSprite FireWork02;

gxSprite StartScreen0;
gxSprite StartScreen1;
gxSprite DieScreen0;
gxSprite DieScreen1;
gxSprite WinScreen0;
gxSprite WinScreen1;
gxSprite Score01;
gxSprite Gum01;

sxSound DustyToJump;
sxSound DustyJumps;
sxSound WallJump;
sxSound LaunchSound01;

sxSound VacuumSound;
sxSound Clogged;
sxSound Clang01;
sxSound GumStick01;

sxSound BackgroundSong01;
sxSound BackgroundSong02;
sxSound BackgroundSong03;

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
	
	gxLoadSprite("Data/Hop01Resize.png", &DustyHop01);
	gxLoadSprite("Data/Hop11Resize.png", &DustyHop02);	
	gxLoadSprite("Data/Hop05Resize.png", &DustyHop03);
	gxLoadSprite("Data/Hop06Resize.png", &DustyHop04);
	gxLoadSprite("Data/Hop09Resize.png", &DustyHop05);	
	gxLoadSprite("Data/HopLeft01Resize.png", &LeftFaceStanding01);
	gxLoadSprite("Data/HopLeft01Resize.png", &LeftFaceStanding02);
	gxLoadSprite("Data/HopLeft01Resize.png",&DustyHopLeft01);
	gxLoadSprite("Data/HopLeft11Resize.png",&DustyHopLeft02);
	gxLoadSprite("Data/HopLeft05Resize.png",&DustyHopLeft03);
	gxLoadSprite("Data/HopLeft06Resize.png",&DustyHopLeft04);
	gxLoadSprite("Data/HopLeft09Resize.png",&DustyHopLeft06);
    gxLoadSprite("Data/DustyLeftWallJumpResize.png", &LeftFaceWallJump01);
	gxLoadSprite("Data/DustyRightWallJumpResize.png", &RightFaceWallJump01);
	gxLoadSprite("Data/DustyDieResize.png", &DustyDie);
    gxLoadSprite("Data/WoodPlatformThin.png", &WoodBox_Platform01);
    gxLoadSprite("Data/LargeBackground.png", &Background01);
	gxLoadSprite("Data/wtf.png", &UnknownBlock);
	gxLoadSprite("Data/can.png", &BarrelSprite);
	gxLoadSprite("Data/start-screen-vert2.png", &StartScreen0);
    gxLoadSprite("Data/start-screen-vert.png", &StartScreen1);
    gxLoadSprite("Data/DustyLose01.png", &DieScreen0);
    gxLoadSprite("Data/DustyLose02.png", &DieScreen1);
    gxLoadSprite("Data/DustyWin01.png", &WinScreen0);
    gxLoadSprite("Data/DustyWin02.png", &WinScreen1);
	
	gxLoadSprite("Data/vacuum.png", &VacuumSprite);
	gxLoadSprite("Data/vacuum-front.png", &VacuumFrontSprite);
	gxLoadSprite("Data/wind-arrow.png", &ArrowSprite);
	gxLoadSprite("Data/dust-mote.png", &DustMoteSprite);

    gxLoadSprite("Data/coin0001.png", &Coin01);
    gxLoadSprite("Data/coin0002.png", &Coin02);
    gxLoadSprite("Data/coin0003.png", &Coin03);
    gxLoadSprite("Data/coin0004.png", &Coin04);
    gxLoadSprite("Data/coin0005.png", &Coin05);
    gxLoadSprite("Data/coin0006.png", &Coin06);
    gxLoadSprite("Data/coin0007.png", &Coin07);
    gxLoadSprite("Data/coin0008.png", &Coin08);
    gxLoadSprite("Data/Coin09.png", &Coin09);
    gxLoadSprite("Data/Coin10.png", &Coin010);
    gxLoadSprite("Data/Coin11.png", &Coin011);
    gxLoadSprite("Data/Coin12.png", &Coin012);

    gxLoadSprite("Data/TennisBallRollLeft01.png", &Ball01);
    gxLoadSprite("Data/TennisBallRollLeft02.png", &Ball02);
    gxLoadSprite("Data/TennisBallRollLeft03.png", &Ball03);
	gxLoadSprite("Data/TennisBallRollLeft04.png", &Ball04);

	gxLoadSprite("Data/FireWork01.png", &FireWork01);
	gxLoadSprite("Data/FireWork02.png", &FireWork02);
	
	gxLoadSprite("Data/Score01.png", &Score01);
	gxLoadSprite("Data/Gum01.png", &Gum01);

    sxLoadWav ("Data/yaahooo.wav", &DustyToJump);
    sxLoadWav ("Data/yaahooo.wav", &DustyJumps);
    sxLoadWav ("Data/cork_pop.wav", &WallJump);
    sxLoadWav ("Data/cork_pop.wav", &LaunchSound01);
    sxLoadWav ("Data/Song1.wav", &BackgroundSong01);
    sxLoadWav ("Data/Song2.wav", &BackgroundSong02);
    sxLoadWav ("Data/Song3.wav", &BackgroundSong03);
	sxLoadWav ("Data/vacuum_cleaner_3.wav", &VacuumSound);
	sxLoadWav ("Data/clogged.wav", &Clogged);
	sxLoadWav ("Data/clang.wav", &Clang01);
	sxLoadWav ("Data/GumSquish.wav", &GumStick01);
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
#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
	return msButton1;
#endif	
}

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
		gxDrawSprite( 0, 0, &StartScreen0 );
	}
	else
	{
		gxDrawSprite( 0, 0, &StartScreen1 );
	}
}

void UpdateGame_StartScreen()
{
#ifdef PLATFORM_WINDOWS
	TitleScreenButtonPressed = kbIsKeyDown(KB_RETURN);

	// Advance to playing state when return key is released.
	if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
	{
		SetGameState_Playing();
		return;
	}
#endif
#ifdef PLATFORM_IPHONE
	// TODO: iPhone needs a rectangle check.
	TitleScreenButtonPressed = msButton1;
	
	// Advance to playing state when return key is released.
	if (!msButton1 && !msOldButton1)
	{
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
		gxDrawSprite(0, 0, &DieScreen1);
	}
	else
	{
		gxDrawSprite(0, 0, &DieScreen0);
	}
}

void UpdateGame_DieScreen()
{
#ifdef PLATFORM_WINDOWS
	RetryScreenButtonPressed = kbIsKeyDown(KB_RETURN);

	// Advance to playing state when return key is released.
	if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
	{
		SetGameState_Playing();
		return;
	}
#endif
#ifdef PLATFORM_IPHONE
	// TODO: iPhone needs a rectangle check.
	RetryScreenButtonPressed = msButton1;
	
	// Advance to playing state when return key is released.
	if (!msButton1 && !msOldButton1)
	{
		SetGameState_Playing();
		return;
	}
#endif
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
        gxDrawSprite( 0, 0, &WinScreen1);
    }
    else
    {
        gxDrawSprite( 0, 0, &WinScreen0);
    }
}

void UpdateGame_WinScreen()
{
#ifdef PLATFORM_WINDOWS
    NextPageButtonPressed = kbIsKeyDown(KB_RETURN);

    // Advance to playing state when return key is released.
    if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
    {
        Score.ScoreCounter = 0;
        SetGameState_Playing();
        return;
    }
#endif
#ifdef PLATFORM_IPHONE
	// TODO: iPhone needs a rectangle check.
	NextPageButtonPressed = msButton1;
	
	// Advance to playing state when return key is released.
	if (!msButton1 && !msOldButton1)
	{
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
	InitDusty();
	InitVacuum();
	InitDust();
    LoadChapter("Docs/Design.txt");
	GameState = GAMESTATE_PLAYING;
}

void DisplayGame_Playing()
{
	// Calculate scrolling.
	CalculateScrollY();
	
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Non-scrolling background Drawing                                                      //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	gxDrawSprite( BackgroundX, BackgroundY, &Background01 );

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Chapter Drawing                                                                       //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	DisplayChapter();

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Vacuum Drawing                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	DisplayVacuum_BeforeDusty();

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Dusty Drawing                                                                         //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	DisplayDusty();

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Barrel Drawing                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

	DisplayBarrels();

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Coin Drawing                                                                          //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	DisplayCoins();
	
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Ball Drawing                                                                          //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    DisplayBall();

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Dust Drawing                                                                          //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	DisplayDust();

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   FireWork Drawing                                                                      //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	DisplayFireWorks();
	
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Vacuum Drawing                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	DisplayVacuum_AfterDusty();

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Score Update                                                                          //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

    DisplayScore();    

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Debugging aids                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	if (DevMode)
	{
		float FPS = 0;

#ifdef PLATFORM_WINDOWS
		static DWORD LastTime = 0;
		DWORD Time = timeGetTime();
		FPS = (1000.0f / ((float)Time - (float)LastTime) + 0.5f);
		LastTime = Time;

		FPS = (float)(int(FPS / 10.0f + 0.5f) * 10);
#endif
#ifdef PLATFORM_IPHONE
		// iPhone TODO: Calculate FPS here.
#endif

		// Status of common variables
		gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "FPS: %.0f ( %.1f %.1f ) State: %d Col: %d%d%d%d\nLastWall: %d Direction: %d", 
			FPS, Dusty.FloatX, Dusty.FloatY, Dusty.State, Dusty.CollideWithLeftSide, Dusty.CollideWithRightSide, Dusty.CollideWithTopSide, Dusty.CollideWithBottomSide,
			Dusty.LastWall, Dusty.Direction);
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

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Dusty Update                                                                          //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//	
	UpdateDusty();

	if (Dusty.State != DUSTYSTATE_DIE)
	{
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Barrel Update                                                                         //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		UpdateBarrels();

		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Coin Update                                                                           //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		UpdateCoins();   
		
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Ball Update                                                                           //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        UpdateBall();  

		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   FireWork Update                                                                       //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		UpdateFireWorks();
		
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Score Update                                                                       //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		UpdateScore();
	}

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Dust Update                                                                           //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	UpdateDust();

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Vacuum Update                                                                         //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    UpdateVacuum();  
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

#ifdef PLATFORM_WINDOWS
	if (DisplayHelp)
	{
		gxDrawString(20,  2*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
		gxDrawString(20,  3*16, 16, gxRGB32(255, 255, 255), "     Super Dust Bunny Keyboard Commands      ");
		gxDrawString(20,  4*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20,  5*16, 16, gxRGB32(255, 255, 255), " Global:                                     ");
		gxDrawString(20,  6*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20,  7*16, 16, gxRGB32(255, 255, 255), " F1    - Toggle help                         ");
		gxDrawString(20,  8*16, 16, gxRGB32(255, 255, 255), " F2    - Emulate iPhone display size         ");
		gxDrawString(20,  9*16, 16, gxRGB32(255, 255, 255), " F3    - Emulate iPad display size           ");
		gxDrawString(20, 10*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, 11*16, 16, gxRGB32(255, 255, 255), " While playing:                              ");
		gxDrawString(20, 12*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, 13*16, 16, gxRGB32(255, 255, 255), " Home  - Toggle development mode             ");
		gxDrawString(20, 14*16, 16, gxRGB32(255, 255, 255), " \\     - Hold for slow motion                ");
		gxDrawString(20, 15*16, 16, gxRGB32(255, 255, 255), " ]     - Step one frame (in slow motion)     ");
		gxDrawString(20, 16*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, 17*16, 16, gxRGB32(255, 255, 255), " A     - Hop left                            ");
		gxDrawString(20, 18*16, 16, gxRGB32(255, 255, 255), " D     - Hop right                           ");
		gxDrawString(20, 19*16, 16, gxRGB32(255, 255, 255), " Space - Jump                                ");
		gxDrawString(20, 20*16, 16, gxRGB32(255, 255, 255), " Enter - Advance menus                       ");
		gxDrawString(20, 21*16, 16, gxRGB32(255, 255, 255), "                                             ");
		gxDrawString(20, 22*16, 16, gxRGB32(255, 255, 255), " - - - - - - - - - - - - - - - - - - - - - - ");
	}
	else
	{
		gxDrawString(20, gxScreenHeight - 36, 16, gxRGB32(255, 255, 255), "Press F1 for help");
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
            sxPlaySound(&BackgroundSong01);
        }
        
        if (SongDuration == 2000 && SongCounter == 2)
        {
            sxPlaySound(&BackgroundSong03);
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

	return true;
}
