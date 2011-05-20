#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#define gxRGBA32(r,g,b,a) (((b)<<16)|((g)<<8)|((r)<<0)|((a)<<24))
#define gxRGB32(r,g,b)    gxRGBA32(r,g,b,255)

extern GLuint gxOpenGLESVersion;

enum
{
    GX_ATTRIB_VERTEX,
    GX_ATTRIB_TEXCOORD,
    GX_ATTRIB_COLOR
};


struct gxSprite
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
};

enum gxDisplayType
{
	GXDISPLAY_IPHONE_PORTRAIT,
	GXDISPLAY_IPHONE_LANDSCAPE,
    
	GXDISPLAY_IPHONE_RETINA_PORTRAIT,
	GXDISPLAY_IPHONE_RETINA_LANDSCAPE,
    
	GXDISPLAY_IPAD_PORTRAIT,
	GXDISPLAY_IPAD_LANDSCAPE,
};

extern int gxScreenWidth;
extern int gxScreenHeight;

void gxInit();
void gxDeinit();

gxDisplayType gxGetDisplayType();

void gxLoadSprite(const char* name, gxSprite* sprite);
void gxDestroySprite(gxSprite* sprite);

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color);

void gxDrawString(int x, int y, int ptsize, int color, const char* text, ...);

void gxGetResourceFileName(const char* relativePath, char* buffer, int bufferSize);

FILE* gxOpenFile(const char* relativePath, const char* mode);

enum gxAlphaMode
{
	GXALPHA_NONE,
	GXALPHA_BLEND,
	GXALPHA_ADD,
};

void _gxSetAlpha( gxAlphaMode mode );
void _gxSetTexture( gxSprite* spr );
void _gxDrawQuad( float x, float y, float w, float h, unsigned int color = gxRGBA32(255,255,255,255), float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );

void gxCreateRenderTarget(int Width, int Height, gxSprite* Sprite, bool Alpha);
void gxSetRenderTarget(gxSprite* Sprite);

void gxClearColor(unsigned int Color);

struct gxShader
{
    GLuint VertexShader;
    GLuint PixelShader;
    GLuint Program;
};

void gxCreateShader(const char* VertexSource, const char* PixelSource, gxShader* Shader);
void gxSetShader(gxShader* Shader);

typedef GLint gxShaderConstant;

gxShaderConstant gxGetShaderConstantByName(gxShader* Shader, const char* Name);
void gxSetShaderConstant(gxShaderConstant Constant, float x);
void gxSetShaderConstant(gxShaderConstant Constant, float x, float y);
void gxSetShaderConstant(gxShaderConstant Constant, float x, float y, float z);
void gxSetShaderConstant(gxShaderConstant Constant, float x, float y, float z, float w);
void gxSetShaderSampler(gxShaderConstant Constant, gxSprite* Sprite);

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

void Init();
void Exit();
bool Update();
void Display();

#endif
