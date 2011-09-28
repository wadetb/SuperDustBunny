//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef TUTORIAL_H
#define TUTORIAL_H

struct STutorialOverrides
{
    bool FocusOnVacuum;
    bool FreezeDusty;
    
    float SavedScrollY;
    float Timer;
};

extern STutorialOverrides TutorialOverrides;

struct SBlock;
struct STutorialProperties;
void ParseTutorialProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode);

void CreateTutorial(int X, int Y, STutorialProperties* Props);
void ClearTutorials();

void InitTutorial();
void UpdateTutorial();
void DisplayTutorial();

#endif
