#include "graphics.h"
#include "mouse.h"
#include <limits.h>

int gxScreenWidth;
int gxScreenHeight;

GLuint _gxDefaultFrameBuffer;
GLuint _gxDefaultFrameBufferWidth;
GLuint _gxDefaultFrameBufferHeight;

GLuint gxOpenGLESVersion;

struct gxSpriteVertex
{
	float x, y, z, w;
	unsigned int color;
	float u, v;
};

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

gxShader gxSpriteShader;

void gxInitSpriteShader()
{
    gxCreateShader(gxSpriteVertexShaderSource, gxSpritePixelShaderSource, &gxSpriteShader);
}

void gxInit()
{
    gxScreenWidth = 768;
    gxScreenHeight = 1024;
	
	msSetMouseRange( 0, 0, gxScreenWidth, gxScreenHeight );	
	
    glEnable(GL_TEXTURE_2D);
    
    if (gxOpenGLESVersion == 2)
    {
        glEnableVertexAttribArray(GX_ATTRIB_VERTEX);
        glEnableVertexAttribArray(GX_ATTRIB_TEXCOORD);
        glEnableVertexAttribArray(GX_ATTRIB_COLOR);
    }
    else
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
    }
    
	gxInitFontSprite();
    
    if (gxOpenGLESVersion == 2)
    {
        gxInitSpriteShader();
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(0, gxScreenWidth, gxScreenHeight, 0, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}

void gxDeinit()
{
}

gxDisplayType gxGetDisplayType()
{
    float Version = [[[UIDevice currentDevice] systemVersion] floatValue];
    if (Version < 3.2f)
        return GXDISPLAY_IPHONE_PORTRAIT;

    UIScreen* MainScreen = [UIScreen mainScreen];
    int Width = MainScreen.currentMode.size.width;
    int Height = MainScreen.currentMode.size.height;

    if (Width == 768 && Height == 1024)
        return GXDISPLAY_IPAD_PORTRAIT;

    if (Width == 640 && Height == 960)
        return GXDISPLAY_IPHONE_RETINA_PORTRAIT;
    
    return GXDISPLAY_IPHONE_PORTRAIT;    
}

void gxLoadSprite(const char* filename, gxSprite* sprite)
{
	NSString* nsname = [[NSString alloc] initWithCString:filename];
	CGImageRef image = [[[UIImage imageNamed:nsname] retain] CGImage];
	if (image == nil)
	{
		NSLog(@"could not find %@", nsname);
		[nsname release];
		return;
	}
	[nsname release];
	
	sprite->width = CGImageGetWidth(image);
	sprite->height = CGImageGetHeight(image);
	
	sprite->texWidth = 1;
	while (sprite->texWidth < sprite->width) 
		sprite->texWidth *= 2;

	sprite->texHeight = 1;
	while (sprite->texHeight < sprite->height) 
		sprite->texHeight *= 2;
	
	void* pixels = calloc(sprite->texWidth * sprite->texHeight * 4, 1);
	CGColorSpaceRef colorSpace = CGImageGetColorSpace(image);
	CGContextRef context = CGBitmapContextCreate(pixels, sprite->texWidth, sprite->texHeight, 8, 4 * sprite->texWidth, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
	CGColorSpaceRelease(colorSpace);
	CGContextDrawImage(context, CGRectMake(0, sprite->texHeight-sprite->height, sprite->width, sprite->height), image);
	
	glEnable(GL_TEXTURE_2D);
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
    gxSpriteVertex v[4];
	
    v[0].x = x; v[0].y = y;
	v[0].color = color;
	v[0].u = u1; v[0].v = v1;
	
	v[1].x = x+w; v[1].y = y;
	v[1].color = color;
	v[1].u = u2; v[1].v = v1;
	
	v[2].x = x+w; v[3].y = y+h;
	v[2].color = color;
	v[2].u = u2; v[3].v = v2;
	
	v[3].x = x; v[2].y = y+h;
	v[3].color = color;
	v[3].u = u1; v[2].v = v2;
	
    if (gxOpenGLESVersion == 2)
    {
        glVertexAttribPointer(GX_ATTRIB_VERTEX, 2, GL_FLOAT, 0, sizeof(gxSpriteVertex), &v[0].x);
        glVertexAttribPointer(GX_ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(gxSpriteVertex), &v[0].u);
        glVertexAttribPointer(GX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, sizeof(gxSpriteVertex), &v[0].color);
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].x);
        glTexCoordPointer(2, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].u);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(gxSpriteVertex), &v[0].color);
    }

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color)
{
    if (gxOpenGLESVersion == 2)
    {
        gxSetShader(&gxSpriteShader);
    }
    else
    {
        // ES1 TODO
    }
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, width, height, color, 0, 1, 0, 1);
}

