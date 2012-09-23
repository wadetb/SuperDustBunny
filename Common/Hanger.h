//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef HANGER_H
#define HANGER_H

#define MAX_HANGERS 50

struct SBlock;
struct SHangerProperties;

struct SHanger
{
    SHangerProperties* Props;
	float X, Y;
    float VX, VY;
    float Angle;
    float VAngle;
    bool Attached;
    bool DustyOnBoard;
    float PrevAngle;
    int DustySide;
    int DustyClearTimer;
};

extern int NHangers;
extern SHanger Hangers[MAX_HANGERS];

void ParseHangerProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

void CreateHanger(int X, int Y, int Flags, SHangerProperties* Props);
void ClearHangers();

void AddHangerTorque(SHanger* Hanger, float X, float Y, float FX, float FY);

void UpdateHangers();
void DisplayHangers();

#endif
