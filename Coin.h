//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef COIN_H
#define COIN_H


enum ECoinState
{
    COINSTATE_ACTIVE 				,
    COINSTATE_INACTIVE				,
};

struct SCoin
{

        ECoinState State;

        int X, Y;
        
        int Sprite;
        int Transition;
        int SpinTimer;

        bool IsCoinSpinning;

};

    extern SCoin Coin;
    //void CreateCoin(int X, int Y, const char* Desc);
    void UpdateCoins();
    void DisplayCoins();
    void InitCoin();

#endif