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

    Stapler->FromDir = (float)Properties->From;
    Stapler->ToDir = (float)Properties->To;
    Stapler->Dir = Stapler->FromDir;

    Stapler->State = STAPLERSTATE_WAIT;
}

void ClearStaplers()
{
    NStaplers = 0;
}

void DisplayStaplers_BeforeDusty()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];

        AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerBackSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, DegreesToRadians(Stapler->Dir));
    }
}

void DisplayStaplers_AfterDusty()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];

        AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND_NO_SHADOW, &StaplerFrontSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, DegreesToRadians(Stapler->Dir));
        AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND_NO_SHADOW, &StaplerNailSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, 0.0f);
    }
}

float GetDirDifference(float a, float b) 
{
    float mn = a - b;
    while (mn < -180) mn += 360;
    while (mn > 180) mn -= 360;
    return mn;
}

void UpdateStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];

        if (Stapler->State == STAPLERSTATE_WAIT)
        {
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY, Stapler->X, Stapler->Y+60);

            if (Dist < 200)
            {
                float NewDir = AngleBetween(Stapler->X, Stapler->Y, Dusty.FloatX, Dusty.FloatY-60) * 180 / PI;
                float DirDelta = GetDirDifference(NewDir, Stapler->FromDir);
                Stapler->Dir = Stapler->FromDir + Remap(Dist, 200, 100, 0, DirDelta, true);

                if (Dist < 100)
                {
                    Dusty.FloatX = Dusty.FloatX * 0.8f + Stapler->X * 0.2f;
                    Dusty.FloatY = Dusty.FloatY * 0.8f + (Stapler->Y+60) * 0.2f;
                    Dusty.FloatVelocityX = 0;
                    Dusty.FloatVelocityY = 0;
                    //SetDustyState_PrepareLaunch();
                }
            }

            if (Dist < 25)
            {
                if (Tutorial.StaplerDisplayed == false)
                {
                    SetGameState_Tutorial(TUTORIALSTATE_STAPLER);
                    return;
                }

                Dusty.FloatX = (float)Stapler->X;
                Dusty.FloatY = (float)Stapler->Y + 60;
                SetDustyState_PrepareLaunch();

                Stapler->State = STAPLERSTATE_PRELAUNCH;
            }
        }
        else if (Stapler->State == STAPLERSTATE_PRELAUNCH)
        {
            Dusty.FloatX = (float)Stapler->X;
            Dusty.FloatY = (float)Stapler->Y + 60;

            float Diff = GetDirDifference(Stapler->Dir, Stapler->ToDir);
            if (Diff > 5 || Diff < -5)
            {
                if (Diff < 0)
                    Stapler->Dir = fmodf(Stapler->Dir+5, 360);
                else
                    Stapler->Dir = fmodf(Stapler->Dir+355, 360);
            }
            else
            {
                Stapler->Dir = Stapler->ToDir;
                float Angle = DirectionToAngle(Stapler->Dir);
                float Velocity = 20.0f;
                SetDustyState_Launch(Velocity*cosf(Angle), -Velocity*sinf(Angle));
                Stapler->Timer = 30;
                Stapler->State = STAPLERSTATE_LAUNCH;

                Score.Type = 3;
                Score.Raise = true;
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
