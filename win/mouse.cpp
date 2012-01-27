/*--------------------------------------------------------------------------------------------------------

  Mouse Library v1.0 by Wade Brainerd
  
--------------------------------------------------------------------------------------------------------*/
#include "..\Common\Common.h"
#include "mouse.h"

#include "..\Common\Debug.h"
#include "..\Common\Tweak.h"

#include <windows.h>

#define NOWTFUNCTIONS
#include "wintab.h"

#define PACKETDATA	PK_X | PK_Y | PK_BUTTONS | PK_TIME | PK_NORMAL_PRESSURE
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

WTINFOA WTInfoA;
WTOPENA WTOpenA;
WTPACKET WTPacket;
WTPACKETSGET WTPacketsGet;

HINSTANCE hWintab;

extern HINSTANCE hInst;
extern HWND hWnd;

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

void msInit()
{
	HINSTANCE WintabDLL = LoadLibraryA("Wintab32.dll");
	if (!WintabDLL)
		return;

	WTOpenA = (WTOPENA)GetProcAddress(WintabDLL, "WTOpenA");
	WTInfoA = (WTINFOA)GetProcAddress(WintabDLL, "WTInfoA");
	WTPacket = (WTPACKET)GetProcAddress(WintabDLL, "WTPacket");
	WTPacketsGet = (WTPACKETSGET)GetProcAddress(WintabDLL, "WTPacketsGet");

	WTInfoA(WTI_DEFCONTEXT, 0, &lc);

	lc.lcPktData = PACKETDATA;
	lc.lcPktMode = PACKETMODE;
	lc.lcOptions = CXO_MESSAGES;
	WTOpenA(hWnd, &lc, TRUE);
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

extern int gxScreenWidth, gxScreenHeight;

int LastPacketPressure;

void msOnWTPacket(WPARAM wSerial, LPARAM hCtx)
{
	PACKET Packets[8];
	int NPackets = WTPacketsGet((HCTX)hCtx, 8, Packets);

	for (int i = 0; i < NPackets; i++)
	{
		PACKET* Pkt = &Packets[i];

		msNewX = (float)gxScreenWidth * ( ( ( Pkt->pkX - lc.lcInExtX/2 ) * Tweak.WacomXSensitivity + lc.lcInExtX/2 ) / lc.lcInExtX );
		msNewY = gxScreenHeight - (float)gxScreenHeight * ( ( ( Pkt->pkY - lc.lcInExtY/2 ) * Tweak.WacomYSensitivity + lc.lcInExtY/2 ) / lc.lcInExtY );

		msNewButton1 = Pkt->pkNormalPressure > 0;

		//LogMessage("PKT X=%f Y=%f Pressure=%d\n", msNewX, msNewY, Pkt->pkNormalPressure);

		if (Pkt->pkNormalPressure > 0 && LastPacketPressure == 0)
		{
			GetInput_BeginSwipe(msNewX, msNewY, Pkt->pkTime / 1000.0f);
		}
		else if (Pkt->pkNormalPressure == 0 && LastPacketPressure > 0)
		{
			GetInput_EndSwipe(msNewX, msNewY, Pkt->pkTime / 1000.0f);
		}
		else if (Pkt->pkNormalPressure > 0 && LastPacketPressure > 0)
		{
			GetInput_AddToSwipe(msNewX, msNewY, Pkt->pkTime / 1000.0f);
		}

		LastPacketPressure = Pkt->pkNormalPressure;
	}
}
