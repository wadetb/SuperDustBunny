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

void CreateFlame(int X, int Y, SFlameProperties* Props);
void ClearFlames();

void UpdateFlames();
void DisplayFlames();

#endif
