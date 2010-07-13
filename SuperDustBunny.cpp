#include "win/graphics.h"
#include "win/mouse.h"
#include "win/keyboard.h"

bool SlowMotionMode = false;

int BunnyX = 350;
int BunnyY = 545;
int DirectionX = 7;
int DirectionY = 7;
int BunnyLeft = -10;
int BunnyRight = 80;
int BunnyTop = -80;
int BunnyBottom = 55;
int VerticalCounter = 0;
bool IsJumping = false;
int SetJumpDirection = 0;
int JumpRightSprite = 1;
int JumpLeftSprite = 0;
int FallRightSprite = 1;
int FallLeftSprite = 0;
int StandingSprite = 0;
int HopRightSprite = 0;
int HopLeftSprite = 0;
int LastDirectionSprite = 0;
int SpriteTransition = 0;
int BunnyState = 0;
gxSprite BunnyHop01;
gxSprite BunnyHop02;
gxSprite BunnyHop03;
gxSprite BunnyHop04;
gxSprite BunnyHop05;
gxSprite LeftFaceStanding01;
gxSprite LeftFaceStanding02;
gxSprite RightFaceStanding01;
gxSprite RightFaceStanding02;
gxSprite LeftFaceWallJump01;
gxSprite RightFaceWallJump01;
gxSprite BunnyDeath01;
gxSprite DustyHopLeft01;
gxSprite DustyHopLeft02;
gxSprite DustyHopLeft03;
gxSprite DustyHopLeft04;
gxSprite DustyHopLeft05;
gxSprite DustyHopLeft06;

void Init()
{
	gxInit(800, 600, true);

	kbInit();

	gxLoadBmp("Data/bunny hop0001.png", &BunnyHop01, 0);
	gxLoadBmp("Data/bunny hop0011.png", &BunnyHop02, 0);	
	gxLoadBmp("Data/bunny hop0005.png", &BunnyHop03, 0);
	gxLoadBmp("Data/bunny hop0006.png", &BunnyHop04, 0);
	gxLoadBmp("Data/bunny hop0009.png", &BunnyHop05, 0);	
	gxLoadBmp("Data/dustyhopleft0001.png", &LeftFaceStanding01, 0);
	gxLoadBmp("Data/dustyhopleft0001.png", &LeftFaceStanding02, 0);
	gxLoadBmp("Data/dustyhopleft0001.png",&DustyHopLeft01, 0);
	gxLoadBmp("Data/dustyhopleft0011.png",&DustyHopLeft02, 0);
	gxLoadBmp("Data/dustyhopleft0005.png",&DustyHopLeft03, 0);
	gxLoadBmp("Data/dustyhopleft0006.png",&DustyHopLeft04, 0);
    gxLoadBmp("Data/dustyhopleft0009.png",&DustyHopLeft05, 0);
    gxLoadBmp("Data/dustyhoplefttest.png",&DustyHopLeft06, 0);
    
}

void Exit()
{
	kbDeinit();

	gxDeinit();
}

void Display()
{      
	if (BunnyState == 1) //Jumping
	{
            if (JumpRightSprite == 1)     
            {
                gxDrawSprite( BunnyX-25, BunnyY-63, &BunnyHop03 );
            }
              
            if (FallRightSprite == 1)
            {
                gxDrawSprite( BunnyX-25, BunnyY-63, &BunnyHop02 );
            }  
            
            if (JumpLeftSprite == 1)
            {
               gxDrawSprite( BunnyX-23, BunnyY-64, &DustyHopLeft03 );
            }
            
            if (FallLeftSprite == 1)
            {
                gxDrawSprite( BunnyX-23, BunnyY-64, &DustyHopLeft02 );
            }
	}
	if (BunnyState == 2) //Moving Right
	{
		if (HopRightSprite == 1) 
		{
			gxDrawSprite( BunnyX-32, BunnyY-58, &BunnyHop01 );
		} 

		if (HopRightSprite == 2)
		{
			gxDrawSprite( BunnyX-28, BunnyY-62, &BunnyHop02 );
		} 

		if (HopRightSprite == 3)
		{
			gxDrawSprite( BunnyX-25, BunnyY-63, &BunnyHop03 );
		}

		if (HopRightSprite == 4)
		{
			gxDrawSprite( BunnyX-37, BunnyY-46, &BunnyHop04 );
		}    
       
        if (HopRightSprite == 5)
        {
            gxDrawSprite( BunnyX-41, BunnyY-74, &BunnyHop05 );
        }     
       
        if (HopRightSprite == 6)
        {
            gxDrawSprite( BunnyX-32, BunnyY-58, &BunnyHop01 );
        }               
	}

	if (BunnyState == 3) //Moving Left
	{
		if (HopLeftSprite == 1) 
		{
			gxDrawSprite( BunnyX-32, BunnyY-59, &DustyHopLeft01 );
		} 

		if (HopLeftSprite == 2)
		{
			gxDrawSprite( BunnyX-27, BunnyY-60, &DustyHopLeft02 );
		} 

		if (HopLeftSprite == 3)
		{
			gxDrawSprite( BunnyX-23, BunnyY-64, &DustyHopLeft03 );
		}

		if (HopLeftSprite == 4)
		{
			gxDrawSprite( BunnyX-34, BunnyY-45, &DustyHopLeft04 );
		} 
        if (HopLeftSprite == 5)
        {
            gxDrawSprite( BunnyX-41, BunnyY-70, &DustyHopLeft06 );
        }    
        if (HopLeftSprite == 6)
        {
            gxDrawSprite( BunnyX-32, BunnyY-59, &DustyHopLeft01 );
        }              
	}
   
    if (BunnyState == 0 && LastDirectionSprite == 0) //Standing
    {
        gxDrawSprite( BunnyX-32, BunnyY-59, &DustyHopLeft01 );
    }

    if (BunnyState == 0 && LastDirectionSprite == 1)//After moving, stops to face the direction last moved.
    {
        gxDrawSprite( BunnyX-32, BunnyY-58, &BunnyHop01 );
    }
}

