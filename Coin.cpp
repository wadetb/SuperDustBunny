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
//#include "Barrel.h"
//#include "Dusty.h"

//#define MAX_COINS 100

//SCoin Coin;
//int NCoins = 0;
//SCoin Coins[MAX_COINS];

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

SCoin Coin;

void InitCoin()
{
    Coin.State = COINSTATE_ACTIVE;

    Coin.X = 300;
    Coin.Y = 300;

    Coin.Transition = 120;
    Coin.Sprite = 1;

    bool IsCoinSpinning = true;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  State switching function declarations                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetCoinState_Active();
void SetCoinState_InActive();


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  SETCOINSTATE_ACTIVE IMPLEMENTATION                                                     //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetCoinState_Active()
{
    Coin.Transition = 120;
    Coin.State = COINSTATE_ACTIVE;
}

void DisplayCoin_Active()
{

    if (Coin.Sprite == 1) 
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin01 );
    } 

    if (Coin.Sprite == 2)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin02 );
    } 

    if (Coin.Sprite == 3)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin03 );
    }

    if (Coin.Sprite == 4)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin04 );
    } 

    if (Coin.Sprite == 5)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin05 );
    }    

    if (Coin.Sprite == 6)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin06 );
    } 

    if (Coin.Sprite == 7) 
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin07 );
    } 

    if (Coin.Sprite == 8)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin08 );
    } 

    if (Coin.Sprite == 9)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin09 );
    }

    if (Coin.Sprite == 10)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin010 );
    } 
    if (Coin.Sprite == 11)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin011 );
    }    
    if (Coin.Sprite == 12)
    {
        gxDrawSprite( Coin.X-112, (Coin.Y - 221) + ScrollY, &Coin012 );
    }  

}

void UpdateCoin_Active()
{
    if (Coin.Transition == 120)
    {
        Coin.Sprite = 1;
    }  

    if (Coin.Transition == 110)
    {
        Coin.Sprite = 2;
    }  

    if (Coin.Transition == 100)
    {
        Coin.Sprite = 3;
    } 

    if (Coin.Transition == 90)
    {
        Coin.Sprite = 4;
    }  

    if (Coin.Transition == 80)
    {
        Coin.Sprite = 5;
    }  

    if (Coin.Transition == 70)
    {
        Coin.Sprite = 6;
    }  

    if (Coin.Transition == 60)
    {
        Coin.Sprite = 7;
    }  

    if (Coin.Transition == 50)
    {
        Coin.Sprite = 8;
    }  

    if (Coin.Transition == 40)
    {
        Coin.Sprite = 9;
    }            

    if (Coin.Transition == 30)
    {
        Coin.Sprite = 10;
    }  

    if (Coin.Transition == 20)
    {
        Coin.Sprite = 11;
    }  

    if (Coin.Transition == 10)
    {
        Coin.Sprite = 12;
    }  

    if (Coin.Transition == 0)
    {
        Coin.Transition = 120;
        Coin.Sprite = 1;
    } 
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                 SETCOINSTATE_INACTIVE IMPLEMENTATION                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetCoinState_InActive()
{
    Coin.Transition = 120;
    Coin.State = COINSTATE_INACTIVE;
}

void DisplayCoin_InActive()
{

}

void UpdateCoin_InActive()
{

}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                ADDITIONAL FUNCTIONS                                                                     //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void DisplayCoins()
{
    switch (Coin.State)
    {
    case COINSTATE_ACTIVE:			    DisplayCoin_Active(); break;
    case COINSTATE_INACTIVE:		    DisplayCoin_InActive(); break;
    default:						    break;
    }
}

void UpdateCoins()
{
    switch (Coin.State)
    {
    case COINSTATE_ACTIVE:			    UpdateCoin_Active(); break;
    case COINSTATE_INACTIVE:		    UpdateCoin_InActive(); break;
    default:						    break;
    }
}

//void CreateCoin(int X, int Y, const char* Desc)
//{
//    SCoin* Coin = &Coins[NCoins++];
//
//    Coin->X = X + 32;
//    Coin->Y = Y + 32;
//
//    Coin->State = COINSTATE_ACTIVE;
//}


      //if (Coin->State == COINSTATE_ACTIVE)
      //{
      //      if (Dist < 50)
      //      {
      //          //Coin gains bonus points or whatever power_ups associated with coins
      //          Coin->State = COINSTATE_INACTIVE;
      //      }
            
   
             
    /*    else if (Coin->State == COINSTATE_INACTIVE)
        {*/

            //If(something something)
            //{
            //  Coin->State = COINSTATE_ACTIVE;
            //
            //{