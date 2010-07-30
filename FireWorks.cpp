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

	sscanf(Desc, "firework dir=%d", &FireWork->Dir);

	FireWork->State = FIREWORKSTATE_WAIT;
}

extern gxSprite FireWork01;
extern gxSprite FireWork01;

extern int ScrollY;

void DisplayFireWorks()
{
	for (int i = 0; i < NFireWorks; i++)
	{
		SFireWork* FireWork = &FireWorks[i];
		
		if (FireWork->State == FIREWORKSTATE_WAIT || FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			gxDrawSpriteCenteredRotated((int)FireWork->X, (int)FireWork->Y + ScrollY, FireWork->Dir * 3.14159f / 180.0f, &FireWork01);
		}

		if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			gxDrawSpriteCenteredRotated( (int)FireWork->X, (int)FireWork->Y + ScrollY, 0, &FireWork02);				
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
				FireWork->Timer = 10;
				FireWork->State = FIREWORKSTATE_LAUNCH;
			}
		}
		
		if (FireWork->State == FIREWORKSTATE_LAUNCH)
		{
			float Angle = (float)((90 - FireWork->Dir)) * 3.14159f/180.0f;
			float Velocity = 20.0f;
			FireWork->X += Velocity*cosf(Angle);
			FireWork->Y += -Velocity*sinf(Angle);
			
			FireWork->Timer--;

			if (FireWork->Timer == 0)
			{
				FireWork->Timer = 10;
				FireWork->State = FIREWORKSTATE_EXPLODE;
			}
           
            for (int y = 0; y < Chapter.Pages[0].Height; y++)
            {
                for (int x = 0; x < Chapter.Pages[0].Width; x++)
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
			
			        SBlock* Block = &Chapter.Blocks[GetBlockID(x, y)];
			        
                    float XDist = (float)(FireWork->X - x*64);
                    float YDist = (float)(FireWork->Y - y*64);
                    float Dist = sqrtf(XDist*XDist + YDist*YDist);

                    if (Dist < 100)
                    { 
                         if (FireWork ->Y && Block->Destructible)
                        {
                              Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x] = SPECIALBLOCKID_BLANK;
                        }
                    }
                }
            }
		}

		if (FireWork->State == FIREWORKSTATE_EXPLODE)
		{
			FireWork->Timer--;
			if (FireWork->Timer == 0)
			{
				FireWork->State = FIREWORKSTATE_DONE;
			}
		}
	}
}
