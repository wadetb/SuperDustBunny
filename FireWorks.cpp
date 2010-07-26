//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "FireWorks.h"
#include "Dusty.h"
#include "chapter.h"

#define MAX_FIREWORKS 100

int NFireWorks = 0;
SFireWork FireWorks[MAX_FIREWORKS];

void CreateFireWork(int X, int Y, const char* Desc)
{
	SFireWork* FireWork = &FireWorks[NFireWorks++];

	FireWork->X = X + 32;
	FireWork->Y = Y + 32;

	sscanf(Desc, "firework dir=%d", &FireWork->Dir);

	FireWork->State = FIREWORKSTATE_WAIT;
}

extern gxSprite FireWork01;
extern gxSprite FireWork01;

extern int ScrollY;

void DisplayFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];
		
		if (FireWork->State == FIREWORKSTATE_WAIT || FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			gxDrawSpriteCenteredRotated(FireWork->X, FireWork->Y + ScrollY, FireWork->Dir * 3.14159f / 180.0f, &FireWork01);
		}

		if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			gxDrawSpriteCenteredRotated( FireWork->X, FireWork->Y + ScrollY, 0, &FireWork02);				
		}
	}
}

void UpdateFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];

		if (FireWork->State == FIREWORKSTATE_WAIT)
		{
			float XDist = (float)(Dusty.FloatX - FireWork->X);
			float YDist = (float)((Dusty.FloatY-50) - (FireWork->Y));
			float Dist = sqrtf(XDist*XDist + YDist*YDist);

			if (Dist < 70)
			{
				Dusty.FloatX = FireWork->X;
				Dusty.FloatY = (FireWork->Y) + 50;
				FireWork->Timer = 10;
				FireWork->State = FIREWORKSTATE_LAUNCH;
			}
		}
		
		if (FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			float Angle = (float)((90 - FireWork->Dir)) * 3.14159f/180.0f;
			float Velocity = 20.0f;
			FireWork->X += Velocity*cosf(Angle);
			FireWork->Y += -Velocity*sinf(Angle);

			FireWork->Timer--;

			if (FireWork->Timer == 0)
			{
				FireWork->Timer = 10;
				FireWork->State = FIREWORKSTATE_EXPLODE;
			}
		}

		if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			FireWork->Timer--;
			if (FireWork->Timer == 0)
			{
				FireWork->State = FIREWORKSTATE_DONE;
			}
		}
	}
}
