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

extern float msAccelX;
extern float msAccelY;
extern float msAccelZ;

