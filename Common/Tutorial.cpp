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
#include "Input.h"
#include "Baby.h"
#include "Flame.h"
#include "Balloon.h"
#include "Stapler.h"
#include "Hanger.h"
#include "Barrel.h"
#include "Fan.h"


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
    bool Centered;
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
    int Sequence;
    float SequenceTimer;
    float Timer;
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
    Props->Centered = false;
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
        else if (strcmp(Name, "centered") == 0)
        {
            Props->Centered = (bool)atoi(Value);
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
        ReportError("Unable to fit a suitable bubble to the text '%s' with scale %f.  Required dimensions are %f x %f.", 
                    Props->Text, Props->TextScale, Width, Height);
    }
    
	Block->Properties = Props;
}

SStapler* HotDateStapler;
float HotDateVY;

static void CreateHotDateTutorial(STutorial* Tutorial)
{
    Tutorial->Active = false;
    Tutorial->Sequence = 0;
    Tutorial->SequenceTimer = 0;
    Chapter.PageProps.VacuumOff = true;
    HotDateStapler = NULL;
    HotDateVY = 0.0f;
    TutorialOverrides.FocusOnVacuum = false;
}

static void UpdateHotDateTutorial(STutorial* Tutorial)
{
    if (Tutorial->Sequence == 0)
    {
        if (Tutorial->SequenceTimer == 0)
        {
            Dusty.FloatX -= 50;
            sxPlaySound(&HotDateSound);
        }
        
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        if (Tutorial->SequenceTimer >= 3)
        {
            Chapter.PageProps.VacuumOff = false;
            TurnOnVacuum(600, 0.0f, false);
            
            TutorialOverrides.FocusOnVacuum = true;
            TutorialOverrides.SavedScrollY = ScrollY;
            TutorialOverrides.Timer = 0.0f;
            
            Tutorial->Sequence = 4;
            Tutorial->SequenceTimer = 0;  
        }
    }
    else if (Tutorial->Sequence == 4)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;

        if (Tutorial->SequenceTimer >= 5)
        {
            HotDateStapler = CreateStapler(Tutorial->X, Tutorial->Y - 800, STAPLER_STAPLER);
            HotDateStapler->NoShadow = true;
            HotDateVY = 0;
                        
            SFlameProperties* Props = GetFlamePropertiesByName("arrows");
            CreateFlame(3.5f*64, Tutorial->Y-300, 0, Props);
            CreateFlame(7.5f*64, Tutorial->Y-300, 0, Props);

            TutorialOverrides.FocusOnVacuum = false;
            ScrollY = TutorialOverrides.SavedScrollY;
            
            Tutorial->Sequence = 5;
            Tutorial->SequenceTimer = 0;            
        }
    }
    else if (Tutorial->Sequence == 5)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;

        HotDateStapler->Y += HotDateVY;
        HotDateVY += 2.0f;
        
        if (HotDateStapler->Y >= Tutorial->Y + 165)
        {
            HotDateStapler->Y = Tutorial->Y + 165;
            Chapter.PageBlocks[77*Chapter.PageWidth+4] = SPECIALBLOCKID_BLANK;
            Chapter.PageBlocks[77*Chapter.PageWidth+5] = SPECIALBLOCKID_BLANK;
            Chapter.PageBlocks[77*Chapter.PageWidth+6] = SPECIALBLOCKID_BLANK;
            Chapter.PageBlocks[77*Chapter.PageWidth+7] = SPECIALBLOCKID_BLANK;
        }
        
        if (Dusty.State == DUSTYSTATE_LAUNCH)
        {
            Tutorial->Sequence = 6;
            Tutorial->SequenceTimer = 0.0f;
            
            Chapter.PageProps.LightsOff = false;
        }

        for (int i = 0; i < NHangers; i++)
        {
            float SwingStrength = Remap(fabsf(Hangers[i].Y - Vacuum.Y), 2000, 1500, 0.1f, 0.5f, true);
            AddHangerTorque(&Hangers[i], Hangers[i].X, Hangers[i].Y + 100.0f, sinf(Tutorial->SequenceTimer*2.0f)*SwingStrength, 0.0f);
        }
    }
    else if (Tutorial->Sequence == 6)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;

        int Brightness = (int)Remap(Tutorial->SequenceTimer, 0, 1.5f, 0, 128, true);        
        LightState.AmbientColor = gxRGBA32(Brightness, Brightness, Brightness, 255);        
    }
}

