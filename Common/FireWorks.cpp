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
#include "Coin.h"
#include "Gear.h"
#include "TennisBall.h"
#include "Debris.h"
#include "GameScore.h"

#define MAX_FIREWORKS 100

int NFireWorks = 0;
SFireWork FireWorks[MAX_FIREWORKS];

#define MAX_FIREWORK_TRAILS			2048
#define FIREWORK_TRAIL_HISTORY		16

enum EFireWorkTrailType
{
	FIREWORKTRAIL_SINGLE_STAGE,
	FIREWORKTRAIL_DOUBLE_STAGE,
};

struct SFireWorkTrailHistory
{
	float X, Y, Z;
	float Life;
};

struct SFireWorkTrail
{
	EFireWorkTrailType Type;

	float X, Y, Z;
	float VX, VY, VZ;	
	float CenterX, CenterY;

	float Life;
	float Brightness;
	float Length;

	unsigned int Color;

	int HistoryCount;
	SFireWorkTrailHistory History[FIREWORK_TRAIL_HISTORY];

	int NextFireWorkTrail;
};

SFireWorkTrail FireWorkTrails[MAX_FIREWORK_TRAILS];

int FirstInactiveFireWorkTrail = MAX_FIREWORK_TRAILS;
int FirstActiveFireWorkTrail = MAX_FIREWORK_TRAILS;


unsigned int FireWorkColors[] =
{
	//gxRGBA32(0xff,0xe7,0x00,0),
	gxRGBA32(0xe4,0x13,0x47,0),
	gxRGBA32(0x19,0x5b,0xe3,0),
	gxRGBA32(0xe9,0x18,0x4e,0),
	gxRGBA32(0x71,0xe6,0x17,0),
};


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
		float ZNear = 2000.0f;

		float Scale = Lerp(Trail->Z, 0.0f, ZNear, 1.0f, 2.0f);
		float Alpha = Lerp(Trail->Life, 1.0f, 0.0f, 1.0f, 0.0f);

		float R = (float)((Trail->Color>>0)&0xFF);
		float G = (float)((Trail->Color>>8)&0xFF);
		float B = (float)((Trail->Color>>16)&0xFF);
		
		R = Min(R*Trail->Brightness, 255.0f);
		G = Min(G*Trail->Brightness, 255.0f);
		B = Min(B*Trail->Brightness, 255.0f);

		unsigned int Color = gxRGBA32((int)R, (int)G, (int)B, 0);

		if (Trail->HistoryCount > 0)
		{
			float AccumDistance = 0;

			SLitVertex* Verts = AddLitQuad(LIGHTLIST_EFFECTS, &FireWorkWhiteSprite, Trail->HistoryCount*2);

			float Z0 = Max(1, Min(ZNear, Trail->Z));
			float X0 = (Trail->X - Trail->CenterX) * ZNear / (ZNear - Z0);
			float Y0 = (Trail->Y - Trail->CenterY) * ZNear / (ZNear - Z0);

			for (int i = 0; i < Trail->HistoryCount; i++)
			{
				SFireWorkTrailHistory* History = &Trail->History[i];

				float Z1 = Max(1, Min(ZNear, History->Z));
				float X1 = (History->X - Trail->CenterX) * ZNear / (ZNear - Z1);
				float Y1 = (History->Y - Trail->CenterY) * ZNear / (ZNear - Z1);

				float Width = 16.0f;

				float PerpX = Y0 - Y1;
				float PerpY = -(X0 - X1);
				float Length = sqrtf(PerpX*PerpX + PerpY*PerpY);
				if (Length > 0)
				{
					PerpX *= Width * Scale / Length;
					PerpY *= Width * Scale / Length;
				}

				float U = AccumDistance / Trail->Length;
				AccumDistance += Distance(X0, Y0, X1, Y1);

				unsigned int AlphaColor = gxRGBA32(0,0,0,(int)(255*Alpha));

				Verts->X = Trail->CenterX + X0 - PerpX + ScrollX;
				Verts->Y = Trail->CenterY + Y0 - PerpY + ScrollY;
				Verts->U = U;
				Verts->V = 0.0f;
				Verts->Color = Color | AlphaColor;
				Verts++;

				Verts->X = Trail->CenterX + X0 + PerpX + ScrollX;
				Verts->Y = Trail->CenterY + Y0 + PerpY + ScrollY;
				Verts->U = U;
				Verts->V = 1.0f;
				Verts->Color = Color | AlphaColor;
				Verts++;

				X0 = X1;
				Y0 = Y1;
			}
		}
	}
}

void UpdateFireWorkTrails()
{
	// Advance Trails.
	for (int i = FirstActiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
	{
		SFireWorkTrail* Trail = &FireWorkTrails[i];

		float XDist = Trail->X - Trail->History[1].X;
		float YDist = Trail->Y - Trail->History[1].Y;
		float Dist = sqrtf(XDist*XDist + YDist*YDist);

		if (Dist >= Trail->Length / (float)(FIREWORK_TRAIL_HISTORY/3))
		{
			if (Trail->HistoryCount < FIREWORK_TRAIL_HISTORY)
				Trail->HistoryCount++;

			for (int j = Trail->HistoryCount-1; j > 0; j--)
				Trail->History[j] = Trail->History[j-1];

			SFireWorkTrailHistory* History = &Trail->History[0]; 
			History->X = Trail->X;
			History->Y = Trail->Y;
			History->Z = Trail->Z;
			History->Life = 1.0f;
		}

		Trail->X += Trail->VX;
		Trail->Y += Trail->VY;
		Trail->Z += Trail->VZ;
		
		Trail->VX *= 0.975f;
		Trail->VY *= 0.97f;
		Trail->VZ *= 0.97f;
		Trail->VY += 0.015f;

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
		}
	}
}

