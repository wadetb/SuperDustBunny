//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Hanger.h"
#include "Dusty.h"
#include "Chapter.h"


#define MAX_HANGERS 50

struct SHangerProperties
{
    gxSprite* Sprite;
    
    float XOrigin;
    float YOrigin;

    int Width;
    int Height;
    
    int SubX1;
    int SubY1;
    int SubX2;
    int SubY2;
    
    float Mass;
};

struct SHanger
{
    SHangerProperties* Props;
	float X, Y;
    float Angle;
    float VAngle;
    bool DustyOnBoard;
};


int NHangers;
SHanger Hangers[MAX_HANGERS];

void ParseHangerProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SHangerProperties* Props = (SHangerProperties*)malloc(sizeof(SHangerProperties));
    
    Props->XOrigin = 0;
    Props->YOrigin = 0;
    Props->Width = 0;
    Props->Height = 0;
    Props->Mass = 1.0f;
    
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* ConstValue = PropertyNode->first_attribute("value")->value();
        char* WritableValue = strdup(ConstValue);
        char* Value = WritableValue;
        
        if (strcmp(Name, "origin") == 0)
        {
            Props->XOrigin = strtof(Value, &Value);
            Props->YOrigin = strtof(Value, &Value);
        }
        else if (strcmp(Name, "size") == 0)
        {
            Props->Width = (int)strtof(Value, &Value);
            Props->Height = (int)strtof(Value, &Value);
        }
        else if (strcmp(Name, "mass") == 0)
        {
            Props->Mass = strtof(Value, &Value);
        }
		else if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
		{
			ReportError("Unrecognized Hanger property '%s'='%s'.", Name, Value);
		}
        
        free(WritableValue);
        
		PropertyNode = PropertyNode->next_sibling("property");
	}
    
    if (Props->Width == 0 || Props->Height == 0)
    {
        ReportError("Hanger is missing the 'size' property, or one or more dimensions is 0.");
    }
    
    Props->Sprite = &Chapter.TileSets[Block->TileSet].Sprite;
    Props->SubX1 = Block->SubX;
    Props->SubY1 = Block->SubY;
    Props->SubX2 = Block->SubX + Props->Width*64;
    Props->SubY2 = Block->SubY + Props->Height*64;

	Block->Properties = Props;
}

void CreateHanger(int X, int Y, SHangerProperties* Props)
{
	if (NHangers >= MAX_HANGERS)
		ReportError("Exceeded the maximum of %d total Hangers.", MAX_HANGERS);
    
	SHanger* Hanger = &Hangers[NHangers++];
    
    Hanger->Props = Props;
    
	Hanger->X = (float)X*64 + Props->XOrigin;
	Hanger->Y = (float)Y*64 + Props->YOrigin;
    Hanger->Angle = 0;
    Hanger->VAngle = 0;
    Hanger->DustyOnBoard = false;
    
    for (int y = Y; y < Y+Props->Height; y++)
        for (int x = X; x < X+Props->Width; x++)
            EraseBlock(x, y);
}

void ClearHangers()
{
	NHangers = 0;
}

void DisplayHangers()
{
    for (int i = 0; i < NHangers; i++)
    {
        SHanger* Hanger = &Hangers[i];
        
        SHangerProperties* Props = Hanger->Props;
        
        gxSprite* Sprite = Props->Sprite;
        
        float X = Hanger->X;
        float Y = Hanger->Y;
        float XOrigin = Props->XOrigin;
        float YOrigin = Props->YOrigin;
        float Angle = Hanger->Angle;
        
        AddLitSubSpriteOriginScaledRotatedAlpha(LIGHTLIST_FOREGROUND, Sprite, 
                                                X + ScrollX, Y + ScrollY, XOrigin, YOrigin, 
                                                Props->SubX1, Props->SubY1, Props->SubX2, Props->SubY2, 
                                                1.0f, Angle, 1.0f);
    }
}

