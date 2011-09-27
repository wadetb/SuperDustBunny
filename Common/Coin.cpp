//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Coin.h"
#include "Dusty.h"
#include "Vacuum.h"
#include "Chapter.h"
#include "GameScore.h"
#include "Smoke.h"
#include "Recorder.h"


#define MAX_COINS 100


int NCoins = 0;
SCoin Coins[MAX_COINS];


void CreateCoin(int X, int Y)
{
	if (NCoins >= MAX_COINS)
		ReportError("Exceeded the maximum of %d total coins.", MAX_COINS);

	SCoin* Coin = &Coins[NCoins++];

	Coin->State = COINSTATE_ACTIVE;

    Coin->X = (float)X + 32;
    Coin->Y = (float)Y + 32;
    Coin->FloatVelocityY = 0.0f;
    Coin->FloatVelocityX = 0.0f;
    
    Coin->Transition = 5;
	Coin->Sprite = 1;
}

void ClearCoins()
{
	NCoins = 0;
}

void DisplayCoins()
{
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];
        
		if (Coin->State == COINSTATE_INACTIVE)
			continue;

		gxSprite* CoinSprites[6] =
		{
			&CoinSpin1Sprite,
			&CoinSpin2Sprite,
			&CoinSpin3Sprite,
			&CoinSpin4Sprite,
			&CoinSpin5Sprite,
			&CoinSpin6Sprite,
		};

		AddLitSpriteCenteredScaledRotated( LIGHTLIST_VACUUM, CoinSprites[Coin->Sprite], Coin->X + ScrollX, Coin->Y + ScrollY, 1.0f, 0.0f );
    }
}

void UpdateCoins()
{
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];
        
		if (Coin->State == COINSTATE_ACTIVE)
		{
            float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, Coin->X, Coin->Y);
		
			if (Dist < 100)
			{
				Coin->State = COINSTATE_COLLECTED;
				
                Create1UpBubble(Coin->X, Coin->Y);
                UpdateMinimap(MINIMAP_EXTRA_LIFE);

				sxPlaySound(&CoinVacuumedUpSound);  
			} 	                       	    
		}
		else if (Coin->State == COINSTATE_COLLECTED)
		{
			if (Coin->X < 400)
			{
				Coin->Y += Coin->FloatVelocityY;
				Coin->FloatVelocityY -= 1.0f;
				Coin->X += -Coin->FloatVelocityX;
				Coin->FloatVelocityX -= 1.0f;
			}
			else if (Coin->X >= 400 && Coin->X < 500)
			{
				Coin->Y += Coin->FloatVelocityY;
				Coin->FloatVelocityY -= 1.5f;
				Coin->X += -Coin->FloatVelocityX;
				Coin->FloatVelocityX -= 1.3f;
			}
			else if (Coin->X >= 501)
			{
				Coin->Y += Coin->FloatVelocityY;
				Coin->FloatVelocityY -= 2.0f;
				Coin->X += -Coin->FloatVelocityX;
				Coin->FloatVelocityX -= 0.2f;
			}

			if (Coin->Y + ScrollY < 0)
			{
				Dusty.GainLife = true;

				Coin->State = COINSTATE_INACTIVE;
			}       
		}
				
		Coin->Transition -= 1;
		if (Coin->Transition == 0)
		{
			Coin->Transition = 5;

			Coin->Sprite += 1;
			if (Coin->Sprite == 6)
			{
				Coin->Sprite = 0;
			}  
		}
    }  
}
