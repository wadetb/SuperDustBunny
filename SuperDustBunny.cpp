//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                           //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define PLATFORM_IPHONE

#elif TARGET_OS_MAC
#define PLATFORM_MAC

#elif defined(_MSC_VER)
#define PLATFORM_WINDOWS
#endif

#if defined(PLATFORM_WINDOWS)
#include "win/graphics.h"
#include "win/mouse.h"
#include "win/keyboard.h"
#include "win/sound.h"
#elif defined(PLATFORM_IPHONE)
#include "iPhone/graphics.h"
#include "iPhone/mouse.h"
#include "iPhone/sound.h"
#elif defined(PLATFORM_MAC)
#include "Mac/graphics.h"
#include "Mac/mouse.h"
#include "Mac/keyboard.h"
#include "Mac/sound.h"
#endif

enum EDustyState
{
	DUSTYSTATE_STAND				= 0,
	DUSTYSTATE_JUMP					= 1,
	DUSTYSTATE_HOP_RIGHT			= 2,
	DUSTYSTATE_HOP_LEFT				= 3,
    DUSTYSTATE_WALLJUMP_RIGHT		= 4,
	DUSTYSTATE_WALLJUMP_LEFT		= 5,
};

enum EPlatformState
{
    PLATFORMSTATE_STATIONARY        = 6,
};

bool SlowMotionMode = false;
int Max(int a, int b);
int Min(int a, int b);

EDustyState DustyState = DUSTYSTATE_STAND;
EPlatformState PlatformState = PLATFORMSTATE_STATIONARY;

int DustyX = 350;
int DustyY = 1024-110;
int DirectionX = 7;
int DirectionY = 7;
int DustyLeft = -10;
int DustyRight = 120;
int DustyTop = -110;
int DustyBottom = 110;

int PlatformX = 340;//Location of Platform from left to right on the X axis
int PlatformY = 750;//Location of Platform from top to bottom on the Y axis
int PlatformLeft = -10;
int PlatformRight = 170;
int PlatformTop = -10;
int PlatformBottom = 40;
int PlatformVisible = 1;
int OnPlatform = false;

int BackgroundX = 0;
int BackgroundY = 0;
int BackgroundVisible = 1;
int VerticalCounter = 0;
int JumpQueue = 0;
int JumpRightSprite = 2;
int JumpLeftSprite = 2;
int WallStickTimer = 0;
int FallRightSprite = 2;
int FallLeftSprite = 2;
int HopRightSprite = 0;
int HopLeftSprite = 0;
int LastDirectionSprite = 1;
int SpriteTransition = 0;

bool RightSideIsInPlatform = false;
bool LeftSideIsInPlatform = false;
bool TopSideIsInPlatform = false;
bool BottomSideIsInPlatform = false;
bool AreRecsIntersecting = false;
bool IsJumping = false;
bool IsCollided = false;

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

sxSound DustyToJump;
sxSound DustyJumps;
sxSound WallJump;
sxSound BackgroundSong01;
sxSound BackgroundSong02;
sxSound BackgroundSong03;
int SongCounter = 1;
int SongDuration = 0;
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
    gxLoadSprite("Data/WoodPlatformThin.png", &WoodBox_Platform01);
    gxLoadSprite("Data/LargeBackground.png", &Background01);
    
    sxLoadWav ("Data/yaahooo.wav", &DustyToJump);
    sxLoadWav ("Data/yaahooo.wav", &DustyJumps);
    sxLoadWav ("Data/cork_pop.wav", &WallJump);
    sxLoadWav ("Data/Song1.wav", &BackgroundSong01);
    sxLoadWav ("Data/Song2.wav", &BackgroundSong02);
    sxLoadWav ("Data/Song3.wav", &BackgroundSong03);
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
//                                                  State switching function declarations                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand();
void SetDustyState_Jump( bool OffWall );
void SetDustyState_Hop_Right();
void SetDustyState_Hop_Left();
void SetDustyState_WallJump_Right();
void SetDustyState_WallJump_Left();

