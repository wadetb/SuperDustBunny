#include "win/graphics.h"

void Init()
{
	gxInit(800, 600, true);
}
void Exit()
{
	gxDeinit();
}
void Display()
{
	gxDrawRect(0, 0, 400, 300, gxRGB32(0, 0, 255));
}
bool Update(){return true;}