SBarrelProperties BedtimeBarrelProps;
SBarrel* BedtimeBarrel;
float BedtimeVY;

static void CreateBedtimeTutorial(STutorial* Tutorial)
{
    Tutorial->Active = false;
    Tutorial->Sequence = 0;
    Tutorial->SequenceTimer = 0;
    
    BedtimeBarrelProps.From = 180;
    BedtimeBarrelProps.To = 0;
    BedtimeBarrelProps.Power = 25;
    
    BedtimeBarrel = NULL;
    BedtimeVY = 0;
    
    Chapter.PageProps.VacuumOff = true;
    TutorialOverrides.FocusOnVacuum = false;
    TutorialOverrides.NoBabyHop = true;
}

static void UpdateBedtimeTutorial(STutorial* Tutorial)
{
    if (Tutorial->Sequence == 0)
    {        
        if (Tutorial->SequenceTimer == 0)
        {
            Dusty.FloatX += 64;
            if (NHangers > 0)
            {
                Hangers[NHangers-1].X += 0.0f;
                Hangers[NHangers-1].Y += 80.0f;
            }
            sxPlaySound(&BedtimeSound);
        }

        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        if (Tutorial->SequenceTimer >= 3)
        {
            Chapter.PageProps.VacuumOff = false;
            TurnOnVacuum(600, 0.0f, false);
            
            TutorialOverrides.FocusOnVacuum = true;
            TutorialOverrides.SavedScrollY = ScrollY;
            TutorialOverrides.Timer = 0.0f;
            
            Tutorial->Sequence = 4;
            Tutorial->SequenceTimer = 0;  
        }
    }
    else if (Tutorial->Sequence == 4)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        if (Tutorial->SequenceTimer >= 5)
        {
            BedtimeBarrel = CreateBarrel(Tutorial->X, Tutorial->Y - 800, &BedtimeBarrelProps);
            BedtimeVY = 0;
            
            SFlameProperties* Props = GetFlamePropertiesByName("arrows");
            CreateFlame(2.5f*64, Tutorial->Y + 100, 0, Props)->Angle = 45.0f;
            CreateFlame(8.5f*64, Tutorial->Y + 100, 0, Props)->Angle = -45.0f;
            
            TutorialOverrides.FocusOnVacuum = false;
            ScrollY = TutorialOverrides.SavedScrollY;
            
            TutorialOverrides.NoBabyHop = false;

            if (NHangers > 0)
                Hangers[NHangers-1].Attached = false;
            
            Tutorial->Sequence = 5;
            Tutorial->SequenceTimer = 0;            
        }
    }
    else if (Tutorial->Sequence == 5)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        BedtimeBarrel->Y += BedtimeVY;
        BedtimeVY += 2.0f;
        
        if (BedtimeBarrel->Y >= Tutorial->Y)
        {
            BedtimeBarrel->Y = Tutorial->Y;
            Chapter.PageBlocks[78*Chapter.PageWidth+0] = SPECIALBLOCKID_BLANK;
            Chapter.PageBlocks[78*Chapter.PageWidth+Chapter.PageWidth-1] = SPECIALBLOCKID_BLANK;
        }
        
        if (Dusty.State == DUSTYSTATE_LAUNCH)
        {
            Tutorial->Sequence = 6;
            Tutorial->SequenceTimer = 0.0f;
            
            Chapter.PageProps.LightsOff = false;
        }
        
        for (int i = 0; i < NHangers; i++)
        {
            float SwingStrength = Remap(fabsf(Hangers[i].Y - Vacuum.Y), 2000, 1500, 0.1f, 0.5f, true);
            AddHangerTorque(&Hangers[i], Hangers[i].X, Hangers[i].Y + 100.0f, sinf(Tutorial->SequenceTimer*2.0f)*SwingStrength, 0.0f);
        }
    }
    else if (Tutorial->Sequence == 6)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        int Brightness = (int)Remap(Tutorial->SequenceTimer, 0, 1.5f, 0, 128, true);        
        LightState.AmbientColor = gxRGBA32(Brightness, Brightness, Brightness, 255);        
    }
}

