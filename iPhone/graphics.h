#ifndef GRAPHICS_H
#define GRAPHICS_H

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

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color);

void gxDrawString(int x, int y, int ptsize, int color, const char* text, ...);

void gxGetFileName(const char* relativePath, char* buffer, int bufferSize);

FILE* gxOpenFile(const char* relativePath, const char* mode);

void gxDrawSpriteCenteredRotated(int x, int y, int dir, gxSprite* spr);

#endif
