//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Stapler.h"
#include "Chapter.h"
#include "Dusty.h"
#include "Tutorial.h"
#include "GameScore.h"

#define MAX_STAPLERS 100

int NStaplers = 0;
SStapler Staplers[MAX_STAPLERS];

void ParseStaplerProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
    SStaplerProperties* StaplerProperties = (SStaplerProperties*)malloc(sizeof(SStaplerProperties));

    // Scan properties for values.
    rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
    while (PropertyNode)
    {
        const char* Name = PropertyNode->first_attribute("name")->value();
        const char* Value = PropertyNode->first_attribute("value")->value();

        if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
        {
            ReportError("Unrecognized stapler property '%s'='%s'.", Name, Value);
        }

        PropertyNode = PropertyNode->next_sibling("property");
    }

    Block->Properties = StaplerProperties;
}

void CreateStapler(int X, int Y, SStaplerProperties* Properties)
{
    if (NStaplers >= MAX_STAPLERS)
        ReportError("Exceeded the maximum of %d total Staplers.", MAX_STAPLERS);

    SStapler* Stapler = &Staplers[NStaplers++];

    Stapler->X = (float)X + 32;
    Stapler->Y = (float)Y + 32;
    
    Stapler->PowerJump = 0;

    Stapler->State = STAPLERSTATE_WAIT;
}

void ClearStaplers()
{
    NStaplers = 0;
}

void DisplayStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];

        if (Stapler->State == STAPLERSTATE_WAIT)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerUpSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, 0.0f);
        }
        else if (Stapler->State == STAPLERSTATE_PRELAUNCH)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerDownSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, 0.0f);
            
            switch(Stapler->PowerJumpCounter)
            {
            case 10: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump1Sprite, Stapler->X + 50, (Stapler->Y) + ScrollY, 1.0f, 0.0f); break;
            case 20: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump2Sprite, Stapler->X + 50, (Stapler->Y + 10) + ScrollY, 1.0f, 0.0f); break;
            case 30: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump3Sprite, Stapler->X + 50, (Stapler->Y + 20) + ScrollY, 1.0f, 0.0f); break;
            case 40: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump4Sprite, Stapler->X + 50, (Stapler->Y + 30) + ScrollY, 1.0f, 0.0f); break;
            case 50: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump5Sprite, Stapler->X + 50, (Stapler->Y + 40) + ScrollY, 1.0f, 0.0f); break;
            case 60: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump6Sprite, Stapler->X + 50, (Stapler->Y + 50) + ScrollY, 1.0f, 0.0f); break;
            case 70: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump7Sprite, Stapler->X + 50, (Stapler->Y + 60) + ScrollY, 1.0f, 0.0f); break;
            case 80: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump8Sprite, Stapler->X + 50, (Stapler->Y + 70) + ScrollY, 1.0f, 0.0f); break;
            case 90: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump9Sprite, Stapler->X + 50, (Stapler->Y + 80) + ScrollY, 1.0f, 0.0f); break;
            case 100: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump10Sprite, Stapler->X + 50, (Stapler->Y + 90) + ScrollY, 1.0f, 0.0f); break;
            default: continue;
            }
        }
        else if (Stapler->State == STAPLERSTATE_LAUNCH)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerExtendUpSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, 0.0f);
        }            
    }
}
#ifdef PLATFORM_WINDOWS

void UpdateStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];
        
        if (Stapler->State == STAPLERSTATE_WAIT)
        {
                //if (Tutorial.StaplerDisplayed == false)
                //{
                //    SetGameState_Tutorial(TUTORIALSTATE_STAPLER);
                //    return;
                //}        
        }
        
        if (Stapler->State == STAPLERSTATE_PRELAUNCH)
        {   
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY, Stapler->X, Stapler->Y+60);
         
            if (Dist < 100 && Dusty.CollideWithBottomSide)
            {     
                if (kbIsKeyDown(KB_SPACE))
                {
                    if (Stapler->PowerJumpCounter > 100)
                    {
                        Stapler->PowerJumpCounter = 100;
                    }
                    
                    Stapler->PowerJumpCounter += 1;    
                }
                
                if (kbWasKeyDown(KB_SPACE))
                {
                    switch(Stapler->PowerJumpCounter)
                    {
                    case 10: Stapler->PowerJump = 0.8f; break;
                    case 20: Stapler->PowerJump = 0.9f; break;
                    case 30: Stapler->PowerJump = 1.0f; break;
                    case 40: Stapler->PowerJump = 1.1f; break;
                    case 50: Stapler->PowerJump = 1.2f; break;
                    case 60: Stapler->PowerJump = 1.3f; break;
                    case 70: Stapler->PowerJump = 1.4f; break;
                    case 80: Stapler->PowerJump = 1.5f; break;
                    case 90: Stapler->PowerJump = 1.6f; break;
                    case 100: Stapler->PowerJump = 1.7f; break;
                    default: continue;
                    }
                    
                    Stapler->State = STAPLERSTATE_LAUNCH; 
                }
            }
        
            if (Stapler->State == STAPLERSTATE_LAUNCH)
            {
                Dusty.FloatY = Dusty.FloatY * Stapler->PowerJump;
                
                Stapler->PowerJumpCounter = 0;
                Stapler->PowerJump = 0;
                
                Stapler->State = STAPLERSTATE_WAIT;
            }      
        }             
    }  
}
#endif
