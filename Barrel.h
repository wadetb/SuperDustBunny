#ifndef BARREL_H
#define BARREL_H

enum EBarrelState
{
	BARRELSTATE_WAIT,
	BARRELSTATE_TURN,
	BARRELSTATE_LAUNCH,
	BARRELSTATE_RESET,
};

struct SBarrel
{
	int X, Y;

	EBarrelState State;

	int FromDir;
	int ToDir;
	int Dir;

	int Timer;
};

void CreateBarrel(int X, int Y, const char* Desc);

void UpdateBarrels();
void DisplayBarrels();

#endif
