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
#include "Chapter.h"


SDustMote DustMotes[MAX_DUST_MOTES];


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
	for (int i = 0; i < MAX_DUST_MOTES; i++)
	{
		SDustMote* Mote = &DustMotes[i];

		float Alpha = Mote->Alpha;
		if (Mote->Time < 1.0f)
			Alpha *= Remap(Mote->Time, 0.0f, 0.25f, 0.0f, 1.0f, true);
		if (Mote->Time > Mote->Life - 1.0f)
			Alpha *= Remap(Mote->Time, Mote->Life-0.25f, Mote->Life, 1.0f, 0.0f, true);

		AddLitSpriteCenteredScaledColor(LIGHTLIST_DUST, &DustMoteSprite, Mote->X + ScrollX, Mote->Y + ScrollY*Mote->Depth, Mote->Size, gxRGBA32(192,192,192,(int)(255*Alpha)));
	}
}

void UpdateDust()
{
    float DustySpeed = Length(Dusty.FloatVelocityX, Dusty.FloatVelocityY);
    if (DustySpeed < 5)
        DustySpeed = 5;
    if (DustySpeed > 20)
        DustySpeed = 20;
    DustySpeed /= 200.0f;
    
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
		float XDist = (float)(Dusty.FloatX) - Mote->X;
		float YDist = (float)(Dusty.FloatY-50) - Mote->Y;
		float Dist = sqrtf(XDist*XDist + YDist*YDist);

		if (Dist < 250)
		{
			Mote->VX -= XDist/Dist * Remap(Dist, 100, 250, DustySpeed, 0.0f, true);
			Mote->VY -= YDist/Dist * Remap(Dist, 100, 250, DustySpeed, 0.0f, true);
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
		float VacuumStrength = 1.5f;

		float DirX, DirY;
		GetVacuumForce(Mote->X, Mote->Y, &DirX, &DirY, VacuumStrength, true);
		Mote->VX += DirX;
		Mote->VY += DirY;

		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Landing on stuff                                                                      //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        int BlockX = (int)(Mote->X / 64.0f);
        int BlockY = (int)(Mote->Y / 64.0f);
        if (Vacuum.State != VACUUMSTATE_ONSCREEN && BlockX >= 0 && BlockX < Chapter.PageWidth && BlockY >= 0 && BlockY < Chapter.PageHeight)
        {
            if (Chapter.PageBlocks[BlockY*Chapter.PageWidth+BlockX] != SPECIALBLOCKID_BLANK)
            {
                Mote->VX = 0;
                Mote->VY = 0;
            }
        }

		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
		//                                                   Dust Mote Recycling                                                                   //
		// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

		// Check to see if the dust mote has left the screen, if so recycle it and make a new one.
		bool Recycle = false;

		// If it's expired or gone off the side of the screen, reintroduce it somewhere random.
		if (Mote->Time >= Mote->Life || Mote->X >= Chapter.PageWidth*64 || Mote->X < 0 )
		{
			Recycle = true;
			Mote->Y = Random(0.0f, (float)LitScreenHeight) - (float)ScrollY;
		}

		// If it's gone off the bottom of the screen, reintroduce it at the top.
		if (Mote->Y + ScrollY*Mote->Depth > LitScreenHeight + 50)
		{
			Recycle = true;
			Mote->Y = Random(-100.0f, 100.0f) - (float)ScrollY;
		}

		// If it's gone off the top of the screen, reintroduce it at the bottom.
		if (Mote->Y + ScrollY*Mote->Depth < -50)
		{
			Recycle = true;
			Mote->Y = Random(LitScreenHeight-100.0f, LitScreenHeight+100.0f) - (float)ScrollY;
		}

		// Recycle this dust mote with new parameters if requested.
		if (Recycle)
		{
			Mote->X = Random(0.0f, (float)Chapter.PageWidth*64);

			Mote->VX = Random(-0.3f, 0.3f);
			Mote->VY = Random(-0.3f, 0.3f);

			Mote->Size = Random(0.75f, 1.75f);
			Mote->Depth = 1.0f; //Random(0.999f, 1.001f);
			Mote->Life = Random(8.0f, 10.0f);

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