void gxDrawString( int x, int y, int ptsize, int color, const char* text, ... )
{
	char work[16384];
	
	va_list args;
	va_start(args, text);
	vsnprintf(work, sizeof(work), text, args);
	va_end(args);
	
    if (gxOpenGLESVersion == 2)
    {
        gxSetShader(&gxSpriteShader);
    }
    else
    {
        // ES1 TODO
    }
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gxFontSprite.tex);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	for (const char* p = work; *p; p++)
	{
		unsigned char ch = *p;
		if (ch < 32 || ch > 127)
			continue;
		
		float v0 = (float)( ch - 32 ) * 8 / gxFontSprite.height;
		float v1 = (float)( ch - 32 + 1 ) * 8 / gxFontSprite.height;
		
		_gxDrawQuad( x, y, ptsize, ptsize, color, 0.0f, v0, 1.0f, v1 );
		
		x += ptsize;
	}
}

void gxGetResourceFileName(const char* relativePath, char* buffer, int bufferSize)
{
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyBundleURL(mainBundle);
	UInt8 bundlePath[1024];
	CFURLGetFileSystemRepresentation(url, YES, bundlePath, sizeof(bundlePath));
	CFRelease(url);
	
	snprintf(buffer, bufferSize, "%s/%s", (char*)bundlePath, relativePath);
}

FILE* gxOpenFile(const char* relativePath, const char* mode)
{
	char work[1024];
	gxGetResourceFileName(relativePath, work, sizeof(work));
	return fopen(work, mode);
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

void gxSetShaderConstant(gxShaderConstant Constant, float x)
{
    glUniform1f(Constant, x);
}

void gxSetShaderConstant(gxShaderConstant Constant, float x, float y)
{
    glUniform2f(Constant, x, y);
}

void gxSetShaderConstant(gxShaderConstant Constant, float x, float y, float z)
{
    glUniform3f(Constant, x, y, z);
}

void gxSetShaderConstant(gxShaderConstant Constant, float x, float y, float z, float w)
{
    glUniform4f(Constant, x, y, z, w);
}

void gxSetShaderSampler(gxShaderConstant Constant, gxSprite* Sprite)
{
    glActiveTexture(GL_TEXTURE0 + Constant);
    glEnable(GL_TEXTURE_2D);
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
        glBindFramebuffer(GL_FRAMEBUFFER, _gxDefaultFrameBuffer);    
        glViewport(0, 0, _gxDefaultFrameBufferWidth, _gxDefaultFrameBufferHeight);
    }

//    GLenum attach[] = { GL_COLOR_ATTACHMENT0 };
//    glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, attach);    
    glClearColor(0, 0, 0, 0);
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

void gxCopyRenderTarget(gxSprite* From, gxSprite* To)
{
	gxSetRenderTarget(To);
 
	_gxSetAlpha( GXALPHA_NONE );
	_gxSetTexture(From);
    
	float HalfPixelX = -0.5f;
	float HalfPixelY = -0.5f;
    
	float HalfPixelU = 0.0f;
	float HalfPixelV = 0.0f;
    
	gxSpriteVertex v[4];
	v[0].x = 0.0f + HalfPixelX; 
	v[0].y = 0.0f + HalfPixelY; 
	v[0].color = gxRGBA32(255, 255, 255, 255);
	v[0].u = 0.0f+HalfPixelU; 
	v[0].v = 0.0f+HalfPixelV;
    
	v[1].x = (float)768 + HalfPixelX; 
	v[1].y = 0.0f + HalfPixelY; 
	v[1].color = gxRGBA32(255, 255, 255, 255);
	v[1].u = 1.0f+HalfPixelU; 
	v[1].v = 0.0f+HalfPixelV;
    
	v[2].x = (float)768 + HalfPixelX; 
	v[2].y = (float)1024 + HalfPixelY; 
	v[2].color = gxRGBA32(255, 255, 255, 255);
	v[2].u = 1.0f+HalfPixelU; 
	v[2].v = 1.0f+HalfPixelV;
    
	v[3].x = 0.0f + HalfPixelX; 
	v[3].y = (float)1024 + HalfPixelY; 
	v[3].color = gxRGBA32(255, 255, 255, 255);
	v[3].u = 0.0f+HalfPixelU; 
	v[3].v = 1.0f+HalfPixelV;
    
    if (gxOpenGLESVersion == 2)
    {
        glVertexAttribPointer(GX_ATTRIB_VERTEX, 2, GL_FLOAT, 0, sizeof(gxSpriteVertex), &v[0].x);
        glVertexAttribPointer(GX_ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(gxSpriteVertex), &v[0].u);
        glVertexAttribPointer(GX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, sizeof(gxSpriteVertex), &v[0].color);
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].x);
        glTexCoordPointer(2, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].u);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(gxSpriteVertex), &v[0].color);
    }
    
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
