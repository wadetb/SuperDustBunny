//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef __FLAME_H__
#define __FLAME_H__

struct SBlock;
struct SFlameProperties;

void ParseFlameProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

SFlameProperties* GetFlamePropertiesByName(const char* Name);
void ClearFlamePropertiesList();

struct SFlame
{
    SFlameProperties* Props;
	float X, Y;
    float Angle;
	int Frame;
    bool Active;
};

SFlame* CreateFlame(float X, float Y, unsigned int Flags, SFlameProperties* Props);
void ClearFlames();

void DestroyFlame(int FlameID);

void UpdateFlames();
void DisplayFlames();

#endif
