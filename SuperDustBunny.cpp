#include "win/graphics.h"
#include "win/mouse.h"
#include "win/keyboard.h"

int BunnyX = 350;
int BunnyY = 600;
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


void Init()
{
	gxInit(800, 600, true);

	msInit();
	
	kbInit();
	
	gxLoadBmp("Data/bunny hop0001.png", &BunnyHop01, 0);	
	gxLoadBmp("Data/bunny hop0005.png", &BunnyHop02, 0);
	gxLoadBmp("Data/bunny hop0006.png", &BunnyHop03, 0);
	gxLoadBmp("Data/bunny hop0009.png", &BunnyHop04, 0);
    gxLoadBmp("Data/bunny standing0012.png", &LeftFaceStanding01, 0);
    gxLoadBmp("Data/bunny standing0012.png", &LeftFaceStanding02, 0);
    gxLoadBmp("Data/dustyhopleft0001.png",&DustyHopLeft01, 0);
    gxLoadBmp("Data/dustyhopleft0001.png",&DustyHopLeft02, 0);
    gxLoadBmp("Data/dustyhopleft0001.png",&DustyHopLeft03, 0);
    gxLoadBmp("Data/dustyhopleft0001.png",&DustyHopLeft04, 0);
}

void Exit()
{
	msDeinit();
	
	kbDeinit();

	gxDeinit();
}

void Display()
{      
    kbUpdateKeys();

    gxUpdateScreen();
     
    if (kbIsKeyDown(KB_D))// && !kbWasKeyDown(KB_D) )
    {
        gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
    }     
    if (kbIsKeyDown(KB_D) && SpriteTransition == 10)
    {
        gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
    }  
            
    if (kbIsKeyDown(KB_D) && SpriteTransition == 8) 
    {
        gxDrawSprite( BunnyX, BunnyY, &BunnyHop02 );
    } 
    
    if (kbIsKeyDown(KB_D) && SpriteTransition == 6)
    {
        gxDrawSprite( BunnyX, BunnyY, &BunnyHop03 );
    } 
       
    if (kbIsKeyDown(KB_D) && SpriteTransition == 4)
    {
        gxDrawSprite( BunnyX, BunnyY, &BunnyHop04 );
    }
    
    if (kbIsKeyDown(KB_D) && SpriteTransition == 2)
    {
        gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
    }                 
             
   if (kbIsKeyDown(KB_A))// && !kbWasKeyDown(KB_A))
    {
        gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft01 );
    }
    
   //if (LastSprite == 1)
   // {
   //     gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
   // }
   // else
   // {
  //     gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft01 );
   // }
   
}

bool Update()
{
	msUpdateMouse();
	
	kbUpdateKeys();
	
	gxUpdateScreen();
		
    if (BunnyY != gxScreenHeight - BunnyHeight)      
        if (VerticalCounter == 0)
        {
            BunnyY = BunnyY + DirectionY;
        }
        else
        {     
            VerticalCounter -= 1;
            BunnyY -= 10;
        }
        
	if (BunnyX + BunnyWidth >= gxScreenWidth )
	    BunnyX = gxScreenWidth - BunnyWidth;
	      	
    if (BunnyX <= 0)
        BunnyX = 0;
   
	if (BunnyY + BunnyHeight > gxScreenHeight )
	{	
        BunnyY = gxScreenHeight - BunnyHeight;
    }        
        if (BunnyY == gxScreenHeight - BunnyHeight)
        {
            IsJumping = false;
        }
				
    if ( kbIsKeyDown(KB_D) )//&& !kbWasKeyDown(KB_D)
    {
        BunnyX = BunnyX + DirectionX;
         
   }
    
    if ( kbIsKeyDown(KB_A) )//&& !kbWasKeyDown(KB_A)
    {
        BunnyX = BunnyX - DirectionX;	
    }
  
    if (IsJumping == true && kbIsKeyDown (KB_SPACE) && !kbWasKeyDown(KB_SPACE) )   
    {
        IsJumping = true;
    }
    else              
       if ( IsJumping == false && kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE) )      
        { 
            VerticalCounter = 20;
            BunnyY -= 10;
            IsJumping = true;
        }   
            
    if (kbIsKeyDown(KB_A))// && !kbWasKeyDown(KB_A))
    {
        LastSprite = 0;
    }
    
    if (kbIsKeyDown(KB_D))// && !kbWasKeyDown(KB_D) )
    {      
        LastSprite = 1;
        SetHopRightSprite = 1;
        SpriteTransition = 300;     
    }  
    
    if (kbIsKeyDown(KB_D) && SpriteTransition == 300)
    {
        SetHopRightSprite = 2;
        SpriteTransition -= 1;
    }  

    if (kbIsKeyDown(KB_D) && SpriteTransition == 240) 
    {
        SetHopRightSprite = 3;
        SpriteTransition -= 1;
    } 

    if (kbIsKeyDown(KB_D) && SpriteTransition == 170)
    {
        SetHopRightSprite = 4;
        SpriteTransition -= 1;
    } 

    if (kbIsKeyDown(KB_D) && SpriteTransition == 110)
    {
        SetHopRightSprite = 5;
        SpriteTransition -= 1;
    }

    if (kbIsKeyDown(KB_D) && SpriteTransition == 55)
    {
        SetHopRightSprite = 0;
        SpriteTransition = 0;
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