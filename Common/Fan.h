//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef FAN_H
#define FAN_H

struct SFanProperties
{
	int Dir;
};

struct SFan
{
	float X, Y;

	float Dir;
};

void ParseFanProperties(struct SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

SFan* CreateFan(int X, int Y, SFanProperties* Properties);
void ClearFans();

void UpdateFans();
void DisplayFans();

#endif
