#include "win/graphics.h"
#include "win/mouse.h"
#include "win/keyboard.h"

int BunnyX = 350;
int BunnyY = 490;
int DirectionX = 7;
int DirectionY = 7;
int BunnyWidth = 100;
int BunnyHeight = 110;
int VerticalCounter = 0;
bool IsJumping = false;
int SetJumpDirection = 0;
int JumpRightSprite = 0;
int JumpLeftSprite = 0;
int StandingSprite = 0;
int HopRightSprite = 0;
int HopLeftSprite = 0;
int LastDirectionSprite = 0;
int SpriteTransition = 0;
int JumpRightTrans = 20;
int JumpLeftTrans = 20;
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

void Init()
{
	gxInit(800, 600, true);

	msInit();

	kbInit();

	gxLoadBmp("Data/bunny hop0001.png", &BunnyHop01, 0);	
	gxLoadBmp("Data/bunny hop0005.png", &BunnyHop02, 0);
	gxLoadBmp("Data/bunny hop0006.png", &BunnyHop03, 0);
	gxLoadBmp("Data/bunny hop0009.png", &BunnyHop04, 0);
	gxLoadBmp("Data/bunny hop0011.png", &BunnyHop05, 0);
	gxLoadBmp("Data/dustyhopleft0001.png", &LeftFaceStanding01, 0);
	gxLoadBmp("Data/dustyhopleft0001.png", &LeftFaceStanding02, 0);
	gxLoadBmp("Data/dustyhopleft0001.png",&DustyHopLeft01, 0);
	gxLoadBmp("Data/dustyhopleft0005.png",&DustyHopLeft02, 0);
	gxLoadBmp("Data/dustyhopleft0006.png",&DustyHopLeft03, 0);
    gxLoadBmp("Data/dustyhopleft0009.png",&DustyHopLeft04, 0);
    gxLoadBmp("Data/dustyhopleft0011.png",&DustyHopLeft05, 0);
}

void Exit()
{
	msDeinit();

	kbDeinit();

	gxDeinit();
}

void Display()
{      
	if (BunnyState == 1) //Jumping
	{
            if (JumpRightSprite == 1)     
            {
                gxDrawSprite( BunnyX, BunnyY, &BunnyHop02 );
            }
                
            if (JumpRightSprite == 2)
            {
                gxDrawSprite( BunnyX, BunnyY, &BunnyHop05 );
            }


            if (JumpLeftSprite == 1)
            {
               gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft02 );
            }

            if (JumpLeftSprite == 2)
            {
               gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft05 );
            }
	}


	if (BunnyState == 2) //Moving Right
	{
		if (HopRightSprite == 1) 
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
		} 

		if (HopRightSprite == 2)
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop02 );
		} 

		if (HopRightSprite == 3)
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop03 );
		}

		if (HopRightSprite == 4)
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop04 );
		}    
        if (HopRightSprite == 5)
        {
            gxDrawSprite( BunnyX, BunnyY, &BunnyHop05 );
        }     
       
        if (HopRightSprite == 6)
        {
            gxDrawSprite( BunnyX, BunnyY, &BunnyHop02 );
        }              
	}

	if (BunnyState == 3) //Moving Left
	{
		if (HopLeftSprite == 1) 
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft01 );
		} 

		if (HopLeftSprite == 2)
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft02 );
		} 

		if (HopLeftSprite == 3)
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft03 );
		}

		if (HopLeftSprite == 4)
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft04 );
		} 
        if (HopLeftSprite == 5)
        {
            gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft05 );
        }    
        if (HopLeftSprite == 6)
        {
            gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft02 );
        }                 
	}
   
    if (BunnyState == 0 && LastDirectionSprite == 0) //Standing
    {
        gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft01 );
    }

    if (BunnyState == 0 && LastDirectionSprite == 1)//After moving, stops to face the direction last moved.
    {
        gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
    }
}

