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

#define MAX_COINS 100

int NCoins = 0;
SCoin Coins[MAX_COINS];

void CreateCoin(int X, int Y, const char* Desc)
{
    SCoin* Coin = &Coins[NCoins++];

    Coin->X = X + 32;
    Coin->Y = Y + 32;
    Coin->FloatVelocityY = 0.0f;
    Coin->FloatGravity = 0.5f;
    
    Coin->Transition = 5;
	Coin->Sprite = 1;
    
    Coin->Collided = false;
}

void ClearCoins()
{
	NCoins = 0;
}

extern int ScrollY;

void DisplayCoins()
{
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];
        
        if (Coin->Sprite == 1) 
        {
            gxDrawSpriteCenteredRotated( Coin->X, Coin->Y + ScrollY, 0, &CoinSpin1Sprite );
        } 
		
        if (Coin->Sprite == 2)
        {
            gxDrawSpriteCenteredRotated( Coin->X, Coin->Y + ScrollY, 0, &CoinSpin2Sprite );
        } 

        if (Coin->Sprite == 3)
        {
            gxDrawSpriteCenteredRotated( Coin->X, Coin->Y + ScrollY, 0, &CoinSpin3Sprite );
        }

        if (Coin->Sprite == 4)
        {
            gxDrawSpriteCenteredRotated( Coin->X, Coin->Y + ScrollY, 0, &CoinSpin4Sprite );
        } 

        if (Coin->Sprite == 5)
        {
            gxDrawSpriteCenteredRotated( Coin->X, Coin->Y + ScrollY, 0, &CoinSpin5Sprite );
        }
		
        if (Coin->Sprite == 6)
        {
            gxDrawSpriteCenteredRotated( Coin->X, Coin->Y + ScrollY, 0, &CoinSpin6Sprite );
        }    
		
    }    
}

void UpdateCoins()
{
    
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];
        
        float XDist = (float)(Dusty.FloatX - Coin->X);
        float YDist = (float)((Dusty.FloatY-50) - (Coin->Y));
        float Dist = sqrtf(XDist*XDist + YDist*YDist);
        
        if (Dist < 50)
        {
            Coin->Collided = true;        
        }      
        
        if (Coin->Collided == true)
        {
            Coin->Y += Coin->FloatVelocityY;
            Coin->FloatVelocityY += Coin->FloatGravity;
            
            if (Tutorial.CoinDisplayed == false)
            {
                SetGameState_Crumb(TUTORIALSTATE_COIN);
                return;
            }
            
            //sxPlaySound(&Clang01);  
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