//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Tutorial.h"
#include "Chapter.h"
#include "Dusty.h"
#include "Gear.h"
#include "Coin.h"
#include "Vacuum.h"
#include "Text.h"


#define MAX_TUTORIALS 10


struct STutorialBubble
{
    int SubX1, SubY1, SubX2, SubY2;
    float TextXOffset, TextYOffset;
};

struct STutorialProperties
{
    char* Text;
    char* Actions;
    float XOffset, YOffset;
    float OriginXOffset, OriginYOffset;
    float TextScale;
    bool ScreenAligned;
    bool Dots;
    STutorialBubble* Bubble;
};

struct STutorial
{
    float X, Y;
    STutorialProperties* Props;
    bool Active;
    float Alpha;
    float WiggleTimer;
    bool PauseActive;
    float PauseTimer;
    bool Disabled;
};


int NTutorials;
STutorial Tutorials[MAX_TUTORIALS];

STutorialBubble TutorialBubbles[] =
{
    { 0, 262, 384, 500, 0, -10 },
    { 420, 236, 750, 512 },
    { 90, 530, 310, 680 },
    { 396, 524, 644, 690 },
    { 75, 0, 700, 230, 0, 10 },
    { 0, 692, 768, 1024 },
};

STutorialOverrides TutorialOverrides;


void ParseTutorialProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	STutorialProperties* Props = (STutorialProperties*)malloc(sizeof(STutorialProperties));
    
    Props->Text = NULL;
    Props->Actions = NULL;
    Props->XOffset = 100;
    Props->YOffset = -150;
    Props->OriginXOffset = 0;
    Props->OriginYOffset = 0;
    Props->TextScale = 0.9f;
    Props->ScreenAligned = false;
    Props->Dots = true;
    Props->Bubble = NULL;
    
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* ConstValue = PropertyNode->first_attribute("value")->value();
        char* WritableValue = strdup(ConstValue);
        char* Value = WritableValue;
        
		if (strcmp(Name, "text") == 0)
		{
            Props->Text = strdup(Value);
        }
		else if (strcmp(Name, "actions") == 0)
		{
            Props->Actions = strdup(Value);
        }
        else if (strcmp(Name, "offset") == 0)
        {
            Props->XOffset = strtof(Value, &Value);
            Props->YOffset = strtof(Value, &Value);
        }
        else if (strcmp(Name, "originoffset") == 0)
        {
            Props->OriginXOffset = strtof(Value, &Value);
            Props->OriginYOffset = strtof(Value, &Value);
        }
        else if (strcmp(Name, "textscale") == 0)
        {
            Props->TextScale = strtof(Value, &Value);
        }
        else if (strcmp(Name, "screenaligned") == 0)
        {
            Props->ScreenAligned = (bool)atoi(Value);
        }
        else if (strcmp(Name, "dots") == 0)
        {
            Props->Dots = (bool)atoi(Value);
        }
        
        free(WritableValue);
        
		PropertyNode = PropertyNode->next_sibling("property");
	}
    
    if (Props->Text == NULL)
        ReportError("Tutorial is missing the 'text' property.");

    
    float Width, Height;
    GetMultilineStringDimensions(Props->Text, Props->TextScale, &Width, &Height);

    for (int i = 0; i < sizeof(TutorialBubbles)/sizeof(TutorialBubbles[0]); i++)
    {
        STutorialBubble* Bubble = &TutorialBubbles[i];
        float BubbleWidth = Bubble->SubX2 - Bubble->SubX1;
        float BubbleHeight = Bubble->SubY2 - Bubble->SubY1;
        if (BubbleWidth*0.7f >= Width && BubbleHeight*0.7f >= Height)
        {
            Props->Bubble = Bubble;
            break;
        }
    }
    if (Props->Bubble == NULL)
    {
        ReportError("Unable to fit a suitable bubble to the text '%s' with scale %f.  Required dimensions are %d x %d.", 
                    Props->Text, Props->TextScale, Width, Height);
    }
    
	Block->Properties = Props;
}

void CreateTutorial(int X, int Y, STutorialProperties* Props)
{
	if (NTutorials >= MAX_TUTORIALS)
		ReportError("Exceeded the maximum of %d total tutorials.", MAX_TUTORIALS);
    
	STutorial* Tutorial = &Tutorials[NTutorials++];
    
    Tutorial->Props = Props;
    
	Tutorial->X = (float)X + 32 + Props->OriginXOffset;
	Tutorial->Y = (float)Y + 86 + Props->OriginYOffset;

    Tutorial->Alpha = 0.0f;

    if (Props->Actions && strstr(Props->Actions, "hideuntil"))
        Tutorial->Active = false;
    else
        Tutorial->Active = true;
    
    Tutorial->PauseActive = false;
    Tutorial->PauseTimer = 0;
    Tutorial->Disabled = false;
    
    Tutorial->WiggleTimer = Random(0.0f, 10.0f);
}

void ClearTutorials()
{
	NTutorials = 0;
    
    TutorialOverrides.FocusOnVacuum = false;
    TutorialOverrides.FreezeDusty = false;
}

void InitTutorial()
{
}

