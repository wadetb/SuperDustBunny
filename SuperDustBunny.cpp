//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "common.h"

#include "chapter.h"
#include "barrel.h"

// Shows Debug Text and Rectangle Outlines
bool DevMode = true;

enum EDustyState
{
	DUSTYSTATE_STAND				,
	DUSTYSTATE_JUMP					,
	DUSTYSTATE_FALL					,
	DUSTYSTATE_HOP_RIGHT			,
	DUSTYSTATE_HOP_LEFT				,
    DUSTYSTATE_WALLJUMP_RIGHT		,
	DUSTYSTATE_WALLJUMP_LEFT		
};

enum EPlatformState
{
    PLATFORMSTATE_STATIONARY       ,
};

bool SlowMotionMode = false;
int Max(int a, int b);
int Min(int a, int b);

EDustyState DustyState = DUSTYSTATE_STAND;
EPlatformState PlatformState = PLATFORMSTATE_STATIONARY;
void UpdateDusty_JumpCommon();

int DustyX = 350;
int DustyY = 1024;
int DirectionX = 6;
int DirectionY = 6;
int DustyLeft = -40;
int DustyRight = 40;
int DustyTop = -120;
int DustyBottom = 0;

int Platform1X = 0;//Location of Platform from left to right on the X axis
int Platform1Y = 64*12;//Location of Platform from top to bottom on the Y axis
int Platform2X = 64*8;
int Platform2Y = 64*9;
int Platform3X = 64*8;
int Platform3Y = 64*3;
int PlatformLeft = 0;
int PlatformRight = 64*4;
int PlatformTop = 0;
int PlatformBottom = 64;
int PlatformVisible = 1;

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
int LeftSide = 0;
int RightSide = 768;
int TopSide = 0;
int BottomSide = 1024;

bool RightSideIsInPlatform = false;
bool LeftSideIsInPlatform = false;
bool TopSideIsInPlatform = false;
bool BottomSideIsInPlatform = false;
bool AreRecsIntersecting = false;
bool IsJumping = false;
bool CanWallJump = true;
bool OnPlatform = false;
bool StraightFallTrigger = false;
bool CollideWithLeftSide = false;
bool CollideWithRightSide = false;
bool CollideWithTopSide = false;
bool CollideWithBottomSide = false;

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

sxSound DustyToJump;
sxSound DustyJumps;
sxSound WallJump;
sxSound BackgroundSong01;
sxSound BackgroundSong02;
sxSound BackgroundSong03;
int SongCounter = 1;
int SongDuration = 0;
int BackgroundMusic = 0;

SChapter Chapter;

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
	gxLoadSprite("Data/wtf.png", &UnknownBlock);
	gxLoadSprite("Data/gear.png", &BarrelSprite);

    sxLoadWav ("Data/yaahooo.wav", &DustyToJump);
    sxLoadWav ("Data/yaahooo.wav", &DustyJumps);
    sxLoadWav ("Data/cork_pop.wav", &WallJump);
    sxLoadWav ("Data/Song1.wav", &BackgroundSong01);
    sxLoadWav ("Data/Song2.wav", &BackgroundSong02);
    sxLoadWav ("Data/Song3.wav", &BackgroundSong03);


	LoadChapter("Docs/Design.txt", &Chapter);
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
void SetDustyState_Fall();
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
	CanWallJump = true;
}

