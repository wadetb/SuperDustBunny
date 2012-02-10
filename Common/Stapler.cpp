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
#include "GameScore.h"

#define MAX_STAPLERS 100

int NStaplers = 0;
SStapler Staplers[MAX_STAPLERS];

SStapler* CreateStapler(int X, int Y, EStaplerType Type)
{
    if (NStaplers >= MAX_STAPLERS)
        ReportError("Exceeded the maximum of %d total Staplers.", MAX_STAPLERS);

    SStapler* Stapler = &Staplers[NStaplers++];

    Stapler->X = (float)X + 32;
    Stapler->Y = (float)Y + 32;

    Stapler->Type = Type;

    switch (Stapler->Type)
    {
    default:
    case STAPLER_STAPLER:
    case STAPLER_CLOTHESPIN:
        Stapler->Left = -160;
        Stapler->Right = 160;
        Stapler->Top = -70;
        Stapler->Bottom = 0;
        break;
    case STAPLER_JELLO:
        Stapler->Left = -100;
        Stapler->Right = 100;
        Stapler->Top = -85;
        Stapler->Bottom = 0;
        break;
    }
    
    Stapler->Timer = 0;
    
    Stapler->ChargeTime = 0;

	Stapler->CollideWithLeftSide = false;
	Stapler->CollideWithRightSide = false;
	Stapler->CollideWithTopSide = false;

    Stapler->State = STAPLERSTATE_WAIT;
    
    Stapler->NoShadow = false;
    
    return Stapler;
}

void ClearStaplers()
{
    NStaplers = 0;
}

gxSprite* StaplerSprites[] =
{
    &Stapler1Sprite,
    &Stapler1Sprite,
    &Stapler2Sprite,
    &Stapler3Sprite,
    &Stapler1Sprite,
};

gxSprite* JelloSprites[] =
{
    &Jello1Sprite,
    &Jello1Sprite,
    &Jello2Sprite,
    &Jello3Sprite,
    &Jello4Sprite,
    &Jello5Sprite,
    &Jello2Sprite,
    &Jello5Sprite,
    &Jello1Sprite,
};

gxSprite* ClothespinSprites[] = 
{
    &Clothespin1Sprite,
    &Clothespin1Sprite,
    &Clothespin2Sprite,
    &Clothespin1Sprite,
};

void DisplayStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];

        int FrameCount;
        gxSprite** Frames;
        float OffsetY;
        
        switch (Stapler->Type)
        {
        default:
        case STAPLER_STAPLER: 
            Frames = StaplerSprites; 
            FrameCount = sizeof(StaplerSprites)/sizeof(gxSprite*); 
            OffsetY = -60;
            break;
        case STAPLER_JELLO: 
            Frames = JelloSprites; 
            FrameCount = sizeof(JelloSprites)/sizeof(gxSprite*); 
            OffsetY = -60;
            break;
        case STAPLER_CLOTHESPIN: 
            Frames = ClothespinSprites; 
            FrameCount = sizeof(ClothespinSprites)/sizeof(gxSprite*); 
            OffsetY = -30;
            break;
        }
        
        ELightList LightList = Stapler->NoShadow ? LIGHTLIST_FOREGROUND_NO_SHADOW : LIGHTLIST_FOREGROUND;
        
        if (Stapler->State == STAPLERSTATE_CHARGE)
        {
            int Frame = 2;
            if (Frame >= FrameCount) 
                Frame = FrameCount-1;
            AddLitSpriteCenteredScaledRotated(LightList, Frames[Frame], Stapler->X + ScrollX, Stapler->Y + ScrollY + OffsetY, 1.0f, 0.0f);
        }
        else if (Stapler->State == STAPLERSTATE_LAUNCH)
        {
            int Frame = 1+Stapler->Timer/5;
            if (Frame >= FrameCount) 
                Frame = FrameCount-1;
            AddLitSpriteCenteredScaledRotated(LightList, Frames[Frame], Stapler->X + ScrollX, Stapler->Y + ScrollY + OffsetY, 1.0f, 0.0f);
        }
        else
            AddLitSpriteCenteredScaledRotated(LightList, Frames[0], Stapler->X + ScrollX, Stapler->Y + ScrollY + OffsetY, 1.0f, 0.0f);            
    }
}

void UpdateStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];
				
        if (Stapler->State == STAPLERSTATE_CHARGE)
        {
            Stapler->ChargeTime += 1.0f/60.0f;
            
            if (Stapler->ChargeTime >= 0.25f)
                Dusty.FloatX += Random(-1.0f, 1.0f);
            
            if (!msButton1)
            {
                Stapler->State = STAPLERSTATE_LAUNCH;
                Stapler->Timer = 9;
            }
        }
        else if (Stapler->State == STAPLERSTATE_LAUNCH)
        {
            Stapler->Timer++;
            if (Stapler->Timer == 10)
            {
                Dusty.FloatY -= 10;
                
                if (Stapler->ChargeTime >= 0.25f)
                {
                    Dusty.PowerUpTimer = 30;
                    SetDustyState_Launch(0, -24); 
                }
                else
                    SetDustyState_Launch(0, -21); 
            }
            if (Stapler->Timer >= 20)
            {
                Stapler->State = STAPLERSTATE_WAIT; 
                return;
            }
        }
        
		if (Stapler->State == STAPLERSTATE_WAIT)
		{
            Stapler->Timer++;
        }
    }  
}

void UpdateStapler_Collision()
{
	for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];
	
		Stapler->CollideWithLeftSide = false;
		Stapler->CollideWithRightSide = false;
		Stapler->CollideWithTopSide = false;

		if (Max(Dusty.FloatY + Dusty.Top, Stapler->Y + Stapler->Top) <= Min(Dusty.FloatY + Dusty.Bottom, Stapler->Y + Stapler->Bottom))
		{
			// Check to see if The Dusty rectangle also overlaps with the Stapler.
			if(Max(Dusty.FloatX + Dusty.Left, Stapler->X + Stapler->Left) <= Min(Dusty.FloatX + Dusty.Right, Stapler->X + Stapler->Right))
			{    
				// Calculate the distance Dusty would have to be pushed in each possible direction to get him out of intersecting the Stapler.
				float LeftDistance	= (Dusty.FloatX + Dusty.Right)  - (Stapler->X + Stapler->Left);
				float RightDistance	= (Stapler->X + Stapler->Right) - (Dusty.FloatX +  Dusty.Left);
				float UpDistance	= (Dusty.FloatY + Dusty.Bottom) - (Stapler->Y + Stapler->Top);

				// Prefer to collide with the side of the Stapler that would push Dusty out the least distance.
				if (LeftDistance < RightDistance && LeftDistance < UpDistance)
				{
					Stapler->CollideWithLeftSide = true;//Collision with Dusty's Right Side but the left side of the Stapler
                    Dusty.CollideWithRightSide = true;
					Dusty.FloatX -= LeftDistance;
                    if (Dusty.FloatVelocityX > 0)
                        Dusty.FloatVelocityX = 0;
				}

				if (RightDistance < LeftDistance && RightDistance < UpDistance)
				{
					Stapler->CollideWithRightSide = true;//Collision with Dusty's Left Side but the right side of the Stapler
                    Dusty.CollideWithLeftSide = true;
					Dusty.FloatX += RightDistance;
                    if (Dusty.FloatVelocityX < 0)
                        Dusty.FloatVelocityX = 0;
				}

				if (UpDistance < RightDistance && UpDistance < LeftDistance)
				{
					Stapler->CollideWithTopSide = true;//Collision with Dusty's Bottom Side but the Top side of the Stapler.
					Dusty.CollideWithBottomSide = true;
					Dusty.FloatY -= UpDistance;
					if (Dusty.FloatVelocityY > 0)
						Dusty.FloatVelocityY = 0;
				}
				
                if (Stapler->CollideWithTopSide && Dusty.CollideWithBottomSide && Stapler->State == STAPLERSTATE_WAIT)
                {
                    Stapler->ChargeTime = 0;
                    Stapler->Timer = 0;
                    if (msButton1)
                        Stapler->State = STAPLERSTATE_CHARGE;
                    else
                        Stapler->State = STAPLERSTATE_LAUNCH;
                    SetDustyState_PrepareLaunch();
                    Dusty.FloatY -= 35;
                }
			}
		}
	}
}
