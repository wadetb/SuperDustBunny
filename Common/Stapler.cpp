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

void CreateStapler(int X, int Y)
{
    if (NStaplers >= MAX_STAPLERS)
        ReportError("Exceeded the maximum of %d total Staplers.", MAX_STAPLERS);

    SStapler* Stapler = &Staplers[NStaplers++];

    Stapler->X = (float)X + 32;
    Stapler->Y = (float)Y + 32;

	Stapler->Left = -160;
	Stapler->Right = 160;
	Stapler->Top = -70;
	Stapler->Bottom = 0;
    
    Stapler->PowerJump = 0;

	Stapler->CollideWithLeftSide = false;
	Stapler->CollideWithRightSide = false;
	Stapler->CollideWithTopSide = false;

	Stapler->CanLaunch = true;
	Stapler->TimerWait = 30;

    Stapler->State = STAPLERSTATE_WAIT;
}

void ClearStaplers()
{
    NStaplers = 0;
}

void DisplayStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];

        if (Stapler->State == STAPLERSTATE_WAIT)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerUpSprite, Stapler->X, Stapler->Y + ScrollY - 50, 1.0f, 0.0f);
        }
        else if (Stapler->State == STAPLERSTATE_PRELAUNCH)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerDownSprite, Stapler->X, Stapler->Y + ScrollY - 50, 1.0f, 0.0f);
            
#ifdef PLATFORM_WINDOWS
            if (Stapler->PowerJumpCounter > 0)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump1Sprite, Stapler->X + 80, Stapler->Y + ScrollY - 160, 2, 0, 13, 110);
            }
            
            if (Stapler->PowerJumpCounter > 15)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump2Sprite, Stapler->X + 90, Stapler->Y + ScrollY - 160, 14, 0, 26, 110); 
            }
            
            if (Stapler->PowerJumpCounter > 30)
            {
               AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump3Sprite, Stapler->X + 100, Stapler->Y + ScrollY - 160, 27, 0, 40, 110);
            }
            
            if (Stapler->PowerJumpCounter > 45)
            {
               AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump4Sprite, Stapler->X + 110, Stapler->Y + ScrollY - 160, 40, 0, 53, 110);
            }
            
            if (Stapler->PowerJumpCounter > 60)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump5Sprite, Stapler->X + 120, Stapler->Y + ScrollY - 160, 54, 0, 65, 110); 
            }
            
            if (Stapler->PowerJumpCounter > 75)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump6Sprite, Stapler->X + 130, Stapler->Y + ScrollY - 160, 67, 0, 78, 110);
            }
            
            if (Stapler->PowerJumpCounter > 90)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump7Sprite, Stapler->X + 140, Stapler->Y + ScrollY - 160, 80, 0, 92, 110);
            }
            
            if (Stapler->PowerJumpCounter > 105)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump8Sprite, Stapler->X + 150, Stapler->Y + ScrollY - 160, 93, 0, 104, 110);
            }
            
            if (Stapler->PowerJumpCounter > 120)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump9Sprite, Stapler->X + 160, Stapler->Y + ScrollY - 160, 106, 0, 118, 110);
            }
            
            if (Stapler->PowerJumpCounter > 135)
            {
                AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump10Sprite, Stapler->X + 170, Stapler->Y + ScrollY - 160, 119, 0, 130, 110);
            }
#endif
        }
        else if (Stapler->CanLaunch == false)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerExtendUpSprite, Stapler->X, Stapler->Y + ScrollY - 50, 1.0f, 0.0f);
        }            
    }
}

void UpdateStaplers()
{
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];
					
        /*if (Stapler->State == STAPLERSTATE_WAIT)
        {
                if (Tutorial.StaplerDisplayed == false)
                {
                    SetGameState_Tutorial(TUTORIALSTATE_STAPLER);
                    return;
                }        
        }*/
        
        if (Stapler->State == STAPLERSTATE_PRELAUNCH)
        {
#ifdef PLATFORM_WINDOWS
			if (kbIsKeyDown(KB_SPACE))
            {
				SetDustyState_PrepareLaunch();
                
                Stapler->PowerJumpCounter += 1;    
            }
			else
			{
				if (Stapler->CollideWithTopSide == false)
				{
					Stapler->State = STAPLERSTATE_WAIT;
					Stapler->TimerWait = 30;
				}
			}
            
            if (!kbIsKeyDown(KB_SPACE) && kbWasKeyDown(KB_SPACE))//Check for release of spacebar
            {
				Stapler->PowerJump = Remap(Stapler->PowerJumpCounter, 0, 135, 20, 28, true);

				Stapler->State = STAPLERSTATE_LAUNCH; 
            }        
#endif
#ifdef PLATFORM_IPHONE
            Stapler->PowerJumpCounter += 1;
            if (Stapler->PowerJumpCounter >= 10)
            {
                Stapler->PowerJumpCounter = 0;
                Stapler->PowerJump = 27;
                Stapler->State = STAPLERSTATE_LAUNCH; 
                Dusty.FloatY -= 10;
                SetDustyState_Launch(0, -Stapler->PowerJump); 
                return;
            }
#endif
        }
        
        if (Stapler->State == STAPLERSTATE_LAUNCH)
        {        
            Stapler->PowerJumpCounter += 1;
            if (Stapler->PowerJumpCounter >= 10)
            {
                Stapler->State = STAPLERSTATE_WAIT;
                Stapler->TimerWait = 30;
            }

        }
        
		if (Stapler->State == STAPLERSTATE_WAIT)
		{
			if (Stapler->TimerWait == 0)
			{
				Stapler->CanLaunch = true;
			}
			else
			{
				Stapler->TimerWait --;
			}   
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
				
                if (Stapler->CollideWithTopSide && Dusty.CollideWithBottomSide && Stapler->CanLaunch)
                {
                    Stapler->CanLaunch = false;
                    Stapler->State = STAPLERSTATE_PRELAUNCH;
                    Stapler->PowerJumpCounter = 0;
                    SetDustyState_PrepareLaunch();
                    Dusty.FloatY -= 35;
                }
			}
		}
	}
}
