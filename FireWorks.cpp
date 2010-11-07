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
}

extern int ScrollY;

void DisplayFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];
		
		if (FireWork->State == FIREWORKSTATE_WAIT || FireWork->State == FIREWORKSTATE_FUSE || FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			gxDrawSpriteCenteredRotated((int)FireWork->X, (int)FireWork->Y + ScrollY, FireWork->FlightDir * 3.14159f / 180.0f, &FireWorkRocketSprite);
		}
		else if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			gxDrawSpriteCenteredScaledAlphaAdd((int)FireWork->X, (int)FireWork->Y + ScrollY, 1.0f, 1.0f, 1.0f, &FireWorkBangSprite);				
		}
	}
}

void ExplodeFireWork(float X, float Y, int Size)
{
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
			float Angle = (float)((90 - FireWork->FlightDir)) * 3.14159f/180.0f;
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
