#include "win/graphics.h"
#include "win/mouse.h"

int BunnyX = 0;
int BunnyY = 0;
int DirectionX = 7;
int DirectionY = 3;
int BunnyWidth = 100;
int BunnyHeight = 100;
gxSprite BunnyHop01;

void Init()
{
	gxInit(800, 600, true);

	msInit();

	gxLoadBmp("Data/bunny hop0001.png", &BunnyHop01, 0);
}

void Exit()
{
	msDeinit();

	gxDeinit();
}

void Display()
{
	
	gxDrawSprite( BunnyX, BunnyY, &BunnyHop01 );

}

bool Update()
{
	msUpdateMouse();

	DirectionY += 1;
	
	BunnyX = BunnyX + DirectionX;
	BunnyY = BunnyY + DirectionY;

	if (BunnyX + BunnyWidth >= gxScreenWidth || BunnyX <= 0)
	{
		DirectionX = -DirectionX;
	}

	if (BunnyY + BunnyHeight >= gxScreenHeight || BunnyY <= 0)
	{
		DirectionY = -DirectionY;
	}

	if (BunnyY + BunnyHeight > gxScreenHeight )
		BunnyY = gxScreenHeight - BunnyHeight ;

	if (msButton1 && !msOldButton1)
	{
		DirectionY -= 10;
		//int Temp = DirectionX;
		//DirectionX = DirectionY;
		//DirectionY = -Temp;
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
