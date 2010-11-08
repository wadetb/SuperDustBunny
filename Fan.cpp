//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Fan.h"
#include "Chapter.h"
#include "Dusty.h"
#include "Dust.h"

#define MAX_FANS 100

int NFans = 0;
SFan Fans[MAX_FANS];

void ParseFanProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SFanProperties* FanProperties = (SFanProperties*)malloc(sizeof(SFanProperties));

	// Set default values.
	FanProperties->Dir = 0;

	// Scan properties for values.
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* Value = PropertyNode->first_attribute("value")->value();

		if (strcmp(Name, "dir") == 0)
		{
			FanProperties->Dir = atoi(Value);
		}
		else if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
		{
			ReportError("Unrecognized barrel property '%s'='%s'.", Name, Value);
		}

		PropertyNode = PropertyNode->next_sibling("property");
	}

	Block->Properties = FanProperties;
}

void CreateFan(int X, int Y, SFanProperties* Properties)
{
	if (NFans >= MAX_FANS)
		ReportError("Exceeded the maximum of %d total fans.", MAX_FANS);

	SFan* Fan = &Fans[NFans++];

	Fan->X = (float)X + 32;
	Fan->Y = (float)Y + 32;

	Fan->Dir = (float)Properties->Dir;
}

void ClearFans()
{
	NFans = 0;
}

void DisplayFans()
{
	for (int i = 0; i < NFans; i++)
	{
		SFan* Fan = &Fans[i];

		gxDrawSpriteCenteredRotated((int)Fan->X, (int)(Fan->Y + ScrollY), Fan->Dir * 3.14159f / 180.0f, &FanSprite);
	}
}

void UpdateFans()
{
	for (int i = 0; i < NFans; i++)
	{
		SFan* Fan = &Fans[i];

		// TODO: Update a few dust motes per frame?
		for (int i = 0; i < MAX_DUST_MOTES; i++)
		{
			if (DustMotes[i].X >= Fan->X - 100 && DustMotes[i].X <= Fan->X + 100)
				DustMotes[i].VY -= 0.6f;
		}

		if (Dusty.State == DUSTYSTATE_JUMP || Dusty.State == DUSTYSTATE_LAUNCH)
		{
			// ...
		}
	}
}
