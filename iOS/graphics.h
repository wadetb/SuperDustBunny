#ifndef GRAPHICS_H
#define GRAPHICS_H

extern "C" {
void Init();
void Exit();
bool Update();
void Display();
}

#include <stdio.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#define gxRGBA32(r,g,b,a) (((b)<<16)|((g)<<8)|((r)<<0)|((a)<<24))
#define gxRGB32(r,g,b)    gxRGBA32(r,g,b,255)

struct gxSprite
{
	GLuint tex;
	int width;
	int height;
	int texWidth;
	int texHeight;
};

enum gxDisplayType
{
	GXDISPLAY_IPHONE_PORTRAIT,
	GXDISPLAY_IPHONE_LANDSCAPE,

	GXDISPLAY_IPAD_PORTRAIT,
	GXDISPLAY_IPAD_LANDSCAPE,
};

extern int gxScreenWidth;
extern int gxScreenHeight;

void gxInit(gxDisplayType disp);
void gxDeinit();

void gxLoadSprite(const char* name, gxSprite* sprite);
void gxDestroySprite(gxSprite* sprite);

void gxDrawSprite(int x, int y, gxSprite* sprite);
void gxDrawSpriteScaled(int x, int y, float scalex, float scaley, gxSprite* sprite);
void gxDrawSpriteAlpha(int x, int y, float alpha, gxSprite* sprite);

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color);

void gxDrawString(int x, int y, int ptsize, int color, const char* text, ...);

void gxGetFileName(const char* relativePath, char* buffer, int bufferSize);

FILE* gxOpenFile(const char* relativePath, const char* mode);

void gxDrawSpriteCenteredRotated(int x, int y, float a, gxSprite* spr);
void gxDrawSpriteCenteredScaledAlphaAdd(int x, int y, float scalex, float scaley, float alpha, gxSprite* spr);

enum gxAlphaMode
{
	GXALPHA_NONE,
	GXALPHA_BLEND,
	GXALPHA_ADD,
};

void _gxSetAlpha( gxAlphaMode mode );
void _gxSetTexture( gxSprite* spr );
void _gxDrawQuad( float x, float y, float w, float h, unsigned int color = gxRGBA32(255,255,255,255), float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );
void _gxDrawQuad(
                 unsigned int Color,
                 float X0, float Y0, float U0, float V0, 
                 float X1, float Y1, float U1, float V1, 
                 float X2, float Y2, float U2, float V2, 
                 float X3, float Y3, float U3, float V3);

void gxCreateRenderTarget(int Width, int Height, gxSprite* Sprite);
void gxSetRenderTarget(gxSprite* Sprite);

void gxClearColor(unsigned int Color);

struct gxPixelShader
{
	GLuint Shader;
};

void gxCreatePixelShader(const char* Source, gxPixelShader* Shader);
void gxSetPixelShader(gxPixelShader* Shader);
void gxSetPixelShaderConst(int Index, float x, float y=0.0f, float z=0.0f, float w=0.0f);

struct gxVertexShader
{
	GLuint Shader;
};

void gxCreateVertexShader(const char* Source, gxVertexShader* Shader);
void gxSetVertexShader(gxVertexShader* Shader);
void gxSetVertexShaderConst(int Index, float x, float y=0.0f, float z=0.0f, float w=0.0f);

void gxCopyRenderTarget(gxSprite* From, gxSprite* To);


#endif
