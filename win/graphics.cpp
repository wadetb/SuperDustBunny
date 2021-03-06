#define _CRT_SECURE_NO_WARNINGS

#include "../Common/Common.h"
#include "graphics.h"

#include "mouse.h"

#define D3D_DEBUG_INFO

#include <d3d9.h>
#include <d3dx9.h>
#include <shlwapi.h>
#include <windowsx.h>
#include "wintab.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"winmm.lib")

#pragma warning(disable:4244)

#include <assert.h>
#include <stdio.h>

HINSTANCE hInst;
HWND hWnd;

int gxWindowActive;

int gxScreenWidth = 0;
int gxScreenHeight = 0;

bool gxQuitRequested = false;

IDirect3D9* gxD3D = NULL;
IDirect3DDevice9* gxDev = NULL; 

extern int gxViewX1 = 0;
extern int gxViewY1 = 0;
extern int gxViewX2 = 0;
extern int gxViewY2 = 0;

// 8x8 monochrome font from 32 to 127 by Richard Mitton
BYTE gxFontData[768] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00,
	0x00, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x7e, 0x24, 0x24, 0x7e, 0x24, 0x00,
	0x00, 0x08, 0x3e, 0x28, 0x3e, 0x0a, 0x3e, 0x08, 0x00, 0x62, 0x64, 0x08, 0x10, 0x26, 0x46, 0x00,
	0x00, 0x10, 0x28, 0x10, 0x2a, 0x44, 0x3a, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x00, 0x00, 0x20, 0x10, 0x10, 0x10, 0x10, 0x20, 0x00,
	0x00, 0x00, 0x14, 0x08, 0x3e, 0x08, 0x14, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00,
	0x00, 0x3c, 0x46, 0x4a, 0x52, 0x62, 0x3c, 0x00, 0x00, 0x18, 0x28, 0x08, 0x08, 0x08, 0x3e, 0x00,
	0x00, 0x3c, 0x42, 0x02, 0x3c, 0x40, 0x7e, 0x00, 0x00, 0x3c, 0x42, 0x0c, 0x02, 0x42, 0x3c, 0x00,
	0x00, 0x08, 0x18, 0x28, 0x48, 0x7e, 0x08, 0x00, 0x00, 0x7e, 0x40, 0x7c, 0x02, 0x42, 0x3c, 0x00,
	0x00, 0x3c, 0x40, 0x7c, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x7e, 0x02, 0x04, 0x08, 0x10, 0x10, 0x00,
	0x00, 0x3c, 0x42, 0x3c, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x3e, 0x02, 0x3c, 0x00,
	0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x10, 0x20,
	0x00, 0x00, 0x04, 0x08, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x10, 0x08, 0x04, 0x08, 0x10, 0x00, 0x00, 0x3c, 0x42, 0x04, 0x08, 0x00, 0x08, 0x00,
	0x00, 0x3c, 0x4a, 0x56, 0x5e, 0x40, 0x3c, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x00,
	0x00, 0x7c, 0x42, 0x7c, 0x42, 0x42, 0x7c, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x42, 0x3c, 0x00,
	0x00, 0x78, 0x44, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x7e, 0x40, 0x7c, 0x40, 0x40, 0x7e, 0x00,
	0x00, 0x7e, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x4e, 0x42, 0x3c, 0x00,
	0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00,
	0x00, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x00,
	0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00, 0x42, 0x66, 0x5a, 0x42, 0x42, 0x42, 0x00,
	0x00, 0x42, 0x62, 0x52, 0x4a, 0x46, 0x42, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00,
	0x00, 0x7c, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x52, 0x4a, 0x3c, 0x00,
	0x00, 0x7c, 0x42, 0x42, 0x7c, 0x44, 0x42, 0x00, 0x00, 0x3c, 0x40, 0x3c, 0x02, 0x42, 0x3c, 0x00,
	0x00, 0xfe, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00,
	0x00, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x5a, 0x24, 0x00,
	0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, 0x00, 0x82, 0x44, 0x28, 0x10, 0x10, 0x10, 0x00,
	0x00, 0x7e, 0x04, 0x08, 0x10, 0x20, 0x7e, 0x00, 0x00, 0x0e, 0x08, 0x08, 0x08, 0x08, 0x0e, 0x00,
	0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x70, 0x00,
	0x00, 0x10, 0x38, 0x54, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
	0x00, 0x1c, 0x22, 0x78, 0x20, 0x20, 0x7e, 0x00, 0x00, 0x00, 0x38, 0x04, 0x3c, 0x44, 0x3c, 0x00,
	0x00, 0x20, 0x20, 0x3c, 0x22, 0x22, 0x3c, 0x00, 0x00, 0x00, 0x1c, 0x20, 0x20, 0x20, 0x1c, 0x00,
	0x00, 0x04, 0x04, 0x3c, 0x44, 0x44, 0x3c, 0x00, 0x00, 0x00, 0x38, 0x44, 0x78, 0x40, 0x3c, 0x00,
	0x00, 0x0c, 0x10, 0x18, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x44, 0x3c, 0x04, 0x38,
	0x00, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x00, 0x00, 0x10, 0x00, 0x30, 0x10, 0x10, 0x38, 0x00,
	0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x24, 0x18, 0x00, 0x20, 0x28, 0x30, 0x30, 0x28, 0x24, 0x00,
	0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0c, 0x00, 0x00, 0x00, 0x68, 0x54, 0x54, 0x54, 0x54, 0x00,
	0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00,
	0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x00, 0x00, 0x3c, 0x44, 0x44, 0x3c, 0x04, 0x06,
	0x00, 0x00, 0x1c, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x38, 0x40, 0x38, 0x04, 0x78, 0x00,
	0x00, 0x10, 0x38, 0x10, 0x10, 0x10, 0x0c, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00,
	0x00, 0x00, 0x44, 0x44, 0x28, 0x28, 0x10, 0x00, 0x00, 0x00, 0x44, 0x54, 0x54, 0x54, 0x28, 0x00,
	0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x3c, 0x04, 0x38,
	0x00, 0x00, 0x7c, 0x08, 0x10, 0x20, 0x7c, 0x00, 0x00, 0x0e, 0x08, 0x30, 0x08, 0x08, 0x0e, 0x00,
	0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x70, 0x10, 0x0c, 0x10, 0x10, 0x70, 0x00,
	0x00, 0x14, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x99, 0xa1, 0xa1, 0x99, 0x42, 0x3c
};

