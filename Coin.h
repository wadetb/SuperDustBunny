//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef COIN_H
#define COIN_H


enum ECoinState
{
    COINSTATE_ACTIVE,
	COINSTATE_COLLECTED,
    COINSTATE_INACTIVE,
};

struct SCoin
{
    ECoinState State;

    float X, Y;
    float FloatVelocityY;
    float FloatVelocityX;
    
    int Sprite;
    int Transition;
};

extern SCoin Coin;
extern SCoin Coins[];
extern int NCoins;

void CreateCoin(int X, int Y);
void ClearCoins();

void UpdateCoins();
void DisplayCoins();

#endif