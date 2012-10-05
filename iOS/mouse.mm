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
}

