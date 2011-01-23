#include "mouse.h"

int msButton1;
int msButton2;
int msOldButton1;
int msOldButton2;

int msX;
int msY;

int msXDelta;
int msYDelta;

float msAccelX;
float msAccelY;
float msAccelZ;

int msRangeX1;
int msRangeY1;
int msRangeX2;
int msRangeY2;

extern int gxScreenWidth;
extern int gxScreenHeight;

int _msNewX;
int _msNewY;
int _msNewButton1;

float _msNewAccelX;
float _msNewAccelY;
float _msNewAccelZ;

void msInit()
{
}

void msExit()
{
}

void msUpdateMouse()
{
    msOldButton1 = msButton1;
    msOldButton2 = msButton2;
    
    msXDelta = _msNewX - msX;
    msYDelta = _msNewY - msY;
	msX = _msNewX;
	msY = _msNewY;

    msButton1 = _msNewButton1;
    
	msAccelX = _msNewAccelX;
	msAccelY = _msNewAccelY;
	msAccelZ = _msNewAccelZ;
	
    //  Perform the range check
    if ( msRangeX2 < msRangeX1 )
    {
        int Temp = msRangeX1;
        msRangeX1 = msRangeX2;
        msRangeX2 = Temp;
    }
    
    if ( msRangeY2 < msRangeY1 )
    {
        int Temp = msRangeY1;
        msRangeY1 = msRangeY2;
        msRangeY2 = Temp;
    }
    
    if ( msRangeX1 < 0 ) msRangeX1 = 0;
    if ( msRangeY2 < 0 ) msRangeX1 = 0;
    if ( msRangeX1 > gxScreenWidth ) msRangeX1 = gxScreenWidth;   
    if ( msRangeY2 > gxScreenHeight ) msRangeX1 = gxScreenHeight;
    
    if ( msX < msRangeX1 ) msX = msRangeX1;
    if ( msX > msRangeX2 ) msX = msRangeX2;
    if ( msY < msRangeY1 ) msY = msRangeY1;
    if ( msY > msRangeY2 ) msY = msRangeY2;
}

void msSetMouseRange( int x1, int y1, int x2, int y2 )
{
	msRangeX1 = x1;
	msRangeY1 = y1;
	msRangeX2 = x2;
	msRangeY2 = y2;
}