gxSprite gxFontSprite;

void gxInitFontSprite()
{
	D3DXCreateTexture( gxDev, 8, 1024, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &gxFontSprite.tex );
	gxFontSprite.width = 8;
	gxFontSprite.height = 1024;

	D3DLOCKED_RECT lock;
	gxFontSprite.tex->LockRect(0, &lock, NULL, 0);

	BYTE* pixels = (BYTE*)lock.pBits;
	for (int y = 0; y < 768; y++)
	{
		BYTE bits = gxFontData[y];

		for (int x = 0; x < 8; x++)
		{
			BYTE solid = ( bits & (0x80>>x) ) ? 0xFF : 0x00;
			pixels[x*4+0] = solid;
			pixels[x*4+1] = solid;
			pixels[x*4+2] = solid;
			pixels[x*4+3] = solid;
		}

		pixels += lock.Pitch;
	}

	gxFontSprite.tex->UnlockRect(0);
}

void gxGetDisplayResolution( gxDisplayType disp, int* w, int* h )
{
	switch (disp) 
	{
	case GXDISPLAY_IPHONE_PORTRAIT:
		*w = 320;
		*h = 480;
		break;
	case GXDISPLAY_IPHONE_LANDSCAPE:
		*w = 480;
		*h = 320;
		break;
	case GXDISPLAY_IPAD_PORTRAIT: 
		*w = 768;
		*h = 1024;
		break;
	case GXDISPLAY_IPAD_LANDSCAPE: 
		*w = 1024;
		*h = 768;
		break;
	}
}

void gxGetDisplaySize( gxDisplayType disp, int* w, int* h )
{
	switch (disp) 
	{
	case GXDISPLAY_IPHONE_PORTRAIT:
		*w = (int)(320/6.3f);
		*h = (int)(480/6.3f);
		break;
	case GXDISPLAY_IPHONE_LANDSCAPE:
		*w = (int)(480/6.3f);
		*h = (int)(320/6.3f);
		break;
	case GXDISPLAY_IPAD_PORTRAIT: 
		*w = (int)(768/5.2f);
		*h = (int)(1024/5.2f);
		break;
	case GXDISPLAY_IPAD_LANDSCAPE: 
		*w = (int)(1024/5.2f);
		*h = (int)(768/5.2f);
		break;
	}
}

//2016mm x 3024mm

