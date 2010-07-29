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
#include "Vacuum.h"


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
#define WIND_HEIGHT		10


EWindDirection MidWindDir[WIND_HEIGHT * WIND_WIDTH] =
{
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
	WD_SE, WD_SE, WD_S,  WD_SW, WD_SW,
	WD_SE, WD_SE, WD_S,  WD_SW, WD_SW,
	WD_SE, WD_SE, WD_S,  WD_SW, WD_SW,
	WD_SE, WD_SE, WD_S,  WD_SW, WD_SW,
};

int MidWindStrength[WIND_HEIGHT * WIND_WIDTH] =
{
	1,     1,     1,     1,     1,    
	1,     5,     5,     5,     1,    
	1,     5,     5,     5,     1,    
	5,     7,     10,     7,     5,    
	5,     7,     10,     7,     5,    
	5,     7,     10,     7,     5,    
	5,     7,     10,     7,     5,    
	5,     7,     10,     7,     5,    
	7,     7,     10,     7,     7,    
	7,     7,     10,     7,     7,    
};

int HighWindStrength[WIND_HEIGHT * WIND_WIDTH] =
{
	30,     30,     30,     30,     30,   
	30,     40,     40,     40,     30,   
	30,     40,     40,     40,     30,   
	40,     60,     80,     60,     40,  
	40,     60,     80,     60,     40,  
	40,     60,     80,     60,     40,  
	40,     60,     80,     60,     40,  
	40,     60,     80,     60,     40,  
	60,     60,     80,     60,     60,  
	60,     60,     80,     60,     60,  
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
		EWindDirection* WindDirTable = NULL;
		int* WindStrengthTable = NULL;

		if (Vacuum.State == VACUUMSTATE_NEAR)
		{
			WindDirTable = MidWindDir;
			WindStrengthTable = MidWindStrength;
		}
		else if (Vacuum.State == VACUUMSTATE_ONSCREEN)
		{
			WindDirTable = MidWindDir;
			WindStrengthTable = HighWindStrength;
		}

		float StepX = (float)gxScreenWidth / WIND_WIDTH;
		float StepY = (float)gxScreenHeight / WIND_HEIGHT;

		for (int x = 0; x < WIND_WIDTH; x++)
		{
			for (int y = 0; y < WIND_HEIGHT; y++)
			{
				float Dir = (WindDirTable[y * WIND_WIDTH + x] * 45) * PI / 180.0f;
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

		gxDrawSpriteCenteredScaledAlphaAdd((int)(Mote->X), (int)(Mote->Y + ScrollY*Mote->Size), Mote->Size*1.5f, Mote->Size*1.5f, Alpha, &DustMoteSprite);
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
			Mote->VX -= XDist/Dist * RemapFloat(Dist, 0, 150, 0.1f, 0.0f, true);
			Mote->VY -= YDist/Dist * RemapFloat(Dist, 0, 150, 0.1f, 0.0f, true);
		}

		// Apply terminal velocity.
		float Velocity = sqrtf(Mote->VX*Mote->VX + Mote->VY*Mote->VY);
		if (Velocity > 1.0f)
		{
			Mote->VX = Mote->VX * 0.95f;
			Mote->VY = Mote->VY * 0.95f;
		}

		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Vacuum Wind Effects                                                                   //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		EWindDirection* WindDirTable = NULL;
		int* WindStrengthTable = NULL;

		if (Vacuum.State == VACUUMSTATE_NEAR)
		{
			WindDirTable = MidWindDir;
			WindStrengthTable = MidWindStrength;
		}
		else if (Vacuum.State == VACUUMSTATE_ONSCREEN)
		{
			WindDirTable = MidWindDir;
			WindStrengthTable = HighWindStrength;
		}

		if (WindDirTable != NULL && WindStrengthTable != NULL)
		{
			// Map mote position onto wind grid.
			int WindX = (int)(Mote->X / gxScreenWidth * WIND_WIDTH);
			int WindY = (int)((Mote->Y + ScrollY*Mote->Size) / gxScreenHeight * WIND_HEIGHT);

			if (WindX < 0) WindX = 0;
			if (WindX >= WIND_WIDTH) WindX = WIND_WIDTH-1;
			if (WindY < 0) WindY = 0;
			if (WindY >= WIND_HEIGHT) WindY = WIND_HEIGHT-1;

			EWindDirection WindDir = WindDirTable[WindY * WIND_WIDTH + WindX];
			float WindStrength = WindStrengthTable[WindY * WIND_WIDTH + WindX] / 50.0f;

			if (WindStrength > 0)
			{
				Mote->VX += cosf((90 - WindDir * 45) * PI / 180.0f) * WindStrength;
				Mote->VY += -sinf((90 - WindDir * 45) * PI / 180.0f) * WindStrength;
			}
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
		if (Mote->Y + ScrollY*Mote->Size > gxScreenHeight + 50)
		{
			Recycle = true;
			Mote->Y = RandomFloat(-100.0f, 100.0f) - (float)ScrollY;
		}

		// If it's gone off the top of the screen, reintroduce it at the bottom.
		if (Mote->Y + ScrollY*Mote->Size < -50)
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

			Mote->Size = RandomFloat(0.75f, 1.25f);
			Mote->Life = RandomFloat(2.0f, 10.0f);

			Mote->Time = 0;
		}
	}
}

