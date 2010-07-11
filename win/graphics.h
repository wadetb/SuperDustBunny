/*--------------------------------------------------------------------------------------------------------
 
  Graphics Library v2.0 - 3D accelerated

  Library Overview:

  Structures:
    gxSprite                Sprite structure, contains an image that can be drawn to the screen.                                  
    gxFont                  Font structure, used for drawing text to the screen.                                                  
                                                                                                                                  
  Macros:                                                                                                                         
    gxRGB32                 Converts (R,G,B) to a 32bit color value.                                                              
                                                                                                                                  
  Global variables:                                                                                                               
    gxScreenWidth           Width of the screen as set by gxInit.                                                         
    gxScreenHeight          Height of the screen as set by gxInit.                                                        
                                                                                                                                  
    gxLocked                Set if the screen is locked for drawing.                                                              
                                                                                                                                  
    gxViewX1                Left edge of the drawing rectangle.                                                                   
    gxViewY1                Top edge of the drawing rectangle.                                                                    
    gxViewX2                Right edge of the drawing rectangle.                                                                  
    gxViewY2                Bottom edge of the drawing rectangle.                                                                 
                                                                                                                                  
    gxScreenPtr8            8bit array interface to screen pixels.                                                                
    gxScreenPtr32           32bit array interface to screen pixels.                                                               
	gxPitch                 Number of bytes per row of screen pixels.
                                                                                                                                  
    gxDDSSecondary          DirectDraw interface to the back Screen.                                                              
                                                                                                                                  
  Functions:                                                                                                                      
    gxInit                  Initializes the Direct3D display system.
    gxDeinit                Restores the windows desktop.                                                                         

    gxLockScreen            Locks the display for drawing.                                                            
    gxUnlockScreen          Unlocks the display.                                                                      
                                                                                                                      
    gxSetClipRange          Sets the rectangle that clips drawing routines.                                           
                                                                                                                      
    gxCreateSprite          Creates a new sprite.                                                                     
    gxDestroySprite         Destroys a sprite.                                                                        
                                                                                                                      
    gxDrawSprite            Draws a sprite.                                                                           
    gxDrawSprite0           Draws a sprite treating black (RGB 0,0,0) as transparent.                                 
    gxDrawSpriteFilled      Draws a sprite with transparency, however solid pixels are drawn a specific color.        
    gxDrawSpriteScaled      Draws a sprite with scaling and optional antialiasing.                                    
    gxDrawSpriteAlpha       Draws a sprite with a variable level of translucency.                                     
    gxDrawSpriteAdd         Draws a sprite by adding the pixels to the background.                                    
                                                                                                                      
    gxGetSpriteFromSprite   Copies data from one sprite into another sprite.                                          
    gxGetSpriteFromScreen   Copies data from the screen into a sprite.                                                
                                                                                                                      
    gxDrawRect              Draws a filled rectangle.                                                                 
                                                                                                                      
    gxUpdateScreen          Causes the current frame to be visible.                                                   
                                                                                                                      
    gxLoadFont              Loads a font from a *.raw file.                                                           
    gxDrawText              Draws text to the screen.                                                                 
    gxDrawInt               Draws a number to the screen.                                                             

--------------------------------------------------------------------------------------------------------*/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h> // - x
#include <d3dx9.h>

/*--------------------------------------------------------------------------------------------------------

  gxRGB32
  
  This macro takes Red, Green and Blue components and returns a 32bit color value.

--------------------------------------------------------------------------------------------------------*/
#define gxRGB32(r,g,b) D3DCOLOR_RGBA(r,g,b,255)
#define gxRGBA32(r,g,b,a) D3DCOLOR_RGBA(r,g,b,a)

/*--------------------------------------------------------------------------------------------------------

  gxSprite Structure

  Defines a 2D sprite/picture/bitmap, basically a rectangle with graphics in it.  Members define
  the data storage, width and height of the sprite.
  Note that in this version of the graphics library, sprites must be powers of 2 in both width and height.
  Powers of 2 are: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, etc.

--------------------------------------------------------------------------------------------------------*/
struct gxSprite
{
	gxSprite() : tex(NULL),width(0),height(0), fileName(NULL){}
  IDirect3DTexture9* tex;
  int width;
  int height;
  char* fileName;
};

/*--------------------------------------------------------------------------------------------------------

  gxInit, gxDeinit

  Call these two functions at the beginning and end of your program. xres and yres specify the 
  resolution you want to run in.

  If you pass true for windowed, the program will run in a window instead of full screen.

  Example:

  // Initialize the display to 640x480.
  gxInit( 640, 480 );

--------------------------------------------------------------------------------------------------------*/
void gxInit( int xres, int yres, bool windowed );
void gxDeinit();