static void AddHangerTorque(SHanger* Hanger, float X, float Y, float FX, float FY)
{
    float PX = Y - Hanger->Y;
    float PY = -(X - Hanger->X);
    
    float L = Length(PX, PY);
    if (L < 0.001f)
        return;
    
//    AddDebugLine(Hanger->X + ScrollX, Hanger->Y + ScrollY, X + ScrollX, Y + ScrollY, gxRGB32(255, 255, 0), 1.0f/60.0f);

    float NPX = PX/L;
    float NPY = PY/L;
    
    float D = NPX*FX + NPY*FY;
    
    float Torque = L * D;
    
    Hanger->VAngle -= Torque * 0.001f;
}

void UpdateHangers()
{
    for (int i = 0; i < NHangers; i++)
    {
        SHanger* Hanger = &Hangers[i];
        
        SHangerProperties* Props = Hanger->Props;

        float Gravity = 1.0f;

        float Force = 0.25f * Props->Mass * Gravity;
        
        float X0 = -Props->XOrigin;
        float Y0 = -Props->YOrigin;
        float X1 = Props->Width*64 - Props->XOrigin;
        float Y1 = Props->Height*64 - Props->YOrigin;
        
        float ca = cosf(Hanger->Angle);
        float sa = sinf(Hanger->Angle);
        
        float P0X = X0*ca - Y0*sa;
        float P0Y = X0*sa + Y0*ca;
        AddHangerTorque(Hanger, Hanger->X + P0X, Hanger->Y + P0Y, 0, Force);
        
        float P1X = X1*ca - Y0*sa;
        float P1Y = X1*sa + Y0*ca;
        AddHangerTorque(Hanger, Hanger->X + P1X, Hanger->Y + P1Y, 0, 0.1f+Force);

        float P2X = X1*ca - Y1*sa;
        float P2Y = X1*sa + Y1*ca;
        AddHangerTorque(Hanger, Hanger->X + P2X, Hanger->Y + P2Y, 0, 0.1f+Force);

        float P3X = X0*ca - Y1*sa;
        float P3Y = X0*sa + Y1*ca;
        AddHangerTorque(Hanger, Hanger->X + P3X, Hanger->Y + P3Y, 0, Force);
        
        if (Hanger->DustyOnBoard)
        {
            float Mass = 0.25f;
            AddHangerTorque(Hanger, Dusty.FloatX, Dusty.FloatY, 0, Mass * Gravity);            
        }
        
        float DustyRadius = 50;
        
        if (Distance(Dusty.FloatX, Dusty.FloatY, P0X, P0Y) < DustyRadius)
        {
            Dusty.FloatVelocityX = 0;
            Dusty.FloatVelocityY = 0;
            
        }
        
        Hanger->Angle += Hanger->VAngle;
        Hanger->VAngle *= 0.97f;
        
//        AddDebugLine(Hanger->X + P0X + ScrollX, Hanger->Y + P0Y + ScrollY, Hanger->X + P1X + ScrollX, Hanger->Y + P1Y + ScrollY, gxRGB32(255, 0, 0), 1.0f/60.0f);
//        AddDebugLine(Hanger->X + P1X + ScrollX, Hanger->Y + P1Y + ScrollY, Hanger->X + P2X + ScrollX, Hanger->Y + P2Y + ScrollY, gxRGB32(255, 0, 0), 1.0f/60.0f);
//        AddDebugLine(Hanger->X + P2X + ScrollX, Hanger->Y + P2Y + ScrollY, Hanger->X + P3X + ScrollX, Hanger->Y + P3Y + ScrollY, gxRGB32(255, 0, 0), 1.0f/60.0f);
//        AddDebugLine(Hanger->X + P3X + ScrollX, Hanger->Y + P3Y + ScrollY, Hanger->X + P0X + ScrollX, Hanger->Y + P0Y + ScrollY, gxRGB32(255, 0, 0), 1.0f/60.0f);
    }
}