SFanProperties GraduationFanProps;
SFan* GraduationFan;
float GraduationVY;

static void CreateGraduationTutorial(STutorial* Tutorial)
{
    Tutorial->Active = false;
    Tutorial->Sequence = 0;
    Tutorial->SequenceTimer = 0;
    
    GraduationFanProps.Dir = 0;
    
    GraduationFan = NULL;
    GraduationVY = 0;
    
    Chapter.PageProps.VacuumOff = true;
    
    TutorialOverrides.FocusOnVacuum = false;
    TutorialOverrides.NoBabyHop = true;
    TutorialOverrides.NoBabyPickup = true;
}

static void UpdateGraduationTutorial(STutorial* Tutorial)
{
    if (Tutorial->Sequence == 0)
    {
        if (Tutorial->SequenceTimer == 0)
        {
            CreateBaby(6, Dusty.FloatY/64-1, 0, DUSTYHAT_GRADUATION, false);
            CreateBaby(7, Dusty.FloatY/64-1, 0, DUSTYHAT_GRADUATION, false);
            CreateBaby(8, Dusty.FloatY/64-1, 0, DUSTYHAT_GRADUATION, false);
            CreateBaby(9, Dusty.FloatY/64-1, 0, DUSTYHAT_GRADUATION, false);
            sxPlaySound(&GraduationSound);
        }
        
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        if (Tutorial->SequenceTimer >= 3)
        {
            Chapter.PageProps.VacuumOff = false;
            TurnOnVacuum(600, 0.0f, false);
            
            TutorialOverrides.FocusOnVacuum = true;
            TutorialOverrides.SavedScrollY = ScrollY;
            TutorialOverrides.Timer = 0.0f;
            
            Tutorial->Sequence = 4;
            Tutorial->SequenceTimer = 0;  
        }
    }
    else if (Tutorial->Sequence == 4)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        if (Tutorial->SequenceTimer >= 5)
        {
            GraduationFan = CreateFan(Tutorial->X, Tutorial->Y - 800, &GraduationFanProps);
            GraduationVY = 0;
            
            SFlameProperties* Props = GetFlamePropertiesByName("arrows");
            CreateFlame(5.5f*64, Tutorial->Y - 200, 0, Props);
            
            TutorialOverrides.FocusOnVacuum = false;
            ScrollY = TutorialOverrides.SavedScrollY;
            
            TutorialOverrides.NoBabyHop = false;
            TutorialOverrides.NoBabyPickup = false;

            Tutorial->Sequence = 5;
            Tutorial->SequenceTimer = 0;            
        }
    }
    else if (Tutorial->Sequence == 5)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        GraduationFan->Y += GraduationVY;
        GraduationVY += 2.0f;
      
        if (GraduationFan->Y >= Tutorial->Y)
        {
            GraduationFan->Y = Tutorial->Y;
            Chapter.PageBlocks[79*Chapter.PageWidth+0] = SPECIALBLOCKID_BLANK;
            Chapter.PageBlocks[79*Chapter.PageWidth+Chapter.PageWidth-1] = SPECIALBLOCKID_BLANK;
        }
        
        if (Dusty.FloatY <= Tutorial->Y - 800)
        {
            Tutorial->Sequence = 6;
            Tutorial->SequenceTimer = 0.0f;
            
            Chapter.PageProps.LightsOff = false;
        }
        
        for (int i = 0; i < NHangers; i++)
        {
            float SwingStrength = Remap(fabsf(Hangers[i].Y - Vacuum.Y), 2000, 1500, 0.1f, 0.5f, true);
            AddHangerTorque(&Hangers[i], Hangers[i].X, Hangers[i].Y + 100.0f, sinf(Tutorial->SequenceTimer*2.0f)*SwingStrength, 0.0f);
        }
    }
    else if (Tutorial->Sequence == 6)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        int Brightness = (int)Remap(Tutorial->SequenceTimer, 0, 1.5f, 0, 128, true);        
        LightState.AmbientColor = gxRGBA32(Brightness, Brightness, Brightness, 255);        
    }
}

