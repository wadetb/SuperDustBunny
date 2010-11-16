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
#include "Tutorial.h"
#include "chapter.h"
#include "Coin.h"
#include "Gear.h"
#include "TennisBall.h"

#define MAX_FIREWORKS 100

int NFireWorks = 0;
SFireWork FireWorks[MAX_FIREWORKS];

#define MAX_FIREWORK_TRAILS			1024
#define FIREWORK_TRAIL_HISTORY		32

enum EFireWorkTrailType
{
	FIREWORKTRAIL_SINGLE_STAGE,
	FIREWORKTRAIL_DOUBLE_STAGE,
};

struct SFireWorkTrail
{
	EFireWorkTrailType Type;

	float X, Y, Z;
	float VX, VY, VZ;	
	float CenterX, CenterY;

	float Life;

	int HistoryCount;
	float XHistory[FIREWORK_TRAIL_HISTORY];
	float YHistory[FIREWORK_TRAIL_HISTORY];
	float ZHistory[FIREWORK_TRAIL_HISTORY];

	int NextFireWorkTrail;
};

SFireWorkTrail FireWorkTrails[MAX_FIREWORK_TRAILS];

int FirstInactiveFireWorkTrail = -1;
int FirstActiveFireWorkTrail = -1;


void ParseFireWorkProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SFireWorkProperties* FireWorkProperties = (SFireWorkProperties*)malloc(sizeof(SFireWorkProperties));

	// Set default values.
	FireWorkProperties->Dir = 0;
	FireWorkProperties->Dist = 5;
	FireWorkProperties->Fuse = 0;
	FireWorkProperties->Size = 3;

	// Scan properties for values.
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* Value = PropertyNode->first_attribute("value")->value();

		if (strcmp(Name, "dir") == 0)
		{
			FireWorkProperties->Dir = atoi(Value);
		}
		else if (strcmp(Name, "dist") == 0)
		{
			FireWorkProperties->Dist = atoi(Value);
		}
		else if (strcmp(Name, "fuse") == 0)
		{
			FireWorkProperties->Fuse = atoi(Value);
		}
		else if (strcmp(Name, "size") == 0)
		{
			FireWorkProperties->Size = atoi(Value);
		}
		else if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
		{
			ReportError("Unrecognized firework property '%s'='%s'.", Name, Value);
		}

		PropertyNode = PropertyNode->next_sibling("property");
	}

	Block->Properties = FireWorkProperties;
}

void CreateFireWork(int X, int Y, SFireWorkProperties* Properties)
{
	if (NFireWorks >= MAX_FIREWORKS)
		ReportError("Exceeded the maximum of %d total fireworks.", MAX_FIREWORKS);

	SFireWork* FireWork = &FireWorks[NFireWorks++];

	FireWork->X = (float)X + 32;
	FireWork->Y = (float)Y + 32;

	FireWork->OriginalX = FireWork->X;
	FireWork->OriginalY = FireWork->Y;

	FireWork->VelocityX = 0;
	FireWork->VelocityY = 0;

	FireWork->FlightDir = Properties->Dir;
	FireWork->FlightDistance = Properties->Dist;
	FireWork->Timer = Properties->Fuse;
	FireWork->ExplosionSize = Properties->Size;

	FireWork->State = FIREWORKSTATE_WAIT;
}

void ClearFireWorks()
{
	NFireWorks = 0;

	for (int i = 0; i < MAX_FIREWORK_TRAILS; i++)
		FireWorkTrails[i].NextFireWorkTrail = i+1;
	FirstInactiveFireWorkTrail = 0;

	FirstActiveFireWorkTrail = MAX_FIREWORK_TRAILS;
}