bool Update()
{
	msUpdateMouse();

	kbUpdateKeys();

	gxUpdateScreen();

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
			SpriteTransition = 55;  
			JumpRightTrans = 20;
		}

		if ( kbIsKeyDown(KB_A) )
		{
			BunnyState = 3; //Moving Left
			HopLeftSprite = 1;
			SpriteTransition = 55;
			JumpLeftTrans = 20;   
		}
	}

	if (BunnyState == 1)//Jumping
	{		

        if (JumpRightTrans == 20)//Arrange for one animation, use an if/else statement based on transition. keep it simple     
        {
            JumpRightSprite = 1;
        }  

        if (JumpRightTrans == 10) 
        {
            JumpRightSprite = 2;
        }
        
        if (JumpRightTrans == 0)
        {
            JumpRightTrans = 20;
        }
        
        if (JumpLeftTrans == 20)
        {
            JumpLeftSprite = 1;
        }  

        if (JumpLeftTrans == 10) 
        {
            JumpLeftSprite = 2;
        } 

        if (JumpLeftTrans == 0)
        {
            JumpLeftTrans = 20;
        }
        
        //Update Animations
        JumpLeftTrans -= 1;
        JumpRightTrans -= 1;
		if (VerticalCounter == 0)
		{
			BunnyY = BunnyY + DirectionY;
		}
		else
		{     
			VerticalCounter -= 1;
			BunnyY -= 10;
		}

		if (BunnyY + BunnyHeight > gxScreenHeight )
		{	
			BunnyY = gxScreenHeight - BunnyHeight;
			BunnyState = 0; //Standing
		}

	     
	}   

	if (BunnyState == 2)//Moving right
	{
		BunnyX = BunnyX + DirectionX;  

		// Update animation
		SpriteTransition -= 1;

		// Collision with right side of screen
		if (BunnyX + BunnyWidth >= gxScreenWidth )
			BunnyX = gxScreenWidth - BunnyWidth;

		if ( !kbIsKeyDown(KB_D) )
		{			
            LastDirectionSprite = 1;                 
			BunnyState = 0; //Standing		
		}

		if (SpriteTransition == 55)
		{
			HopRightSprite = 1;
		}  

		if (SpriteTransition == 45) 
		{
			HopRightSprite = 2;
		} 

		if (SpriteTransition == 35)
		{
			HopRightSprite = 3;
		} 

		if (SpriteTransition == 25)
		{
			HopRightSprite = 4;
		}
        
        if (SpriteTransition == 15)
        {
            HopRightSprite = 5;
        }

        if (SpriteTransition == 5)
        {
            HopRightSprite = 6;
        }

		if (SpriteTransition == 0)
		{
			SpriteTransition = 55;
		}
	}

	if (BunnyState == 3)//Moving left
	{	
		BunnyX = BunnyX - DirectionX;	

		// Update animation
		SpriteTransition -= 1;

		// Collision with left side of screen
		if (BunnyX <= 0)
			BunnyX = 0;

		if ( !kbIsKeyDown(KB_A) )
		{
            LastDirectionSprite = 0;
			BunnyState = 0; //Standing		
		}

		if (SpriteTransition == 55)
		{
			HopLeftSprite = 1;
		}  

		if (SpriteTransition == 45) 
		{
			HopLeftSprite = 2;
		} 

		if (SpriteTransition == 35)
		{
			HopLeftSprite = 3;
		} 

		if (SpriteTransition == 25)
		{
			HopLeftSprite = 4;
		}
        
        if (SpriteTransition == 15)
        {
            HopLeftSprite = 5;
        }
        
        if (SpriteTransition == 5)
        {
            HopLeftSprite = 5;
        }
		
		if (SpriteTransition == 0)
		{
			SpriteTransition = 55;
		}
	}

	if (msButton2)
	{
		return false;
	}
	else
	{
		return true;
	}
}