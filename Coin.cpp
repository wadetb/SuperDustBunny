//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Coin.h"
#include "Common.h"
#include "Barrel.h"
#include "Dusty.h"

#define MAX_COINS 100

int NCoins = 0;
int CoinSprite = 1;
int CoinTransition = 120;

SCoin Coins[MAX_COINS];

void CreateCoin(int X, int Y, const char* Desc)
{
    SCoin* Coin = &Coins[NCoins++];

    Coin->X = X + 32;
    Coin->Y = Y + 32;

    Coin->State = COINSTATE_ACTIVE;
}

extern gxSprite Coin01;
extern gxSprite Coin02;
extern gxSprite Coin03;
extern gxSprite Coin04;
extern gxSprite Coin05;
extern gxSprite Coin06;
extern gxSprite Coin07;
extern gxSprite Coin08;
extern gxSprite Coin09;
extern gxSprite Coin010;
extern gxSprite Coin011;
extern gxSprite Coin012;

extern int ScrollY;

void DisplayCoin_Active()
{
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];

        gxDrawSprite(Coin->X, Coin->Y + ScrollY, &Coin01);
        
        if (CoinSprite == 1) 
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin01 );
        } 

        if (CoinSprite == 2)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin02 );
        } 

        if (CoinSprite == 3)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin03 );
        }

        if (CoinSprite == 4)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin04 );
        } 

        if (CoinSprite == 5)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin05 );
        }    

        if (CoinSprite == 6)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin06 );
        } 

        if (CoinSprite == 7) 
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin07 );
        } 

        if (CoinSprite == 8)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin08 );
        } 

        if (CoinSprite == 9)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin09 );
        }

        if (CoinSprite == 10)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin010 );
        } 
        
        if (CoinSprite == 11)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin011 );
        }    
       
        if (CoinSprite == 12)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin012 );
        }  
    }  
}

void UpdateCoin_Active()
{
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];
        
        float XDist = (float)(Dusty.X - Coin->X);
        float YDist = (float)((Dusty.Y-50) - (Coin->Y));
        float Dist = sqrtf(XDist*XDist + YDist*YDist);

        if (Dist < 50)
        {
            //Coin gains bonus points or whatever power_ups associated with coins
            Coin->State = COINSTATE_INACTIVE;
            return;
        }      
        else if (Coin->State == COINSTATE_INACTIVE)
        {
            //If(something something)
            Coin->State = COINSTATE_ACTIVE;
            return;
        }    
    }
    
    if (CoinTransition == 120)
    {
        CoinSprite = 1;
    }  

    if (CoinTransition == 110)
    {
        CoinSprite = 2;
    }  

    if (CoinTransition == 100)
    {
        CoinSprite = 3;
    } 

    if (CoinTransition == 90)
    {
        CoinSprite = 4;
    }  

    if (CoinTransition == 80)
    {
        CoinSprite = 5;
    }  

    if (CoinTransition == 70)
    {
        CoinSprite = 6;
    }  

    if (CoinTransition == 60)
    {
        CoinSprite = 7;
    }  

    if (CoinTransition == 50)
    {
        CoinSprite = 8;
    }  

    if (CoinTransition == 40)
    {
        CoinSprite = 9;
    }            

    if (CoinTransition == 30)
    {
        CoinSprite = 10;
    }  

    if (CoinTransition == 20)
    {
        CoinSprite = 11;
    }  

    if (CoinTransition == 10)
    {
        CoinSprite = 12;
    }  

    if (CoinTransition == 0)
    {
        CoinTransition = 120;
        CoinSprite = 1;
    } 
}