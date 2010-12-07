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


EWindDirection WindDir[WIND_HEIGHT * WIND_WIDTH] =
{
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_S,  WD_S,  WD_S,  WD_S,  WD_S,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
	WD_SE, WD_S,  WD_S,  WD_S,  WD_SW,
};

int WindStrength[WIND_HEIGHT * WIND_WIDTH] =
{
	3,     3,     3,     3,     3,    
	3,     5,     5,     5,     1,    
	3,     5,     5,     5,     1,    
	5,     7,     7,     7,     5,    
	5,     7,     10,     7,     5,    
	5,     7,     10,     7,     5,    
	5,     7,     10,     7,     5,    
	5,     7,     10,     7,     5,    
	7,     7,     10,     7,     7,    
	7,     7,     10,     7,     7,     
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
				float Dir = (WindDir[y * WIND_WIDTH + x] * 45) * PI / 180.0f;
				AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &DustArrowSprite, (x+0.5f) * StepX, (y+0.5f) * StepY, 1.0f, Dir);
			}
		}
	}

	for (int i = 0; i < MAX_DUST_MOTES; i++)
	{
		SDustMote* Mote = &DustMotes[i];

		float Alpha = Mote->Alpha;
		if (Mote->Time < 1.0f)
			Alpha *= Remap(Mote->Time, 0.0f, 0.25f, 0.0f, 1.0f, true);
		if (Mote->Time > Mote->Life - 1.0f)
			Alpha *= Remap(Mote->Time, Mote->Life-0.25f, Mote->Life, 1.0f, 0.0f, true);

		AddLitSpriteCenteredScaledColor(LIGHTLIST_DUST, &DustMoteSprite, Mote->X, Mote->Y + ScrollY*Mote->Depth, Mote->Size, gxRGBA32(192,192,192,(int)(255*Alpha)));
	}
}

void UpdateDust()
{
	for (int i = 0; i < MAX_DUST_MOTES; i++)
	{
		SDustMote* Mote = &DustMotes[i];

		Mote->X += Mote->VX;
		Mote->Y += Mote->VY;

		Mote->Time += 1.0f/60.0f;
		if (Length(Mote->VX, Mote->VY) >= 3.0f)
			Mote->Time += 1.5f/60.0f;

		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Dusty Wind Effects                                                                    //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		float XDist = (float)(Dusty.FloatX - Mote->X);
		float YDist = (float)((Dusty.FloatY-50 + ScrollY) - (Mote->Y + ScrollY*Mote->Size));
		float Dist = sqrtf(XDist*XDist + YDist*YDist);

		if (Dist < 150)
		{
			Mote->VX -= XDist/Dist * Remap(Dist, 0, 150, 0.1f, 0.0f, true);
			Mote->VY -= YDist/Dist * Remap(Dist, 0, 150, 0.1f, 0.0f, true);
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
		float VacuumStrength = 0.0f;
		if (Vacuum.State == VACUUMSTATE_FAR)
		{
			VacuumStrength = Lerp((float)Vacuum.Timer, 500, 0, 0.0f, 3.0f); 
		}
		else if (Vacuum.State == VACUUMSTATE_ONSCREEN)
		{
			// TODO for reverse direction
			if (Vacuum.Dir == VACUUMDIR_UP)
				VacuumStrength = Lerp(Vacuum.Y, Dusty.FloatY + 1000, Dusty.FloatY + 200, 1.5f, 20.0f); 
			else
				VacuumStrength = Lerp(Vacuum.Y, Dusty.FloatY - 1000, Dusty.FloatY - 200, 1.5f, 20.0f); 
		}

		// Map mote position onto wind grid.
		//int WindX = (int)(Mote->X / gxScreenWidth * WIND_WIDTH);
		//int WindY = (int)((Mote->Y + ScrollY*Mote->Depth) / gxScreenHeight * WIND_HEIGHT);

		//if (WindX < 0) WindX = 0;
		//if (WindX >= WIND_WIDTH) WindX = WIND_WIDTH-1;
		//if (WindY < 0) WindY = 0;
		//if (WindY >= WIND_HEIGHT) WindY = WIND_HEIGHT-1;

		//EWindDirection Dir = WindDir[WindY * WIND_WIDTH + WindX];
		//float Strength = VacuumStrength *  WindStrength[WindY * WIND_WIDTH + WindX] / 50.0f;

		//if (Strength > 0)
		//{
		//	Mote->VX += cosf(DirectionToAngle(Dir * 45.0f)) * Strength;
		//	Mote->VY += -sinf(DirectionToAngle(Dir * 45.0f)) * Strength;
		//}

		float DirX = (float)gxScreenWidth/2 - Mote->X;
		float DirY = (Vacuum.Y + ScrollY) - (Mote->Y + ScrollY*Mote->Depth);
		float Length = sqrtf(DirX*DirX + DirY*DirY);

		Mote->VX += DirX/Length * VacuumStrength * 0.1f;
		Mote->VY += DirY/Length * VacuumStrength * 0.1f;


		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Dust Mote Recycling                                                                   //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

		// Check to see if the dust mote has left the screen, if so recycle it and make a new one.
		bool Recycle = false;

		// If it's expired or gone off the side of the screen, reintroduce it somewhere random.
		if (Mote->Time >= Mote->Life || Mote->X >= gxScreenWidth || Mote->X < 0 )
		{
			Recycle = true;
			Mote->Y = Random(0.0f, (float)gxScreenHeight) - (float)ScrollY;
		}

		// If it's gone off the bottom of the screen, reintroduce it at the top.
		if (Mote->Y + ScrollY*Mote->Depth > gxScreenHeight + 50)
		{
			Recycle = true;
			Mote->Y = Random(-100.0f, 100.0f) - (float)ScrollY;
		}

		// If it's gone off the top of the screen, reintroduce it at the bottom.
		if (Mote->Y + ScrollY*Mote->Depth < -50)
		{
			Recycle = true;
			Mote->Y = Random(gxScreenHeight-100.0f, gxScreenHeight+100.0f) - (float)ScrollY;
		}

		// Recycle this dust mote with new parameters if requested.
		if (Recycle)
		{
			Mote->X = Random(0.0f, (float)gxScreenWidth);

			Mote->VX = Random(-0.3f, 0.3f);
			Mote->VY = Random(-0.3f, 0.3f);

			Mote->Size = Random(0.75f, 1.75f);
			Mote->Depth = 1.0f; //Random(0.999f, 1.001f);
			Mote->Life = Random(2.0f, 3.0f);

			Mote->Alpha = 1.0f;
			Mote->Time = 0;
		}
	}
}

void MakeDustMote(float X, float Y)
{
	SDustMote* Mote = &DustMotes[(int)Random(0, MAX_DUST_MOTES-1)];

	Mote->X = X;
	Mote->Y = Y;

	Mote->VX = Random(-3.0f, 3.0f);
	Mote->VY = Random(-3.0f, 3.0f);

	Mote->Size = Random(1.0f, 2.0f);
	Mote->Depth = 1.0f;
	Mote->Life = Random(2.0f, 10.0f);

	Mote->Alpha = 1.0f;
	Mote->Time = 1.0f;
}

