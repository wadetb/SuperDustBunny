//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Flame.h"
#include "Dusty.h"
#include "Chapter.h"
#include "Tutorial.h"


#define MAX_FLAMES 50
#define MAX_FLAME_FRAMES 10


enum EFlameAnglularType
{
    ANGULAR_NONE,
    ANGULAR_ROTATE,
    ANGULAR_PENDULUM,
    ANGULAR_STEP,
};

struct SFlameProperties
{
    int NFrames;
    gxSprite Frames[MAX_FLAME_FRAMES];
    int FrameTime;
    
    float XOffset;
    float YOffset;
    
    float XOrigin;
    float YOrigin;
    
    float Scale;
    
    EFlameAnglularType AngularType;
    float AngularSpeed;
    float AngularSwing;
    float AngularStep;
    
    bool IsFire;
    bool ReplaceBlock;
    
    const char* Name;
    SFlameProperties* Next;
};

struct SFlame
{
    SFlameProperties* Props;
	float X, Y;
    float Angle;
	int Frame;
};

SFlameProperties* FlamePropertiesHead;

int NFlames;
SFlame Flames[MAX_FLAMES];

void ParseFlameProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SFlameProperties* Props = (SFlameProperties*)malloc(sizeof(SFlameProperties));
    memset(Props, 0, sizeof(SFlameProperties));
    
    Props->Next = FlamePropertiesHead;
    FlamePropertiesHead = Props;
    
    Props->NFrames = 0;
    Props->FrameTime = 1;
    Props->XOffset = 0;
    Props->YOffset = 0;
    Props->XOrigin = 0;
    Props->YOrigin = 0;
    Props->AngularSpeed = 0.0f;
    Props->AngularSwing = 0.0f;
    Props->AngularStep = 0.0f;
    Props->Scale = 1.0f;
    Props->IsFire = false;
    Props->ReplaceBlock = false;
    Props->Name = NULL;

	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* ConstValue = PropertyNode->first_attribute("value")->value();
        char* WritableValue = strdup(ConstValue);
        char* Value = WritableValue;
        
        if (strcmp(Name, "name") == 0)
        {
            Props->Name = strdup(Value);
        }
		else if (strcmp(Name, "frames") == 0)
		{
            if (Props->NFrames)
                ReportError("Flame has more than one 'frames' property.");
            
            char* Save;
            for (char* p = strtok_r(Value, " ", &Save); p; p = strtok_r(NULL, " ", &Save))
            {
                if (Props->NFrames >= MAX_FLAME_FRAMES)
                    ReportError("Flame has more than the maximum of %d frames.", MAX_FLAME_FRAMES);
                
                LoadSpriteAsset(p, &Props->Frames[Props->NFrames]);
                Props->NFrames++;
            }
        }
        else if (strcmp(Name, "frametime") == 0)
        {
            Props->FrameTime = atoi(Value);
        }
        else if (strcmp(Name, "xoffset") == 0)
        {
            Props->XOffset = (float)atof(Value);
        }
        else if (strcmp(Name, "yoffset") == 0)
        {
            Props->YOffset = (float)atof(Value);
        }
        else if (strcmp(Name, "origin") == 0)
        {
            Props->XOrigin = strtof(Value, &Value);
            Props->YOrigin = strtof(Value, &Value);
        }
        else if (strcmp(Name, "offset") == 0)
        {
            Props->XOffset = strtof(Value, &Value);
            Props->YOffset = strtof(Value, &Value);
        }
        else if (strcmp(Name, "angle") == 0)
        {
            if (strncmp(Value, "rotate ", 7) == 0)
            {
                Props->AngularType = ANGULAR_ROTATE;
                Value += 6;
                Props->AngularSpeed = strtof(Value, &Value);
            }
            else if (strncmp(Value, "pendulum ", 9) == 0)
            {
                Props->AngularType = ANGULAR_PENDULUM;
                Value += 8;
                Props->AngularSpeed = strtof(Value, &Value);
                Props->AngularSwing = strtof(Value, &Value);
            }
            else if (strncmp(Value, "step ", 5) == 0)
            {
                Props->AngularType = ANGULAR_STEP;
                Value += 5;
                Props->AngularSpeed = strtof(Value, &Value);
                Props->AngularStep = strtof(Value, &Value);
            }
            else
                Props->AngularType = ANGULAR_NONE;
        }
        else if (strcmp(Name, "scale") == 0)
        {
            Props->Scale = (float)atof(Value);
        }
        else if (strcmp(Name, "fire") == 0)
        {
            Props->IsFire = atoi(Value) != 0;
        }
        else if (strcmp(Name, "replaceblock") == 0)
        {
            Props->ReplaceBlock = atoi(Value) != 0;
        }
		else if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
		{
			ReportError("Unrecognized flame property '%s'='%s'.", Name, Value);
		}
        
        free(WritableValue);
        
		PropertyNode = PropertyNode->next_sibling("property");
	}
    
    if (Props->NFrames == 0)
    {
        ReportError("Flame is missing the 'frames' property.");
    }
    
	Block->Properties = Props;
}