void SpawnFireWorkTrail(float X, float Y, float Length, float Speed, float Life, float Brightness, unsigned int Color)
{
	if (FirstInactiveFireWorkTrail == MAX_FIREWORK_TRAILS)
		ReportError("Exceeded the maximum of %d firework trails.", MAX_FIREWORK_TRAILS);

	SFireWorkTrail* Trail = &FireWorkTrails[FirstInactiveFireWorkTrail];

	int NextInactive = Trail->NextFireWorkTrail;
	Trail->NextFireWorkTrail = FirstActiveFireWorkTrail;
	FirstActiveFireWorkTrail = FirstInactiveFireWorkTrail;
	FirstInactiveFireWorkTrail = NextInactive;

//	for (int i = FirstInactiveFireWorkTrail; i != MAX_FIREWORK_TRAILS; i = FireWorkTrails[i].NextFireWorkTrail)
//		;

	Trail->CenterX = X;
	Trail->CenterY = Y;

	Trail->X = X;
	Trail->Y = Y;
	Trail->Z = 0;

	// Pick a semi-random direction on a hemisphere (no guarantee the distribution is even though).
	Trail->VX = Random(-1.0f, 1.0f);
	Trail->VY = Random(-1.0f, 1.0f);
	Trail->VZ = Random(0.5f, 0.7f);
	
	float V = sqrtf(Trail->VX*Trail->VX + Trail->VY*Trail->VY + Trail->VZ*Trail->VZ);
	if (V < 0.00001f)
		V = 0.00001f;

	Trail->VX *= Speed/V;
	Trail->VY *= Speed/V;
	Trail->VZ *= Speed/V;

	Trail->Length = Length;
	Trail->Life = Life;
	Trail->Brightness = Brightness;

	Trail->Color = Color;

	for (int i = 0; i < FIREWORK_TRAIL_HISTORY; i++)
	{
		Trail->History[i].X = Trail->X - Trail->VX*i*0.001f;
		Trail->History[i].Y = Trail->Y - Trail->VY*i*0.001f;
		Trail->History[i].Z = Trail->Z - Trail->VZ*i*0.001f;
		Trail->History[i].Life = 1.0f;
	}
	Trail->HistoryCount = FIREWORK_TRAIL_HISTORY;
}

void DisplayFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];
		
		if (FireWork->State == FIREWORKSTATE_WAIT || FireWork->State == FIREWORKSTATE_FUSE || FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_VACUUM, &FireWorkRocketSprite, FireWork->X + ScrollX, FireWork->Y + ScrollY, 1.0f, DegreesToRadians((float)FireWork->FlightDir));
		}
		if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			if (Chapter.PageProps.LightsOff)
			{
				float Alpha = 0.5f;
				Alpha *= Lerp((float)FireWork->Timer, 90.0f, 0.0f, 1.0f, 0.0f);
				Alpha *= Lerp((float)FireWork->Timer, 180.0f, 160.0f, 0.0f, 1.0f);
				AddLitSpriteCenteredScaledAlpha(LIGHTLIST_LIGHTING, &LightFlashlightSprite, FireWork->X + ScrollX, FireWork->Y + ScrollY, 1.3f, Alpha);
			}
		}
	}

	DisplayFireWorkTrails();
}

void ExplodeFireWork(float X, float Y, int Size)
{
	// Spawn Trails
	unsigned int Color = FireWorkColors[Random(0, sizeof(FireWorkColors)/sizeof(FireWorkColors[0]))];
	for (int i = 0; i < 50; i++)
	{
		SpawnFireWorkTrail(X, Y, Random(300.0f, 320.0f), Random(8.75f, 10.25f), Random(2.0f, 2.2f), Random(2.0f, 3.0f), Color);
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
                        EraseBlock(x, y);

						CreateDebris((float)x*64+32, (float)y*64+32, 0.0f, BlockID);
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
			Coin->State = COINSTATE_COLLECTED;     
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
				FireWork->Timer = 180;
				FireWork->State = FIREWORKSTATE_EXPLODE;

				ExplodeFireWork(FireWork->X, FireWork->Y, FireWork->ExplosionSize);
			}    	          
        }     
		else if (FireWork->State == FIREWORKSTATE_EXPLODE)
        {   
			float Angle = DirectionToAngle((float)FireWork->FlightDir);
			float Velocity = 1.5f;

			FireWork->VelocityX += Velocity*cosf(Angle);
			FireWork->VelocityY += -Velocity*sinf(Angle);

			FireWork->X += FireWork->VelocityX;
			FireWork->Y += FireWork->VelocityY;

			FireWork->Timer--;
			if (FireWork->Timer == 0)
			{
				FireWork->State = FIREWORKSTATE_DONE;
			}
		}
	}
}
