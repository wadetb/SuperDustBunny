/*--------------------------------------------------------------------------------------------------------
 
  Graphics Library v2.0 - 3D accelerated

  Library Overview:

  Structures:
    gxSprite                Sprite structure, contains an image that can be drawn to the screen.                                  
                                                                                                                                  
  Macros:                                                                                                                         
    gxRGB32                 Converts (R,G,B) to a 32bit color value.                                                              
                                                                                                                                  
  Global variables:                                                                                                               
    gxScreenWidth           Width of the screen as set by gxInit.                                                         
    gxScreenHeight          Height of the screen as set by gxInit.                                                        
                                                                                                                                  
  Functions:                                                                                                                      
    gxInit                  Initializes the Direct3D display system.
    gxDeinit                Restores the windows desktop.                                                                         

	gxSetClipRange          Sets a rectangle to which all drawing will be clipped.

    gxDrawSprite            Draws a sprite.                                                                           
    gxDrawRectangleFilled   Draws a filled rectangle.                                                                 
    gxDrawString            Draws formatted text. 

	gxLoadSprite            Loads a sprite from a file on disk.
	gxDestroySprite         Destroys a previously loaded sprite.

  Screen coordinates:
	X coordinates go from left to right and Y coordinates go from top to bottom, as per this diagram 
	(assuming you're using 640x480):

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

--------------------------------------------------------------------------------------------------------*/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
#include <d3dx9.h>

/*--------------------------------------------------------------------------------------------------------

  gxRGB32
  
  This macro takes Red, Green and Blue components and returns a 32bit color value as an int.
  It is the same as gxRGBA32(r, g, b, 255)

--------------------------------------------------------------------------------------------------------*/
#define gxRGB32(r,g,b)    D3DCOLOR_RGBA(r,g,b,255)

/*--------------------------------------------------------------------------------------------------------

  gxRGBA32
  
  This macro takes Red, Green, Blue and Alpha components and returns a 32bit color value as an int.

--------------------------------------------------------------------------------------------------------*/
#define gxRGBA32(r,g,b,a) D3DCOLOR_RGBA(r,g,b,a)

/*--------------------------------------------------------------------------------------------------------

  gxSprite Structure

  Defines a 2D sprite/picture/bitmap, basically a rectangle with graphics in it.  Members define
  the data storage, width and height of the sprite.

--------------------------------------------------------------------------------------------------------*/
struct gxSprite
{
  IDirect3DTexture9* tex;
  int width;
  int height;
};

/*--------------------------------------------------------------------------------------------------------
 
 gxDisplayType Enumerated Type
 
  Defines a display mode that is supported by the graphics library.  Use this to request a desired display
  mode and the graphics library will do the best it can to provide something like it.
 
  The display mode controls the values of gxScreenWidth and gxScreenHeight.
 
 --------------------------------------------------------------------------------------------------------*/
enum gxDisplayType
{
	GXDISPLAY_IPHONE_PORTRAIT,      // 320x480
	GXDISPLAY_IPHONE_LANDSCAPE,     // 480x320
	
	GXDISPLAY_IPAD_PORTRAIT,        // 768x1024
	GXDISPLAY_IPAD_LANDSCAPE,       // 1024x768
};

/*--------------------------------------------------------------------------------------------------------

  gxInit, gxDeinit

  Call these two functions at the beginning and end of your program. The type specifies the 
  resolution you want to run in.

  Example:

  // Initialize the display to emulate an iPad in portrait mode.
  gxInit( GXDISPLAY_IPAD_PORTRAIT );

--------------------------------------------------------------------------------------------------------*/
void gxInit( gxDisplayType type );
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

  This function sets a rectangle to which all drawing will be clipped.  This is useful in a game where
  you want to leave a status bar at the bottom and don't want your game characters to draw into it.
  Alternatively, you could just redraw the status bar every frame but that will cost you speed.

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

  gxDrawSprite

  These functions manage drawing sprites to the screen.  Each takes an x and y screen coordinate, the 
  sprite to draw and any other parameters required by the function.  See gxSetClipRanges for a description 
  of screen coordinates. 

  Example:

  // Draw a sprite at location (50, 50).
  gxDrawSprite( 50, 50, &spr );

--------------------------------------------------------------------------------------------------------*/
void gxDrawSprite( int x, int y, gxSprite* p );


void gxDrawSpriteScaled( int x, int y, float scalex, float scaley, gxSprite* p );

/*--------------------------------------------------------------------------------------------------------

  gxDrawFilledRectangle

  This function draws a solid colored rectangle to the screen, and is useful for clearing the screen
  and drawing simple interface elements.

  Example:

  // Clear the screen to black.
  gxDrawFilledRectangle( 0, 0, 640, 480, gxRGB32( 0, 0, 0 ) );

--------------------------------------------------------------------------------------------------------*/
void gxDrawRectangleFilled( int x, int y, int width, int height, int col );

/*--------------------------------------------------------------------------------------------------------

  gxDrawString

  This function draws text to the screen in a given color.  It accepts a point size and a color 
  (alpha supported) for the characters.  The standard point size is 8.

  gxDrawString supports all printf-style formatting codes.  Newlines and tabs are not supported.
  
  This is intended for debugging purposes only; it is not a pretty font.

  Example:

  int HeroHealth = 5;

  // Draw the hero's health value at position (5,5), 16pt font, white characeters.
  gxDrawString( 5, 5, 16, gxRGB32( 255, 255, 255 ), "Hero Health=%d", HeroHealth );

--------------------------------------------------------------------------------------------------------*/
void gxDrawString( int x, int y, int ptsize, int color, const char* text, ... );

/*--------------------------------------------------------------------------------------------------------

  gxLoadSprite

  Loads a *.png, *.jpg, *.bmp or other image file into a sprite.  

  Example:

  // Create a new sprite from a 'image.png'.
  gxSprite MySprite;
  gxLoadSprite( "image.png", &MySprite );

--------------------------------------------------------------------------------------------------------*/
void gxLoadSprite( const char* name, gxSprite* spr );

/*--------------------------------------------------------------------------------------------------------

  gxDestroySprite

  Destroys a previously loaded sprite.

--------------------------------------------------------------------------------------------------------*/
void gxDestroySprite();

/*--------------------------------------------------------------------------------------------------------

  Direct3D access

  Use this pointer only if you know exactly what you're doing.  It allows you to access the Direct3D
  subsystem for advanced things that aren't covered by this library.

--------------------------------------------------------------------------------------------------------*/
extern IDirect3DDevice9* gxDev; 


/*--------------------------------------------------------------------------------------------------------

  SuperDustBunny extensions

--------------------------------------------------------------------------------------------------------*/
void gxCreateASCIIBlockSprite(gxSprite* Sprite, const char* Key);
void gxDrawSpriteCenteredRotated(int x, int y, float a, gxSprite* spr);

#endif