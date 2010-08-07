//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef CRUMB_H
#define CRUMB_H

enum ECrumbState
{
    CRUMBSTATE_SPEAK,
    CRUMBSTATE_WAIT,
};

struct SCrumb
{
    float X, Y;

    ECrumbState State;

    int Timer;
};

#endif