void msInit();
void msExit();
#define msDeinit msExit

void msUpdateMouse();

extern int msButton1;
extern int msButton2;
extern int msOldButton1;
extern int msOldButton2;

extern int msX;
extern int msY;

extern int msXDelta;
extern int msYDelta;

void msSetMouseRange( int x1, int y1, int x2, int y2 );

extern int msRangeX1;
extern int msRangeY1;
extern int msRangeX2;
extern int msRangeY2;

extern float msAccelX;
extern float msAccelY;
extern float msAccelZ;