SFlameProperties* GetFlamePropertiesByName(const char* Name)
{
    for (SFlameProperties* Props = FlamePropertiesHead; Props; Props = Props->Next)
        if (Props->Name && strcmp(Props->Name, Name) == 0)
            return Props;
    return NULL;
}

void ClearFlamePropertiesList()
{
    FlamePropertiesHead = NULL;
}

static float BlockFlagsToAngle(unsigned int Flags)
{
    if (Flags == (SPECIALBLOCKID_FLIP_X | SPECIALBLOCKID_FLIP_DIAGONAL)) return PI/2;
    return 0;
}

void CreateFlame(float X, float Y, unsigned int Flags, SFlameProperties* Props)
{
	if (NFlames >= MAX_FLAMES)
		ReportError("Exceeded the maximum of %d total flames.", MAX_FLAMES);
    
	SFlame* Flame = &Flames[NFlames++];
    
    Flame->Props = Props;
    
	Flame->X = (float)X + 32;
	Flame->Y = (float)Y + 32;
    Flame->Angle = BlockFlagsToAngle(Flags);
    
    Flame->Frame = Random(0, 10);
    
    if (Props->ReplaceBlock)
        EraseBlock((int)X/64, (int)Y/64);
}

void ClearFlames()
{
	NFlames = 0;
}

void UpdateFlames()
{
    for (int i = 0; i < NFlames; i++)
    {
        SFlame* Flame = &Flames[i];

        Flame->Frame++;
        
        SFlameProperties* Props = Flame->Props;

         if (!TutorialOverrides.NoFlameDamage && Props->IsFire)
        {
            if (Dusty.State != DUSTYSTATE_HURT)
            {
                float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, Flame->X, Flame->Y - 64);
                if (Dist < 100)
                {
                    Dusty.OnFireTimer = 30;
                    SetDustyState_Hurt();
                    Dusty.FloatVelocityX = Dusty.FloatX > Flame->X ? 10.0f : -10.0f;
                    Dusty.FloatVelocityY = -10;
                } 	                       	    
            }
        }
    }
}

void DisplayFlames()
{
    for (int i = 0; i < NFlames; i++)
    {
        SFlame* Flame = &Flames[i];
        
        SFlameProperties* Props = Flame->Props;
        
        gxSprite* Sprite = &Props->Frames[(Flame->Frame / Props->FrameTime) % Props->NFrames];

        float X = Flame->X + Props->XOffset;
        float Y = Flame->Y + Props->YOffset;
        float Scale = Props->Scale;
        
        float XOrigin = Props->XOrigin;
        float YOrigin = Props->YOrigin;
        
        float Angle = Flame->Angle;
        if (Props->AngularType == ANGULAR_ROTATE)
            Angle += DegreesToRadians(Flame->Frame * Props->AngularSpeed/60.0f);
        else if (Props->AngularType == ANGULAR_PENDULUM)
            Angle += DegreesToRadians(Props->AngularSwing * sinf(DegreesToRadians(Flame->Frame * Props->AngularSpeed/60.0f)));
        else if (Props->AngularType == ANGULAR_STEP)
        {
            Angle += DegreesToRadians(Flame->Frame * Props->AngularSpeed/60.0f);
            Angle -= fmodf(Angle, DegreesToRadians(Props->AngularStep));
        }
        
        AddLitSpriteOriginScaledRotatedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, Sprite, X + ScrollX, Y + ScrollY, XOrigin, YOrigin, Scale, Angle, 1.0f);

        if (Chapter.PageProps.LightsOff && Props->IsFire)
        {
            float Alpha = 0.2f + 0.025f * sinf(Flame->Frame*0.1f) + 0.025f * cosf(Flame->Frame*0.3f);
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_LIGHTING, &LightFlashlightSprite, X + ScrollX, Y + ScrollY, 2.0f, Alpha);
        }
    }
}