static void CreateBirthdayTutorial(STutorial* Tutorial)
{
    SPage* Page = &Chapter.Pages[Chapter.PageNum];
    
    for (int i = 1; i < Page->NLayers; i++)
    {
        SPageLayer* Layer = &Page->Layers[i];
        Layer->Alpha = 0;
    }
    
    Tutorial->Y = (int)(Tutorial->Y/64) * 64;
    
    Tutorial->Active = false;
}

static void UpdateBirthdayTutorial(STutorial* Tutorial)
{
    if (Tutorial->Sequence == 0)
    {
        if (Dusty.FloatY < Tutorial->Y)
        {
            Tutorial->Sequence = 1;
        }

        int Brightness = (int)Remap(Dusty.FloatY - Tutorial->Y, 300, 0, 128, 0, true);        
        LightState.AmbientColor = gxRGBA32(Brightness, Brightness, Brightness, 255);
    }
    else if (Tutorial->Sequence == 1)
    {
        if (Dusty.FloatY + Dusty.Bottom >= Tutorial->Y)
        {
            LightState.ForegroundShadows = false;

            sxPlaySound(&BirthdaySound);

            // Turn on cake.
            SPage* Page = &Chapter.Pages[Chapter.PageNum];
            for (int i = 1; i < Page->NLayers; i++)
            {
                SPageLayer* Layer = &Page->Layers[i];
                Layer->Alpha = 1.0f;
            }

            int y = Tutorial->Y/64;

            // Make flames
            TutorialOverrides.NoFlameDamage = true;
            SFlameProperties* Props = GetFlamePropertiesByName("candle");
            CreateFlame(3.75f*64, (y-7.75)*64, 0, Props);
            CreateFlame(5.5f*64, (y-7.75)*64, 0, Props);
            CreateFlame(7.5f*64, (y-7.65f)*64, 0, Props);
            
            // Fill in solid collision row.
            Dusty.FloatY = Tutorial->Y - Dusty.Bottom;            
            for (int x = 0; x < Page->Width; x++)
                Chapter.PageBlocks[y*Chapter.PageWidth+x] = SPECIALBLOCKID_SOLID;

            // Make babies.
            TutorialOverrides.NoBabyPickup = true;
            //CreateBaby(1, y-1, 0, DUSTYHAT_PINK_BOW, false);
            CreateBaby(4, y-1, 0, DUSTYHAT_PARTY, false);
            //CreateBaby(8, y-1, SPECIALBLOCKID_FLIP_X, DUSTYHAT_PARTY, false);
            CreateBaby(9, y-1, SPECIALBLOCKID_FLIP_X, DUSTYHAT_PINK_BOW, false);
            CreateBaby(11, y-1, SPECIALBLOCKID_FLIP_X, DUSTYHAT_PARTY, false);
            
            Dusty.Hat = DUSTYHAT_PARTY;
            
            Tutorial->Sequence = 3;
            Tutorial->SequenceTimer = 0;
        }
    }
    else if (Tutorial->Sequence == 3)
    {
        int Brightness = (int)Remap(Tutorial->SequenceTimer, 2.5f, 3.5f, 0, 72, true);        
        LightState.AmbientColor = gxRGBA32(Brightness, Brightness, Brightness, 255);
        
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        if (Tutorial->SequenceTimer >= 0.5f)
            Chapter.PageProps.LightsOff = true;

        if (Tutorial->SequenceTimer >= 6)
        {
            Chapter.PageProps.VacuumOff = false;
            TurnOnVacuum(50, 0.0f, false);

            TutorialOverrides.FocusOnVacuum = true;
            TutorialOverrides.SavedScrollY = ScrollY;
            TutorialOverrides.Timer = 0.5f;
            
            ClearFlames();

            Tutorial->Sequence = 4;
            Tutorial->SequenceTimer = 0;  
        }
    }
    else if (Tutorial->Sequence == 4)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;

        if (Tutorial->SequenceTimer >= 1.0f && Tutorial->SequenceTimer-1.0f/60.0f < 1.0f)
        {
            // Replace flames with balloons.
            int y = Tutorial->Y/64;
            CreateBalloon(3.5f, y-8);
            CreateBalloon(5.5f, y-8);
            CreateBalloon(7.5f, y-8);
        }
        
        if (Tutorial->SequenceTimer >= 5)
        {
            Tutorial->Active = true;

            TutorialOverrides.FocusOnVacuum = false;
            ScrollY = TutorialOverrides.SavedScrollY;
            
            TutorialOverrides.NoBabyPickup = false;
            
            Tutorial->Sequence = 5;
            Tutorial->SequenceTimer = 0;            
        }
    }
    else if (Tutorial->Sequence == 5)
    {
        Tutorial->SequenceTimer += 1.0f/60.0f;
        
        int Brightness = (int)Remap(Tutorial->SequenceTimer, 0, 1.5f, 0, 128, true);        
        LightState.AmbientColor = gxRGBA32(Brightness, Brightness, Brightness, 255);

        if (Tutorial->SequenceTimer >= 3)
        {
            Tutorial->Active = false;
            
            LightState.ForegroundShadows = true;
            Chapter.PageProps.LightsOff = false;
        }
    }
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
    Tutorial->Sequence = 0;

    Tutorial->WiggleTimer = Random(0.0f, 10.0f);    
    
    if (Props->Actions && strstr(Props->Actions, "birthday"))
    {
        CreateBirthdayTutorial(Tutorial);
    }
    if (Props->Actions && strstr(Props->Actions, "hotdate"))
    {
        CreateHotDateTutorial(Tutorial);
    }
    if (Props->Actions && strstr(Props->Actions, "bedtime"))
    {
        CreateBedtimeTutorial(Tutorial);
    }
    if (Props->Actions && strstr(Props->Actions, "graduation"))
    {
        CreateGraduationTutorial(Tutorial);
    }
}

