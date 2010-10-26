//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Barrel.h"
#include "Chapter.h"
#include "Dusty.h"
#include "Tutorial.h"

#define MAX_BARRELS 100

int NBarrels = 0;
SBarrel Barrels[MAX_BARRELS];

void ParseBarrelProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SBarrelProperties* BarrelProperties = (SBarrelProperties*)malloc(sizeof(SBarrelProperties));

	// Set default values.
	BarrelProperties->From = 0;
	BarrelProperties->To = 0;

	// Scan properties for values.
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* Value = PropertyNode->first_attribute("value")->value();

		if (strcmp(Name, "from") == 0)
		{
			BarrelProperties->From = atoi(Value);
		}
		else if (strcmp(Name, "to") == 0)
		{
			BarrelProperties->To = atoi(Value);
		}
		else if (strcmp(Name, "type") != 0)
		{
			ReportError("Unrecognized barrel property '%s'='%s'.", Name, Value);
		}

		PropertyNode = PropertyNode->next_sibling("property");
	}

	Block->Properties = BarrelProperties;
}

void CreateBarrel(int X, int Y, SBarrelProperties* Properties)
{
	SBarrel* Barrel = &Barrels[NBarrels++];

	Barrel->X = (float)X + 32;
	Barrel->Y = (float)Y + 32;

	Barrel->FromDir = (float)Properties->From;
	Barrel->ToDir = (float)Properties->To;
	Barrel->Dir = Barrel->FromDir;

	Barrel->State = BARRELSTATE_WAIT;
}

void ClearBarrels()
{
	NBarrels = 0;
}

void DisplayBarrels_BeforeDusty()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];
		
		gxDrawSpriteCenteredRotated((int)Barrel->X, (int)(Barrel->Y + ScrollY), Barrel->Dir * 3.14159f / 180.0f, &BarrelBackSprite);
	}
}

void DisplayBarrels_AfterDusty()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];
		
		gxDrawSpriteCenteredRotated((int)Barrel->X, (int)(Barrel->Y + ScrollY), Barrel->Dir * 3.14159f / 180.0f, &BarrelFrontSprite);
		gxDrawSpriteCenteredRotated((int)Barrel->X, (int)(Barrel->Y + ScrollY), 0.0f, &BarrelNailSprite);
	}
}

float GetDirDifference(float a, float b) 
{
	float mn = a - b;
	while (mn < -180) mn += 360;
	while (mn > 180) mn -= 360;
	return mn;
}

void UpdateBarrels()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];

		if (Barrel->State == BARRELSTATE_WAIT)
		{
			float Dist = Distance(Dusty.FloatX, Dusty.FloatY, Barrel->X, Barrel->Y+60);

			if (Dist < 200)
			{
				float NewDir = AngleBetween(Barrel->X, Barrel->Y, Dusty.FloatX, Dusty.FloatY-60) * 180 / PI;
				float DirDelta = GetDirDifference(NewDir, Barrel->FromDir);
				Barrel->Dir = Barrel->FromDir + Remap(Dist, 200, 100, 0, DirDelta, true);
				
				if (Dist < 100)
				{
					Dusty.FloatX = Dusty.FloatX * 0.8f + Barrel->X * 0.2f;
					Dusty.FloatY = Dusty.FloatY * 0.8f + (Barrel->Y+60) * 0.2f;
					Dusty.FloatVelocityX = 0;
					Dusty.FloatVelocityY = 0;
					//SetDustyState_PrepareLaunch();
				}
			}
			
			if (Dist < 25)
			{
			    if (Tutorial.BarrelDisplayed == false)
			    {
			        SetGameState_Tutorial(TUTORIALSTATE_BARREL);
			        return;
			    }
			    
				Dusty.FloatX = (float)Barrel->X;
				Dusty.FloatY = (float)Barrel->Y + 60;
				SetDustyState_PrepareLaunch();
				
				Barrel->State = BARRELSTATE_TURN;
			}
		}
		else if (Barrel->State == BARRELSTATE_TURN)
		{
			Dusty.FloatX = (float)Barrel->X;
			Dusty.FloatY = (float)Barrel->Y + 60;

			float Diff = GetDirDifference(Barrel->Dir, Barrel->ToDir);
			if (Diff > 5 || Diff < -5)
			{
				if (Diff < 0)
					Barrel->Dir = fmodf(Barrel->Dir+5, 360);
				else
					Barrel->Dir = fmodf(Barrel->Dir+355, 360);
			}
			else
			{
				Barrel->Dir = Barrel->ToDir;
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
			float Diff = GetDirDifference(Barrel->Dir, Barrel->FromDir);
			if (Diff > 5 || Diff < -5)
			{
				if (Diff < 0)
					Barrel->Dir = fmodf(Barrel->Dir+5, 360);
				else
					Barrel->Dir = fmodf(Barrel->Dir+355, 360);
			}
			else
			{
				Barrel->Dir = Barrel->FromDir;
				Barrel->State = BARRELSTATE_WAIT;
			}
		}
	}
}
