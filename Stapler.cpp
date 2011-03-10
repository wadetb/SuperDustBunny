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

	Stapler->FloatX = 0;
	Stapler->FloatY = 0;

	Stapler->Left = -30;
	Stapler->Right = 128;
	Stapler->Top = -323;
	Stapler->Bottom = 0;
    
    Stapler->PowerJump = 0;

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
            case 10: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump1Sprite, Stapler->X + 50, (Stapler->Y) + ScrollY, 1.0f, 0.0f); break;
            case 20: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump2Sprite, Stapler->X + 50, (Stapler->Y + 10) + ScrollY, 1.0f, 0.0f); break;
            case 30: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump3Sprite, Stapler->X + 50, (Stapler->Y + 20) + ScrollY, 1.0f, 0.0f); break;
            case 40: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump4Sprite, Stapler->X + 50, (Stapler->Y + 30) + ScrollY, 1.0f, 0.0f); break;
            case 50: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump5Sprite, Stapler->X + 50, (Stapler->Y + 40) + ScrollY, 1.0f, 0.0f); break;
            case 60: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump6Sprite, Stapler->X + 50, (Stapler->Y + 50) + ScrollY, 1.0f, 0.0f); break;
            case 70: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump7Sprite, Stapler->X + 50, (Stapler->Y + 60) + ScrollY, 1.0f, 0.0f); break;
            case 80: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump8Sprite, Stapler->X + 50, (Stapler->Y + 70) + ScrollY, 1.0f, 0.0f); break;
            case 90: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump9Sprite, Stapler->X + 50, (Stapler->Y + 80) + ScrollY, 1.0f, 0.0f); break;
            case 100: AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &PowerJump10Sprite, Stapler->X + 50, (Stapler->Y + 90) + ScrollY, 1.0f, 0.0f); break;
            default: continue;
            }
        }
        else if (Stapler->State == STAPLERSTATE_LAUNCH)
        {
            AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &StaplerExtendUpSprite, Stapler->X, Stapler->Y + ScrollY, 1.0f, 0.0f);
        }            
    }
}