void gxInit()
{
	gxGetDisplayResolution( GXDISPLAY_IPAD_PORTRAIT, &gxScreenWidth, &gxScreenHeight );

	gxD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = gxScreenWidth;
	d3dpp.BackBufferHeight = gxScreenHeight;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	gxD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &gxDev );

	for (int i = 0; i < 16; i++)
	{
		gxDev->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		gxDev->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		gxDev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		gxDev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		gxDev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}

	gxDev->BeginScene();
	gxDev->Clear( 0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0 );
	gxDev->EndScene();

	gxDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ); 

	// Set up default modulation.
	gxDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	gxDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gxDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	gxDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	gxDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	gxDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	gxInitFontSprite();

	gxEmulateDisplaySize( GXDISPLAY_IPAD_PORTRAIT );
}

void gxEmulateDisplaySize( gxDisplayType disp )
{
	int scrw, scrh;
	gxGetDisplaySize( disp, &scrw, &scrh );

	HDC dc = GetDC(hWnd);
	int mmw = GetDeviceCaps(dc, HORZSIZE);
	int mmh = GetDeviceCaps(dc, VERTSIZE);
	int pxw = GetDeviceCaps(dc, HORZRES);
	int pxh = GetDeviceCaps(dc, VERTRES);

	int winw = 1.5f * scrw * pxw / mmw;
	int winh = 1.5f * scrh * pxh / mmh;

	RECT r = { 0, 0, winw, winh };
	AdjustWindowRect( &r, GetWindowStyle( hWnd ), FALSE );

	SetWindowPos( hWnd, HWND_TOP, 0, 0, r.right-r.left+1, r.bottom-r.top+1, SWP_NOMOVE );
	ShowWindow( hWnd, SW_SHOW );
}

void gxDeinit()
{
	gxDev->EndScene();

	gxDev->Release();
	gxDev = NULL;
	gxD3D->Release();
	gxD3D = NULL;
}

void _gxDrawQuad( float x, float y, float w, float h, unsigned int color, float u1, float v1, float u2, float v2 )
{
	gxSpriteVertex v[4];
	v[0].x = x; v[0].y = y; v[0].z = 0.0f; v[0].w = 1.0f;
	v[0].color = color;
	v[0].u = u1; v[0].v = v1;

	v[1].x = x+w; v[1].y = y; v[1].z = 0.0f; v[1].w = 1.0f;
	v[1].color = color;
	v[1].u = u2; v[1].v = v1;

	v[2].x = x; v[2].y = y+h; v[2].z = 0.0f; v[2].w = 1.0f;
	v[2].color = color;
	v[2].u = u1; v[2].v = v2;

	v[3].x = x+w; v[3].y = y+h; v[3].z = 0.0f; v[3].w = 1.0f;
	v[3].color = color;
	v[3].u = u2; v[3].v = v2;

	gxDev->SetVertexShader(NULL);
	gxDev->SetFVF( gxSpriteVertexFVF );
	gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(gxSpriteVertex) );
}

void _gxSetTexture( gxSprite* spr )
{
	gxDev->SetTexture( 0, spr->tex );
}

