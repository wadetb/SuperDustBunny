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


#define MAX_FLAMES 50


struct SFlame
{
	float X, Y;
	int Frame;
};

int NFlames;
SFlame Flames[MAX_FLAMES];


void CreateFlame(int X, int Y)
{
	if (NFlames >= MAX_FLAMES)
		ReportError("Exceeded the maximum of %d total flames.", MAX_FLAMES);
    
	SFlame* Flame = &Flames[NFlames++];
    
	Flame->X = (float)X + 32;
	Flame->Y = (float)Y + 32;
    
    Flame->Frame = Random(0, 1);
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
        if ( Flame->Frame > 8 )
            Flame->Frame = 0;
        
        if (Dusty.State != DUSTYSTATE_HURT)
        {
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, Flame->X, Flame->Y - 64);
            if (Dist < 100)
            {
                SetDustyState_Hurt();
                Dusty.FloatVelocityX = Dusty.FloatX > Flame->X ? 10.0f : -10.0f;
                Dusty.FloatVelocityY = -10;
            } 	                       	    
        }
    }
}

void DisplayFlames()
{
    for (int i = 0; i < NFlames; i++)
    {
        SFlame* Flame = &Flames[i];
        
        if (Flame->Frame < 4) 
        {
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND_NO_SHADOW, &Flame1Sprite, Flame->X + ScrollX, Flame->Y + ScrollY - 64, 2.0f, 0.0f);
        }
        else 
        {
			AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND_NO_SHADOW, &Flame2Sprite, Flame->X + ScrollX, Flame->Y + ScrollY - 64, 2.0f, 0.0f);
        } 

        if (Chapter.PageProps.LightsOff)
        {
            float Alpha = 0.5f + 0.1f * Flame->Frame;
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_LIGHTING, &LightFlashlightSprite, Flame->X + ScrollX, Flame->Y + ScrollY, 1.3f, Alpha);
        }
    }
}

