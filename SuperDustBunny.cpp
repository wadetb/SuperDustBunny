//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "common.h"

#include "chapter.h"
#include "barrel.h"
#include "Dusty.h"
#include "Coin.h"

enum EGameState
{
	GAMESTATE_START_SCREEN,
	GAMESTATE_PLAYING
};

EGameState GameState = GAMESTATE_START_SCREEN;

bool TitleScreenButtonPressed = false;

bool DevMode = true;
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
gxSprite WoodBox_Platform01;
gxSprite Background01;
gxSprite UnknownBlock;
gxSprite BarrelSprite;

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

gxSprite StartScreen0;
gxSprite StartScreen1;

sxSound DustyToJump;
sxSound DustyJumps;
sxSound WallJump;
sxSound LaunchSound01;
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

	InitDusty();

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
    gxLoadSprite("Data/WoodPlatformThin.png", &WoodBox_Platform01);
    gxLoadSprite("Data/LargeBackground.png", &Background01);
	gxLoadSprite("Data/wtf.png", &UnknownBlock);
	gxLoadSprite("Data/can.png", &BarrelSprite);
	gxLoadSprite("Data/start-screen-vert2.png", &StartScreen0);
	gxLoadSprite("Data/start-screen-vert.png", &StartScreen1);

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

    sxLoadWav ("Data/yaahooo.wav", &DustyToJump);
    sxLoadWav ("Data/yaahooo.wav", &DustyJumps);
    sxLoadWav ("Data/cork_pop.wav", &WallJump);
    sxLoadWav ("Data/cork_pop.wav", &LaunchSound01);
    sxLoadWav ("Data/Song1.wav", &BackgroundSong01);
    sxLoadWav ("Data/Song2.wav", &BackgroundSong02);
    sxLoadWav ("Data/Song3.wav", &BackgroundSong03);

	LoadChapter("Docs/Design.txt");
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
//                                                   Central Display and Update functions                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//


void Display()
{
	if (GameState == GAMESTATE_START_SCREEN)
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
	else if (GameState == GAMESTATE_PLAYING)
	{
		CalculateScrollY();

		gxDrawSprite( BackgroundX, BackgroundY, &Background01 );

		DisplayChapter();
	}

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
	//                                                   Debugging aids                                                                        //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		if (DevMode)
		{		
			// Status of common variables
			gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "( %03d, %03d ) State: %d, Col: %d%d%d%d, JumpQ: %d\n,", Dusty.X, Dusty.Y, Dusty.State, Dusty.CollideWithLeftSide, Dusty.CollideWithRightSide,
				Dusty.CollideWithTopSide, Dusty.CollideWithBottomSide, Dusty.JumpQueue);
        }
        
    DisplayCoins();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Debugging aids                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	if (DevMode)
	{		
		// Status of common variables
		gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "( %03d, %03d ) State: %d, Col: %d%d%d%d, JumpQ: %d", Dusty.X, Dusty.Y, Dusty.State, Dusty.CollideWithLeftSide, Dusty.CollideWithRightSide,
			Dusty.CollideWithTopSide, Dusty.CollideWithBottomSide, Dusty.JumpQueue);
		// Indicator for when slow motion is activated.
			if (SlowMotionMode)
			{
				gxDrawString(gxScreenWidth-101, 5, 16, gxRGB32(255, 255, 0), "[SLOW]");
			}
			// Draw a red + at Dusty's root location.
			gxDrawString(Dusty.X-4, Dusty.Y-4, 8, gxRGB32(255, 0, 0), "+");
    }
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
	
#endif
	if (GameState == GAMESTATE_START_SCREEN)	
#ifdef PLATFORM_WINDOWS
		TitleScreenButtonPressed = kbIsKeyDown(KB_RETURN);
		
		// Advance to playing state when return key is released.
		if (!kbIsKeyDown(KB_RETURN) && kbWasKeyDown(KB_RETURN))
		{
			GameState = GAMESTATE_PLAYING;
		}
#endif
#ifdef PLATFORM_IPHONE
		// TODO: iPhone uses a finger tap on the button.
#endif

	else if (GameState == GAMESTATE_PLAYING)
    
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
#endif
	
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Dusty Update                                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//	
		
		UpdateDusty();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Barrel Update                                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

		UpdateBarrels();
	
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Coin Update                                                                           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

        UpdateCoins();   
    return true;
}