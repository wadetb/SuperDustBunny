#include "graphics.h"
#include "mouse.h"
#include <limits.h>

int gxScreenWidth;
int gxScreenHeight;

GLint _gxDefaultFrameBuffer;
GLint _gxDefaultFrameBufferWidth;
GLint _gxDefaultFrameBufferHeight;

GLuint gxOpenGLESVersion;

typedef struct 
{
	float x, y, z, w;
	unsigned int color;
	float u, v;
} gxSpriteVertex;

// 8x8 monochrome font from 32 to 127 by Richard Mitton
GLubyte gxFontData[768] =
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
	void* buffer = calloc(8 * 1024 * 4, 1);
	
	gxFontSprite.width = 8;
	gxFontSprite.height = 1024;

	gxFontSprite.texWidth = 8;
	gxFontSprite.texHeight = 1024;

	GLubyte* pixels = (GLubyte*)buffer;
	for (int y = 0; y < 768; y++)
	{
		GLubyte bits = gxFontData[y];
		
		for (int x = 0; x < 8; x++)
		{
			GLubyte solid = ( bits & (0x80>>x) ) ? 0xFF : 0x00;
			pixels[x*4+0] = solid;
			pixels[x*4+1] = solid;
			pixels[x*4+2] = solid;
			pixels[x*4+3] = solid;
		}
		
		pixels += 8 * 4;
	}
	
	glGenTextures(1, &gxFontSprite.tex);
	glBindTexture(GL_TEXTURE_2D, gxFontSprite.tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	
	free(buffer);	
}

#if TARGET_OS_MAC && !TARGET_OS_IPHONE

const char* gxSpriteVertexShaderSource =
"#version 150 core\n"
"in vec2 PositionAttr;\n"
"in vec2 TexCoordAttr;\n"
"in vec4 ColorAttr;\n"
"\n"
"out vec2 TexCoordInterp;\n"
"out vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(PositionAttr.x/768.0*2.0-1.0, (1.0-PositionAttr.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* gxSpritePixelShaderSource =
"#version 150 core\n"
"uniform sampler2D ColorSampler;\n"
"\n"
"in vec2 TexCoordInterp;\n"
"in vec4 ColorInterp;\n"
"\n"
"out vec4 Color;\n"
"\n"
"void main()\n"
"{\n"
"	Color = texture(ColorSampler, TexCoordInterp) * ColorInterp;\n"
"}\n";

#endif

#if TARGET_OS_IPHONE 

