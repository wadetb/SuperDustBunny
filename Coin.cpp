﻿//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
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
    
    Coin->Transition = 65;
    Coin->Sprite = 1;
    
    Coin->State = COINSTATE_ACTIVE;
}

extern int ScrollY;

void DisplayCoins()
{
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];
        
        if (Coin->Sprite == 1) 
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin01 );
        } 

        if (Coin->Sprite == 2)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin02 );
        } 

        if (Coin->Sprite == 3)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin03 );
        }

        if (Coin->Sprite == 4)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin04 );
        } 

        if (Coin->Sprite == 5)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin05 );
        }    

        if (Coin->Sprite == 6)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin06 );
        } 

        if (Coin->Sprite == 7) 
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin07 );
        } 

        if (Coin->Sprite == 8)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin08 );
        } 

        if (Coin->Sprite == 9)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin09 );
        }

        if (Coin->Sprite == 10)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin010 );
        } 

        if (Coin->Sprite == 11)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin011 );
        }    

        if (Coin->Sprite == 12)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin012 );
        } 
        
        if (Coin->Sprite == 13)
        {
            gxDrawSprite( Coin->X, Coin->Y + ScrollY, &Coin01 );
        } 
    }    
}

void UpdateCoins()
{
    
    for (int i = 0; i < NCoins; i++)
    {
        SCoin* Coin = &Coins[i];
        
        //float XDist = (float)(Dusty.FloatX - Coin->X);
        //float YDist = (float)((Dusty.FloatY-50) - (Coin->Y));
        //float Dist = sqrtf(XDist*XDist + YDist*YDist);
        
        if (Coin->Transition == 65)
        {
            Coin->Sprite = 1;
        }  

        if (Coin->Transition == 60)
        {
            Coin->Sprite = 2;
        }  

        if (Coin->Transition == 55)
        {
            Coin->Sprite = 3;
        } 

        if (Coin->Transition == 50)
        {
            Coin->Sprite = 4;
        }  

        if (Coin->Transition == 45)
        {
            Coin->Sprite = 5;
        }  

        if (Coin->Transition == 40)
        {
            Coin->Sprite = 6;
        }  

        if (Coin->Transition == 35)
        {
            Coin->Sprite = 7;
        }  

        if (Coin->Transition == 30)
        {
            Coin->Sprite = 8;
        }  

        if (Coin->Transition == 25)
        {
            Coin->Sprite = 9;
        }            

        if (Coin->Transition == 20)
        {
            Coin->Sprite = 10;
        }  

        if (Coin->Transition == 15)
        {
            Coin->Sprite = 11;
        }  

        if (Coin->Transition == 10)
        {
            Coin->Sprite = 12;
        }  
        
        if (Coin->Transition == 5)
        {
            Coin->Sprite = 13;
        } 

        if (Coin->Transition == 0)
        {
            Coin->Transition = 65;
            Coin->Sprite = 1;
        }  
        
        Coin->Transition -= 1;
                 
        //if (Dist < 50)
        //{
        //    //Coin gains bonus points or whatever power_ups associated with coins
        //    Coin->State = COINSTATE_INACTIVE;
        //    return;
        //}      
        //else if (Coin->State == COINSTATE_INACTIVE)
        //{
        //    //If(something something)
        //    Coin->State = COINSTATE_ACTIVE;
        //    return;
        
    }  
}