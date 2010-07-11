/*--------------------------------------------------------------------------------------------------------

  Keyboard Library v1.0 by Wade Brainerd
  
--------------------------------------------------------------------------------------------------------*/
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <assert.h>
#include "keyboard.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

bool kbInitialized = false;

BYTE kbKeyStatus[256];  //  The reported status of each key.
BYTE kbOldKeyStatus[256]; //  The previous reported status of each key.

LPDIRECTINPUTDEVICE kbDIKeyboard = NULL;
LPDIRECTINPUT kbDirectInput;

extern HWND hWnd;
extern HINSTANCE hInst;

void kbInit( void )
{
    assert( !kbInitialized );
    memset( kbKeyStatus, 0, sizeof(kbKeyStatus) );
    
	DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&kbDirectInput, NULL );
    
    IDirectInput_CreateDevice( kbDirectInput, GUID_SysKeyboard, &kbDIKeyboard, NULL );
    assert( kbDIKeyboard != NULL );
    
    IDirectInputDevice_SetDataFormat( kbDIKeyboard, &c_dfDIKeyboard );
    IDirectInputDevice_SetCooperativeLevel( kbDIKeyboard, hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
    
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = 256;
    IDirectInputDevice_SetProperty( kbDIKeyboard, DIPROP_BUFFERSIZE, &dipdw.diph );
    
    IDirectInputDevice_Acquire( kbDIKeyboard );
    
    kbInitialized = true;
}

void kbDeinit( void )
{
    if ( !kbInitialized ) return;
    
    if ( kbDIKeyboard ) 
    {
        IDirectInputDevice_Unacquire( kbDIKeyboard );
        IDirectInputDevice_Release( kbDIKeyboard );
        kbDIKeyboard = NULL;
    }
    
    if ( kbDirectInput )
    {
        IDirectInput_Release( kbDirectInput );
        kbDirectInput = NULL;
    }
    
    kbInitialized = false;
}

void kbUpdateKeys( void )
{
    assert( kbInitialized );
    
	// Attempt to re-acquire the keyboard in case it was lost.  This allows Alt-Tab and the debugger to not break the keyboard.
	IDirectInputDevice_Acquire( kbDIKeyboard );

    memcpy( kbOldKeyStatus, kbKeyStatus, sizeof(kbOldKeyStatus) );
    IDirectInputDevice_GetDeviceState( kbDIKeyboard, sizeof(kbKeyStatus), kbKeyStatus );
}

bool kbIsKeyDown(int key)
{
	return kbKeyStatus[key] != 0;
}

bool kbWasKeyDown(int key)
{
	return kbOldKeyStatus[key] != 0;
}
