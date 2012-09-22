//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Flashlight.h"
#include "Dusty.h"
#include "Chapter.h"


#define MAX_FLASHLIGHT_WAYPOINTS 50


struct SFlashlightWaypoint
{
	float X, Y;
	float Delay;
//    float Toggle;
};

int NFlashlightWaypoints;
SFlashlightWaypoint FlashlightWaypoints[MAX_FLASHLIGHT_WAYPOINTS];


SFlashlight Flashlight;




void ParseFlashlightWaypointProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SFlashlightWaypointProperties* FlashlightWaypointProperties = (SFlashlightWaypointProperties*)malloc(sizeof(SFlashlightWaypointProperties));

	// Set default values.
	FlashlightWaypointProperties->Delay = 0;

	// Scan properties for values.
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* Value = PropertyNode->first_attribute("value")->value();

		if (strcmp(Name, "delay") == 0)
		{
			FlashlightWaypointProperties->Delay = (float)atof(Value);
		}
        else if (strcmp(Name, "toggle") == 0)
        {
            FlashlightWaypointProperties->Toggle = (float)atof(Value);
        }
		else if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
		{
			ReportError("Unrecognized FlashlightWaypoint property '%s'='%s'.", Name, Value);
		}

		PropertyNode = PropertyNode->next_sibling("property");
	}

	Block->Properties = FlashlightWaypointProperties;
}

void CreateFlashlightWaypoint(int X, int Y, SFlashlightWaypointProperties* Properties)
{
	if (NFlashlightWaypoints >= MAX_FLASHLIGHT_WAYPOINTS)
		ReportError("Exceeded the maximum of %d total flashlight waypoints.", MAX_FLASHLIGHT_WAYPOINTS);

	SFlashlightWaypoint* Waypoint = &FlashlightWaypoints[NFlashlightWaypoints++];

	Waypoint->X = (float)X + 32;
	Waypoint->Y = (float)Y + 32;

	Waypoint->Delay = Properties->Delay;    
//    Waypoint->Toggle = Properties->Toggle;
}

void ClearFlashlightWaypoints()
{
	NFlashlightWaypoints = 0;
}

void InitFlashlight()
{
    Flashlight.Active = NFlashlightWaypoints > 0;
        
	Flashlight.CurWaypoint = NFlashlightWaypoints-1;
	if (Flashlight.CurWaypoint >= 0)
	{
		SFlashlightWaypoint* Waypoint = &FlashlightWaypoints[Flashlight.CurWaypoint];
		Flashlight.X = Waypoint->X;
		Flashlight.Y = Waypoint->Y;
		Flashlight.Timer = Waypoint->Delay;
//        Flashlight.Lights = Waypoint->Toggle;
	}
}

void UpdateFlashlight()
{
	if (Flashlight.CurWaypoint >= 0)
	{
		SFlashlightWaypoint* Waypoint = &FlashlightWaypoints[Flashlight.CurWaypoint];

		// If the flashlight reaches the waypoint, or if dusty passes the waypoint, advance to the next one.
		bool PassedIt = Dusty.FloatY <= Waypoint->Y;
        
		if (PassedIt || Distance(Waypoint->X, Waypoint->Y, Flashlight.X, Flashlight.Y) < 32)
		{
//            if(Flashlight.Lights == 1)
//            {
//                //Turn Lights ON
//                Chapter.PageProps.LightsOff = false;
//                LightState.LightsOff = Chapter.PageProps.LightsOff;
//                ResetLightState();
//            }
//            
//            if (Flashlight.Lights == 0)
//            {
//                //Turn Lights OFF
//                Chapter.PageProps.LightsOff = true;
//                LightState.LightsOff = Chapter.PageProps.LightsOff;
//                LightState.AmbientColor = gxRGBA32(16, 16, 16, 255);
//            }
            
			Flashlight.CurWaypoint--;

			if (Flashlight.CurWaypoint >= 0)
			{
				Waypoint = &FlashlightWaypoints[Flashlight.CurWaypoint];
				
				if (PassedIt)
                {
					Flashlight.Timer = 0;
                }
				else
                {
					Flashlight.Timer = Waypoint->Delay;
//                    Flashlight.Lights = Waypoint->Toggle;
                }
			}
		}

		if (Flashlight.Timer > 0)
		{
			Flashlight.Timer -= 1.0f/60.0f;
		}
		else
		{
			Flashlight.X = Flashlight.X * 0.97f + Waypoint->X * 0.03f;
			Flashlight.Y = Flashlight.Y * 0.97f + Waypoint->Y * 0.03f;
		}
	}
}

void DisplayFlashlight()
{
	if (Chapter.PageProps.LightsOff)
		AddLitSpriteCenteredScaledAlpha(LIGHTLIST_LIGHTING, &LightFlashlightSprite, Flashlight.X + ScrollX, Flashlight.Y + ScrollY, 1.5f, 0.8f);
}

