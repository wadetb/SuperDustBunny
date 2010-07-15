#ifndef GRAPHICS_H
#define GRAPHICS_H

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#define gxRGBA32(r,g,b,a) (((r)<<16)|((g)<<8)|((b)<<0)|((a)<<24))
#define gxRGB32(r,g,b)    gxRGBA32(r,g,b,255)

struct gxSprite
{
	GLuint tex;
	int width;
	int height;
	int texWidth;
	int texHeight;
};

extern int gxScreenWidth;
extern int gxScreenHeight;

void gxInit(int xres, int yres, bool window);
void gxDeinit();

void gxLoadSprite(const char* name, gxSprite* sprite);
void gxDestroySprite(gxSprite* sprite);

void gxDrawSprite(int x, int y, gxSprite* sprite);

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color);

void gxGetFileName(const char* relativePath, char* buffer, int bufferSize);
void gxGetResourceFileName(const char* relativePath, char* buffer, int bufferSize);

#endif
