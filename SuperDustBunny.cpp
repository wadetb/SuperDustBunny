#include "win/graphics.h"
#include "win/mouse.h"
#include "win/keyboard.h"

int BunnyX = 350;
int BunnyY = 600;
int DirectionX = 7;
int DirectionY = 7;
int BunnyWidth = 100;
int BunnyHeight = 110;
int Climb = 0;
gxSprite BunnyHop01;
gxSprite DustyHopLeft01;
gxSprite BunnyHop02;
gxSprite BunnyHop03;
gxSprite BunnyHop04;
gxSprite BunnyHop05;
gxSprite Standing01;
gxSprite Standing02;
gxSprite WallJump01;
gxSprite BunnyDeath01;

void Init()
{
	gxInit(800, 600, true);

	msInit();
	
	kbInit();
	
	gxLoadBmp("Data/bunny hop0001.png", &BunnyHop01, 0);
	gxLoadBmp("Data/dustyhopleft0001.png", &DustyHopLeft01, 0);
	gxLoadBmp("Data/bunny hop0005.png", &BunnyHop02, 0);
	gxLoadBmp("Data/bunny hop0006.png", &BunnyHop03, 0);
	gxLoadBmp("Data/bunny hop0009.png", &BunnyHop04, 0);
	gxLoadBmp("Data/bunny standing0012.png", &Standing01, 0);
}

void Exit()
{
	msDeinit();
	
	kbDeinit();

	gxDeinit();
}

void Display()
{
    gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );
    //gxDrawSprite( BunnyX, BunnyY, &DustyHopLeft01 );
}

bool Update()
{
	msUpdateMouse();
	
	kbUpdateKeys();
	
	gxUpdateScreen();
	
    if (BunnyY != gxScreenHeight - BunnyHeight)
        if (Climb == 0)
    {
        BunnyY = BunnyY + DirectionY;
    }
        else
        {     
            Climb -= 1;
            BunnyY -= 10;
        }
        
	//DirectionY += 2;
	
	//BunnyX = BunnyX + DirectionX;
	//BunnyY = BunnyY + DirectionY;

	if (BunnyX + BunnyWidth >= gxScreenWidth )
	    BunnyX = gxScreenWidth - BunnyWidth;
		
    if (BunnyX <= 0)
        BunnyX = 0;
   
	//if (BunnyY + BunnyHeight >= gxScreenHeight || BunnyY <= 0)
	//{
	//	DirectionY = -DirectionY;
	//}

	if (BunnyY + BunnyHeight > gxScreenHeight )
		BunnyY = gxScreenHeight - BunnyHeight ;
		
	//if (msButton1 && !msOldButton1)
	//{
	//	DirectionY -= 100;
		//int Temp = DirectionX;
		//DirectionX = DirectionY;
		//DirectionY = -Temp;
	//}

    if ( kbIsKeyDown(KB_D) )//&& !kbWasKeyDown(KB_D)
    {
        BunnyX = BunnyX + DirectionX;	
    }
    
    if ( kbIsKeyDown(KB_A) )//&& !kbWasKeyDown(KB_A)
    {
        BunnyX = BunnyX - DirectionX;	
    }
              
    if ( kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE) )
    {  
         Climb = 20;
         BunnyY -= 10;
    }
    
	if (msButton2)
	{
		return false;
	}
	else
	{
		return true;
	}
   
	if ( kbIsKeyDown(KB_D) )
	{
	    return false;
	}
	else
	{
	    return true;
	}
	
    if ( kbIsKeyDown(KB_A) )
    {
        return false;
    }
    else
    {
        return true;
    }

    if ( kbIsKeyDown(KB_SPACE) )
    {
        return false;
    }
    else
    {
        return true;
    }
}