void DisplayTutorial()
{
    for (int i = 0; i < NTutorials; i++)
    {
        STutorial* Tutorial = &Tutorials[i];
        
        if (Tutorial->Alpha > 0)
        {
            STutorialProperties* Props = Tutorial->Props;
            STutorialBubble* Bubble = Props->Bubble;
            
            float X, Y;
            
            if (Props->ScreenAligned)
            {
                X = 384 - (Bubble->SubX2 - Bubble->SubX1)/2;
                Y = 300;
                
                AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, X, Y, Bubble->SubX1, Bubble->SubY1, Bubble->SubX2, Bubble->SubY2, Tutorial->Alpha);
            }
            else
            {
                X = Tutorial->X + Props->XOffset + ScrollX;
                Y = Tutorial->Y + Props->YOffset + ScrollY;
                
                X += cosf(Tutorial->WiggleTimer*4.0f) * 2.5f + cosf(Tutorial->WiggleTimer*1.0f/3.0f) * 2.5f;
                Y += sinf(Tutorial->WiggleTimer*4.0f) * 2.5f + sinf(Tutorial->WiggleTimer*1.0f/3.0f) * 2.5f;
                
                if (Props->Dots)
                {
                    float Dot1X = Remap(0.75f, 0, 1.0f, Tutorial->X + ScrollX, X, true);
                    float Dot1Y = Remap(0.75f, 0, 1.0f, Tutorial->Y + ScrollY, Y, true);

                    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, Dot1X-22, Dot1Y-22, 0, 40, 40, 74, Tutorial->Alpha);
                    
                    float Dot2X = Remap(1.10f, 0, 1.0f, Tutorial->X + ScrollX, X, false);
                    float Dot2Y = Remap(1.10f, 0, 1.0f, Tutorial->Y + ScrollY, Y, false);

                    AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, Dot2X-22, Dot2Y-22, 0, 0, 45, 42, Tutorial->Alpha);
                }
                
                Y -= Bubble->SubY2 - Bubble->SubY1;
                
                AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, X, Y, Bubble->SubX1, Bubble->SubY1, Bubble->SubX2, Bubble->SubY2, Tutorial->Alpha);
            }
            
            float TextX = X + (Bubble->SubX2-Bubble->SubX1)*0.5f + Bubble->TextXOffset;
            float TextY = Y + (Bubble->SubY2-Bubble->SubY1)*0.5f + Bubble->TextYOffset;
            
            DisplayMultilineStringAlpha(LIGHTLIST_VACUUM, Props->Text, FORMAT_CENTER_X | FORMAT_CENTER_Y, TextX, TextY, Props->TextScale, Tutorial->Alpha);
        }
    }
}

void UpdateTutorial()
{   
    TutorialOverrides.Timer += 1.0f/60.0f;

    for (int i = 0; i < NTutorials; i++)
    {
        STutorial* Tutorial = &Tutorials[i];
        STutorialProperties* Props = Tutorial->Props;
        
        Tutorial->Alpha = Clamp(Tutorial->Alpha + ( Tutorial->Active ? 0.1f : -0.1f ), 0.0f, 1.0f);
        
        Tutorial->WiggleTimer += 1.0f/60.0f;
        
        if (!Tutorial->Disabled)
        {
            if (Tutorial->PauseActive)
            {
                Tutorial->PauseTimer -= 1.0f/60.0f;
                if (Tutorial->PauseTimer < 3.5f && Tutorial->PauseTimer + 1.0f/60.0f >= 3.5f)
                {
                    if (Props->Actions && strstr(Props->Actions, "turnonvacuum"))
                    {
                        TutorialOverrides.FocusOnVacuum = true;
                        TutorialOverrides.SavedScrollY = ScrollY;
                        TutorialOverrides.Timer = 0.0f;
                    }
                    
                }
                if (Tutorial->PauseTimer <= 0)
                {
                    Tutorial->PauseActive = false;
                    
                    if (Props->Actions && strstr(Props->Actions, "turnonvacuum"))
                    {
                        TutorialOverrides.FocusOnVacuum = false;
                        TutorialOverrides.FreezeDusty = false;
                        Tutorial->Active = false;
                        Tutorial->Disabled = true;
                    }
                }
            }
            else if (!Tutorial->Active)
            {
                if (Props->Actions && strstr(Props->Actions, "hideuntildusty"))
                {
                    if (Dusty.FloatY <= Tutorial->Y)
                    {
                        Tutorial->Active = true;
                    }
                }
                
                if (Tutorial->Active)
                {
                    if (Props->Actions && strstr(Props->Actions, "turnonvacuum"))
                    {
                        TutorialOverrides.FreezeDusty = true;
                        
                        Tutorial->PauseActive = true;
                        Tutorial->PauseTimer = 5.0f;

                        Chapter.PageProps.VacuumOff = false;
                        TurnOnVacuum(600, 2.0f);
                    }
                }
            }
            else
            {
                if (Props->Actions && strstr(Props->Actions, "showuntiljump"))
                {
                    if (Dusty.State == DUSTYSTATE_JUMP)
                    {
                        Tutorial->Active = false;
                    }
                }
                else if (Props->Actions && strstr(Props->Actions, "showuntillaunch"))
                {
                    if (Dusty.State == DUSTYSTATE_LAUNCH)
                    {
                        Tutorial->Active = false;
                    }
                }
                else if (Props->Actions && strstr(Props->Actions, "showuntilgear"))
                {
                    for (int i = 0; i < NGears; i++)
                        if (Gears[i].State != GEARSTATE_ACTIVE)
                            Tutorial->Active = false;
                }
                else if (Props->Actions && strstr(Props->Actions, "showuntilcoin"))
                {
                    for (int i = 0; i < NCoins; i++)
                        if (Coins[i].State != COINSTATE_ACTIVE)
                            Tutorial->Active = false;
                }
            }
        }
    }
}
