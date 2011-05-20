//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef __FLASHLIGHT_H__
#define __FLASHLIGHT_H__

struct SFlashlightWaypointProperties
{
	float Delay;
};

struct SFlashlight
{
	float X, Y;
	int CurWaypoint;
	float Timer;
};


extern SFlashlight Flashlight;


void ParseFlashlightWaypointProperties(struct SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);
void CreateFlashlightWaypoint(int X, int Y, SFlashlightWaypointProperties* Properties);
void ClearFlashlightWaypoints();

void InitFlashlight();
void UpdateFlashlight();
void DisplayFlashlight();

#endif
