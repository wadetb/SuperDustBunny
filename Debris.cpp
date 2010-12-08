//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Debris.h"
#include "Chapter.h"
#include "Vacuum.h"


#define MAX_DEBRIS 200


struct SDebris
{
	float X, Y;
	float Angle;
	float VX, VY;
	float VAngle;
	gxSprite* Sprite;
	int BlockID;
	bool Used;
};


int NDebris;

SDebris Debris[MAX_DEBRIS];


void InitDebris()
{
	NDebris = 0;

	for (int i = 0; i < MAX_DEBRIS; i++)
		Debris[i].Used = false;
}

void CreateDebris(float X, float Y, float Angle, gxSprite* Sprite)
{
	if (NDebris >= MAX_DEBRIS)
		ReportError("Exceeded the maximum of %d total debris.", MAX_DEBRIS);
	NDebris++;

	int i = 0;
	for (; i < MAX_DEBRIS; i++)
		if (Debris[i].Used == false)
			break;

	SDebris* Deb = &Debris[i];
	Deb->Used = true;
	Deb->X = X;
	Deb->Y = Y;
	Deb->Angle = Y;
	Deb->Sprite = Sprite;
	Deb->BlockID = -1;
	Deb->VX = 0;
	Deb->VY = 0;
	Deb->VAngle = Random(-0.05f, 0.05f);
}

void CreateDebris(float X, float Y, float Angle, int BlockID)
{
	if (NDebris >= MAX_DEBRIS)
		ReportError("Exceeded the maximum of %d total debris.", MAX_DEBRIS);
	NDebris++;

	int i = 0;
	for (; i < MAX_DEBRIS; i++)
		if (Debris[i].Used == false)
			break;

	SDebris* Deb = &Debris[i];
	Deb->Used = true;
	Deb->X = X;
	Deb->Y = Y;
	Deb->Angle = Y;
	Deb->Sprite = NULL;
	Deb->BlockID = BlockID;
	Deb->VX = 0;
	Deb->VY = 0;
	Deb->VAngle = Random(-0.05f, 0.05f);
}

void DisplayDebris()
{
	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		SDebris* Deb = &Debris[i];

		if (!Deb->Used)
			continue;

		if (Deb->Sprite)
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, Deb->Sprite, Deb->X, Deb->Y, 1.0f, Deb->Angle);
		else
		{
			SBlock* Block = &Chapter.Blocks[Deb->BlockID];
			STileSet* TileSet = &Chapter.TileSets[Block->TileSet];
			int SubX = Block->SubX;
			int SubY = Block->SubY;

			AddLitSubSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TileSet->Sprite, Deb->X, Deb->Y + ScrollY, (float)SubX, (float)SubY, (float)SubX + 64, (float)SubY + 64, 1.0f, Deb->Angle);
		}
	}
}

void UpdateDebris()
{
	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		SDebris* Deb = &Debris[i];

		if (!Deb->Used)
			continue;

		float DirX, DirY;
		GetVacuumForce(Deb->X, Deb->Y, &DirX, &DirY, 0.1f);

		Deb->VX += DirX;
		Deb->VY += DirY;

		Deb->VY += 0.125f;

		Deb->X += Deb->VX;
		Deb->Y += Deb->VY;
		Deb->Angle += Deb->VAngle;

		if (Deb->Y + ScrollY >= gxScreenHeight || Distance(Deb->X, Deb->Y, (float)gxScreenWidth/2, Vacuum.Y) < 100.0f)
		{
			Deb->Used = false;
		}
	}
}
