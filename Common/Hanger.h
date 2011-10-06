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

struct SBlock;
struct SHangerProperties;

void ParseHangerProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

void CreateHanger(int X, int Y, SHangerProperties* Props);
void ClearHangers();

void UpdateHangers();
void DisplayHangers();

#endif
