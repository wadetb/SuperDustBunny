/*--------------------------------------------------------------------------------------------------------

  Mouse Library v1.0 by Wade Brainerd
  
--------------------------------------------------------------------------------------------------------*/
#include "mouse.h"

#include <windows.h>

#include "wintab.h"

#define PACKETDATA	PK_X | PK_Y | PK_BUTTONS
#define PACKETMODE	0				
#include "pktdef.h"

typedef UINT ( API * WTINFOA ) ( UINT, UINT, LPVOID );
typedef HCTX ( API * WTOPENA )( HWND, LPLOGCONTEXTA, BOOL );
typedef BOOL ( API * WTGETA ) ( HCTX, LPLOGCONTEXT );
typedef BOOL ( API * WTSETA ) ( HCTX, LPLOGCONTEXT );
typedef BOOL ( API * WTCLOSE ) ( HCTX );
typedef BOOL ( API * WTENABLE ) ( HCTX, BOOL );
typedef BOOL ( API * WTPACKET ) ( HCTX, UINT, LPVOID );
typedef BOOL ( API * WTOVERLAP ) ( HCTX, BOOL );
typedef BOOL ( API * WTSAVE ) ( HCTX, LPVOID );
typedef BOOL ( API * WTCONFIG ) ( HCTX, HWND );
typedef HCTX ( API * WTRESTORE ) ( HWND, LPVOID, BOOL );
typedef BOOL ( API * WTEXTSET ) ( HCTX, UINT, LPVOID );
typedef BOOL ( API * WTEXTGET ) ( HCTX, UINT, LPVOID );
typedef BOOL ( API * WTQUEUESIZESET ) ( HCTX, int );
typedef int  ( API * WTDATAPEEK ) ( HCTX, UINT, UINT, int, LPVOID, LPINT);
typedef int  ( API * WTPACKETSGET ) (HCTX, int, LPVOID);

WTINFOA gpWTInfoA = NULL;
WTOPENA gpWTOpenA = NULL;
WTPACKET gpWTPacket = NULL;

HINSTANCE hWintab = NULL;

extern HINSTANCE hInst;
extern HWND hWnd;

HCTX hCtx;
LOGCONTEXT lc;

int msOldButton1;
int msOldButton2;
int msButton1;
int msButton2;

float msX;
float msY;

float msXDelta;
float msYDelta;

float msNewX;
float msNewY;
int msNewButton1;
int msNewButton2;

#define GETPROCADDRESS(type, func) gp##func = (type)GetProcAddress(hWintab, #func);

void msInit()
{
	hWintab = LoadLibraryA("Wintab32.dll");
	if (!hWintab)
		return;

	GETPROCADDRESS( WTOPENA, WTOpenA );
	GETPROCADDRESS( WTINFOA, WTInfoA );
	GETPROCADDRESS( WTPACKET, WTPacket );

	gpWTInfoA(WTI_DEFCONTEXT, 0, &lc);

	lc.lcPktData = PACKETDATA;
	lc.lcPktMode = PACKETMODE;
	lc.lcOptions = CXO_MESSAGES;
	hCtx = gpWTOpenA(hWnd, &lc, TRUE);
}

void msDeinit( void )
{
}

void msUpdateMouse( void )
{
	msXDelta = msNewX - msX;
	msYDelta = msNewY - msY;

	msX = msNewX;
	msY = msNewY;

	msOldButton1 = msButton1;
	msOldButton2 = msButton2;
	msButton1 = msNewButton1;
	msButton2 = msNewButton2;
}

void GetInput_BeginSwipe(float X, float Y, double Time);
void GetInput_AddToSwipe(float X, float Y, double Time);
void GetInput_EndSwipe(float X, float Y, double Time);

#undef GetCurrentTime
double GetCurrentTime();

void msOnWTPacket(WPARAM wSerial, LPARAM hCtx)
{
	PACKET pkt;
	gpWTPacket((HCTX)hCtx, wSerial, &pkt);

	msNewX = (float)pkt.pkX / lc.lcInExtX;
	msNewY = (float)pkt.pkY / lc.lcInExtY;

	msNewButton1 = pkt.pkButtons;
	msNewButton2 = pkt.pkButtons;

	if (msNewButton1 && !msButton1)
	{
		GetInput_BeginSwipe(msNewX, msNewY, GetCurrentTime());
		return;
	}
	if (!msNewButton1 && msButton1)
	{
		GetInput_EndSwipe(msNewX, msNewY, GetCurrentTime());
		return;
	}
	if (msNewButton1 && msButton1)
	{
		GetInput_AddToSwipe(msNewX, msNewY, GetCurrentTime());
	}
}
