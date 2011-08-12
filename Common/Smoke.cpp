//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Smoke.h"


#define MAX_SMOKE 100


struct SSmoke
{
    bool Active;
    gxSprite* Sprite;
    float Age;
    float Life;
    float Birth;
    float Alpha;
    float X, Y;
    float Scale;
    float Angle;
    float VX, VY;
    float VScale;
    float VAngle;
    float FX, FY;
    float FAngle;
};


SSmoke AllSmoke[MAX_SMOKE];

int NextSmoke = 0;

void InitSmoke()
{
    memset(AllSmoke, 0, sizeof(AllSmoke));    
}

void DisplaySmoke()
{
    for (int i = 0; i < MAX_SMOKE; i++)
    {
        SSmoke* Smoke = &AllSmoke[i];
        
        if (!Smoke->Active)
            continue;
        
        if (Smoke->Age >= Smoke->Life)
            continue;
        
        float Alpha = Smoke->Alpha;
        Alpha *= Remap(Smoke->Age, 0, Smoke->Life*1.0f/3.0f, 0.0f, 1.0f, true);
        Alpha *= Remap(Smoke->Age, Smoke->Life*2.0f/3.0f, Smoke->Life, 1.0f, 0.0f, true);
        
        AddLitSpriteCenteredScaledRotatedAlpha(LIGHTLIST_SMOKE, Smoke->Sprite, Smoke->X + ScrollX, Smoke->Y + ScrollY, Smoke->Scale, Smoke->Angle, Alpha);
    }
}

void UpdateSmoke()
{
    for (int i = 0; i < MAX_SMOKE; i++)
    {
        SSmoke* Smoke = &AllSmoke[i];
        
        if (!Smoke->Active)
            continue;
        
        Smoke->Age += 1.0f/15.0f;
        
        if (Smoke->Age >= Smoke->Life)
        {
            Smoke->Active = false;
            continue;
        }
        
        Smoke->X += Smoke->VX;
        Smoke->Y += Smoke->VY;
        Smoke->Scale += Smoke->VScale;
        Smoke->Angle += Smoke->VAngle;
        
        if (Smoke->Scale > 1.5f)
            Smoke->Scale = 1.5f;
        
        Smoke->VX *= Smoke->FX;
        Smoke->VY *= Smoke->FY;
        Smoke->VAngle *= Smoke->FAngle;
    }
}

void CreateSmoke(float X, float Y, int Count)
{
    for (int i = 0; i < Count; i++)
    {
        SSmoke* Smoke = &AllSmoke[NextSmoke];
        
        NextSmoke++;
        if (NextSmoke >= MAX_SMOKE)
            NextSmoke = 0;
        
        Smoke->Active = true;

        Smoke->Sprite = &VacuumSmokeSprite;
        
        Smoke->Age = 0.0f;
        Smoke->Life = Random(9.0f, 10.0f);
        Smoke->Birth = Random(0.0f, 0.25f);
        
        Smoke->Alpha = 0.65f;
        
        Smoke->X = Random(X - 75.0f, X + 75.0f);
        Smoke->Y = Random(Y, Y + 50.0f);
        Smoke->VX = Random(-5.0f, 5.0f);
        Smoke->VY = Random(-5.0f, -10.0f);
        
        Smoke->Scale = Random(0.75f, 1.25f);
        Smoke->VScale = Random(0.002f, 0.01f);
        
        Smoke->Angle = Random(DegreesToRadians(0), DegreesToRadians(360.0f));
        Smoke->VAngle = Random(DegreesToRadians(-1), DegreesToRadians(1));
        
        Smoke->FX = 0.95f;
        Smoke->FY = 0.95f;
        Smoke->FAngle = 0.99f;
    }
}

void CreateSmallSmoke(float X, float Y)
{
    SSmoke* Smoke = &AllSmoke[NextSmoke];
    
    NextSmoke++;
    if (NextSmoke >= MAX_SMOKE)
        NextSmoke = 0;
    
    Smoke->Active = true;
    
    Smoke->Sprite = &VacuumSmokeSprite;
    
    Smoke->Age = 0.0f;
    Smoke->Life = Random(9.0f, 10.0f);
    Smoke->Birth = Random(0.0f, 0.25f);
    
    Smoke->Alpha = 0.65f;
    
    Smoke->X = Random(X - 15.0f, X + 15.0f);
    Smoke->Y = Random(Y - 15.0f, Y + 15.0f);
    Smoke->VX = Random(-5.0f, 5.0f);
    Smoke->VY = Random(-5.0f, -10.0f);
    
    Smoke->Scale = Random(0.3f, 0.35f);
    Smoke->VScale = Random(0.0005f, 0.001f);
    
    Smoke->Angle = Random(DegreesToRadians(0), DegreesToRadians(360.0f));
    Smoke->VAngle = Random(DegreesToRadians(-1), DegreesToRadians(1));
    
    Smoke->FX = 0.95f;
    Smoke->FY = 0.95f;
    Smoke->FAngle = 0.99f;
}

void CreateBonus(float X, float Y)
{
    SSmoke* Smoke = &AllSmoke[NextSmoke];
    
    NextSmoke++;
    if (NextSmoke >= MAX_SMOKE)
        NextSmoke = 0;
    
    Smoke->Active = true;
    
    Smoke->Sprite = &BubbleMinus1SecSprite;
    
    Smoke->Age = 0.0f;
    Smoke->Life = 10.0f;
    Smoke->Birth = 0.0f;
    
    Smoke->Alpha = 1.0f;
    
    Smoke->X = X;
    Smoke->Y = Y;
    Smoke->VX = 0;
    Smoke->VY = -2.0f;
    
    Smoke->Scale = 0.4f;
    Smoke->VScale = 0.0f;
    
    Smoke->Angle = 0.0f;
    Smoke->VAngle = 0.0f;
    
    Smoke->FX = 0.0f;
    Smoke->FY = 0.99f;
    Smoke->FAngle = 0.0f;
}

void Create1UpBubble(float X, float Y)
{
    SSmoke* Smoke = &AllSmoke[NextSmoke];
    
    NextSmoke++;
    if (NextSmoke >= MAX_SMOKE)
        NextSmoke = 0;
    
    Smoke->Active = true;
    
    Smoke->Sprite = &Bubble1UpSprite;
    
    Smoke->Age = 0.0f;
    Smoke->Life = 10.0f;
    Smoke->Birth = 0.0f;
    
    Smoke->Alpha = 1.0f;
    
    Smoke->X = X;
    Smoke->Y = Y;
    Smoke->VX = 0;
    Smoke->VY = -2.0f;
    
    Smoke->Scale = 0.4f;
    Smoke->VScale = 0.0f;
    
    Smoke->Angle = 0.0f;
    Smoke->VAngle = 0.0f;
    
    Smoke->FX = 0.0f;
    Smoke->FY = 0.99f;
    Smoke->FAngle = 0.0f;
}