bool Update()
{
	kbUpdateKeys();

	gxUpdateScreen();

	// Pressing escape quits the program.
	if (kbIsKeyDown(KB_ESCAPE))
	{
		return false;
	}

	// Backspace toggles slow motion mode.
	if (kbIsKeyDown(KB_BACKSLASH) && !kbWasKeyDown(KB_BACKSLASH))
	{
		SlowMotionMode = !SlowMotionMode;
	}

	if (SlowMotionMode)
	{
		bool StepOneFrame;
		if (kbIsKeyDown(KB_RETURN) && !kbWasKeyDown(KB_RETURN))
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

	if (BunnyState == 0)//Standing Still, include Space bar check.
	{
		if ( kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE) )      
		{ 			
			VerticalCounter = 20;
			BunnyY -= 10;    
			BunnyState = 1; //Jumping
		}

		if ( kbIsKeyDown(KB_D) )
		{
			BunnyState = 2; //Moving Right
			HopRightSprite = 1;
			SpriteTransition = 30;  
			JumpRightSprite = 1;
		}

		if ( kbIsKeyDown(KB_A) )
		{
			BunnyState = 3; //Moving Left
			HopLeftSprite = 1;
			SpriteTransition = 30;
			JumpLeftSprite = 1;   
		}
	}

	if (BunnyState == 1)//Jumping
	{		   
		if (VerticalCounter == 0)
		{
			BunnyY = BunnyY + DirectionY;
            JumpLeftSprite = 0;
            FallLeftSprite = 0;
            JumpRightSprite = 0;
            FallRightSprite = 1;

            if (LastDirectionSprite == 0)
            {
                JumpRightSprite = 0;
                JumpLeftSprite = 0;
                FallRightSprite = 0;
                FallLeftSprite = 1;
            }
		}
		else
		{     
			VerticalCounter -= 1;
			BunnyY -= 10;
		}

		if (BunnyY + BunnyBottom > gxScreenHeight )
		{	
			BunnyY = gxScreenHeight - BunnyBottom;
			BunnyState = 0; //Standing
		}    
	}   

	if (BunnyState == 2)//Moving right
	{
		BunnyX = BunnyX + DirectionX;  

		// Update animation
		SpriteTransition -= 1;
		JumpLeftSprite = 0;
		FallLeftSprite = 0;
		JumpRightSprite = 1;

		// Collision with right side of screen
		if (BunnyX + BunnyRight >= gxScreenWidth )
			BunnyX = gxScreenWidth - BunnyRight;

		if ( !kbIsKeyDown(KB_D) && SpriteTransition == 0 )
		{			                         
			BunnyState = 0; //Standing
			LastDirectionSprite = 1;		
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

	if (BunnyState == 3)//Moving left
	{	
		BunnyX = BunnyX - DirectionX;	

		// Update animation
		SpriteTransition -= 1;
		JumpRightSprite = 0;
		FallRightSprite = 0;
		JumpLeftSprite = 1;

		// Collision with left side of screen
		if (BunnyX + BunnyLeft <= 0)
			BunnyX = 0;

		if ( !kbIsKeyDown(KB_A) && SpriteTransition == 0 )
		{          
			BunnyState = 0; //Standing
			LastDirectionSprite = 0;	
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
			SpriteTransition = 40;
		}
	}

	return true;
}