void DisplayFireWorkTrails()
{
	for (int i = FirstActiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
	{
		SFireWorkTrail* Trail = &FireWorkTrails[i];

		// Fake 3D projection, centered around the explosion.
		float ZNear = 3000.0f;
		float Z = Max(1, Min(ZNear, Trail->Z));
		float X = (Trail->X - Trail->CenterX) * ZNear / (ZNear - Z);
		float Y = (Trail->Y - Trail->CenterY) * ZNear / (ZNear - Z);

		float Scale = Lerp(Z, 0.0f, ZNear, 1.0f, 2.0f) * 2.0f;
		float Alpha = Lerp(Trail->Life, 1.0f, 0.0f, 1.0f, 0.0f);

		AddLitSpriteCenteredScaledColor(LIGHTLIST_EFFECTS, &DustMoteSprite, Trail->CenterX + X, Trail->CenterY + Y + ScrollY, Scale, gxRGBA32(255,192,192,(int)(255*Alpha)));

		int Count = Trail->HistoryCount;
		for (int i = 0; i < Count - 1; i++)
		{
			int HistoryIndex0 = (Trail->HistoryCount - (i + 0));
			int HistoryIndex1 = (Trail->HistoryCount - (i + 1));

			float Z0 = Max(1, Min(ZNear, Trail->ZHistory[HistoryIndex0]));
			float X0 = (Trail->XHistory[HistoryIndex0] - Trail->CenterX) * ZNear / (ZNear - Z);
			float Y0 = (Trail->YHistory[HistoryIndex0] - Trail->CenterY) * ZNear / (ZNear - Z);

			float Z1 = Max(1, Min(ZNear, Trail->ZHistory[HistoryIndex1]));
			float X1 = (Trail->XHistory[HistoryIndex1] - Trail->CenterX) * ZNear / (ZNear - Z);
			float Y1 = (Trail->YHistory[HistoryIndex1] - Trail->CenterY) * ZNear / (ZNear - Z);

			float Width = 4.0f;

			float TrailScale0 = (1.0f - (float)(i+0)/(float)Count);
			float TrailScale1 = (1.0f - (float)(i+1)/(float)Count);

			float PerpX = Y0 - Y1;;
			float PerpY = -(X0 - X1);
			float Length = sqrtf(PerpX*PerpX + PerpY*PerpY);
			if (Length > 0)
			{
				PerpX *= Width * Scale / Length;
				PerpY *= Width * Scale / Length;
			}

			float Alpha = Lerp(Trail->Life, 1.0f, 0.0f, 1.0f, 0.0f) * TrailScale0;

			AddLitQuad(LIGHTLIST_EFFECTS, &WipeDiagonalSprite, gxRGBA32(192,128,128,(int)(255*Alpha)),
				Trail->CenterX + X0 - PerpX*TrailScale0, Trail->CenterY + Y0 - PerpY*TrailScale0 + ScrollY, 0.0f, 0.0f,
				Trail->CenterX + X0 + PerpX*TrailScale0, Trail->CenterY + Y0 + PerpY*TrailScale0 + ScrollY, 1.0f, 0.0f,
				Trail->CenterX + X1 + PerpX*TrailScale1, Trail->CenterY + Y1 + PerpY*TrailScale1 + ScrollY, 1.0f, 0.0f,
				Trail->CenterX + X1 - PerpX*TrailScale1, Trail->CenterY + Y1 - PerpY*TrailScale1 + ScrollY, 0.0f, 0.0f);
		}
	}
}

void UpdateFireWorkTrails()
{
	// Advance Trails.
	for (int i = FirstActiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
	{
		SFireWorkTrail* Trail = &FireWorkTrails[i];

		int HistoryIndex0 = Trail->HistoryCount-0;
		int HistoryIndex1 = Trail->HistoryCount-1;
		int HistoryIndex2 = Trail->HistoryCount-2;

		float Angle0 = atan2f(Trail->Y - Trail->YHistory[HistoryIndex1], Trail->X - Trail->XHistory[HistoryIndex1]);
		float Angle1 = atan2f(Trail->YHistory[HistoryIndex1] - Trail->YHistory[HistoryIndex2], Trail->XHistory[HistoryIndex1] - Trail->XHistory[HistoryIndex2]);

		float XDist = Trail->X - Trail->XHistory[HistoryIndex1];
		float YDist = Trail->Y - Trail->YHistory[HistoryIndex1];
		float Dist = sqrtf(XDist*XDist + YDist*YDist);

		if (Trail->HistoryCount < FIREWORK_TRAIL_HISTORY-1 && (fabsf(Angle1-Angle0) > PI/32.0f || Dist > 10.0f))
			Trail->HistoryCount++;

		Trail->XHistory[Trail->HistoryCount] = Trail->X;
		Trail->YHistory[Trail->HistoryCount] = Trail->Y;
		Trail->ZHistory[Trail->HistoryCount] = Trail->Z;

		Trail->X += Trail->VX;
		Trail->Y += Trail->VY;
		Trail->Z += Trail->VZ;
		
		Trail->VX *= 0.95f;
		Trail->VY *= 0.95f;
		Trail->VY += 0.001f;

		Trail->Life -= 1.0f/60.0f;
	}

	// Cull dead trails.
	for (int i = FirstActiveFireWorkTrail; i != MAX_FIREWORK_TRAILS;)
	{
		SFireWorkTrail* Trail = &FireWorkTrails[i];

		i = FireWorkTrails[i].NextFireWorkTrail;

		if (Trail->Life <= 0)
		{
			int TrailIndex = Trail - FireWorkTrails;

			if (FirstActiveFireWorkTrail == TrailIndex)
				FirstActiveFireWorkTrail = Trail->NextFireWorkTrail;
			else
			{
				for (int i = FirstActiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
				{
					if (FireWorkTrails[i].NextFireWorkTrail == TrailIndex)
					{
						FireWorkTrails[i].NextFireWorkTrail = Trail->NextFireWorkTrail;
						break;
					}
				}
			}

			Trail->NextFireWorkTrail = FirstInactiveFireWorkTrail;
			FirstInactiveFireWorkTrail = TrailIndex;

			for (int i = FirstActiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
				;
			for (int i = FirstInactiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
				;
		}
	}
}

void SpawnFireWorkTrail(float X, float Y, float Speed, float Life)
{
	if (FirstInactiveFireWorkTrail == MAX_FIREWORK_TRAILS)
		ReportError("Exceeded the maximum of %d firework trails.", MAX_FIREWORK_TRAILS);

	SFireWorkTrail* Trail = &FireWorkTrails[FirstInactiveFireWorkTrail];

	int NextInactive = Trail->NextFireWorkTrail;
	Trail->NextFireWorkTrail = FirstActiveFireWorkTrail;
	FirstActiveFireWorkTrail = FirstInactiveFireWorkTrail;
	FirstInactiveFireWorkTrail = NextInactive;

	for (int i = FirstActiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
		;
	for (int i = FirstInactiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
		;

	Trail->CenterX = X;
	Trail->CenterY = Y;

	Trail->X = X;
	Trail->Y = Y;
	Trail->Z = 0;

	// Pick a semi-random direction on a hemisphere (no guarantee the distribution is even though).
	Trail->VX = Random(-1.0f, 1.0f);
	Trail->VY = Random(-1.0f, 1.0f);
	Trail->VZ = Random(0.0f, 1.0f);
	
	float Length = sqrtf(Trail->VX*Trail->VX + Trail->VY*Trail->VY + Trail->VZ*Trail->VZ);
	if (Length < 0.00001f)
		Length = 0.00001f;

	Trail->VX *= Speed/Length;
	Trail->VY *= Speed/Length;
	Trail->VZ *= Speed/Length;

	Trail->Life = Life;

	for (int i = 0; i < FIREWORK_TRAIL_HISTORY; i++)
	{
		Trail->XHistory[i] = Trail->X;
		Trail->YHistory[i] = Trail->Y;
		Trail->ZHistory[i] = Trail->Z;
	}
	Trail->HistoryCount = 2;
}

void DisplayFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];
		
		if (FireWork->State == FIREWORKSTATE_WAIT || FireWork->State == FIREWORKSTATE_FUSE || FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_VACUUM, &FireWorkRocketSprite, FireWork->X, FireWork->Y + ScrollY, 1.0f, DegreesToRadians((float)FireWork->FlightDir));
		}
		else if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
		}
	}

	DisplayFireWorkTrails();
}

void ExplodeFireWork(float X, float Y, int Size)
{
	// Spawn Trails
	for (int i = 0; i < 20; i++)
	{
		SpawnFireWorkTrail(X, Y, 10.0f, 2.0f);
	}

	for (int y = 0; y < Chapter.PageHeight; y++)
	{
		for (int x = 0; x < Chapter.PageWidth; x++)
		{
			int BlockID = GetBlockID(x, y);
			if (BlockID < SPECIALBLOCKID_FIRST)
			{
				SBlock* Block = &Chapter.Blocks[BlockID];						
				if (Block->Material == MATERIAL_BREAKABLE || Block->Material == MATERIAL_ICE)
				{
					if (Distance(X, Y, (float)x*64+32, (float)y*64+32) < Size*64)
					{ 
						sxPlaySound( &BlockBreakSound );
						Chapter.PageBlocks[y * Chapter.PageWidth + x] = SPECIALBLOCKID_BLANK;
					}
				}       					           
			}	
		}
	}							    

	for (int i = 0; i < NCoins; i++)//Need extern in header file to accomplish
	{
		SCoin* Coin = &Coins[i];
		float Dist =(Distance(X, Y, Coin->X, Coin->Y));
		if (Dist < Size*64)
		{                    
			Coin->State = COINSTATE_FALLING;     
		}        
	}

	for (int i = 0; i < NGears; i++)
	{
		SGear* Gear = &Gears[i];
		float Dist = (Distance(X, Y, Gear->X, Gear->Y));
		if (Dist < Size*64)
		{
			Gear->State = GEARSTATE_FALLING;
		}                                       
	}

	for (int i = 0; i < NBalls; i++)
	{
		SBall* Ball = &Balls[i];
		float Dist = (Distance(X, Y, Ball->X, Ball->Y));
		if (Dist < Size*64)
		{
			Ball->State = BALLSTATE_FALLING;
		}              
	}

	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];

		float Dist = (Distance(X, Y, FireWork->X, FireWork->Y));
		if (Dist < Size*64 && Dist != 0 && FireWork->State == FIREWORKSTATE_WAIT)
		{
			FireWork->State = FIREWORKSTATE_FUSE;
		}
	}               
}

