//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef FIREWORKS_H
#define FIREWORKS_H

enum EFireWorkState
{
	FIREWORKSTATE_WAIT,
	FIREWORKSTATE_FUSE,
	FIREWORKSTATE_LAUNCH,
	FIREWORKSTATE_EXPLODE,
	FIREWORKSTATE_DONE,
};

struct SFireWorkProperties
{
	int Dir;
	int Dist;
	int Fuse;
	int Size;
};

struct SFireWork
{
	EFireWorkState State;

	float X, Y;
	float OriginalX, OriginalY;
	float VelocityX, VelocityY;

	int FlightDistance;
	int FlightDir;
	int ExplosionSize;

	int Timer;
};

void ParseFireWorkProperties(struct SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

extern SFireWork FireWork;
void CreateFireWork(int X, int Y, SFireWorkProperties* Properties);
void ClearFireWorks();

void DisplayFireWorks();
void UpdateFireWorks();

#endif