/*--------------------------------------------------------------------------------------------------------

  gxScreenWidth and gxScreenHeight

  These contain the current width and height of the screen, as set by gxInit.  They are useful for placing 
  things on or near the right and bottom edges, for example:

  Example:

  // Draw sprite 'spr' at the right edge of the screen, 10 pixels from the top of the screen.
  gxDrawSprite( gxScreenWidth - spr.width, 10, &spr );

--------------------------------------------------------------------------------------------------------*/
extern int gxScreenWidth;
extern int gxScreenHeight;

/*--------------------------------------------------------------------------------------------------------

  gxSetClipRange

  gxThis function sets a rectangle to which all drawing will be clipped.  This is useful in a game where
  you want to leave a status bar at the bottom and don't want your game characters to draw into it.
  Alternatively, you could just redraw the status bar every frame but that will cost you speed.

  Ranges are set in screen coordinates, as per this diagram (assuming you're using 640x480):

  +----------------------+
  |(0,0)          (639,0)|
  |                      |
  |                      |
  |                      |
  |                      |
  |                      |
  |                      |
  |(0,479)      (639,479)|
  +----------------------+

  Example:

  // Only draw to the left half of the screen assuming 640x480 mode.
  gxSetClipRange( 0, 0, 320, 479 );

--------------------------------------------------------------------------------------------------------*/
void gxSetClipRange( int x1, int y1, int x2, int y2 );

/*--------------------------------------------------------------------------------------------------------

  gxViewX1, gxViewY1, gxViewX2, gxViewY2

  These define the the current drawing rectangle, as set by gxSetClipRange.  

--------------------------------------------------------------------------------------------------------*/
extern int gxViewX1;
extern int gxViewY1;
extern int gxViewX2;
extern int gxViewY2;

/*--------------------------------------------------------------------------------------------------------
  
  gxCreateSprite, gxDestroySprite

  Sprites are 2D images that can be created/loaded and drawn to the screen.  These two functions manage
  the creation and destruction of sprites.

  Example:

  // Create an empty 64x64 sprite.
  gxSprite spr;
  gxCreateSprite( &spr, 64, 64 );

  // (do stuff with the sprite)

  // Destroy it.  Don't forget to do this when you're done with it, otherwise you'll leak memory!
  gxDestroySprite( &spr );

--------------------------------------------------------------------------------------------------------*/
void gxCreateSprite( gxSprite* spr, int w, int h );
void gxDestroySprite( gxSprite* spr );

/*--------------------------------------------------------------------------------------------------------

  gxDrawSpriteXXXX

  These functions manage drawing sprites to the screen.  Each takes an x and y screen coordinate, the 
  sprite to draw and any other parameters required by the function.  See gxSetClipRanges for a description 
  of screen coordinates. 

  Example:

  // Lock the screen and draw a sprite.
  gxDrawSprite( 50, 50, &spr );

--------------------------------------------------------------------------------------------------------*/
// Basic sprite drawing function, no effects.
void gxDrawSprite( int x, int y, gxSprite* p );

// Color keyed.  Any instance of pure black (RGB 0,0,0) is treated as transparent.
void gxDrawSprite0( int x, int y, gxSprite* p );

// Color replacement.  Pure black is treated as transparent, any other color is drawn modulated by col (see gxRGB32).
void gxDrawSpriteFilled( int x, int y, gxSprite* p, DWORD col );

// Scales the sprite to w x h dimensions and color replacement.
void gxDrawSpriteScaledFilled( int x, int y, gxSprite* p, int w, int h, DWORD col  );

// Scales the sprite to w x h dimensions.
void gxDrawSpriteScaled( int x, int y, gxSprite* p, int w, int h );

// Alpha blending.  Alpha ranges from 0 (fully translucent/invisible) to 255 (fully opaque).
void gxDrawSpriteAlpha( int x, int y, gxSprite* p, int alpha );

// Additive blending.  Causes sprite pixels to be added to the background, useful for special effects.
void gxDrawSpriteAdd( int x, int y, gxSprite* p );

/*--------------------------------------------------------------------------------------------------------

  GetSpriteFromSprite, GetSpriteFromScreen

  These two functions fill an already created sprite with data from another sprite or from the screen.

  The x and y coordinates specify where in the 'source' sprite to take the data from, the size of the
  rectangle copied is determined by the size of the 'dest' sprite.

  Example:

  // Create a 64x64 sprite and copy the top left corner of the screen into it.
  gxSprite spr;
  gxCreateSprite( &spr, 64, 64 );
  gxGetSpriteFromScreen( 0, 0, &spr );

--------------------------------------------------------------------------------------------------------*/
void gxGetSpriteFromSprite( int x, int y, gxSprite* dest, gxSprite* source );
void gxGetSpriteFromScreen( int x, int y, gxSprite* dest );
void gxDrawSpriteClipped( int x, int y, gxSprite* p, int srcx, int srcy, int srcw, int srch );
/*--------------------------------------------------------------------------------------------------------

  gxDrawRect

  This function draws a solid colored rectangle to the screen, and is useful for clearing the screen
  and drawing simple interface elements.

  Example:

  // Clear the screen to black.
  gxDrawRect( 0, 0, 640, 480, gxRGB32( 0, 0, 0 ) );

--------------------------------------------------------------------------------------------------------*/
void gxDrawRect( int x, int y, int width, int height, int col );

