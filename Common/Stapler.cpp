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

	Stapler->Left = -196;
	Stapler->Right = 168;
	Stapler->Top = -90;
	Stapler->Bottom = 0;
    
    Stapler->PowerJump = 0;

	Stapler->CollideWithLeftSide = false;
	Stapler->CollideWithRightSide = false;
	Stapler->CollideWithTopSide = false;

	Stapler->CanLaunch = true;
	Stapler->TimerWait = 0;

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
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerUpSprite, Stapler->X, Stapler->Y + ScrollY - 30, 1.0f, 0.0f);
        }
        else if (Stapler->State == STAPLERSTATE_PRELAUNCH)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerDownSprite, Stapler->X, Stapler->Y + ScrollY - 30, 1.0f, 0.0f);
            
            switch(Stapler->PowerJumpCounter)
            {
            case 10: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump1Sprite, Stapler->X + 50, Stapler->Y, 2, 0, 13, 110); break;
            case 20: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump2Sprite, Stapler->X + 50, (Stapler->Y + 10), 14, 0, 26, 110); break;
            case 30: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump3Sprite, Stapler->X + 50, (Stapler->Y + 20), 27, 0, 40, 110); break;
            case 40: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump4Sprite, Stapler->X + 50, (Stapler->Y + 30), 40, 0, 53, 110); break;
            case 50: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump5Sprite, Stapler->X + 50, (Stapler->Y + 40), 54, 0, 65, 110); break;
            case 60: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump6Sprite, Stapler->X + 50, (Stapler->Y + 50), 67, 0, 78, 110); break;
            case 70: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump7Sprite, Stapler->X + 50, (Stapler->Y + 60), 80, 0, 92, 110); break;
            case 80: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump8Sprite, Stapler->X + 50, (Stapler->Y + 70), 93, 0, 104, 110); break;
            case 90: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump9Sprite, Stapler->X + 50, (Stapler->Y + 80), 106, 0, 118, 110); break;
            case 100: AddLitSubSprite(LIGHTLIST_FOREGROUND, &PowerJump10Sprite, Stapler->X + 50, (Stapler->Y + 90), 119, 0, 130, 110); break;
            default: continue;
            }
        }
        else if (Stapler->State == STAPLERSTATE_LAUNCH)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerExtendUpSprite, Stapler->X, Stapler->Y + ScrollY - 30, 1.0f, 0.0f);
        }            
    }
}

void UpdateStaplers()
{
#ifdef PLATFORM_WINDOWS
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];
			
		if (Stapler->CollideWithTopSide && Stapler->CanLaunch)
		{
			Stapler->State = STAPLERSTATE_PRELAUNCH;
		}
		else
		{
			Stapler->State = STAPLERSTATE_WAIT;
		} 
		
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
			SetDustyState_PrepareLaunch();

			if (kbIsKeyDown(KB_SPACE))
            {
                if (Stapler->PowerJumpCounter > 100)
                {
                    Stapler->PowerJumpCounter = 100;
                }
                
                Stapler->PowerJumpCounter += 1;    
            }
            
            if (!kbIsKeyDown(KB_SPACE) && kbWasKeyDown(KB_SPACE))//Check for release of spacebar
            {
                switch(Stapler->PowerJumpCounter)
                {
                case 10: Stapler->PowerJump = 10.0f; break;
                case 20: Stapler->PowerJump = 20.0f; break;
                case 30: Stapler->PowerJump = 30.0f; break;
                case 40: Stapler->PowerJump = 40.0f; break;
                case 50: Stapler->PowerJump = 50.0f; break;
                case 60: Stapler->PowerJump = 60.0f; break;
                case 70: Stapler->PowerJump = 70.0f; break;
                case 80: Stapler->PowerJump = 80.0f; break;
                case 90: Stapler->PowerJump = 90.0f; break;
                case 100: Stapler->PowerJump = 100.0f; break;
                default: break;
                }
              
                Stapler->State = STAPLERSTATE_LAUNCH; 
            }        
        }
        
        if (Stapler->State == STAPLERSTATE_LAUNCH)
        {        
            Stapler->State = STAPLERSTATE_WAIT;
            Stapler->CanLaunch = false;
            Stapler->TimerWait = 500;
            Stapler->PowerJumpCounter = 0;
            Stapler->PowerJump = 0;
            SetDustyState_StaplerLaunch();      
        }
        
        if (Stapler->TimerWait == 0)
        {
            Stapler->CanLaunch = true;
        }
        else
        {
            Stapler->TimerWait --;
        }                    
    }  
#endif
}

void UpdateStapler_Collision()
{
	for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];
	
		if (Max(Dusty.FloatY + Dusty.Top, Stapler->Y + Stapler->Top) <= Min(Dusty.FloatY + Dusty.Bottom, Stapler->Y + Stapler->Bottom))
		{
			// Check to see if The Dusty rectangle also overlaps with the Stapler.
			if(Max(Dusty.FloatX + Dusty.Left, Stapler->X + Stapler->Left) <= Min(Dusty.FloatX + Dusty.Right, Stapler->X + Stapler->Right))
			{    
				// Calculate the distance Dusty would have to be pushed in each possible direction to get him out of intersecting the Stapler.
				float LeftDistance	= (Dusty.FloatX + Dusty.Right)  - (Stapler->X + Stapler->Left);
				float RightDistance	= (Stapler->X + Stapler->Right) - (Dusty.FloatX +  Dusty.Left);
				float DownDistance	= (Stapler->Y + Stapler->Bottom)- (Dusty.FloatY +  Dusty.Top);
				float UpDistance	= (Dusty.FloatY + Dusty.Bottom) - (Stapler->Y + Stapler->Top);

				// Prefer to collide with the side of the Stapler that would push Dusty out the least distance.
				if (LeftDistance < RightDistance && LeftDistance < DownDistance && LeftDistance < UpDistance)
				{
					Stapler->CollideWithLeftSide = true;//Collision with Dusty's Right Side but the left side of the Stapler
					Dusty.FloatX -= LeftDistance;
				}

				if (RightDistance < LeftDistance && RightDistance < DownDistance && RightDistance < UpDistance)
				{
					Stapler->CollideWithRightSide = true;//Collision with Dusty's Left Side but the right side of the Stapler
					Dusty.FloatX += RightDistance;
				}

				if (UpDistance < RightDistance && UpDistance < DownDistance && UpDistance < LeftDistance)
				{
					Stapler->CollideWithTopSide = true;//Collision with Dusty's Bottom Side but the Top side of the Stapler.
					Dusty.CollideWithBottomSide = true;
					Dusty.FloatY -= UpDistance;
					if (Dusty.FloatVelocityY > 0)
						Dusty.FloatVelocityY = 0;
				}
				else
				{
					Stapler->CollideWithTopSide = false;
					Stapler->CanLaunch = true;

				}
			}
		}
	}
}