void SetPlatformState_Stationary();

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
//                                                  DUSTYSTATE_STAND Implementation                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand()
{
	DustyState = DUSTYSTATE_STAND;
}

void DisplayDusty_Stand()
{
	if (LastDirectionSprite == 0) //Standing
	{
 		gxDrawSprite( DustyX-64, DustyY-118, &DustyHopLeft01 );
	}

	if (LastDirectionSprite == 1)//After moving, stops to face the direction last moved.
	{
		gxDrawSprite( DustyX-64, DustyY-118, &DustyHop01 );
	}
}

void UpdateDusty_Stand()
{	
	if (LastDirectionSprite == 1)
	{
		JumpRightSprite = 1;
		JumpLeftSprite = 0;
		//Reset Fall Sprites
		FallRightSprite = 0;
		FallLeftSprite = 0;
	}	   

	if (LastDirectionSprite == 0)
	{
		JumpLeftSprite = 1;
		JumpRightSprite = 0;
		//Reset Fall Sprites
		FallRightSprite = 0;
		FallLeftSprite = 0;
	}

	if ( GetInput_Jump() )      
	{
		SetDustyState_Jump( false );
	}

	if ( GetInput_MoveRight() )
	{
		DustyState = DUSTYSTATE_HOP_RIGHT; //Moving Right
		HopRightSprite = 1;
		SpriteTransition = 30;
		LastDirectionSprite = 1;			  
	}

	if ( GetInput_MoveLeft() )
	{
		DustyState = DUSTYSTATE_HOP_LEFT; //Moving Left
		HopLeftSprite = 1;
		SpriteTransition = 30;
		LastDirectionSprite = 0;   
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_JUMP Implementation                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Jump( bool OffWall )
{
	if ( OffWall )
	{
		sxPlaySound( &WallJump );
	}
	else
	{
		//sxPlaySound( &DustyJumps );
	}

	VerticalCounter = 20;
	DustyY -= 10;
	DustyState = DUSTYSTATE_JUMP;
}

void DisplayDusty_Jump()
{
	if (JumpRightSprite == 1)     
	{
		gxDrawSprite( DustyX-46, DustyY-126, &DustyHop03 );
	}

	if (FallRightSprite == 1)
	{
		gxDrawSprite( DustyX-56, DustyY-125, &DustyHop02 );
	}  

	if (JumpLeftSprite == 1)
	{
		gxDrawSprite( DustyX-46, DustyY-128, &DustyHopLeft03 );
	}

	if (FallLeftSprite == 1)
	{
		gxDrawSprite( DustyX-80, DustyY-128, &DustyHopLeft02 );
	}
}

void UpdateDusty_Jump()
{      
    //Collide right side of screen check
    if (IsCollided == true && DustyX + DustyRight > gxScreenWidth)
    {
       DustyX = gxScreenWidth - DustyRight;
    }
    
    //Collide left side of screen check
    if (IsCollided == true && DustyX + DustyLeft < 0)
    {
       DustyX = -DustyLeft;  
       JumpRightSprite = 1;
       JumpLeftSprite = 0;                      
    }
    
    if (LastDirectionSprite == 1)
    {
      DustyX = DustyX + DirectionX;
      JumpRightSprite = 1;
      JumpLeftSprite = 0;        
      // Collision with right side of screen
      if (DustyX + DustyRight >= gxScreenWidth && IsCollided == false )
      {
          DustyX = gxScreenWidth - DustyRight;
          DustyState = DUSTYSTATE_WALLJUMP_RIGHT;//Collide with Right wall. 
          JumpLeftSprite = 0;
          JumpRightSprite = 0;
          WallStickTimer = 15;
          IsCollided = true;
      }                                                      
    }

    if (LastDirectionSprite == 0)
    {          
      DustyX = DustyX - DirectionX;
      JumpLeftSprite = 1;
      JumpRightSprite = 0;         
      //Collision with the left side of the screen    
      if (DustyX + DustyLeft <= 0 && IsCollided == false)
      {
          DustyX = -DustyLeft;
          DustyState = DUSTYSTATE_WALLJUMP_LEFT;
          JumpLeftSprite = 0;
          JumpRightSprite = 0;
          WallStickTimer = 15;
          IsCollided = true;             
      }                    
    }
    
	if (VerticalCounter == 0)
	{
		DustyY = DustyY + DirectionY;

		if (LastDirectionSprite == 1)
		{
			JumpRightSprite = 0;
			FallRightSprite = 1;
		}

		if (LastDirectionSprite == 0)
		{
			JumpLeftSprite = 0;
			FallLeftSprite = 1;
		}
	}
	else
	{     
		VerticalCounter -= 1;
		DustyY -= 10;
	}

	if (DustyY + DustyBottom > gxScreenHeight )
	{	
		DustyY = gxScreenHeight - DustyBottom;
		IsCollided = false;	
		SetDustyState_Stand();
	}    
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOP_RIGHT Implementation                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Hop_Right()
{
    DustyState = DUSTYSTATE_HOP_RIGHT;
}

void DisplayDusty_Hop_Right()
{
    if (HopRightSprite == 1) 
    {
        gxDrawSprite( DustyX-64, DustyY-116, &DustyHop01 );
    } 

    if (HopRightSprite == 2)
    {
        gxDrawSprite( DustyX-56, DustyY-124, &DustyHop02 );
    } 

    if (HopRightSprite == 3)
    {
        gxDrawSprite( DustyX-50, DustyY-126, &DustyHop03 );
    }

    if (HopRightSprite == 4)
    {
        gxDrawSprite( DustyX-74, DustyY-92, &DustyHop04 );
    }    

    if (HopRightSprite == 5)
    {
        gxDrawSprite( DustyX-102, DustyY-144, &DustyHop05 );
    }     

    if (HopRightSprite == 6)
    {
        gxDrawSprite( DustyX-64, DustyY-116, &DustyHop01 );
    }   
}	

void UpdateDusty_Hop_Right()
{
    DustyX = DustyX + DirectionX;  

    // Update animation
    SpriteTransition -= 1;
    
    // Collision with right side of screen
    if (DustyX + DustyRight >= gxScreenWidth )
        DustyX = gxScreenWidth - DustyRight;

    if ( !GetInput_MoveRight() && SpriteTransition == 0 )
    {			                         
        SetDustyState_Stand();
    }

    if (GetInput_Jump() && SpriteTransition != 0)
    {
        JumpQueue = 1;
        //sxPlaySound( &DustyToJump );
        //sxDestroySound( &DustyToJump ); 
    }

    if (JumpQueue == 1 && SpriteTransition == 0)
    {
        LastDirectionSprite = 1;
        //reset JumpQueue
        JumpQueue = 0;
        SetDustyState_Jump( false );
    }    

    if (GetInput_MoveLeft())
    {
        LastDirectionSprite = 0;
        DustyState = DUSTYSTATE_HOP_LEFT;
    }

    if (SpriteTransition == 30)
    {
        HopRightSprite = 1;
    }  

    if (SpriteTransition == 25) 
    {
        HopRightSprite = 2;
    } 

    if (SpriteTransition == 20)
    {
        HopRightSprite = 3;
    } 

    if (SpriteTransition == 15)
    {
        HopRightSprite = 4;
    }

    if (SpriteTransition == 10)
    {
        HopRightSprite = 5;
    }

    if (SpriteTransition == 5)
    {
        HopRightSprite = 6;
    }

    if (SpriteTransition == 0)
    {
        SpriteTransition = 30;
    }		
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOP_LEFT Implementation                                                     //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
void SetDustyState_Hop_Left()
{
    DustyState = DUSTYSTATE_HOP_LEFT;
}

void DisplayDusty_Hop_Left()
{
    if (HopLeftSprite == 1) 
    {
        gxDrawSprite( DustyX-64, DustyY-118, &DustyHopLeft01 );
    } 

    if (HopLeftSprite == 2)
    {
        gxDrawSprite( DustyX-54, DustyY-120, &DustyHopLeft02 );
    } 

    if (HopLeftSprite == 3)
    {
        gxDrawSprite( DustyX-46, DustyY-128, &DustyHopLeft03 );
    }

    if (HopLeftSprite == 4)
    {
        gxDrawSprite( DustyX-68, DustyY-90, &DustyHopLeft04 );
    } 
    if (HopLeftSprite == 5)
    {
        gxDrawSprite( DustyX-78, DustyY-150, &DustyHopLeft06 );
    }    
    if (HopLeftSprite == 6)
    {
        gxDrawSprite( DustyX-64, DustyY-118, &DustyHopLeft01 );
    }              
}

void UpdateDusty_Hop_Left()
{
    DustyX = DustyX - DirectionX;	

    // Update animation
    SpriteTransition -= 1;

    // Collision with left side of screen
    if (DustyX + DustyLeft <= 0)
        DustyX = -DustyLeft;

    if ( !GetInput_MoveLeft() && SpriteTransition == 0 )
    {          
        SetDustyState_Stand();
    }

    if (GetInput_Jump() && SpriteTransition != 0)
    {
        JumpQueue = 1;
        //sxPlaySound( &DustyToJump );
        //sxDestroySound( &DustyToJump ); 
    }

    if (JumpQueue == 1 && SpriteTransition == 0)
    {
        LastDirectionSprite = 0;
        //Reset JumpQueue
        JumpQueue = 0;
        SetDustyState_Jump( false );
    }  

    if (GetInput_MoveRight())
    {
        LastDirectionSprite = 1;
        DustyState = DUSTYSTATE_HOP_RIGHT;
    }

    if (SpriteTransition == 30)
    {
        HopLeftSprite = 1;
    }  

    if (SpriteTransition == 25) 
    {
        HopLeftSprite = 2;
    } 

    if (SpriteTransition == 20)
    {
        HopLeftSprite = 3;
    } 

    if (SpriteTransition == 15)
    {
        HopLeftSprite = 4;
    }

    if (SpriteTransition == 10)
    {
        HopLeftSprite = 5;
    }

    if (SpriteTransition == 5)
    {
        HopLeftSprite = 6;
    }

    if (SpriteTransition == 0)
    {
        SpriteTransition = 30;
    }
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_RIGHT Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
void SetDustyState_WallJump_Right()
{
    DustyState = DUSTYSTATE_WALLJUMP_RIGHT;
}

void DisplayDusty_WallJump_Right()
{
        gxDrawSprite( DustyX-52, DustyY-142, &LeftFaceWallJump01 );       
}

void UpdateDusty_WallJump_Right()//Collided with Right Wall
  {                                                
    if (WallStickTimer == 0)
    {                   
        VerticalCounter = 0;          
        DustyState = DUSTYSTATE_JUMP;
    }

    if (WallStickTimer != 0)
    {                        
        WallStickTimer -= 1;
    }

    if (GetInput_Jump() && LastDirectionSprite == 1)
    {               
        LastDirectionSprite = 0;
        //Resetting sprites
		FallRightSprite = 0;
        FallLeftSprite = 0;
		JumpRightSprite = 0;
		JumpLeftSprite = 1;
        SetDustyState_Jump( true );
    }
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_LEFT Implementation                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -// 

void SetDustyState_WallJump_Left()
{
    DustyState = DUSTYSTATE_WALLJUMP_LEFT;
} 

void DisplayDusty_WallJump_Left()
{
        gxDrawSprite( DustyX-100, DustyY-144, &RightFaceWallJump01 ); 
}

void UpdateDusty_WallJump_Left()
{

    if (WallStickTimer == 0)
    { 
        VerticalCounter = 0;
        DustyState = DUSTYSTATE_JUMP;
    }

    if (WallStickTimer != 0)
    {        
        WallStickTimer -= 1;
    }       

    if (GetInput_Jump() && LastDirectionSprite == 0)
    {
        LastDirectionSprite = 1;
        //Resetting sprites
		FallRightSprite = 0;
        FallLeftSprite = 0;
        JumpRightSprite = 1;
        SetDustyState_Jump( true );    
    }
} 
  
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UPDATE_REC_COLLISION Implementation                                                           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void UpdateRecCollision()
{
    if ((DustyX + DustyRight >= PlatformX) && (DustyX + DustyRight <= PlatformX + PlatformRight))
    {   
        RightSideIsInPlatform = true;
    }

    if ((DustyX + DustyLeft >= PlatformX) && (DustyX + DustyLeft <= PlatformX + PlatformRight))
    {    
        LeftSideIsInPlatform = true;
    }
    
    if ((DustyY + DustyTop >= PlatformY && (DustyY + DustyTop <= PlatformY + PlatformTop)))
    {
        TopSideIsInPlatform = true;
    }
    
    if ((DustyY + DustyBottom >= PlatformY && DustyY + DustyBottom <= PlatformY + PlatformBottom))
    {
        BottomSideIsInPlatform = true;
    }
    
    //Comparing two numbers and returning the one with greater value, and comparing two numbers and returning the one with lesser value.
    //PlatformX and PlatformX + PlatformRight as well as PlatformY and PlatformY + PlatformTop will always be the same.
	AreRecsIntersecting = false;
    if(Max(DustyX + DustyLeft, PlatformX + PlatformLeft) < Min(DustyX + DustyRight, PlatformX + PlatformRight))
	{
		if (Max(DustyY + DustyTop, PlatformY + PlatformTop) < Min(DustyY + DustyBottom, PlatformY + PlatformBottom))//The Y Axis version of the formula above.
		{    
 			AreRecsIntersecting = true;
		}
	}


    if (AreRecsIntersecting == true && RightSideIsInPlatform == true)
    {
        
    }

    if (AreRecsIntersecting == true && LeftSideIsInPlatform == true)
    {
    
    }
    
    if (AreRecsIntersecting == true && TopSideIsInPlatform == true)
    {
    
    
    }
    
    if (AreRecsIntersecting == true && BottomSideIsInPlatform == true)
    {
    
    
    }
    
    
    
    
    
    
    
}

int Max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int Min(int a, int b)
{
    if ( a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  BOX_Stationary Implementation                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetPlatformState_Stationary()
{
    PlatformState = PLATFORMSTATE_STATIONARY;
}

void DisplayPlatform_Stationary()
{
       gxDrawSprite( PlatformX, PlatformY, &WoodBox_Platform01 );
}

 void UpdatePlatform_Stationary()
 {
 
 
 }

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Central Display and Update functions                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//


void Display()
{
    //if (BackgroundVisible == 1)
    //{
    //    gxDrawSprite( BackgroundX, BackgroundY, &Background01 );
   // }

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Dusty Drawing                                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

	switch (DustyState)
	{
	case DUSTYSTATE_STAND:			    DisplayDusty_Stand(); break;
	case DUSTYSTATE_JUMP:			    DisplayDusty_Jump(); break;
	case DUSTYSTATE_HOP_RIGHT:          DisplayDusty_Hop_Right(); break;
	case DUSTYSTATE_HOP_LEFT:           DisplayDusty_Hop_Left(); break;
	case DUSTYSTATE_WALLJUMP_RIGHT:     DisplayDusty_WallJump_Right(); break;
	case DUSTYSTATE_WALLJUMP_LEFT:      DisplayDusty_WallJump_Left(); break;	
	default:						    break;
    }

	
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Box Drawing                                                                           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

    switch (PlatformState)
    {
      case PLATFORMSTATE_STATIONARY:          DisplayPlatform_Stationary(); break;
      default:                           break;
    }
 
  
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Debugging aids                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

	// Status of common variables
	gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "( %03d, %03d ) DustyState: %d", DustyX, DustyY, DustyState );

	// Indicator for when slow motion is activated.
	if (SlowMotionMode)
	{
		gxDrawString(gxScreenWidth-101, 5, 16, gxRGB32(255, 255, 0), "[SLOW]");
	}

	// Draw a red + at Dusty's root location.
	gxDrawString(DustyX-4, DustyY-4, 8, gxRGB32(255, 0, 0), "+");

	// Draw a red + at all four corners of Dusty.
	gxDrawString(DustyX + DustyLeft,  DustyY + DustyTop,    8, gxRGB32(255, 0, 0), "+");//Upper Left
	gxDrawString(DustyX + DustyRight, DustyY + DustyTop,    8, gxRGB32(255, 0, 0), "+");//Upper Right
	gxDrawString(DustyX + DustyLeft,  DustyY + DustyBottom, 8, gxRGB32(255, 0, 0), "+");//Lower Left
	gxDrawString(DustyX + DustyRight, DustyY + DustyBottom, 8, gxRGB32(255, 0, 0), "+");//Lower Right

	// Draw a + at all four corners of Platform - red if no collision, green if collusion.
	int color;
	if (AreRecsIntersecting)
		color = gxRGB32(0, 255, 0);
	else
		color = gxRGB32(255, 0, 0);
	gxDrawString(PlatformX + PlatformLeft,  PlatformY + PlatformTop,    8, color, "+");//Upper Left
	gxDrawString(PlatformX + PlatformRight, PlatformY + PlatformTop,    8, color, "+");//Upper Right
	gxDrawString(PlatformX + PlatformLeft,  PlatformY + PlatformBottom, 8, color, "+");//Lower Left
	gxDrawString(PlatformX + PlatformRight, PlatformY + PlatformBottom, 8, color, "+");//Lower Right
}

bool Update()
{
#ifdef PLATFORM_WINDOWS
	kbUpdateKeys();
#endif
	
#ifdef PLATFORM_IPHONE
	msUpdateMouse();
#endif
	
	//Background Music
if (BackgroundMusic == 1)
{	
	if (SongCounter == 1)
	{
        SongDuration = 500;//2580;    
        sxPlaySound(&BackgroundSong01);
    }
    
    if (SongDuration == 0)
    {
        SongCounter += 1;   
    }
    
    if (SongDuration != 0)
    {
        SongDuration -= 1;     
    }
        
    if (SongCounter == 2)
    {
         SongDuration = 500;//2580;
         sxPlaySound(&BackgroundSong02);
    }
    
    if (SongCounter == 3)
    {
         SongDuration = 500;//1952;
         sxPlaySound(&BackgroundSong03);
    }
    if (SongCounter >= 4)
    {
        SongCounter = 1;
    }    
}

#ifdef PLATFORM_WINDOWS
	// Pressing escape quits the program.
	if (kbIsKeyDown(KB_ESCAPE))
	{
		return false;
	}
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
#endif
	
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	//                                                   Dusty Update                                                                          //
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//	
	
	UpdateRecCollision();

    switch (DustyState)
    {
		case DUSTYSTATE_STAND:			    UpdateDusty_Stand(); break;
		case DUSTYSTATE_JUMP:			    UpdateDusty_Jump(); break;
		case DUSTYSTATE_HOP_RIGHT:          UpdateDusty_Hop_Right(); break;
		case DUSTYSTATE_HOP_LEFT:           UpdateDusty_Hop_Left(); break;
		case DUSTYSTATE_WALLJUMP_RIGHT:     UpdateDusty_WallJump_Right(); break;
		case DUSTYSTATE_WALLJUMP_LEFT:      UpdateDusty_WallJump_Left(); break;
		default:						    break;
    }
    	
    switch (PlatformState)
    {
        case PLATFORMSTATE_STATIONARY:           UpdatePlatform_Stationary(); break;
        default:                           break;
    }	
	return true;
}