void ClearTutorials()
{
	NTutorials = 0;
    
    TutorialOverrides.FocusOnVacuum = false;
    TutorialOverrides.FreezeDusty = false;
    TutorialOverrides.NoBabyPickup = false;
    TutorialOverrides.NoBabyHop = false;
    TutorialOverrides.NoFlameDamage = false;
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
                if (Props->Centered)
                {
                    X = Tutorial->X + Props->XOffset - (Bubble->SubX2-Bubble->SubX1)/2 + ScrollX;
                    Y = Tutorial->Y + Props->YOffset - (Bubble->SubY2-Bubble->SubY1)/2 + ScrollY;
                }
                else
                {
                    X = Tutorial->X + Props->XOffset + ScrollX;
                    Y = Tutorial->Y + Props->YOffset + ScrollY;
                }
                
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
        
        Tutorial->Timer += 1.0f/60.0f;
        
        Tutorial->WiggleTimer += 1.0f/60.0f;
        
        if (Props->Actions && strstr(Props->Actions, "birthday"))
        {
            UpdateBirthdayTutorial(Tutorial);
            continue;
        }
        if (Props->Actions && strstr(Props->Actions, "hotdate"))
        {
            UpdateHotDateTutorial(Tutorial);
            continue;
        }
        if (Props->Actions && strstr(Props->Actions, "bedtime"))
        {
            UpdateBedtimeTutorial(Tutorial);
            continue;
        }
        if (Props->Actions && strstr(Props->Actions, "graduation"))
        {
            UpdateGraduationTutorial(Tutorial);
            continue;
        }
        
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
                        ScrollY = TutorialOverrides.SavedScrollY;
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
                        Tutorial->Timer = 0;
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
                        TurnOnVacuum(Portfolio.VacuumDistance, 2.0f, false);
                    }
                }
            }
            else
            {
                if (Props->Actions && strstr(Props->Actions, "showfor3secs"))
                {
                    if (Tutorial->Timer >= 3.0f)
                    {
                        Tutorial->Active = false;
                        Tutorial->Disabled = true;
                    }
                }
                else if (Props->Actions && strstr(Props->Actions, "showuntiljump"))
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
