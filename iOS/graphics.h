#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>

#if TARGET_OS_MAC && !TARGET_OS_IPHONE
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#define glMapBufferOES glMapBuffer
#define glUnmapBufferOES glUnmapBuffer
#define GL_WRITE_ONLY_OES GL_WRITE_ONLY
#endif

#if TARGET_OS_IPHONE
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#define gxRGBA32(r,g,b,a) (((b)<<16)|((g)<<8)|((r)<<0)|((a)<<24))
#define gxRGB32(r,g,b)    gxRGBA32(r,g,b,255)

extern GLuint gxOpenGLESVersion;

enum
{
    GX_ATTRIB_VERTEX,
    GX_ATTRIB_TEXCOORD,
    GX_ATTRIB_COLOR
};


typedef struct 
{
	GLuint tex;
    GLuint framebuffer;
	int width;
	int height;
	int texWidth;
	int texHeight;
    int left;
    int top;
    int right;
    int bottom;
} gxSprite;

typedef enum 
{
	GXDISPLAY_IPHONE_PORTRAIT,
	GXDISPLAY_IPHONE_LANDSCAPE,
    
	GXDISPLAY_IPHONE_RETINA_PORTRAIT,
	GXDISPLAY_IPHONE_RETINA_LANDSCAPE,
    
	GXDISPLAY_IPAD_PORTRAIT,
	GXDISPLAY_IPAD_LANDSCAPE,
} gxDisplayType;

extern int gxScreenWidth;
extern int gxScreenHeight;

void gxInit();
void gxDeinit();

gxDisplayType gxGetDisplayType();

void gxLoadSprite(const char* name, gxSprite* sprite);
void gxDestroySprite(gxSprite* sprite);

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color);

void gxDrawString(int x, int y, int ptsize, int color, const char* text, ...);

typedef enum 
{
	GXALPHA_NONE,
	GXALPHA_BLEND,
	GXALPHA_ADD,
} gxAlphaMode;

void _gxSetAlpha( gxAlphaMode mode );
void _gxSetTexture( gxSprite* spr );

void gxCreateRenderTarget(int Width, int Height, gxSprite* Sprite, bool Alpha);
void gxSetRenderTarget(gxSprite* Sprite);

void gxClearColor(unsigned int Color);

typedef struct gxShader
{
    GLuint VertexShader;
    GLuint PixelShader;
    GLuint Program;
} gxShader;

void gxCreateShader(const char* VertexSource, const char* PixelSource, gxShader* Shader);
void gxSetShader(gxShader* Shader);

typedef GLint gxShaderConstant;

gxShaderConstant gxGetShaderConstantByName(gxShader* Shader, const char* Name);
void gxSetShaderConstant1(gxShaderConstant Constant, float x);
void gxSetShaderConstant2(gxShaderConstant Constant, float x, float y);
void gxSetShaderConstant3(gxShaderConstant Constant, float x, float y, float z);
void gxSetShaderConstant4(gxShaderConstant Constant, float x, float y, float z, float w);
void gxSetShaderSampler(gxShaderConstant Constant, gxSprite* Sprite);

void Init();
void Exit();
bool Update();
void Display();

#endif
