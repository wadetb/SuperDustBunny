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

    // Set default values.
    StaplerProperties->From = 0;
    StaplerProperties->To = 0;

    // Scan properties for values.
    rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
    while (PropertyNode)
    {
        const char* Name = PropertyNode->first_attribute("name")->value();
        const char* Value = PropertyNode->first_attribute("value")->value();

        if (strcmp(Name, "from") == 0)
        {
            StaplerProperties->From = atoi(Value);
        }
        else if (strcmp(Name, "to") == 0)
        {
            StaplerProperties->To = atoi(Value);
        }
        else if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
        {
            ReportError("Unrecognized Stapler property '%s'='%s'.", Name, Value);
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

        AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerUpSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, 0.0f);
    }
}

void UpdateStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];

        if (Stapler->State == STAPLERSTATE_WAIT)
        {
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY, Stapler->X, Stapler->Y+60);

            if (Dusty.CollideWithBottomSide && Dist < 25)
            {
                //if (Tutorial.StaplerDisplayed == false)
                //{
                //    SetGameState_Tutorial(TUTORIALSTATE_STAPLER);
                //    return;
                //}
                
                Stapler->State = STAPLERSTATE_PRELAUNCH;
                Dusty.State = SetDustyState_PrepareLaunch();
                
            }
        }
        else if (Stapler->State == STAPLERSTATE_PRELAUNCH)
        {       
            while (kbIsKeyDown(KB_SPACE))
            {
                if (Stapler->PowerJumpCounter > 100)
                {
                    Stapler->PowerJumpCounter = 100;
                }
                
                Stapler->PowerJumpCounter += 1;
                
                switch(Stapler->PowerJumpCounter)
                {
                    case 10: Stapler->PowerJump = 1; break;
                    case 20: Stapler->PowerJump = 2; break;
                    case 30: Stapler->PowerJump = 3; break;
                    case 40: Stapler->PowerJump = 4; break;
                    case 50: Stapler->PowerJump = 5; break;
                    case 60: Stapler->PowerJump = 6; break;
                    case 70: Stapler->PowerJump = 7; break;
                    case 80: Stapler->PowerJump = 8; break;
                    case 90: Stapler->PowerJump = 9; break;
                    case 100: Stapler->PowerJump = 10; break;
                    default: break;
                }      
            }
           
            if (kbWasKeyDown(KB_SPACE))
            {
                Stapler->State == STAPLERSTATE_LAUNCH;
                
                switch(Stapler->PowerJump)
                {
                    case 1:  
                    case 2: 
                    case 3: 
                    case 4: 
                    case 5: 
                    case 6: 
                    case 7: 
                    case 8: 
                    case 9: 
                    case 10:              
                    default: break;
                }

                Stapler->PowerJumpCounter = 0;
                Stapler->PowerJump = 0;
            }          
        }
        else if (Stapler->State == STAPLERSTATE_LAUNCH)
        {
            Stapler->Timer--;
            if (Stapler->Timer == 0)
            {
                Stapler->State = STAPLERSTATE_RESET;
            }
        }
        else if (Stapler->State == STAPLERSTATE_RESET)
        {
            float Diff = GetDirDifference(Stapler->Dir, Stapler->FromDir);
            if (Diff > 5 || Diff < -5)
            {
                if (Diff < 0)
                    Stapler->Dir = fmodf(Stapler->Dir+5, 360);
                else
                    Stapler->Dir = fmodf(Stapler->Dir+355, 360);
            }
            else
            {
                Stapler->Dir = Stapler->FromDir;
                Stapler->State = STAPLERSTATE_WAIT;
            }
        }
    }
}