void _gxSetAlpha( gxAlphaMode mode )
{
	if (mode == GXALPHA_NONE)
	{
		gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}
	else if (mode == GXALPHA_BLEND)
	{
		gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
	else if (mode == GXALPHA_ADD)
	{
		gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	}
}

void gxLoadSprite( const char* name, gxSprite* spr )
{
	assert( gxDev && "If this assert fires, you need to call gxInit earlier." );
	HRESULT Err = NULL;
	D3DXIMAGE_INFO info = { 0 };
	D3DXCreateTextureFromFileEx( gxDev, name,
		  D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
		  D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &spr->tex );

	spr->width = info.Width;
	spr->height = info.Height;
	spr->texWidth = info.Width;
	spr->texHeight = info.Height;
    spr->left = 0;
    spr->right = info.Width;
    spr->top = 0;
    spr->bottom = info.Height;
}

void gxDestroySprite( gxSprite* spr )
{
	if ( spr->tex )
		spr->tex->Release();
	spr->tex = NULL;
}

void gxDrawString( int x, int y, int ptsize, int color, const char* text, ... )
{
	char work[16384];

	va_list args;
	va_start(args, text);
	vsprintf_s(work, sizeof(work), text, args);
	va_end(args);

	gxDev->SetTexture( 0, gxFontSprite.tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	int OrigX = x;

	for (const char* p = work; *p; p++)
	{
		char ch = *p;

		if (ch == '\n')
		{
			y += ptsize;
			x = OrigX;
			continue;
		}

		if (ch < 32 || ch > 127)
			continue;

		float v0 = (float)( ch - 32 ) * 8 / gxFontSprite.height + (0.5f/gxFontSprite.height);
		float v1 = (float)( ch - 32 + 1 ) * 8 / gxFontSprite.height + (0.5f/gxFontSprite.height);

		_gxDrawQuad( x, y, ptsize, ptsize, color, 0.0f, v0, 1.0f, v1 );

		x += ptsize;
	}
}

void Init();
void Exit();
bool Update();
void Display();

extern bool InErrorHandler;

void gxDisplayWindow()
{
    if (InErrorHandler)
        return;

	gxDev->Clear( 0, NULL, D3DCLEAR_TARGET, gxRGB32(0,0,0), 1.0f, 0 );
	gxDev->BeginScene();
	Display();
	gxDev->EndScene();
	gxDev->Present( NULL, NULL, 0, NULL ); // Change the third input to 0 for Default, rest NULL
}

LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
	case WM_ACTIVATE:
		gxWindowActive = wparam != WA_INACTIVE;
		break;
	case WM_PAINT:
		gxDisplayWindow();
		break;
	case WT_PACKET:
		msOnWTPacket(wparam, lparam);
		break;
	default:
		break;
	}

	return DefWindowProc( hwnd, msg, wparam, lparam );
}

void CheckMessages()
{
	MSG msg;
	while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if ( msg.message == WM_QUIT ) 
			gxQuitRequested = true;
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	hInst = hInstance;

	// Change directory to the path containing the exe file.
	char buf[1024];
	GetModuleFileName(NULL, buf, sizeof(buf));
	PathRemoveFileSpec(buf);
	SetCurrentDirectory(buf);

	// Register the window class
	WNDCLASSEX wc = 
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L,
		hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION)), LoadCursor(NULL, IDC_ARROW), NULL, NULL,
		"graphics", NULL 
	};
	RegisterClassEx( &wc );

	// Create the application's window
#ifdef _DEBUG
	const char* WindowTitle = "Super Dust Bunny [debug]";
#else
	const char* WindowTitle = "Super Dust Bunny";
#endif
	hWnd = CreateWindow( "graphics", WindowTitle, WS_OVERLAPPEDWINDOW, 100, 100, 640, 480, GetDesktopWindow(), NULL, wc.hInstance, NULL );

	Init();

	while ( !gxQuitRequested )
	{
		CheckMessages();

		if (gxWindowActive)
		{
			if ( !Update() ) 
				break;
		}

		gxDisplayWindow();
	}

	Exit();

	return 0;
}

void gxCreateASCIIBlockSprite(gxSprite* Sprite, const char* Key)
{
	D3DXCreateTexture( gxDev, 64, 64, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &Sprite->tex );
	Sprite->width = 64;
	Sprite->height = 64;

	D3DLOCKED_RECT lock;
	Sprite->tex->LockRect(0, &lock, NULL, 0);

	BYTE* pixels = (BYTE*)lock.pBits;
	memset(pixels, 0, 64*64*4);

	for (int i = 0; i < 9; i++)
	{
		char ch = Key[i];
		if (ch < 32)
			continue;

		int keyx = 8 + (i%3) * 16;
		int keyy = 8 + (i/3) * 16;

		for (int y = 0; y < 16; y++)
		{
			BYTE bits = gxFontData[(ch-32)*8 + y/2];

			for (int x = 0; x < 16; x++)
			{
				BYTE* p = pixels + (keyy+y)*lock.Pitch + (keyx+x)*4;

				BYTE solid = ( bits & (0x80 >> (x/2)) ) ? 0xFF : 0x00;
				p[0] = solid;
				p[1] = solid;
				p[2] = solid;
				p[3] = solid;
			}
		}
	}

	gxFontSprite.tex->UnlockRect(0);
}

void gxCreateRenderTarget(int Width, int Height, gxSprite* Sprite, bool Alpha)
{
	Sprite->width = Sprite->texWidth = Width;
	Sprite->height = Sprite->texHeight = Height;
	D3DXCreateTexture(gxDev, Width, Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &Sprite->tex);
    Sprite->left = 0;
    Sprite->top = 0;
    Sprite->right = Sprite->width;
    Sprite->bottom = Sprite->height;
}

