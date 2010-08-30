//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "FireWorks.h"
#include "Dusty.h"
#include "chapter.h"

#define MAX_FIREWORKS 100

int NFireWorks = 0;
SFireWork FireWorks[MAX_FIREWORKS];

void CreateFireWork(int X, int Y, const char* Desc)
{
	SFireWork* FireWork = &FireWorks[NFireWorks++];

	FireWork->X = (float)X + 32;
	FireWork->Y = (float)Y + 32;

	FireWork->Dir = 0;
	FireWork->FlightTimer = 15;
	FireWork->FuseTimer = 0;

	sscanf(Desc, "firework dir=%d flight=%d fuse=%d", &FireWork->Dir, &FireWork->FlightTimer, &FireWork->FuseTimer);

	FireWork->State = FIREWORKSTATE_WAIT;
}

void ClearFireWorks()
{
	NFireWorks = 0;
}

extern int ScrollY;

void DisplayFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];
		
		if (FireWork->State == FIREWORKSTATE_WAIT || FireWork->State == FIREWORKSTATE_FUSE || FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			gxDrawSpriteCenteredRotated((int)FireWork->X, (int)FireWork->Y + ScrollY, FireWork->Dir * 3.14159f / 180.0f, &FireWorkRocketSprite);
		}
		else if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			gxDrawSpriteCenteredRotated((int)FireWork->X, (int)FireWork->Y + ScrollY, 0, &FireWorkBangSprite);				
		}
	}
}

void UpdateFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];

		if (FireWork->State == FIREWORKSTATE_WAIT)
		{
			float XDist = (float)(Dusty.FloatX - FireWork->X);
			float YDist = (float)((Dusty.FloatY-50) - (FireWork->Y));
			float Dist = sqrtf(XDist*XDist + YDist*YDist);

			if (Dist < 70)
			{
			    if (Tutorial.FireworkDisplayed == false)
			    {
			        SetGameState_Crumb(TUTORIALSTATE_FIREWORK);
			        return;
			    }
		        		        
				FireWork->State = FIREWORKSTATE_FUSE;
			}
		}
		else if (FireWork->State == FIREWORKSTATE_FUSE)
		{
			FireWork->FuseTimer--;
			if (FireWork->FuseTimer <= 0)
			{
				FireWork->State = FIREWORKSTATE_LAUNCH;
			}
		}
		else if (FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			float Angle = (float)((90 - FireWork->Dir)) * 3.14159f/180.0f;
			float Velocity = 1.5f;

			FireWork->VelocityX += Velocity*cosf(Angle);
			FireWork->VelocityY += -Velocity*sinf(Angle);
			
			FireWork->X += FireWork->VelocityX;
			FireWork->Y += FireWork->VelocityY;

			FireWork->FlightTimer--;

			if (FireWork->FlightTimer <= 0)
			{
				FireWork->FlightTimer = 10;
				FireWork->State = FIREWORKSTATE_EXPLODE;

				for (int y = 0; y < Chapter.StitchedHeight; y++)
				{
					for (int x = 0; x < Chapter.StitchedWidth; x++)
					{
						int BlockID = GetBlockID(x, y);
						if (BlockID < SPECIALBLOCKID_FIRST)
						{
							SBlock* Block = &Chapter.Blocks[BlockID];						
							if (Block->Destructible)
							{
								float XDist = (float)(FireWork->X - (x*64+32));
								float YDist = (float)(FireWork->Y - (y*64+32));
								float Dist = sqrtf(XDist*XDist + YDist*YDist);

								if (Dist < 100)
								{ 
									sxPlaySound( &BlockBreakSound );
									Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
								}
							}
						}
					}
				}
            }
		}

		if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			FireWork->FlightTimer--;
			if (FireWork->FlightTimer == 0)
			{
				FireWork->State = FIREWORKSTATE_DONE;
			}
		}
	}
}
