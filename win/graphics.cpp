#define _CRT_SECURE_NO_WARNINGS

#include "graphics.h"

#include "mouse.h"

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#pragma warning(disable:4244)

#include <assert.h>
#include <stdio.h>

HINSTANCE hInst;
HWND hWnd;

int gxScreenWidth = 0;
int gxScreenHeight = 0;

bool gxQuitRequested = false;

IDirect3D9* gxD3D = NULL;
IDirect3DDevice9* gxDev = NULL; 

extern int gxViewX1 = 0;
extern int gxViewY1 = 0;
extern int gxViewX2 = 0;
extern int gxViewY2 = 0;

struct gxSpriteVertex
{
	float x, y, z, w;
	DWORD color;
	float u, v;
};

#define gxSpriteVertexFVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

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

void gxInit( int xres, int yres, bool window )
{
	gxD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.Windowed = window;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = xres;
	d3dpp.BackBufferHeight = yres;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	gxD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &gxDev );
	gxDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	gxDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	gxDev->BeginScene();
	gxDev->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );
	gxDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ); 

	gxScreenWidth = xres;
	gxScreenHeight = yres;
	MoveWindow( hWnd, 0, 0, xres, yres, TRUE );
	ShowWindow( hWnd, SW_SHOW );

	// Set up default modulation.
	gxDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	gxDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gxDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	gxDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	gxDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	gxDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	msSetMouseRange( 0, 0, gxScreenWidth, gxScreenHeight );

	gxInitFontSprite();
}

void gxDeinit()
{
	gxDev->EndScene();

	gxDev->Release();
	gxDev = NULL;
	gxD3D->Release();
	gxD3D = NULL;
}

void gxSetClipRange( int x1, int y1, int x2, int y2 )
{
	gxViewX1 = x1;
	gxViewY1 = y1;
	gxViewX2 = x2;
	gxViewY2 = y2;

	RECT r = { x1, y1, x2, y2 };
	gxDev->SetScissorRect( &r );
}

// Presumes render states have been set up already.
// Sets the texture and submits the polygon.
void _gxDrawQuad( int x, int y, int w, int h, DWORD color = D3DCOLOR_RGBA(255,255,255,255), float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f )
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

	gxDev->SetFVF( gxSpriteVertexFVF );
	gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(gxSpriteVertex) );
}

void gxDrawSprite( int x, int y, gxSprite* p )
{
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	_gxDrawQuad( x, y, p->width, p->height );
}

void gxDrawSprite0( int x, int y, gxSprite* p )
{
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	_gxDrawQuad( x, y, p->width, p->height );
}

void gxDrawSpriteFilled( int x, int y, gxSprite* p, DWORD col )
{
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	_gxDrawQuad( x, y, p->width, p->height, col );
}
void gxDrawSpriteScaledFilled( int x, int y, gxSprite* p, int w, int h, DWORD col )
{
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	_gxDrawQuad( x, y, w, h, col );
}

void gxDrawSpriteScaled( int x, int y, gxSprite* p, int w, int h )
{
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	_gxDrawQuad( x, y, w, h );
}

void gxDrawSpriteAlpha( int x, int y, gxSprite* p, int alpha )
{
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	_gxDrawQuad( x, y, p->width, p->height, D3DCOLOR_RGBA(255, 255, 255, alpha) );
}

void gxDrawSpriteAdd( int x, int y, gxSprite* p )
{
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	_gxDrawQuad( x, y, p->width, p->height );
}

