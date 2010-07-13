#ifndef KEYBOARD_H
#define KEYBOARD_H

void kbInit();
void kbExit();
#define kbDeinit kbExit

void kbUpdateKeys();

bool kbIsKeyDown(int key);
bool kbWasKeyDown(int key);

#define KB_H        0
#define KB_R        0
#define KB_M        0
#define KB_S		0
#define KB_D		0
#define KB_LSHIFT	0
#define KB_UP		0
#define KB_DOWN		0
#define KB_LEFT		0
#define KB_RIGHT	0
#define KB_SPACE	0
#define KB_RETURN	0
#define KB_ESCAPE	0
#define KB_RSHIFT	0

#endif
