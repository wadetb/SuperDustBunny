/*--------------------------------------------------------------------------------------------------------

  Mouse Library v1.0 by Wade Brainerd

  Library Overview:

  Global Variables:
    msButton1               Current state of the mouse buttons.
    msButton2               
    msOldButton1            State of button before the last call to msUpdateMouse.
    msOldButton2            

    msX                     Current screen coordinates of the mouse.
    msY                     

    msXDelta                Amount the mouse has moved since the last call to msUpdateMouse.
    msYDelta                

    msRangeX1               Range in which the mouse is allowed to move.
    msRangeY1
    msRangeX2
    msRangeY2

  Functions:
    msInit                  Captures the mouse from Windows.
    msDeinit                Restores the mouse to Windows.

    msUpdateMouse           Reads the position of the mouse from the hardware.

    msSetMouseRange         Sets the range within which the mouse can move.
  
--------------------------------------------------------------------------------------------------------*/
#ifndef MOUSE_H
#define MOUSE_H

/*--------------------------------------------------------------------------------------------------------

  msInit

  Initializes the mouse device.

--------------------------------------------------------------------------------------------------------*/
void msInit();

/*--------------------------------------------------------------------------------------------------------

  msDeinit

  Deinitializes the mouse device.

--------------------------------------------------------------------------------------------------------*/
void msDeinit( void );

/*--------------------------------------------------------------------------------------------------------

  msUpdateMouse

  Updates the mouse device.  The current status of the mouse is stored in global variables.

--------------------------------------------------------------------------------------------------------*/
void msUpdateMouse( void );

/*--------------------------------------------------------------------------------------------------------

  msButtonX, msOldButtonX

  msButtonX contains the current status of each mouse button.  If the button is pressed it contains a 1,
  otherwise 0.  
  
  msOldButtonX contains the status of the button before the most recent call to msUpdateMouse, and is
  useful for detecting clicks.

  Example:

  // Check for a click of button 1.
  if ( msButton1 && !msOldButton1 )
  {
    // Button 1 was clicked...
  }

--------------------------------------------------------------------------------------------------------*/
extern int msButton1;
extern int msButton2;
extern int msOldButton1;
extern int msOldButton2;

/*--------------------------------------------------------------------------------------------------------
  
  msX, msY

  These variables contain the current location of the mouse on the screen.

--------------------------------------------------------------------------------------------------------*/
extern int msX;
extern int msY;

/*--------------------------------------------------------------------------------------------------------

  msXDelta, msYDelta

  These variables contain the amount moved by the mouse since the last call to msUpdateMouse.  Note that
  they are unconstrained by msRangeXX and can be useful for detecting mouse motion as opposed to position.

--------------------------------------------------------------------------------------------------------*/
extern int msXDelta;
extern int msYDelta;

/*--------------------------------------------------------------------------------------------------------

  msSetMouseRange

  Limits the range of the mouse to within a certain area.

--------------------------------------------------------------------------------------------------------*/
void msSetMouseRange( int x1, int y1, int x2, int y2 );

/*--------------------------------------------------------------------------------------------------------

  msRangeX1, msRangeY1, msRangeX2, msRangeY2

  These variables hold the current mouse range, as set by msSetMouseRange.

--------------------------------------------------------------------------------------------------------*/
extern int msRangeX1;
extern int msRangeY1;
extern int msRangeX2;
extern int msRangeY2;

#endif