void gxDrawSpriteClipped( int x, int y, gxSprite* p, int srcx, int srcy, int srcw, int srch )
{
	float u0 = float(srcx)/p->width;
	float u1 = float(srcx + srcw)/p->width;
	float v0 = float(srcy)/p->height;
	float v1 = float(srcy + srch)/p->height;
	gxDev->SetTexture( 0, p->tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	_gxDrawQuad( x, y, srcw, srch, D3DCOLOR_RGBA(255,255,255,255),u0,v0,u1,v1);
}

void gxDrawRectangleFilled( int x, int y, int width, int height, int col )
{
	gxDev->SetTexture( 0, NULL );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	_gxDrawQuad( x, y, width, height, col );
}

void gxLoadSprite( const char* name, gxSprite* spr )
{
	assert( gxDev && "If this assert fires, you need to call gxInit earlier." );

	D3DXIMAGE_INFO info = { 0 };
	D3DXCreateTextureFromFileEx( gxDev, name,
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &spr->tex );

	spr->width = info.Width;
	spr->height = info.Height;
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

	for (const char* p = work; *p; p++)
	{
		char ch = *p;
		if (ch < 32 || ch > 127)
			continue;

		float v0 = (float)( ch - 32 ) * 8 / gxFontSprite.height;
		float v1 = (float)( ch - 32 + 1 ) * 8 / gxFontSprite.height;

		_gxDrawQuad( x, y, ptsize, ptsize, color, 0.0f, v0, 1.0f, v1 );

		x += ptsize;
	}
}

#if 0

void gxLoadFont( char* filename, gxFont* f, int width, int height, int spacing )
{
	assert( gxDev && "If this assert fires, you need to call gxInit earlier." );

	FILE* in = fopen( filename, "rb" );
	if ( !in )
		return;

	// Fill in the font structure.
	for ( int x = 0; x < 256; x++ ) 
		f->pos[x] = 0;

	f->spacing = spacing;

	// Create a sprite to hold the letters.
	f->spr.width = width;
	f->spr.height = height;
	D3DXCreateTexture( gxDev, width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &f->spr.tex );

	IDirect3DSurface9* surf;
	f->spr.tex->GetSurfaceLevel( 0, &surf );

	D3DLOCKED_RECT rect;
	surf->LockRect( &rect, NULL, 0 );

	// Read in the texture, converting from RGB to ARGB.
	BYTE* line = (BYTE*)malloc(width*3);
	for ( int y = 0; y < height; y++ )
	{
		BYTE* dest = (BYTE*)rect.pBits + y*rect.Pitch;
		fread( line, 1, width*3, in );
		for ( int x = 0; x < width; x++ )
		{
			dest[x*4+0] = line[x*3+2];
			dest[x*4+1] = line[x*3+2];
			dest[x*4+2] = line[x*3+2];
			dest[x*4+3] = line[x*3+2]; // red copied to alpha for now
		}
	}
	free( line );

	fclose( in );

	// Scan for characters in the texture.
	BYTE* ptr = (BYTE*)rect.pBits;
	int xpos = 0;
	for ( int ch = 32; ch < 128; ch++ )
	{
		f->pos[ch] = xpos + 1;
		while ( ptr[xpos*4+0] != 0xff )
			xpos++;
		f->width[ch] = xpos - f->pos[ch];
		xpos++;
	}

	surf->UnlockRect();
	surf->Release();
}

void gxDestroyFont( gxFont* f )
{
	gxDestroySprite( &f->spr );
}

void gxDrawText(int x, int y, char* text, gxFont* f, DWORD color, int type)
{
	gxDev->SetTexture( 0, f->spr.tex );
	gxDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	gxDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	gxDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	int xpos = x;
	for ( char* ch = text; *ch; ++ch )
	{
		_gxDrawQuad( 
			xpos, y, f->width[*ch]+1, f->spr.height+1, color, 
			(float)f->pos[*ch] / f->spr.width, 0.0f, (float)(f->pos[*ch]+f->width[*ch]) / f->spr.width, 1.0f );
		xpos += f->width[*ch] + f->spacing;
	}
}

void gxDrawInt(int x, int y, int txt, gxFont* f, DWORD color, int type)
{
	char strin[64];
	sprintf(strin, "%d", txt);//_itoa(txt, strin, 10);
	gxDrawText(x, y, strin, f, color, type);

	/*char* strin = "QQQQQQQQQQQQQQQQQQQQQQQQQQQQ";
	sprintf(strin, "%d", txt);//_itoa(txt, strin, 10);
	gxDrawText(x, y, strin, f, color, type);
	*/
}

#endif

LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
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

void Init();
void Exit();
bool Update();
void Display();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,  LPSTR lpCmdLine, int nCmdShow )
{
	hInst = hInstance;

	// Register the window class
	WNDCLASSEX wc = 
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L,
		GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL,IDC_ARROW), NULL, NULL,
		"Training", NULL 
	};
	RegisterClassEx( &wc );

	// Create the application's window
	hWnd = CreateWindow( "Training", "", WS_OVERLAPPEDWINDOW, 100, 100, 640, 480, GetDesktopWindow(), NULL, wc.hInstance, NULL );

	Init();

	while ( !gxQuitRequested )
	{
		CheckMessages();
		if ( !Update() ) 
			break;

		gxDev->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, gxRGB32(0,0,0), 1.0f, 0 );
		gxDev->BeginScene();
		Display();
		gxDev->EndScene();
		gxDev->Present( NULL, NULL, 0, NULL ); // Change the third input to 0 for Default, rest NULL

		CheckMessages();

	}

	Exit();

	return 0;
}
