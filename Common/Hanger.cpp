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
    float PrevAngle;
    int DustySide;
    int DustyClearTimer;
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
    
    //AddDebugLine(Hanger->X + ScrollX, Hanger->Y + ScrollY, X + ScrollX, Y + ScrollY, gxRGB32(255, 255, 0), 1.0f/60.0f);
    //AddDebugLine(X + ScrollX, Y + ScrollY, X + FX*100 + ScrollX, Y + FY*100 + ScrollY, gxRGB32(255, 0, 0), 1.0f/60.0f);

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
        
        float P0X = Hanger->X + X0*ca - Y0*sa;
        float P0Y = Hanger->Y + X0*sa + Y0*ca;
        AddHangerTorque(Hanger, P0X, P0Y, 0, Force);
        
        float P1X = Hanger->X + X1*ca - Y0*sa;
        float P1Y = Hanger->Y + X1*sa + Y0*ca;
        AddHangerTorque(Hanger, P1X, P1Y, 0, Force);

        float P2X = Hanger->X + X1*ca - Y1*sa;
        float P2Y = Hanger->Y + X1*sa + Y1*ca;
        AddHangerTorque(Hanger, P2X, P2Y, 0, Force);

        float P3X = Hanger->X + X0*ca - Y1*sa;
        float P3Y = Hanger->Y + X0*sa + Y1*ca;
        AddHangerTorque(Hanger, P3X, P3Y, 0, Force);
        
        float DustyMass = 0.02f;

        if (Hanger->DustyOnBoard)
        {
            AddHangerTorque(Hanger, Dusty.FloatX, Dusty.FloatY, 0, DustyMass * Gravity);
            
            float AngleDelta = Hanger->Angle - Hanger->PrevAngle;
            
            float DX = Dusty.FloatX - Hanger->X;
            float DY = Dusty.FloatY - Hanger->Y;
            
            ca = cosf(AngleDelta);
            sa = sinf(AngleDelta);
            
            Dusty.FloatX = Hanger->X + DX*ca - DY*sa;
            Dusty.FloatY = Hanger->Y + DX*sa + DY*ca;
            
            if (Dusty.State != DUSTYSTATE_STAND && Dusty.State != DUSTYSTATE_WALLJUMP)
            {
                Hanger->DustyOnBoard = false;
                Hanger->DustyClearTimer = 5;
            }
        }
        
        int ClosestSide = -1;
        float ClosestSideDist = 1.0e6f;

        if (Hanger->DustyClearTimer > 0)
            Hanger->DustyClearTimer--;
        else
        {
            float DustyCenterX = ( (Dusty.FloatX + Dusty.Right) + (Dusty.FloatX + Dusty.Left) ) / 2;
            float DustyCenterY = ( (Dusty.FloatY + Dusty.Bottom) + (Dusty.FloatY + Dusty.Top) ) / 2;
            float DustyRadius = Distance(Dusty.FloatX + Dusty.Right, Dusty.FloatY + Dusty.Bottom, DustyCenterX, DustyCenterY);
                    
            float VL = Length(Dusty.FloatVelocityX, Dusty.FloatVelocityY);

            float VNX = 0;
            float VNY = 0;
            if (VL > 0.001f)
            {
                VNX = Dusty.FloatVelocityX / VL;
                VNY = Dusty.FloatVelocityY / VL;
            }
                
            float PTX[] = { P0X, P1X, P2X, P3X };
            float PTY[] = { P0Y, P1Y, P2Y, P3Y };
            
            int CheckOrder[] = { 0, 2, 1, 3 };
            
            for (int i = 0; i < 4; i++)
            {
                int Side = CheckOrder[i];
                
                float AX = PTX[Side];
                float AY = PTY[Side];
                float BX = PTX[(Side+1)%4];
                float BY = PTY[(Side+1)%4];
                
                //AddDebugLine(AX + ScrollX, AY + ScrollY, BX + ScrollX, BY + ScrollY, gxRGB32(0, 255, 255), 1.0f/60.0f);

                float ABX = BX - AX;
                float ABY = BY - AY;
                
                float ABL2 = ABX*ABX + ABY*ABY;
                
                float ADX = DustyCenterX - AX;
                float ADY = DustyCenterY - AY;
                
                float T = ( ADX*ABX + ADY*ABY ) / ABL2;

                float PX, PY;
                if ( T < 0 )
                {
                    PX = AX;
                    PY = AY;
                }
                else if ( T > 1 )
                {
                    PX = BX;
                    PY = BY;
                }
                else
                {
                    PX = AX + T * ( BX - AX );
                    PY = AY + T * ( BY - AY );
                }
                
                //AddDebugLine(PX + ScrollX, PY + ScrollY, DustyCenterX + ScrollX, DustyCenterY + ScrollY, gxRGB32(0, 255, 255), 1.0f/60.0f);

                float DX = PX - DustyCenterX;
                float DY = PY - DustyCenterY;
                float DL = sqrtf(DX*DX + DY*DY);
                
                if (DL <= DustyRadius)
                {
                    float DNX = DX / DL;
                    float DNY = DY / DL;
                    
                    AddHangerTorque(Hanger, PX, PY, Dusty.FloatVelocityX * DustyMass, Dusty.FloatVelocityY * DustyMass);            
                    
                    float D = VNX*DNX + VNY*DNY;

                    if (DL < ClosestSideDist)
                    {
                        ClosestSideDist = DL;
                        ClosestSide = Side;
                    }
                    
                    if (Side == 2)
                    {
                        Dusty.FloatX -= DNX * (DustyRadius - DL);
                        Dusty.FloatY -= DNY * (DustyRadius - DL);

                        Dusty.FloatVelocityX -= DNX * (D)*VL;
                        Dusty.FloatVelocityY -= DNY * (D)*VL;
                        
                        Dusty.CollideWithTopSide = true;
                    }
                    else
                    {                        
                        Dusty.FloatX -= DNX * (DustyRadius - DL - 15);
                        Dusty.FloatY -= DNY * (DustyRadius - DL - 15);
                        
                        Dusty.FloatVelocityX = 0; //-= DNX * (D)*VL;
                        Dusty.FloatVelocityY = 0; //-= DNY * (D)*VL;
                    
                        Hanger->DustyOnBoard = true;
                        Hanger->DustySide = Side;
                    }
                }
            }
            
            if (ClosestSide >= 0)
            {
                if (ClosestSide == 0)
                    Dusty.CollideWithBottomSide = true;
                else if (ClosestSide == 1)
                    Dusty.CollideWithLeftSide = true;
                else if (ClosestSide == 2)
                    Dusty.CollideWithTopSide = true;
                else if (ClosestSide == 3)
                    Dusty.CollideWithRightSide = true;
            }
        }
        Hanger->PrevAngle = Hanger->Angle;
        
        Hanger->Angle += Hanger->VAngle;
        Hanger->VAngle *= 0.97f;
    }
}
