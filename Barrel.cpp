#include "Common.h"
#include "Barrel.h"
#include "Dusty.h"

#define MAX_BARRELS 100

int NBarrels = 0;
SBarrel Barrels[MAX_BARRELS];

void CreateBarrel(int X, int Y, const char* Desc)
{
	SBarrel* Barrel = &Barrels[NBarrels++];

	Barrel->X = X + 32;
	Barrel->Y = Y + 32;

	sscanf(Desc, "barrel from=%d to=%d", &Barrel->FromDir, &Barrel->ToDir);//

	Barrel->State = BARRELSTATE_WAIT;
	Barrel->Dir = Barrel->FromDir;
}

extern gxSprite BarrelSprite;

extern int ScrollY;

void DisplayBarrels()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];

		gxDrawSpriteCenteredRotated(Barrel->X, Barrel->Y + ScrollY, Barrel->Dir * 3.14159f / 180.0f, &BarrelSprite);
	}
}

void UpdateBarrels()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];

		if (Barrel->State == BARRELSTATE_WAIT)
		{
			float XDist = (float)(Dusty.X - Barrel->X);
			float YDist = (float)((Dusty.Y-50) - (Barrel->Y));
			float Dist = sqrtf(XDist*XDist + YDist*YDist);

			if (Dist < 70)
			{
 				Dusty.X = Barrel->X;
				Dusty.Y = (Barrel->Y) + 50;
				SetDustyState_PrepareLaunch();
                Barrel->State = BARRELSTATE_TURN;
			}
		}
		else if (Barrel->State == BARRELSTATE_TURN)
		{
			Dusty.X = Barrel->X;
			Dusty.Y = (Barrel->Y) + 50;

			if (Barrel->Dir != Barrel->ToDir)
				Barrel->Dir = (Barrel->Dir+5) % 360;
			else
			{
				float Angle = (float)((90 - Barrel->Dir)) * 3.14159f/180.0f;
				float Velocity = 20.0f;
				SetDustyState_Launch(Velocity*cosf(Angle), -Velocity*sinf(Angle));
				Barrel->Timer = 30;
				Barrel->State = BARRELSTATE_LAUNCH;
			}
		}
		else if (Barrel->State == BARRELSTATE_LAUNCH)
		{
			Barrel->Timer--;
			if (Barrel->Timer == 0)
			{
				Barrel->State = BARRELSTATE_RESET;
			}
		}
		else if (Barrel->State == BARRELSTATE_RESET)
		{
			if (Barrel->Dir != Barrel->FromDir)
				Barrel->Dir = (Barrel->Dir+355) % 360;
			else
			{
				Barrel->State = BARRELSTATE_WAIT;
			}
		}
	}
}