const char* gxSpriteVertexShaderSource =
"attribute vec2 PositionAttr;\n"
"attribute vec2 TexCoordAttr;\n"
"attribute vec4 ColorAttr;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"varying vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(PositionAttr.x/768.0*2.0-1.0, (1.0-PositionAttr.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* gxSpritePixelShaderSource =
"uniform lowp sampler2D Sampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(Sampler, TexCoordInterp) * ColorInterp;\n"
"}\n";

#endif

gxShader gxSpriteShader;

void gxInitSpriteShader()
{
    gxCreateShader(gxSpriteVertexShaderSource, gxSpritePixelShaderSource, &gxSpriteShader);
}

#if TARGET_OS_MAC && !TARGET_OS_IPHONE
#define glOrthof glOrtho
#endif

void gxInit()
{
    gxScreenWidth = 768;
    gxScreenHeight = 1024;
    
	msSetMouseRange( 0, 0, gxScreenWidth, gxScreenHeight );	
	
#if TARGET_OS_MAC && !TARGET_OS_IPHONE
    gxOpenGLESVersion = 2;
#endif
    
    if (gxOpenGLESVersion == 1)
    {
        glEnable(GL_TEXTURE_2D);        
    }
    
	gxInitFontSprite();
    
    if (gxOpenGLESVersion == 2)
    {
        gxInitSpriteShader();
    }
#if TARGET_OS_IPHONE
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(0, gxScreenWidth, gxScreenHeight, 0, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
#endif
}

void gxDeinit()
{
}

gxDisplayType gxGetDisplayType()
{
#if TARGET_OS_MAC && !TARGET_OS_IPHONE
    return GXDISPLAY_IPAD_PORTRAIT;
#endif
    
#if TARGET_OS_IPHONE
    float Version = [[[UIDevice currentDevice] systemVersion] floatValue];
    if (Version < 3.2f)
        return GXDISPLAY_IPHONE_PORTRAIT;

    UIScreen* MainScreen = [UIScreen mainScreen];
    int Width = MainScreen.currentMode.size.width;
    int Height = MainScreen.currentMode.size.height;

    if ((Width == 768 && Height == 1024) || (Width == 1024 && Height == 768))
        return GXDISPLAY_IPAD_PORTRAIT;

    if ((Width == 640 && Height == 960) || (Width == 960 && Height == 640))
        return GXDISPLAY_IPHONE_RETINA_PORTRAIT;
    
    NSLog(@"Unknown display resolution; defaulting to iPhone.  Graphics may be blurry!");
    
    return GXDISPLAY_IPHONE_PORTRAIT;  
#endif
}

void gxLoadSprite(const char* filename, gxSprite* sprite)
{
#if TARGET_OS_MAC && !TARGET_OS_IPHONE
    CFStringRef path = CFStringCreateWithCString(NULL, filename, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
    CGImageSourceRef imageSource = CGImageSourceCreateWithURL(url, NULL);
    CGImageRef image = CGImageSourceCreateImageAtIndex(imageSource, 0, NULL);
	if (image == NULL)
	{
		return;
	}
#endif

#if TARGET_OS_IPHONE
	NSString* nsname = [[NSString alloc] initWithUTF8String:filename];
	CGImageRef image = [[[UIImage imageNamed:nsname] retain] CGImage];
	if (image == nil)
	{
		NSLog(@"could not find %@", nsname);
		[nsname release];
		return;
	}
	[nsname release];
#endif
	
	sprite->width = CGImageGetWidth(image);
	sprite->height = CGImageGetHeight(image);
	
	sprite->texWidth = 1;
	while (sprite->texWidth < sprite->width) 
		sprite->texWidth *= 2;

	sprite->texHeight = 1;
	while (sprite->texHeight < sprite->height) 
		sprite->texHeight *= 2;
	
    sprite->left = 0;
    sprite->top = 0;
    sprite->right = sprite->width;
    sprite->bottom = sprite->bottom;
    
	void* pixels = calloc(sprite->texWidth * sprite->texHeight * 4, 1);
	CGColorSpaceRef colorSpace = CGImageGetColorSpace(image);
	CGContextRef context = CGBitmapContextCreate(pixels, sprite->texWidth, sprite->texHeight, 8, 4 * sprite->texWidth, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
	CGColorSpaceRelease(colorSpace);
	CGContextDrawImage(context, CGRectMake(0, sprite->texHeight-sprite->height, sprite->width, sprite->height), image);
	
	glGenTextures(1, &sprite->tex);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->texWidth, sprite->texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	CGContextRelease(context);
	free(pixels);
}

void gxDestroySprite(gxSprite* sprite)
{
	if (sprite->tex)
		glDeleteTextures(1, &sprite->tex);
	
	sprite->tex = 0;
	sprite->width = 0;
	sprite->height = 0;
	sprite->texWidth = 0;
	sprite->texHeight = 0;
}

void _gxDrawQuad( float x, float y, float w, float h, unsigned int color, float u1, float v1, float u2, float v2 )
{
    return;
}

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color)
{
    return;
}

void gxDrawString( int x, int y, int ptsize, int color, const char* text, ... )
{
    return;
}

void _gxSetAlpha( gxAlphaMode mode )
{
    if (mode == GXALPHA_BLEND)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else if (mode == GXALPHA_ADD)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void _gxSetTexture( gxSprite* sprite )
{
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
}

void gxCreateShader(const char* VertexSource, const char* PixelSource, gxShader* Shader)
{
    Shader->VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Shader->VertexShader, 1, &VertexSource, NULL);
    glCompileShader(Shader->VertexShader);
    
    GLint logLength;
    glGetShaderiv(Shader->VertexShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(Shader->VertexShader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
    
    GLint status;
    glGetShaderiv(Shader->VertexShader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        assert(false);
    }
    
    Shader->PixelShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Shader->PixelShader, 1, &PixelSource, NULL);
    glCompileShader(Shader->PixelShader);
    
    glGetShaderiv(Shader->PixelShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(Shader->PixelShader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
    
    glGetShaderiv(Shader->PixelShader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        assert(false);
    }
    
    Shader->Program = glCreateProgram();
    glAttachShader(Shader->Program, Shader->VertexShader);
    glAttachShader(Shader->Program, Shader->PixelShader);
    
    glBindAttribLocation(Shader->Program, GX_ATTRIB_VERTEX, "PositionAttr");
    glBindAttribLocation(Shader->Program, GX_ATTRIB_COLOR, "ColorAttr");
    glBindAttribLocation(Shader->Program, GX_ATTRIB_TEXCOORD, "TexCoordAttr");   

#if TARGET_OS_MAC && !TARGET_OS_IPHONE
    glBindFragDataLocation(Shader->Program, 0, "Color");
#endif
    
    glLinkProgram(Shader->Program);
    
    glGetProgramiv(Shader->Program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(Shader->Program, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(Shader->Program, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        assert(false);
    }
    
    glValidateProgram(Shader->Program);
    glGetProgramiv(Shader->Program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(Shader->Program, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(Shader->Program, GL_VALIDATE_STATUS, &status);
    if (status == 0)
    {
        assert(false);
    }
}

void gxSetShader(gxShader* Shader)
{
    glUseProgram(Shader->Program);
}

gxShaderConstant gxGetShaderConstantByName(gxShader* Shader, const char* Name)
{
    return glGetUniformLocation(Shader->Program, Name);
}

void gxSetShaderConstant1(gxShaderConstant Constant, float x)
{
    glUniform1f(Constant, x);
}

void gxSetShaderConstant2(gxShaderConstant Constant, float x, float y)
{
    glUniform2f(Constant, x, y);
}

void gxSetShaderConstant3(gxShaderConstant Constant, float x, float y, float z)
{
    glUniform3f(Constant, x, y, z);
}

void gxSetShaderConstant4(gxShaderConstant Constant, float x, float y, float z, float w)
{
    glUniform4f(Constant, x, y, z, w);
}

void gxSetShaderSampler(gxShaderConstant Constant, gxSprite* Sprite)
{
    glActiveTexture(GL_TEXTURE0 + Constant);
    glBindTexture(GL_TEXTURE_2D, Sprite->tex);
    glUniform1i(Constant, Constant);
    glActiveTexture(GL_TEXTURE0);
}

void gxCreateRenderTarget(int width, int height, gxSprite* sprite, bool Alpha)
{
	sprite->width = width;
	sprite->height = height;
	
	sprite->texWidth = sprite->width;
	sprite->texHeight = sprite->height;
	
	void* pixels = calloc(sprite->texWidth * sprite->texHeight * 4, 1);

    glGenTextures(1, &sprite->tex);
    glBindTexture(GL_TEXTURE_2D, sprite->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (Alpha)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->texWidth, sprite->texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sprite->texWidth, sprite->texHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
        
	glGenFramebuffers(1, &sprite->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, sprite->framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sprite->tex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _gxDefaultFrameBuffer);
    
    free(pixels);
}

void gxSetRenderTarget(gxSprite* Sprite)
{
    if (Sprite)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, Sprite->framebuffer);    
        glViewport(0, 0, Sprite->width, Sprite->height);
    }
    else
    {
#if TARGET_OS_MAC && !TARGET_OS_IPHONE
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
#else
        glBindFramebuffer(GL_FRAMEBUFFER, _gxDefaultFrameBuffer);    
        glViewport(0, 0, _gxDefaultFrameBufferWidth, _gxDefaultFrameBufferHeight);
#endif
    }

//    GLenum attach[] = { GL_COLOR_ATTACHMENT0 };
//    glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, attach);    
    glClearColor(0, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void gxClearColor(unsigned int Color)
{
    int A = (Color>>24) & 0xFF;
    int B = (Color>>16) & 0xFF;
    int G = (Color>>8) & 0xFF;
    int R = (Color>>0) & 0xFF;
    
    glClearColor(R/255.0f, G/255.0f, B/255.0f, A/255.0f);
    
    glClear(GL_COLOR_BUFFER_BIT);
}

