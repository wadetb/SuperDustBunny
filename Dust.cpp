//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//
#include "Common.h"
#include "Dust.h"
#include "Dusty.h"


#define MAX_DUST_MOTES 50


SDustMote DustMotes[MAX_DUST_MOTES];


enum EWindDirection
{
	WD_N,
	WD_NE,
	WD_E,
	WD_SE,
	WD_S,
	WD_SW,
	WD_W,
	WD_NW
};


#define WIND_WIDTH		5
#define WIND_HEIGHT		5


EWindDirection MidWindDir[WIND_HEIGHT][WIND_WIDTH] =
{
	{  WD_S,  WD_S,  WD_S,  WD_S,  WD_S  },
	{  WD_S,  WD_S,  WD_S,  WD_S,  WD_S  },
	{  WD_SE, WD_S,  WD_S,  WD_S,  WD_SW },
	{  WD_E,  WD_SE, WD_S,  WD_SW, WD_W  },
	{  WD_NE, WD_SE, WD_S,  WD_SW, WD_NW },
};

int MidWindStrength[WIND_HEIGHT][WIND_WIDTH] =
{
	{  1,     1,     1,     1,     1,    },
	{  1,     1,     1,     1,     1,    },
	{  1,     1,     1,     1,     1,    },
	{  1,     1,     1,     1,     1,    },
	{  1,     1,     1,     1,     1,    },
};


void InitDust()
{
	for (int i = 0; i < MAX_DUST_MOTES; i++)
	{
		SDustMote* Mote = &DustMotes[i];

		Mote->X = -1000;
		Mote->Y = -1000;
	}
}

extern gxSprite ArrowSprite;
extern gxSprite DustMoteSprite;

float RemapFloat(float Value, float FromMin, float FromMax, float ToMin, float ToMax, bool Clamp)
{
	float RelativeValue = (Value - FromMin) / (FromMax - FromMin);
	if (Clamp)
	{
		if (RelativeValue < 0.0f) RelativeValue = 0.0f;
		if (RelativeValue > 1.0f) RelativeValue = 1.0f;
	}
	return ToMin + RelativeValue * (ToMax - ToMin);
}

void DisplayDust()
{
	if (DevMode)
	{
		float StepX = (float)gxScreenWidth / WIND_WIDTH;
		float StepY = (float)gxScreenHeight / WIND_HEIGHT;

		for (int x = 0; x < WIND_WIDTH; x++)
		{
			for (int y = 0; y < WIND_HEIGHT; y++)
			{
				float Dir = (MidWindDir[y][x] * 45) * PI / 180.0f;
				gxDrawSpriteCenteredRotated((int)((x+0.5f) * StepX), (int)((y+0.5f) * StepY), Dir, &ArrowSprite);
			}
		}
	}

	for (int i = 0; i < MAX_DUST_MOTES; i++)
	{
		SDustMote* Mote = &DustMotes[i];

		float Alpha = 1.0f;
		if (Mote->Time < 1.0f)
			Alpha = RemapFloat(Mote->Time, 0.0f, 1.0, 0.0f, 1.0f, true);
		if (Mote->Time > Mote->Life - 1.0f)
			Alpha = RemapFloat(Mote->Time, Mote->Life-1.0f, Mote->Life, 1.0f, 0.0f, true);

		gxDrawSpriteCenteredScaledAlphaAdd((int)(Mote->X), (int)(Mote->Y + ScrollY*Mote->Size), Mote->Size*1.5f, Mote->Size*1.5f, 1/Mote->Size * Alpha, &DustMoteSprite);
	}
}

float RandomFloat(float Min, float Max)
{
	unsigned int i;
	rand_s( &i );
	return (float)( Min + ( (double)i / (double)UINT_MAX ) * (Max-Min) );
}

void UpdateDust()
{
	for (int i = 0; i < MAX_DUST_MOTES; i++)
	{
		SDustMote* Mote = &DustMotes[i];

		Mote->X += Mote->VX;
		Mote->Y += Mote->VY;

		Mote->Time += 1.0f/60.0f;


		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Dusty Wind Effects                                                                    //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		float XDist = (float)(Dusty.FloatX - Mote->X);
		float YDist = (float)((Dusty.FloatY-50 + ScrollY) - (Mote->Y + ScrollY*Mote->Size));
		float Dist = sqrtf(XDist*XDist + YDist*YDist);

		if (Dist < 150)
		{
			Mote->VX -= XDist / 500.0f;
			Mote->VY -= YDist / 500.0f;
		}

		// Apply terminal velocity.
		float Velocity = sqrtf(Mote->VX*Mote->VX + Mote->VY*Mote->VY);
		if (Velocity > 1.0f)
		{
			Mote->VX = Mote->VX / Velocity * 1.0f;
			Mote->VY = Mote->VY / Velocity * 1.0f;
		}

		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Dust Mote Recycling                                                                   //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

		// Check to see if the dust mote has left the screen, if so recycle it and make a new one.
		bool Recycle = false;

		// If it's expired or gone off the side of the screen, reintroduce it somewhere random.
		if (Mote->Time >= Mote->Life || Mote->X >= gxScreenWidth || Mote->X < 0 )
		{
			Recycle = true;
			Mote->Y = RandomFloat(0.0f, (float)gxScreenHeight) - (float)ScrollY;
		}

		// If it's gone off the bottom of the screen, reintroduce it at the top.
		if (Mote->Y + ScrollY > gxScreenHeight + 50)
		{
			Recycle = true;
			Mote->Y = RandomFloat(-100.0f, 100.0f) - (float)ScrollY;
		}

		// If it's gone off the top of the screen, reintroduce it at the bottom.
		if (Mote->Y + ScrollY < -50)
		{
			Recycle = true;
			Mote->Y = RandomFloat(gxScreenHeight-100.0f, gxScreenHeight+100.0f) - (float)ScrollY;
		}

		// Recycle this dust mote with new parameters if requested.
		if (Recycle)
		{
			Mote->X = RandomFloat(0, (float)gxScreenWidth);

			Mote->VX = RandomFloat(-0.3f, 0.3f);
			Mote->VY = RandomFloat(-0.3f, 0.3f);

			Mote->Size = RandomFloat(0.5f, 1.5f);
			Mote->Life = RandomFloat(2.0f, 10.0f);

			Mote->Time = 0;
		}
	}
}

