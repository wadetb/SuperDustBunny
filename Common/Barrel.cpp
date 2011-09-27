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
#include "Dusty.h"
#include "Chapter.h"
#include "GameScore.h"

#define MAX_BARRELS 100

int NBarrels = 0;
SBarrel Barrels[MAX_BARRELS];

void ParseBarrelProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SBarrelProperties* BarrelProperties = (SBarrelProperties*)malloc(sizeof(SBarrelProperties));

	// Set default values.
	BarrelProperties->From = 0;
	BarrelProperties->To = 0;
    BarrelProperties->Power = 30.0f;

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
		else if (strcmp(Name, "power") == 0)
		{
			BarrelProperties->Power = atof(Value);
		}
		else if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
		{
			ReportError("Unrecognized barrel property '%s'='%s'.", Name, Value);
		}

		PropertyNode = PropertyNode->next_sibling("property");
	}

	Block->Properties = BarrelProperties;
}

void CreateBarrel(int X, int Y, SBarrelProperties* Properties)
{
	if (NBarrels >= MAX_BARRELS)
		ReportError("Exceeded the maximum of %d total barrels.", MAX_BARRELS);

	SBarrel* Barrel = &Barrels[NBarrels++];

	Barrel->X = (float)X + 32;
	Barrel->Y = (float)Y + 32;

	Barrel->FromDir = (float)Properties->From;
	Barrel->ToDir = (float)Properties->To;
	Barrel->Dir = Barrel->FromDir;
    Barrel->Power = Properties->Power;

	Barrel->State = BARRELSTATE_WAIT;
}

void ClearBarrels()
{
	NBarrels = 0;
}

void DisplayBarrels_BeforeDusty()
{
    gxSprite* Sprite;
    switch (Chapter.Theme)
    {
        default:
        case THEME_STUDY: Sprite = &BarrelMugBackSprite; break;
        case THEME_PANTRY: Sprite = &BarrelBeansBackSprite; break;
        case THEME_FRIDGE: Sprite = &BarrelBeansBackSprite; break;
        case THEME_WORKSHOP: Sprite = &BarrelMetalBackSprite; break;
    }

	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];
	
        AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, Sprite, Barrel->X + ScrollX, Barrel->Y + ScrollY, 1.0f, DegreesToRadians(Barrel->Dir));
	}
}

void DisplayBarrels_AfterDusty()
{
    gxSprite* Sprite;
    switch (Chapter.Theme)
    {
        default:
        case THEME_STUDY: Sprite = &BarrelMugFrontSprite; break;
        case THEME_PANTRY: Sprite = &BarrelBeansFrontSprite; break;
        case THEME_FRIDGE: Sprite = &BarrelBeansFrontSprite; break;
        case THEME_WORKSHOP: Sprite = &BarrelMetalFrontSprite; break;
    }

	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];
		
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND_NO_SHADOW, Sprite, Barrel->X + ScrollX, Barrel->Y + ScrollY, 1.0f, DegreesToRadians(Barrel->Dir));
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND_NO_SHADOW, &BarrelNailSprite, Barrel->X + ScrollX, Barrel->Y + ScrollY, 1.0f, 0.0f);
	}
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
				float DirDelta = AngleDifference(NewDir, Barrel->FromDir);
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
				Dusty.FloatX = (float)Barrel->X;
				Dusty.FloatY = (float)Barrel->Y + 60;
                Dusty.Hidden = true;
				SetDustyState_PrepareLaunch();
				
				Barrel->State = BARRELSTATE_TURN;
			}
		}
		else if (Barrel->State == BARRELSTATE_TURN)
		{
			Dusty.FloatX = (float)Barrel->X;
			Dusty.FloatY = (float)Barrel->Y + 60;

			float Diff = AngleDifference(Barrel->Dir, Barrel->ToDir);
			if (Diff > 5 || Diff < -5)
			{
				if (Diff < 0)
					Barrel->Dir = fmodf(Barrel->Dir+10, 360);
				else
					Barrel->Dir = fmodf(Barrel->Dir+350, 360);
			}
			else
			{
				Barrel->Dir = Barrel->ToDir;
				float Angle = DirectionToAngle(Barrel->Dir);
				SetDustyState_Launch(Barrel->Power*cosf(Angle), -Barrel->Power*sinf(Angle));
                Dusty.Hidden = false;
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
			float Diff = AngleDifference(Barrel->Dir, Barrel->FromDir);
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