void UpdateFireWorks()
{
	UpdateFireWorkTrails();

	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];

		if (FireWork->State == FIREWORKSTATE_WAIT)
		{
			float XDist = (float)(Dusty.FloatX - FireWork->X);
			float YDist = (float)((Dusty.FloatY-50) - (FireWork->Y));
			float Dist = sqrtf(XDist*XDist + YDist*YDist);

			if (Dist < 125)
			{
			    if (Tutorial.FireworkDisplayed == false)
			    {
			        SetGameState_Tutorial(TUTORIALSTATE_FIREWORK);
			        return;
			    }
		        		        
				FireWork->State = FIREWORKSTATE_FUSE;
			}
		}
		else if (FireWork->State == FIREWORKSTATE_FUSE)
		{
			FireWork->Timer--;
			if (FireWork->Timer <= 0)
			{
				FireWork->State = FIREWORKSTATE_LAUNCH;
			}
		}
		else if (FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			float Angle = DirectionToAngle((float)FireWork->FlightDir);
			float Velocity = 1.5f;

			FireWork->VelocityX += Velocity*cosf(Angle);
			FireWork->VelocityY += -Velocity*sinf(Angle);
			
			FireWork->X += FireWork->VelocityX;
			FireWork->Y += FireWork->VelocityY;

			if (Distance(FireWork->X, FireWork->Y, FireWork->OriginalX, FireWork->OriginalY) >= FireWork->FlightDistance*64)
			{
				FireWork->Timer = 30;
				FireWork->State = FIREWORKSTATE_EXPLODE;

				ExplodeFireWork(FireWork->X, FireWork->Y, FireWork->ExplosionSize);
			}    	          
        }     
		else if (FireWork->State == FIREWORKSTATE_EXPLODE)
        {   
			FireWork->Timer--;
			if (FireWork->Timer == 0)
			{
				FireWork->State = FIREWORKSTATE_DONE;
			}
		}
	}
}
