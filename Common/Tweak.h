//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef TWEAK_H
#define TWEAK_H

struct STweak
{
    #define TWEAK_FLOAT(Name, Default, Description) \
        float Name;
        
    #define TWEAK_INT(Name, Default, Description) \
        int Name;

    #include "TweakList.h"
    
    #undef TWEAK_FLOAT
    #undef TWEAK_INT
};

extern STweak Tweak;

void LoadTweaks();

#endif
