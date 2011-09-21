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
};

struct STutorialProperties
{
    char* Text;
    char* Actions;
    float XOffset, YOffset;
    float TextScale;
    STutorialBubble* Bubble;
};

struct STutorial
{
    float X, Y;
    STutorialProperties* Props;
    bool Active;
    float Alpha;
    float WiggleTimer;
};


int NTutorials;
STutorial Tutorials[MAX_TUTORIALS];

STutorialBubble TutorialBubbles[] =
{
    { 75, 0, 700, 230 },
    { 0, 262, 384, 500 },
    { 420, 236, 750, 512 },
    { 90, 530, 310, 680 },
    { 396, 524, 644, 690 },
    { 0, 692, 768, 1024 },
};

void ParseTutorialProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	STutorialProperties* Props = (STutorialProperties*)malloc(sizeof(STutorialProperties));
    
    Props->Text = NULL;
    Props->Actions = NULL;
    Props->XOffset = Props->YOffset = 0;
    Props->TextScale = 1.0f;
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
        else if (strcmp(Name, "textscale") == 0)
        {
            Props->TextScale = strtof(Value, &Value);
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
        if (BubbleWidth*0.9f >= Width && BubbleHeight*0.9f >= Height)
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
    
	Tutorial->X = (float)X + 32;
	Tutorial->Y = (float)Y + 32;

    Tutorial->Alpha = 0.0f;

    if (Props->Actions && strstr(Props->Actions, "hideuntil"))
        Tutorial->Active = false;
    else
        Tutorial->Active = true;
    
    Tutorial->WiggleTimer = Random(0.0f, 10.0f);
}

void ClearTutorials()
{
	NTutorials = 0;
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
            
            float X = Tutorial->X + Props->XOffset + ScrollX;
            float Y = Tutorial->Y + Props->YOffset - ( Bubble->SubY2 - Bubble->SubY1 ) + ScrollY;
            
            X += cosf(Tutorial->WiggleTimer*4.0f) * 2.5f + cosf(Tutorial->WiggleTimer*1.0f/3.0f) * 2.5f;
            Y += sinf(Tutorial->WiggleTimer*4.0f) * 2.5f + sinf(Tutorial->WiggleTimer*1.0f/3.0f) * 2.5f;

            AddLitSubSpriteAlpha(LIGHTLIST_VACUUM, &TextBubblesSprite, X, Y, Bubble->SubX1, Bubble->SubY1, Bubble->SubX2, Bubble->SubY2, Tutorial->Alpha);
            
            float TextX = X + (Bubble->SubX2-Bubble->SubX1)*0.5f;
            float TextY = Y + (Bubble->SubY2-Bubble->SubY1)*0.5f;
            
            DisplayMultilineStringAlpha(LIGHTLIST_VACUUM, Props->Text, FORMAT_CENTER_X | FORMAT_CENTER_Y, TextX, TextY, Props->TextScale, Tutorial->Alpha);
        }
    }
}

void UpdateTutorial()
{   
    for (int i = 0; i < NTutorials; i++)
    {
        STutorial* Tutorial = &Tutorials[i];
        STutorialProperties* Props = Tutorial->Props;
        
        Tutorial->Alpha = Clamp(Tutorial->Alpha + ( Tutorial->Active ? 0.1f : -0.1f ), 0.0f, 1.0f);
        
        if (!Tutorial->Active)
        {
            if (Props->Actions && strstr(Props->Actions, "hideuntiljump"))
            {
                if (Dusty.State == DUSTYSTATE_JUMP)
                {
                    Tutorial->Active = true;
                }
            }
            else if (Props->Actions && strstr(Props->Actions, "hideuntildusty"))
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
                    Chapter.PageProps.VacuumOff = false;
                    TurnOnVacuum();
                    Vacuum.Timer = 0;            
                }
            }
        }
        else
        {
            if (Props->Actions && strstr(Props->Actions, "showuntilgear"))
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
        
        Tutorial->WiggleTimer += 1.0f/60.0f;
    }
}