void DisplayDusty_Stand()
{
	if (LastDirectionSprite == 0) //Standing
	{
 		gxDrawSprite( DustyX-112, DustyY-221, &DustyHopLeft01 );
	}

	if (LastDirectionSprite == 1)//After moving, stops to face the direction last moved.
	{
		gxDrawSprite( DustyX-140, DustyY-221, &DustyHop01 );
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
		return;
	}

	if ( GetInput_MoveRight() )
	{
		SetDustyState_Hop_Right();
		return;
	}

	if ( GetInput_MoveLeft() )
	{
		SetDustyState_Hop_Left();
		return;
	}

	if (CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
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

	OnPlatform = false;
	VerticalCounter = 27;
	DustyY -= 10;
	DustyState = DUSTYSTATE_JUMP;
}

void DisplayDusty_Jump()
{
	if (JumpRightSprite == 1)     
	{
		gxDrawSprite( DustyX-107, DustyY-177, &DustyHop03 );
	} 

	if (JumpLeftSprite == 1)
	{
		gxDrawSprite( DustyX-145, DustyY-177, &DustyHopLeft03 );
	}
}

void UpdateDusty_Jump()
{         
    if (LastDirectionSprite == 1)
    {
      DustyX = DustyX + DirectionX;
      JumpRightSprite = 1;
      JumpLeftSprite = 0;                                                     
    }

    if (LastDirectionSprite == 0)
    {          
      DustyX = DustyX - DirectionX;
      JumpLeftSprite = 1;
      JumpRightSprite = 0;                            
    }
    
	if (VerticalCounter == 0)
	{
		SetDustyState_Fall();
		return;
	}
	else
	{     
		VerticalCounter -= 1;
		DustyY -= 10;
	}

	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UPDATE_REC_COLLISION Implementation                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Fall()
{
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

	OnPlatform = false;
	DustyState = DUSTYSTATE_FALL;
}

void DisplayDusty_Fall()
{
	if (FallLeftSprite == 1)
	{
		gxDrawSprite( DustyX-130, DustyY-217, &DustyHopLeft02 );
	}

	if (FallRightSprite == 1)
	{
		gxDrawSprite( DustyX-125, DustyY-221, &DustyHop02 );
	} 

}

void UpdateDusty_Fall()
{
	DustyY = DustyY + DirectionY;
	       
	if (LastDirectionSprite == 1)
	{
		DustyX = DustyX + DirectionX;
		JumpRightSprite = 1;
		JumpLeftSprite = 0; 	                                                  
	}

	if (LastDirectionSprite == 0)
	{          
		DustyX = DustyX - DirectionX;
		JumpLeftSprite = 1;
		JumpRightSprite = 0;                            
	}

	if (CollideWithBottomSide == true )
	{	
		SetDustyState_Stand();
		return;
	} 

	UpdateDusty_JumpCommon();
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOP_RIGHT Implementation                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Hop_Right()
{
	HopRightSprite = 1;
	SpriteTransition = 30;
	LastDirectionSprite = 1;
	DustyState = DUSTYSTATE_HOP_RIGHT;
}

void DisplayDusty_Hop_Right()
{
    if (HopRightSprite == 1) 
    {
        gxDrawSprite( DustyX-140, DustyY-221, &DustyHop01 );
    } 

    if (HopRightSprite == 2)
    {
        gxDrawSprite( DustyX-140, DustyY-221, &DustyHop02 );
    } 

    if (HopRightSprite == 3)
    {
        gxDrawSprite( DustyX-140, DustyY-221, &DustyHop03 );
    }

    if (HopRightSprite == 4)
    {
        gxDrawSprite( DustyX-140, DustyY-221, &DustyHop04 );
    }    

    if (HopRightSprite == 5)
    {
        gxDrawSprite( DustyX-140, DustyY-221, &DustyHop05 );
    }     

    if (HopRightSprite == 6)
    {
        gxDrawSprite( DustyX-140, DustyY-221, &DustyHop01 );
    }   
}	

void UpdateDusty_Hop_Right()
{
    DustyX = DustyX + DirectionX;  

    // Update animation
    SpriteTransition -= 1;
    
	//Walk off right side of Platform
	if (CollideWithBottomSide == false && OnPlatform == true)
	{
		SetDustyState_Fall();
		return;
	}

    if ( !GetInput_MoveRight() && SpriteTransition == 0 )
    {			                         
        SetDustyState_Stand();
		return;
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
		return;
    }    

    if (GetInput_MoveLeft())
    {
        LastDirectionSprite = 0;
        SetDustyState_Hop_Left();
		return;
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
	HopLeftSprite = 1;
	SpriteTransition = 30;
	LastDirectionSprite = 0;  
	DustyState = DUSTYSTATE_HOP_LEFT;
}

void DisplayDusty_Hop_Left()
{
    if (HopLeftSprite == 1) 
    {
        gxDrawSprite( DustyX-112, DustyY-221, &DustyHopLeft01 );
    } 

    if (HopLeftSprite == 2)
    {
        gxDrawSprite( DustyX-112, DustyY-221, &DustyHopLeft02 );
    } 

    if (HopLeftSprite == 3)
    {
        gxDrawSprite( DustyX-112, DustyY-221, &DustyHopLeft03 );
    }

    if (HopLeftSprite == 4)
    {
        gxDrawSprite( DustyX-112, DustyY-221, &DustyHopLeft04 );
    } 
    if (HopLeftSprite == 5)
    {
        gxDrawSprite( DustyX-112, DustyY-221, &DustyHopLeft06 );
    }    
    if (HopLeftSprite == 6)
    {
        gxDrawSprite( DustyX-112, DustyY-221, &DustyHopLeft01 );
    }              
}

void UpdateDusty_Hop_Left()
{
    DustyX = DustyX - DirectionX;	

    // Update animation
    SpriteTransition -= 1;

	//Walk off left side of Platform
	if (CollideWithBottomSide == false && OnPlatform == true)
	{
		SetDustyState_Fall();
		return;
	}
	
    if ( !GetInput_MoveLeft() && SpriteTransition == 0 )
    {          
        SetDustyState_Stand();
		return;
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
		return;
    }  

    if (GetInput_MoveRight())
    {
        LastDirectionSprite = 1;
        SetDustyState_Hop_Right();
		return;
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
//                                                  UpdateDusty_JumpCommon Implementation										           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void UpdateDusty_JumpCommon()
{
	// Collision with right side of screen
	if (CollideWithRightSide == true && CanWallJump == true && LastDirectionSprite == 1)
	{
		SetDustyState_WallJump_Right();
		return;
	} 

	//Collision with the left side of the screen    
	if (CollideWithLeftSide == true && CanWallJump == true && LastDirectionSprite == 0)
	{	
		SetDustyState_WallJump_Left();
		return;
	}
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_RIGHT Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
void SetDustyState_WallJump_Right()
{   
	JumpLeftSprite = 0;
	JumpRightSprite = 0;
	WallStickTimer = 15;
	CanWallJump = true;

	DustyState = DUSTYSTATE_WALLJUMP_RIGHT;//Collide with Right wall. 
}

void DisplayDusty_WallJump_Right()
{
        gxDrawSprite( DustyX-115, DustyY-200, &LeftFaceWallJump01 );       
}

void UpdateDusty_WallJump_Right()//Collided with Right Wall
  {                                                
    if (WallStickTimer == 0)
    {                   
        VerticalCounter = 0;          
        if (CollideWithRightSide == true)
        {
            DustyY = DustyY + DirectionY;
            if (CollideWithBottomSide == true )
            {	
                SetDustyState_Stand();
                return;
            }  
        }
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
		CanWallJump = true;
        SetDustyState_Jump( true );
		return;
    }
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_LEFT Implementation                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -// 

void SetDustyState_WallJump_Left()// Collided with Left Wall
{	
	CanWallJump = true;
	JumpLeftSprite = 0;
	JumpRightSprite = 0;
	WallStickTimer = 15;
	DustyState = DUSTYSTATE_WALLJUMP_LEFT;
} 

void DisplayDusty_WallJump_Left()
{
    gxDrawSprite( DustyX-145, DustyY-200, &RightFaceWallJump01 ); 
}

void UpdateDusty_WallJump_Left()
{
    if (WallStickTimer == 0)
    { 
        VerticalCounter = 0;
		if (CollideWithLeftSide == true)
		{
		   DustyY = DustyY + DirectionY;
           if (CollideWithBottomSide == true )
           {	
               SetDustyState_Stand();
               return;
           }  
		}
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
		CanWallJump = true;
        SetDustyState_Jump( true );
		return;
    }
    
} 
  
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UPDATE_REC_COLLISION Implementation                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

extern int ScrollY;

void UpdateDusty_Collision()
{
	CollideWithRightSide = false;
	CollideWithLeftSide = false;
	CollideWithBottomSide = false;
	CollideWithTopSide = false;

	// Collision with right side of the right screen and Dusty's right Side
	if (DustyX + DustyRight >= gxScreenWidth)
	{
		CollideWithRightSide = true;
		DustyX = gxScreenWidth - DustyRight;	
	} 

	//Collision with the left side of the screen    
	if (DustyX + DustyLeft <= 0)
	{
		CollideWithLeftSide = true;
		DustyX = -DustyLeft;
	}
	//Collision with the bottom side of the screen
	if (DustyY + DustyBottom >= gxScreenHeight )
	{	
		CollideWithBottomSide = true;
		DustyY = gxScreenHeight - DustyBottom;
	} 


	for (int y = 0; y < Chapter.Pages[0].Height; y++)
	{
		for (int x = 0; x < Chapter.Pages[0].Width; x++)
		{
			int BlockID = Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x];

			if (BlockID >= SPECIALBLOCKID_FIRST)
			{
				continue;
			}
			
			int BlockRight = x*64 + 64;
			int BlockLeft = x*64;
			int BlockTop = y*64 + ScrollY;
			int BlockBottom = y*64 + 64 + ScrollY;

			if(Max(DustyX + DustyLeft, BlockLeft) <= Min(DustyX + DustyRight, BlockRight))
			{
				if (Max(DustyY + DustyTop, BlockTop) <= Min(DustyY + DustyBottom, BlockBottom))
				{    
					int LeftDistance	= (DustyX + DustyRight)  - (BlockLeft);
					int RightDistance	= (BlockRight) - (DustyX +  DustyLeft);
					int DownDistance	= (BlockBottom)- (DustyY +  DustyTop );
					int UpDistance		= (DustyY + DustyBottom) - (BlockTop );

 					if (LeftDistance < RightDistance && LeftDistance < DownDistance && LeftDistance < UpDistance)
					{
						CollideWithRightSide = true;//Collision with Dusty's Right Side but the left side of the platform
						DustyX -= LeftDistance;
					}

					if (RightDistance < LeftDistance && RightDistance < DownDistance && RightDistance < UpDistance)
					{
						CollideWithLeftSide = true;//Collision with Dusty's Left Side but the right side of the platform
						DustyX += RightDistance;
					}

					if (DownDistance < RightDistance && DownDistance < LeftDistance && DownDistance < UpDistance)
					{
						CollideWithTopSide = true;//Collision with Dusty's Top Side but the Bottom side of the platform
						DustyY += DownDistance;
					}

					if (UpDistance < RightDistance && UpDistance < DownDistance && UpDistance < LeftDistance)
					{
						CollideWithBottomSide = true;//Collision with Dusty's Bottom Side but the Top side of the platform
						DustyY -= UpDistance;
					}
				}
			}
		}
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
	//gxDrawSprite( Platform1X, Platform1Y, &WoodBox_Platform01 );
	//gxDrawSprite( Platform2X, Platform2Y, &WoodBox_Platform01 );
}

void UpdatePlatform_Stationary(){}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Central Display and Update functions                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//


void Display()
{
    //if (BackgroundVisible == 1)
    //{
    //    gxDrawSprite( BackgroundX, BackgroundY, &Background01 );
   // }

	RenderChapter(&Chapter);

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Dusty Drawing                                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

	switch (DustyState)
	{
	case DUSTYSTATE_STAND:			    DisplayDusty_Stand(); break;
	case DUSTYSTATE_JUMP:			    DisplayDusty_Jump(); break;
	case DUSTYSTATE_FALL:				DisplayDusty_Fall(); break;
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
 

	DisplayBarrels();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                   Debugging aids                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	if (DevMode)
	{
		// Status of common variables
		gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "( %03d, %03d ) State: %d, Col: %d%d%d%d", DustyX, DustyY, DustyState, CollideWithLeftSide, CollideWithRightSide,
			CollideWithTopSide, CollideWithBottomSide);


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
		gxDrawString(Platform1X + PlatformLeft,  Platform1Y + PlatformTop,    8, color, "+");//Upper Left
		gxDrawString(Platform1X + PlatformRight, Platform1Y + PlatformTop,    8, color, "+");//Upper Right
		gxDrawString(Platform1X + PlatformLeft,  Platform1Y + PlatformBottom, 8, color, "+");//Lower Left
		gxDrawString(Platform1X + PlatformRight, Platform1Y + PlatformBottom, 8, color, "+");//Lower Right
	}
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
	
	UpdateDusty_Collision();

	switch (DustyState)
    {
		case DUSTYSTATE_STAND:			    UpdateDusty_Stand(); break;
		case DUSTYSTATE_JUMP:			    UpdateDusty_Jump(); break;
		case DUSTYSTATE_FALL:				UpdateDusty_Fall(); break;
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

	UpdateBarrels();

	return true;
}