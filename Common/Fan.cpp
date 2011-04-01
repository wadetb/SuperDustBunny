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
			ReportError("Unrecognized fan property '%s'='%s'.", Name, Value);
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

		AddLitSpriteCenteredScaledRotated(LIGHTLIST_VACUUM, &FanSprite, Fan->X, Fan->Y + ScrollY, 1.0f, 0.0f);

		// Debug rendering of fan area.
		//gxDrawRectangleFilled((int)(Fan->X - 200), (int)(Fan->Y - 1000 + ScrollY), 400, 1000, gxRGBA32(255, 255, 255, 32));
	}
}

void ApplyFanWind(SFan* Fan, float X, float Y, float Multiplier, float* VX, float* VY)
{
	float XDist = Fan->X - X;
	float YDist = Fan->Y - Y;
	if (fabsf(XDist) <= 200 && YDist > 0 && YDist < 1000)
	{
		float YStrength = Lerp(YDist, 500, 1000, 1.0f, 0.0f);
		YStrength *= Lerp(fabsf(XDist), 0, 200, 1.0f, 0.0f);
		*VY -= YStrength*1.1f * Multiplier;

		float XStrength = Lerp(YDist, 0, 750, -2.0f, 1.0f);
		*VX -= Sign(XDist) * XStrength*0.3f * Multiplier;
	}
}

void UpdateFans()
{
	for (int i = 0; i < NFans; i++)
	{
		SFan* Fan = &Fans[i];

		// TODO: Update a few dust motes per frame for performance?
		for (int i = 0; i < MAX_DUST_MOTES; i++)
		{
			ApplyFanWind(Fan, DustMotes[i].X, DustMotes[i].Y, 1.0f, &DustMotes[i].VX, &DustMotes[i].VY);
		}

		if (Dusty.State == DUSTYSTATE_JUMP || Dusty.State == DUSTYSTATE_LAUNCH)
		{
			ApplyFanWind(Fan, Dusty.FloatX, Dusty.FloatY, 0.5f, &Dusty.FloatVelocityX, &Dusty.FloatVelocityY);
		}
	}
}
