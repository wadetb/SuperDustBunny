/*--------------------------------------------------------------------------------------------------------

  Mouse Library v1.0 by Wade Brainerd
  
--------------------------------------------------------------------------------------------------------*/
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <assert.h>
#include "mouse.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

bool msInitialized = false;

int msOldButton1;
int msOldButton2;
int msButton1;
int msButton2;

int msX;
int msY;

int msXDelta;
int msYDelta;

int msRangeX1;
int msRangeY1;
int msRangeX2;
int msRangeY2;

LPDIRECTINPUTDEVICE msDIMouse = NULL;
LPDIRECTINPUT msDirectInput;

extern HWND hWnd;
extern HINSTANCE hInst;

void msInit()
{
    assert( !msInitialized );
    
    DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&msDirectInput, NULL );
    
    IDirectInput_CreateDevice( msDirectInput, GUID_SysMouse, &msDIMouse, NULL );
    assert( msDIMouse != NULL );
    
    IDirectInputDevice_SetDataFormat( msDIMouse, &c_dfDIMouse );
    IDirectInputDevice_SetCooperativeLevel( msDIMouse, hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
    
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DIPROPAXISMODE_REL;
    IDirectInputDevice_SetProperty( msDIMouse, DIPROP_AXISMODE, &dipdw.diph );
    
    IDirectInputDevice_Acquire( msDIMouse );
    
    msInitialized = true;
}

void msDeinit( void )
{
    if ( !msInitialized ) return;
    msInitialized = false;
    
    if ( msDIMouse ) 
    {
        IDirectInputDevice_Unacquire( msDIMouse );
        IDirectInputDevice_Release( msDIMouse );
        msDIMouse = NULL;
    }
    
    if ( msDirectInput )
    {
        IDirectInput_Release( msDirectInput );
        msDirectInput = NULL;
    }
}

extern int gxScreenWidth, gxScreenHeight;

void msUpdateMouse( void )
{
    HRESULT DIResult;
    DIMOUSESTATE MouseState;
    
    assert( msInitialized );
    
    msOldButton1 = msButton1;
    msOldButton2 = msButton2;
    
	// Attempt to re-acquire the mouse in case it was lost.  This allows Alt-Tab and the debugger to not break the mouse.
	IDirectInputDevice_Acquire( msDIMouse );

    DIResult = IDirectInputDevice_GetDeviceState( msDIMouse, sizeof(DIMOUSESTATE), &MouseState );
	if (DIResult != S_OK)
		return;
  
	msXDelta = MouseState.lX;
    msYDelta = MouseState.lY;
    msButton1 = MouseState.rgbButtons[0] & 0x80;
    msButton2 = MouseState.rgbButtons[1] & 0x80;
    
    //  Update the mouse position
    msX += msXDelta;
    msY += msYDelta;
    
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
    
    if ( msRangeX1 < 0 )
        msRangeX1 = 0;
    if ( msRangeX1 > gxScreenWidth )
        msRangeX1 = gxScreenWidth;
    
    if ( msRangeY2 < 0 )
        msRangeX1 = 0;
    if ( msRangeY2 > gxScreenHeight )
        msRangeX1 = gxScreenHeight;
    
    if ( msX < msRangeX1 ) msX = msRangeX1;
    if ( msX > msRangeX2 ) msX = msRangeX2;
    if ( msY < msRangeY1 ) msY = msRangeY1;
    if ( msY > msRangeY2 ) msY = msRangeY2;
}

void msSetMouseRange( int X1, int Y1, int X2, int Y2 )
{
    msRangeX1 = X1;
    msRangeY1 = Y1;
    msRangeX2 = X2;
    msRangeY2 = Y2;
}

