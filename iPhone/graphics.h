#ifndef GRAPHICS_H
#define GRAPHICS_H

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#define gxRGBA32(r,g,b,a) (((r)<<16)|((g)<<8)|((b)<<0)|((a)<<24))
#define gxRGB32(r,g,b)    gxRGBA32(r,g,b,255)

struct gxSprite
{
	gxSprite() : tex(0),width(0),height(0),texWidth(0), texHeight(0), fileName(0) {}
	GLuint tex;
	int width;
	int height;
	int texWidth;
	int texHeight;
	char* fileName;
};

struct gxFont
{
	int foo;
};

struct spriteTile{
	int x, y;
	gxSprite* p;
	int srcx, srcy, srcw, srch;
};
struct spriteMap{
	spriteTile* tiles;
	int count;
};

extern int gxScreenWidth;
extern int gxScreenHeight;

void gxInit(int xres, int yres, bool window);
void gxExit();

void gxLoadSprite(const char* name, gxSprite* sprite);
void gxDestroySprite(gxSprite* sprite);

void gxDrawSprite(int x, int y, gxSprite* sprite);
void gxDrawSprite0(int x, int y, gxSprite* sprite);
void gxDrawSpriteAlpha(int x, int y, gxSprite* sprite, int alpha);
void gxDrawSpriteScaledFilled(int x, int y, gxSprite* sprite, int width, int height, unsigned int color);
void gxDrawSpriteScaled(int x, int y, gxSprite* sprite, int width, int height);
void gxDrawSpriteClipped( int x, int y, gxSprite* p, int srcx, int srcy, int srcw, int srch );
//Special tilemap drawing optimized for iPhone.
void gxDrawSpriteMap(int offsetX, int offsetY, int layer);
void gxLoadSpriteMap(spriteMap* map, int count);

void gxDrawRect(int x, int y, int width, int height, unsigned int color);

void gxLoadFont(const char* name, gxFont* font, int x, int y, int z);
void gxDestroyFont(gxFont* font);

void gxDrawText(int x, int y, const char* str, gxFont* font, unsigned int color, int flags);
void gxDrawInt(int x, int y, int value, gxFont* font, unsigned int color, int flags);

void gxUpdateScreen();

void Init();
void Exit();
bool Update();
void Display();

void gxGetFileName(const char* relativePath, char* buffer, int bufferSize);
// Compatibility
inline void gxLoadBmp(const char* name, gxSprite* sprite, unsigned long) { gxLoadSprite(name, sprite); }
#define gxDeinit gxExit

void gxGetResourceFileName(const char* relativePath, char* buffer, int bufferSize);

#endif