void gxSetRenderTarget(gxSprite* Sprite)
{
	IDirect3DSurface9* Surf;
	if (Sprite)
	{
		Sprite->tex->GetSurfaceLevel(0, &Surf);
	}
	else
	{
		gxDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &Surf);
	}
	gxDev->SetRenderTarget(0, Surf);
	Surf->Release();

	D3DVIEWPORT9 vp;
	vp.X      = 0;
	vp.Y      = 0;
	vp.Width  = Sprite ? Sprite->width : gxScreenWidth;
	vp.Height = Sprite ? Sprite->height : gxScreenHeight;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;
	gxDev->SetViewport(&vp);
}

void gxClearColor(unsigned int Color)
{
	gxDev->Clear(0, NULL, D3DCLEAR_TARGET, Color, 0.0f, 0);
}

void gxCreateShader(const char* VertexSource, const char* PixelSource, gxShader* Shader)
{
	LPD3DXBUFFER ErrorMsgs;

	D3DXCompileShader(VertexSource, strlen(VertexSource), NULL, NULL, "main", "vs_3_0", 0, &Shader->VertexCode, &ErrorMsgs, NULL);

	if (Shader->VertexCode)
	{
		gxDev->CreateVertexShader((DWORD*)Shader->VertexCode->GetBufferPointer(), &Shader->VertexShader);
        D3DXGetShaderConstantTable((DWORD*)Shader->VertexCode->GetBufferPointer(), &Shader->VertexConsts);
	}
	else
	{
		ReportError("Failed to compile vertex shader:\n%s", ErrorMsgs->GetBufferPointer());
	}

	if (ErrorMsgs)
		ErrorMsgs->Release();

	D3DXCompileShader(PixelSource, strlen(PixelSource), NULL, NULL, "main", "ps_3_0", 0, &Shader->PixelCode, &ErrorMsgs, NULL);

	if (Shader->PixelCode)
	{
		gxDev->CreatePixelShader((DWORD*)Shader->PixelCode->GetBufferPointer(), &Shader->PixelShader);
        D3DXGetShaderConstantTable((DWORD*)Shader->PixelCode->GetBufferPointer(), &Shader->PixelConsts);
	}
	else
	{
		ReportError("Failed to compile pixel shader:\n%s", ErrorMsgs->GetBufferPointer());
	}

	if (ErrorMsgs)
		ErrorMsgs->Release();
}

void gxSetShader(gxShader* Shader)
{
   	gxDev->SetVertexShader(Shader->VertexShader);
   	gxDev->SetPixelShader(Shader->PixelShader);
}

gxShaderConstant gxGetShaderConstantByName(gxShader* Shader, const char* Name)
{
    gxShaderConstant Result = 0;

    D3DXHANDLE Const;
    D3DXCONSTANT_DESC Desc;
    UINT Count;

    Count = 1;
    Const = Shader->VertexConsts->GetConstantByName(NULL, Name);
    if ( Const )
    {
        Shader->VertexConsts->GetConstantDesc(Const, &Desc, &Count);
        if ( Count )
        {
            Result |= 0x8000 | Desc.RegisterIndex;
        }
    }

    Count = 1;
    Const = Shader->PixelConsts->GetConstantByName(NULL, Name);
    if ( Const )
    {
        Shader->PixelConsts->GetConstantDesc(Const, &Desc, &Count);
        if ( Count )
        {
            Result |= ( 0x8000 | Desc.RegisterIndex ) << 16;
        }
    }

    return Result;
}

void gxSetShaderConstant(gxShaderConstant Constant, float x, float y, float z, float w)
{
	float Floats[4] = {x, y, z, w};
    if ( Constant & 0x0000ffff )
	    gxDev->SetVertexShaderConstantF(Constant & 0x00007fff, Floats, 1);
    if ( Constant & 0xffff0000 )
	    gxDev->SetPixelShaderConstantF((Constant >> 16) & 0x00007fff, Floats, 1);
}

void gxSetShaderSampler(gxShaderConstant Constant, gxSprite* Sprite)
{
    if ( Constant & 0x0000ffff )
	    gxDev->SetTexture(Constant & 0x00007fff, Sprite->tex);
    if ( Constant & 0xffff0000 )
	    gxDev->SetTexture((Constant >> 16) & 0x00007fff, Sprite->tex);
}