void UpdateStaplers()
{
#ifdef PLATFORM_WINDOWS
    for (int i = 0; i < NStaplers; i++)
    {
        SStapler* Stapler = &Staplers[i];
		
		for (int y = 0; y < Chapter.PageHeight; y++)
		{
			for (int x = 0; x < Chapter.PageWidth; x++)
			{
				// Skip empty blocks.
				if (IsBlockEmpty(x, y))
				{
					continue;
				}

				// Determine the bounds of the block.
				float BlockLeft   = (float)x*64;
				float BlockRight  = (float)x*64 + 64;
				float BlockTop    = (float)y*64;
				float BlockBottom = (float)y*64 + 64;

				if (Max(Stapler->FloatY + Stapler->Top, BlockTop) <= Min(Stapler->FloatY + Stapler->Bottom, BlockBottom))
				{
					// Check to see if The Stapler rectangle also overlaps with the block in X.
					if(Max(Stapler->FloatX + Stapler->Left, BlockLeft) <= Min(Stapler->FloatX + Stapler->Right, BlockRight))
					{    
						bool LeftBlockIsEmpty   = IsBlockEmpty(x-1, y);
						bool RightBlockIsEmpty  = IsBlockEmpty(x+1, y);
						bool TopBlockIsEmpty    = IsBlockEmpty(x, y-1);
						bool BottomBlockIsEmpty = IsBlockEmpty(x, y+1);

						// Calculate the distance Stapler would have to be pushed in each possible direction to get him out of intersecting the block.
						float LeftDistance	= (Stapler->FloatX + Stapler->Right)  - (BlockLeft);
						float RightDistance	= (BlockRight) - (Stapler->FloatX +  Stapler->Left);
						float DownDistance	= (BlockBottom)- (Stapler->FloatY +  Stapler->Top );
						float UpDistance	= (Stapler->FloatY + Stapler->Bottom) - (BlockTop );

						bool BlockCollideWithLeftSide = false;
						bool BlockCollideWithRightSide = false;
						bool BlockCollideWithTopSide = false;
						bool BlockCollideWithBottomSide = false;

						int CornerThreshold = 32;

						// Prefer to collide with the side of the block that would push Stapler out the least distance.
						// (Only consider sides that are not adjacent to another solid block).
						if (LeftBlockIsEmpty && LeftDistance < RightDistance && LeftDistance < DownDistance && LeftDistance < UpDistance)
						{
							BlockCollideWithRightSide = true;
							Stapler->CollideWithRightSide = true;//Collision with Stapler's Right Side but the left side of the platform
							Stapler->FloatX -= LeftDistance;

							if (Stapler->FloatVelocityX > 0 && TopBlockIsEmpty && abs(UpDistance - LeftDistance) < CornerThreshold)
							{
 								Stapler->CollideWithBottomRightCorner = true;
								Stapler->FloatY -= UpDistance;
								Stapler->FloatVelocityY = 0;
							}

							if (Stapler->FloatVelocityX > 0)
								Stapler->FloatVelocityX = 0;
						}
						if (RightBlockIsEmpty && RightDistance < LeftDistance && RightDistance < DownDistance && RightDistance < UpDistance)
						{
							BlockCollideWithLeftSide = true;
							Stapler->CollideWithLeftSide = true;//Collision with Stapler's Left Side but the right side of the platform
							Stapler->FloatX += RightDistance;

							if (Stapler->FloatVelocityX < 0 && TopBlockIsEmpty && abs(UpDistance - RightDistance) < CornerThreshold)
							{
								Stapler->CollideWithBottomLeftCorner = true;
								Stapler->FloatY -= UpDistance;
								Stapler->FloatVelocityY = 0;
							}

							if (Stapler->FloatVelocityX < 0)
								Stapler->FloatVelocityX = 0;
						}
						if (BottomBlockIsEmpty && DownDistance < RightDistance && DownDistance < LeftDistance && DownDistance < UpDistance)
						{
							BlockCollideWithTopSide = true;
							Stapler->CollideWithTopSide = true;//Collision with Stapler's Top Side but the Bottom side of the platform
							Stapler->FloatY += DownDistance;
							if (Stapler->FloatVelocityY < 0)
								Stapler->FloatVelocityY = 0;
						}
						if (TopBlockIsEmpty && UpDistance < RightDistance && UpDistance < DownDistance && UpDistance < LeftDistance)
						{
							BlockCollideWithBottomSide = true;
							Stapler->CollideWithBottomSide = true;//Collision with Stapler's Bottom Side but the Top side of the platform
							Stapler->FloatY -= UpDistance;
							if (Stapler->FloatVelocityY > 0)
							{
								Stapler->FloatVelocityY = 0;
							}
						}
					}
				}

		//Upper Left x = 0, y = 0
		//Upper Right x = 323, y = 0 
		//Lower Right x = 323, y = 128
		//Lower Left x = 0, y = 128
		
		if (Dusty.CollideWithBottomSide && Stapler->CollideWithTopSide)
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
            if (kbIsKeyDown(KB_SPACE))
            {
                if (Stapler->PowerJumpCounter > 100)
                {
                    Stapler->PowerJumpCounter = 100;
                }
                
                Stapler->PowerJumpCounter += 1;    
            }
            
            if (kbWasKeyDown(KB_SPACE))
            {
                switch(Stapler->PowerJumpCounter)
                {
                case 10: Stapler->PowerJump = 0.8f; break;
                case 20: Stapler->PowerJump = 0.9f; break;
                case 30: Stapler->PowerJump = 1.0f; break;
                case 40: Stapler->PowerJump = 1.1f; break;
                case 50: Stapler->PowerJump = 1.2f; break;
                case 60: Stapler->PowerJump = 1.3f; break;
                case 70: Stapler->PowerJump = 1.4f; break;
                case 80: Stapler->PowerJump = 1.5f; break;
                case 90: Stapler->PowerJump = 1.6f; break;
                case 100: Stapler->PowerJump = 1.7f; break;
                default: continue;
                }
                
                Stapler->State = STAPLERSTATE_LAUNCH; 
            }
        
            if (Stapler->State == STAPLERSTATE_LAUNCH)
            {
                Dusty.FloatY = Dusty.FloatY * Stapler->PowerJump;
                
                Stapler->PowerJumpCounter = 0;
                Stapler->PowerJump = 0;
                
                Stapler->State = STAPLERSTATE_WAIT;
            }      
        }             
    }  
#endif
}
        
