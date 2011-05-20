
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
#include <d3dx9.h>

#define gxRGB32(r,g,b)    D3DCOLOR_RGBA(r,g,b,255)

#define gxRGBA32(r,g,b,a) D3DCOLOR_RGBA(r,g,b,a)

struct gxSprite
{
  IDirect3DTexture9* tex;
  int width;
  int height;
  int texWidth;
  int texHeight;
  int left;
  int top;
  int right;
  int bottom;
};

enum gxDisplayType
{
	GXDISPLAY_IPHONE_PORTRAIT,      // 320x480
	GXDISPLAY_IPHONE_LANDSCAPE,     // 480x320
	
	GXDISPLAY_IPAD_PORTRAIT,        // 768x1024
	GXDISPLAY_IPAD_LANDSCAPE,       // 1024x768
};

void gxInit();
void gxDeinit();

void gxEmulateDisplaySize( gxDisplayType type );

extern int gxScreenWidth;
extern int gxScreenHeight;

void gxDrawString( int x, int y, int ptsize, int color, const char* text, ... );

void gxLoadSprite( const char* name, gxSprite* spr );

void gxDestroySprite( gxSprite* spr );

extern IDirect3DDevice9* gxDev; 

void gxCreateASCIIBlockSprite(gxSprite* Sprite, const char* Key);

struct gxSpriteVertex
{
	float x, y, z, w;
	DWORD color;
	float u, v;
};

#define gxSpriteVertexFVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


enum gxAlphaMode
{
	GXALPHA_NONE,
	GXALPHA_BLEND,
	GXALPHA_ADD,
};

void _gxSetAlpha( gxAlphaMode mode );
void _gxSetTexture( gxSprite* spr );

void gxCreateRenderTarget(int Width, int Height, gxSprite* Sprite, bool Alpha);
void gxSetRenderTarget(gxSprite* Sprite);

void gxClearColor(unsigned int Color);

struct gxShader
{
	IDirect3DVertexShader9* VertexShader;
	IDirect3DPixelShader9* PixelShader;
    ID3DXBuffer* VertexCode;
    ID3DXBuffer* PixelCode;
    ID3DXConstantTable* VertexConsts;
    ID3DXConstantTable* PixelConsts;
};

typedef int gxShaderConstant;

void gxCreateShader(const char* VertexSource, const char* PixelSource, gxShader* Shader);
void gxSetShader(gxShader* Shader);

gxShaderConstant gxGetShaderConstantByName(gxShader* Shader, const char* Name);
void gxSetShaderConstant(gxShaderConstant Constant, float x, float y=0.0f, float z=0.0f, float w=0.0f);
void gxSetShaderSampler(gxShaderConstant Constant, gxSprite* Sprite);

#endif
