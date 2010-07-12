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
int SetStandingSprite = 0;
int SetHopRightSprite = 0;
int SetHopLeftSprite = 0;
int LastSprite = 1;
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
	gxLoadBmp("Data/bunny hop0001.png", &LeftFaceStanding01, 0);
	gxLoadBmp("Data/bunny hop0001.png", &LeftFaceStanding02, 0);
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
	if (BunnyState == 0) //Standing
	{
		gxDrawSprite( BunnyX, BunnyY, &LeftFaceStanding01 );
	}

	if (BunnyState == 1) //Jumping
	{
		gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
	}

	if (BunnyState == 2) //Moving Right
	{
		if (SetHopRightSprite == 1) 
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
		} 

		if (SetHopRightSprite == 2)
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop02 );
		} 

		if (SetHopRightSprite == 3)
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop03 );
		}

		if (SetHopRightSprite == 4)
		{
			gxDrawSprite( BunnyX, BunnyY, &BunnyHop04 );
		}    
        if (SetHopRightSprite == 5)
        {
            gxDrawSprite( BunnyX, BunnyY, &BunnyHop05 );
        }     
       
        if (SetHopRightSprite == 6)
        {
            gxDrawSprite( BunnyX, BunnyY, &BunnyHop02 );
        }              
	}

	if (BunnyState == 3) //Moving Left
	{
		if (SetHopLeftSprite == 1) 
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft01 );
		} 

		if (SetHopLeftSprite == 2)
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft02 );
		} 

		if (SetHopLeftSprite == 3)
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft03 );
		}

		if (SetHopLeftSprite == 4)
		{
			gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft04 );
		} 
        if (SetHopLeftSprite == 5)
        {
            gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft05 );
        }    
        if (SetHopLeftSprite == 6)
        {
            gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft02 );
        }                 
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

			SetHopRightSprite = 1;
			SpriteTransition = 55;     
		}

		if ( kbIsKeyDown(KB_A) )
		{
			BunnyState = 3; //Moving Left

			SetHopLeftSprite = 1;
			SpriteTransition = 55;     
		}
	}

	if (BunnyState == 1)//Jumping
	{
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
			BunnyState = 0; //Standing
		}

		if (SpriteTransition == 55)
		{
			SetHopRightSprite = 1;
		}  

		if (SpriteTransition == 45) 
		{
			SetHopRightSprite = 2;
		} 

		if (SpriteTransition == 35)
		{
			SetHopRightSprite = 3;
		} 

		if (SpriteTransition == 25)
		{
			SetHopRightSprite = 4;
		}
        
        if (SpriteTransition == 15)
        {
            SetHopRightSprite = 5;
        }

        if (SpriteTransition == 5)
        {
            SetHopRightSprite = 6;
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
			BunnyState = 0; //Standing
		}

		if (SpriteTransition == 55)
		{
			SetHopLeftSprite = 1;
		}  

		if (SpriteTransition == 45) 
		{
			SetHopLeftSprite = 2;
		} 

		if (SpriteTransition == 35)
		{
			SetHopLeftSprite = 3;
		} 

		if (SpriteTransition == 25)
		{
			SetHopLeftSprite = 4;
		}
        
        if (SpriteTransition == 15)
        {
            SetHopLeftSprite = 5;
        }
        
        if (SpriteTransition == 5)
        {
            SetHopLeftSprite = 5;
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