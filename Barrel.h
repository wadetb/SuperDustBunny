//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef BARREL_H
#define BARREL_H

enum EBarrelState
{
	BARRELSTATE_WAIT,
	BARRELSTATE_TURN,
	BARRELSTATE_LAUNCH,
	BARRELSTATE_RESET,
};

struct SBarrelProperties
{
	int From;
	int To;
};

struct SBarrel
{
	float X, Y;

	EBarrelState State;

	float FromDir;
	float ToDir;
	float Dir;

	int Timer;
};

extern SBarrel Barrel;

void ParseBarrelProperties(struct SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

void CreateBarrel(int X, int Y, SBarrelProperties* Properties);
void ClearBarrels();

void UpdateBarrels();
void DisplayBarrels_BeforeDusty();
void DisplayBarrels_AfterDusty();

#endif
