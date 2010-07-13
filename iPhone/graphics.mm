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

void gxUpdateScreen()
{
}

void gxInit(int xres, int yres, bool window)
{
	gxScreenWidth = xres;
	gxScreenHeight = yres;
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	msSetMouseRange( 0, 0, gxScreenWidth, gxScreenHeight );	
}

void gxExit()
{
}

void gxLoadSprite(const char* filename, gxSprite* sprite)
{
	filename = strrchr(filename, '/')+1; // Eliminate path on iPhone.
	
	NSString* nsname = [[NSString alloc] initWithCString:filename];
	CGImageRef image = [[UIImage imageNamed:nsname] CGImage];
	if (image == nil)
	{
		NSLog(@"could not find %@", nsname);
		return;
	}
	[nsname release];
	sprite->width = CGImageGetWidth(image);
	sprite->height = CGImageGetHeight(image);
	
	sprite->texWidth = 1;
	while (sprite->texWidth < sprite->width) sprite->texWidth *= 2;

	sprite->texHeight = 1;
	while (sprite->texHeight < sprite->height) sprite->texHeight *= 2;
	
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
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	printf("GL_ERROR in createSprite: %d", error);
	//A.J.'s Addendum
	sprite->fileName = new char[strlen(filename) + 1];
	strcpy(sprite->fileName, filename);
}

void gxDestroySprite(gxSprite* sprite)
{
	if(sprite->tex != 0)
	glDeleteTextures(1, &(sprite->tex));
	sprite->tex = 0;
	sprite->width = 0;
	sprite->height = 0;
	sprite->texWidth = 0;
	sprite->texHeight = 0;
	
	if(sprite->fileName)
		delete[] sprite->fileName;
	sprite->fileName = NULL;
}

// Presumes render states have been set up already.
// Sets the texture and submits the polygon.
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
	glDisable(GL_BLEND);
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

void gxDrawSpriteAlpha(int x, int y, gxSprite *sprite, int alpha)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, sprite->texWidth, sprite->texHeight, gxRGBA32(255, 255, 255, alpha));
}

void gxDrawSpriteScaledFilled(int x, int y, gxSprite* sprite, int width, int height, unsigned int color)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, width * sprite->texWidth/sprite->width, height * sprite->texHeight/sprite->height);
}

void gxDrawSpriteScaled(int x, int y, gxSprite* sprite, int width, int height)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, width * sprite->texWidth/sprite->width, height * sprite->texHeight/sprite->height);
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


void gxDrawRect(int x, int y, int width, int height, unsigned int color)
{
	GLuint def;
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_gxDrawQuad(x, y, width, height, color, 0, 1, 0, 1);
}

void gxLoadFont(const char* name, gxFont* font, int x, int y, int z)
{
}

void gxDestroyFont(gxFont* font)
{
}

void gxDrawText(int x, int y, const char* str, gxFont* font, unsigned int color, int flags)
{
}

void gxDrawInt(int x, int y, int value, gxFont* font, unsigned int color, int flags)
{
}

void gxGetResourceFileName(const char* relativePath, char* buffer, int bufferSize)
{
	// Get Bundle directory
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyBundleURL(mainBundle);
	UInt8 bundlePath[1024];
	CFURLGetFileSystemRepresentation(url, YES, bundlePath, sizeof(bundlePath));
	CFRelease(url);
	
	// Eliminate path
	const char* slash = strrchr(relativePath, '/');
	const char* fileName;
	if (slash != NULL)
		fileName = slash+1;
	else
		fileName = relativePath;
	
	snprintf(buffer, bufferSize, "%s/%s", (char*)bundlePath, fileName);
}

void gxGetFileName(const char* relativePath, char* buffer, int bufferSize)
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); //creates paths so that you can pull the app's path from it
	NSString *documentsDirectory = [paths objectAtIndex:0]; //gets the applications directory on the users iPhone
	NSString *fileName = [NSString stringWithCString:relativePath]; 
	const char* outputString = [[[documentsDirectory stringByAppendingString:@"/"] stringByAppendingString:fileName] UTF8String];
	strncpy(buffer, outputString, bufferSize);
	return;
	
	/*
	// Get Bundle directory
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyBundleURL(mainBundle);
	UInt8 bundlePath[1024];
	CFURLGetFileSystemRepresentation(url, YES, bundlePath, sizeof(bundlePath));
	CFRelease(url);
	
	// Eliminate path
	const char* slash = strrchr(relativePath, '/');
	const char* fileName;
	if (slash != NULL)
		fileName = slash+1;
	else
		fileName = relativePath;
	
	snprintf(buffer, bufferSize, "%s/Documents/%s", (char*)bundlePath, fileName);
	return;

	
	char name[256];
	char fileType[256];
	strncpy(name, relativePath, (int)(strchr(relativePath, '.') - relativePath));
	strncpy(fileType, relativePath + (int)(strchr(relativePath, '.') - relativePath) + 1, 256);
	
	NSString * path = [[NSBundle mainBundle] pathForResource:@"sdb" ofType: @"sav"];
	
	const char* tempstring = [path UTF8String];
	//buffer = new char[256];
	strncpy(buffer, tempstring, bufferSize);
	return;
	 */
}
