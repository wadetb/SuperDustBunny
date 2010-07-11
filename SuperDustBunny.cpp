#include "win/graphics.h"
#include "win/mouse.h"

int BoxX = 0;
int BoxY = 0;
int DirectionX = 7;
int DirectionY = 3;
int BoxWidth = 100;
int BoxHeight = 100;

void Init()
{
	gxInit(800, 600, true);

	msInit();
}

void Exit()
{
	msDeinit();

	gxDeinit();
}

void Display()
{
	gxDrawRect(0, 0, 400, 300, gxRGB32(0, 0, 255));

	gxDrawRect(BoxX, BoxY, BoxWidth, BoxHeight, gxRGB32(255, 0, 0));
	//gxDrawRect(msX, msY, 100, 100, gxRGB32(255, 0, 0));
}

bool Update()
{
	msUpdateMouse();

	DirectionY += 1;
	
	BoxX = BoxX + DirectionX;
	BoxY = BoxY + DirectionY;

	if (BoxX + BoxWidth >= gxScreenWidth || BoxX <= 0)
	{
		DirectionX = -DirectionX;
	}

	if (BoxY + BoxHeight >= gxScreenHeight || BoxY <= 0)
	{
		DirectionY = -DirectionY;
	}

	if (BoxY + BoxHeight > gxScreenHeight )
		BoxY = gxScreenHeight - BoxHeight ;

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