/*--------------------------------------------------------------------------------------------------------

  gxUpdateScreen

  This function basically takes the frame you're currently building and puts it on the monitor.  
  Call it once per frame after you're done drawing.

  Example:

  // Make the current frame visible to the player.
  gxUpdateScreen();

--------------------------------------------------------------------------------------------------------*/
void gxUpdateScreen();

/*--------------------------------------------------------------------------------------------------------

  gxLoadBmp

  Loads a *.png file into a sprite.  The sprite must NOT have been created yet, unlike gxLoadRaw.
  Note that this function only loads 24bit, uncompressed BMP files.

  Example:

  // Create a new sprite from a 'image.png'.
  gxSprite spr;
  gxLoadBmp( "image.png", &spr );

--------------------------------------------------------------------------------------------------------*/
void gxLoadBmp( char* name, gxSprite* spr, long color );

/*--------------------------------------------------------------------------------------------------------

  gxFont Structure

  Defines up to 256 Sprites that make up  the alphabet, plus extra characters.  It also defines vertical 
  and horizontal character spacing.

--------------------------------------------------------------------------------------------------------*/
struct gxFont
{
	gxSprite spr;
	int pos[256];
	int width[256];
	int spacing;
};

/*--------------------------------------------------------------------------------------------------------

  gxLoadFont, gxDestroyFont

  gxLoadFont loads a new font into a gxFont structure from the *.raw file specified by filename.

  width is the width of the *.raw file, in pixels.
  height is the height of each row of the font.
  spacing is the amount of space (usually 2 pixels) you want between each letter.
  
  Example:
  gxFont font;
  gxLoadFont( "font.raw", &font, 420, 12, 2 );

--------------------------------------------------------------------------------------------------------*/
void gxLoadFont( char* filename, gxFont* f, int width, int height, int spacing );
void gxDestroyFont( gxFont* f );

/*--------------------------------------------------------------------------------------------------------

  gxDrawText, gxDrawInt

  These functions draw text to the screen using a certain color.  The type parameter specifies what kind 
  of text 'shadow' to draw.

--------------------------------------------------------------------------------------------------------*/
void gxDrawText(int x, int y, char* text, gxFont* f, DWORD color, int type);
void gxDrawInt(int x, int y, int txt, gxFont* f, DWORD color, int type);

/*--------------------------------------------------------------------------------------------------------

  gxScreenPtr8 and gxScreenPtr32

  Use these pointers to directly modify the frame Screen pixels as an array of bytes (8 bit, 4 array
  elements per pixel) or dwords (32 bit, one array element per pixel).  

  The method for calculating the array index of a particular screen pixel is:
  
  gxScreenPtr32[X + Y*gxPitch/4] = gxRGB32( R, G, B );

  or

  gxScreenPtr8[X*4 + Y*gxPitch + 0] = Red;
  gxScreenPtr8[X*4 + Y*gxPitch + 1] = Green;
  gxScreenPtr8[X*4 + Y*gxPitch + 2] = Blue;

  The reason for having gxScreenPtr8 is that you can modify a single component without affecting the
  other two.

--------------------------------------------------------------------------------------------------------*/
extern BYTE* gxScreenPtr8;
extern DWORD* gxScreenPtr32;
extern int gxPitch;

/*--------------------------------------------------------------------------------------------------------

Direct3D access

Use this pointer only if you know exactly what you're doing.  It allows you to access the Direct3D
subsystem for advanced things that aren't covered by this library.

--------------------------------------------------------------------------------------------------------*/
extern IDirect3DDevice9* gxDev; 

/*--------------------------------------------------------------------------------------------------------

gxLockScreen, gxUnlockScreen and gxLocked.

Before you can draw to the display using any of the gxScreenPtr arrays you have to call LockScreen, and 
when you're done you need to call UnlockScreen.

These functions take some time, so you should try and batch as many drawing functions together into
a single Lock/Unlock pair as possible.

The Locked variable is set when the screen is Locked.

Example:

// Lock the screen and draw a white pixel.
gxLockScreen();
gxScreenPtr32[0] = gxRGB32(255,255,255);
gxUnlockScreen();

--------------------------------------------------------------------------------------------------------*/
void gxLockScreen();
void gxUnlockScreen();

extern bool	gxLocked;

#endif