#include "graphics.h"
#include "mouse.h"
#include <limits.h>

int gxScreenWidth;
int gxScreenHeight;

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
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &gxFontSprite.tex);
	glBindTexture(GL_TEXTURE_2D, gxFontSprite.tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	
	free(buffer);	
}

void gxInit(gxDisplayType disp)
{
	switch (disp) 
	{
	case GXDISPLAY_IPHONE_PORTRAIT:
		gxScreenWidth = 320;
		gxScreenHeight = 480;
		break;
	case GXDISPLAY_IPHONE_LANDSCAPE:
		gxScreenWidth = 480;
		gxScreenHeight = 320;
		break;
	case GXDISPLAY_IPAD_PORTRAIT: 
		gxScreenWidth = 768;
		gxScreenHeight = 1024;
		break;
	case GXDISPLAY_IPAD_LANDSCAPE: 
		gxScreenWidth = 1024;
		gxScreenHeight = 768;
		break;
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	msSetMouseRange( 0, 0, gxScreenWidth, gxScreenHeight );	
	
	gxInitFontSprite();
}

void gxDeinit()
{
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

void _gxDrawQuad( int x, int y, int w, int h, unsigned color = gxRGBA32(255,255,255,255), float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f )
{
	static gxSpriteVertex v[4];
	v[0].x = x; v[0].y = y; v[0].z = -10.0f; v[0].w = 1.0f;
	v[0].color = color;
	v[0].u = u1; v[0].v = v1;
	
	v[1].x = x+w; v[1].y = y; v[1].z = -10.0f; v[1].w = 1.0f;
	v[1].color = color;
	v[1].u = u2; v[1].v = v1;
	
	v[2].x = x+w; v[3].y = y+h; v[3].z = -10.0f; v[3].w = 1.0f;
	v[2].color = color;
	v[2].u = u2; v[3].v = v2;
	
	v[3].x = x; v[2].y = y+h; v[2].z = -10.0f; v[2].w = 1.0f;
	v[3].color = color;
	v[3].u = u1; v[2].v = v2;
	
	glVertexPointer(3, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].x);
	glTexCoordPointer(2, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].u);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(gxSpriteVertex), &v[0].color);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void gxDrawSprite(int x, int y, gxSprite* sprite)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, sprite->texWidth, sprite->texHeight);
}

void gxDrawSprite0(int x, int y, gxSprite* sprite)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, sprite->texWidth, sprite->texHeight);
}

void gxDrawSpriteAlpha(int x, int y, float alpha, gxSprite *sprite)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, sprite->texWidth, sprite->texHeight, gxRGBA32(255, 255, 255, (int)(alpha*255)));
}

void gxDrawSpriteClipped( int x, int y, gxSprite* p, int srcx, int srcy, int srcw, int srch )
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float u0 = float(srcx)/p->texWidth;
	float u1 = float(srcx + srcw)/p->texWidth;
	float v0 = float(srcy)/p->texHeight;
	float v1 = float(srcy + srch)/p->texHeight;
	_gxDrawQuad(x, y, srcw, srch, gxRGBA32(255,255,255,255), u0, v0, u1, v1);
}

void gxDrawRectangleFilled(int x, int y, int width, int height, unsigned int color)
{
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


void gxDrawSpriteCenteredRotated(int x, int y, float a, gxSprite* spr)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spr->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	float ca = cosf(a);
	float sa = sinf(a);
	
	float w = (float)spr->width * 0.5f;
	float h = (float)spr->height * 0.5f;
	
	gxSpriteVertex v[4];
	v[0].x = x + (-w * ca) - (-h * sa); 
	v[0].y = y + (-w * sa) + (-h * ca); 
	v[0].z = 0.0f; 
	v[0].w = 1.0f;
	v[0].color = 0xffffffff;
	v[0].u = 0.0f; 
	v[0].v = 0.0f;
	
	v[1].x = x + (+w * ca) - (-h * sa); 
	v[1].y = y + (+w * sa) + (-h * ca); 
	v[1].z = 0.0f; 
	v[1].w = 1.0f;
	v[1].color = 0xffffffff;
	v[1].u = float(spr->width)/spr->texWidth; 
	v[1].v = 0.0f;
	
	v[3].x = x + (-w * ca) - (+h * sa); 
	v[3].y = y + (-w * sa) + (+h * ca); 
	v[3].z = 0.0f; 
	v[3].w = 1.0f;
	v[3].color = 0xffffffff;
	v[3].u = 0.0f; 
	v[3].v = float(spr->height)/spr->texHeight;
	
	v[2].x = x + (+w * ca) - (+h * sa); 
	v[2].y = y + (+w * sa) + (+h * ca); 
	v[2].z = 0.0f; 
	v[2].w = 1.0f;
	v[2].color = 0xffffffff;
	v[2].u = float(spr->width)/spr->texWidth; 
	v[2].v = float(spr->height)/spr->texHeight;
	
	glVertexPointer(3, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].x);
	glTexCoordPointer(2, GL_FLOAT, sizeof(gxSpriteVertex), &v[0].u);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(gxSpriteVertex), &v[0].color);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void gxDrawSpriteCenteredScaledAlphaAdd(int x, int y, float scalex, float scaley, float alpha, gxSprite* sprite)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	float w = scalex * sprite->width;
	float h = scaley * sprite->height;
	_gxDrawQuad((float)x - w/2, (float)y - h/2, 
				w * sprite->texWidth / sprite->width, 
				h * sprite->texHeight / sprite->height, 
				gxRGBA32(255,255,255,(int)(255*alpha)));
}


void _gxSetAlpha( gxAlphaMode mode )
{
}

void _gxSetTexture( gxSprite* spr )
{
}

void _gxDrawQuad( float x, float y, float w, float h, unsigned int color, float u1, float v1, float u2, float v2 )
{
    
}
void _gxDrawQuad(
                 unsigned int Color,
                 float X0, float Y0, float U0, float V0, 
                 float X1, float Y1, float U1, float V1, 
                 float X2, float Y2, float U2, float V2, 
                 float X3, float Y3, float U3, float V3)
{
    
}

void gxCreateRenderTarget(int Width, int Height, gxSprite* Sprite)
{
    
}

void gxSetRenderTarget(gxSprite* Sprite)
{
    
}

void gxClearColor(unsigned int Color)
{
    
}

void gxCreatePixelShader(const char* Source, gxPixelShader* Shader)
{
        
}

void gxSetPixelShader(gxPixelShader* Shader)
{
    
}

void gxSetPixelShaderConst(int Index, float x, float y, float z, float w)
{
    
}

void gxCreateVertexShader(const char* Source, gxVertexShader* Shader)
{
    
}

void gxSetVertexShader(gxVertexShader* Shader)
{
    
}

void gxSetVertexShaderConst(int Index, float x, float y, float z, float w)
{
}
    
void gxCopyRenderTarget(gxSprite* From, gxSprite* To)
{
    
}
