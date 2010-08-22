//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Coin.h"
#include "Common.h"
#include "Dusty.h"
#include "Vacuum.h"

#define MAX_COINS 100

int NCoins = 0;
SCoin Coins[MAX_COINS];

void CreateCoin(int X, int Y, const char* Desc)
{
    SCoin* Coin = &Coins[NCoins++];

    Coin->X = (float)X + 32;
    Coin->Y = (float)Y + 32;
    Coin->FloatVelocityY = 0.0f;
    
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
        
        if (Coin->Sprite == 1) 
        {
            gxDrawSpriteCenteredRotated( (int)Coin->X, (int)(Coin->Y + ScrollY), 0, &CoinSpin1Sprite );
        } 
		
        if (Coin->Sprite == 2)
        {
            gxDrawSpriteCenteredRotated( (int)Coin->X, (int)(Coin->Y + ScrollY), 0, &CoinSpin2Sprite );
        } 

        if (Coin->Sprite == 3)
        {
            gxDrawSpriteCenteredRotated( (int)Coin->X, (int)(Coin->Y + ScrollY), 0, &CoinSpin3Sprite );
        }

        if (Coin->Sprite == 4)
        {
            gxDrawSpriteCenteredRotated( (int)Coin->X, (int)(Coin->Y + ScrollY), 0, &CoinSpin4Sprite );
        } 

        if (Coin->Sprite == 5)
        {
            gxDrawSpriteCenteredRotated( (int)Coin->X, (int)(Coin->Y + ScrollY), 0, &CoinSpin5Sprite );
        }
		
        if (Coin->Sprite == 6)
        {
            gxDrawSpriteCenteredRotated( (int)Coin->X, (int)(Coin->Y + ScrollY), 0, &CoinSpin6Sprite );
        }    
		
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
				Coin->State = COINSTATE_FALLING;        

				sxPlaySound( &CoinVacuumedUpSound );  

				if (Tutorial.CoinDisplayed == false)
				{
					SetGameState_Crumb(TUTORIALSTATE_COIN);
					JamVacuum();
					return;
				}
			}      
		}
		else if (Coin->State == COINSTATE_FALLING)
		{
            
            Coin->Y += Coin->FloatVelocityY;
            Coin->FloatVelocityY += 1.0f;
		}
		
		Coin->Transition -= 1;
		if (Coin->Transition == 0)
		{
			Coin->Transition = 5;

			Coin->Sprite += 1;
			if (Coin->Sprite == 7)
			{
				Coin->Sprite = 1;
			}  
		}
    }  
}