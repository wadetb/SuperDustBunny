//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         //
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef HAT_H
#define HAT_H


enum EDustySprite
{
    DUSTYSPRITE_HOP_1,
    DUSTYSPRITE_HOP_2,
    DUSTYSPRITE_HOP_3,
    DUSTYSPRITE_HOP_4,
    DUSTYSPRITE_HOP_5,
    
    DUSTYSPRITE_HOP_2B,
    DUSTYSPRITE_HOP_2C,
    
    DUSTYSPRITE_HOP_3B,
    
    DUSTYSPRITE_HOP_4B,
    DUSTYSPRITE_HOP_4C,
    
    DUSTYSPRITE_HOP_5B,
    
    DUSTYSPRITE_IDLE_1,
    DUSTYSPRITE_IDLE_2,
    DUSTYSPRITE_IDLE_3,
    
    DUSTYSPRITE_SLIDE_1,
    DUSTYSPRITE_SLIDE_2,
    DUSTYSPRITE_SLIDE_3,
    
    DUSTYSPRITE_WALLJUMP,
    DUSTYSPRITE_WALLJUMP_B,
    DUSTYSPRITE_WALLJUMP_C,
    
    DUSTYSPRITE_CORNERJUMP,
    DUSTYSPRITE_CORNERJUMP_B,
    DUSTYSPRITE_CORNERJUMP_C,
    DUSTYSPRITE_CORNERJUMP_D,
    
    DUSTYSPRITE_DEATH,
    
    DUSTYSPRITE_KICK,
    
    DUSTYSPRITE_COUNT
};

enum EDustyHat
{
    DUSTYHAT_NONE,
    
    DUSTYHAT_APPLE,
    DUSTYHAT_BASEBALL_CAP,
    DUSTYHAT_BEE,
    DUSTYHAT_BOWTIE,
    DUSTYHAT_CLOWN_NOSE,
    DUSTYHAT_CROWN,
    DUSTYHAT_DISGUISE,
    DUSTYHAT_EARMUFFS,
    DUSTYHAT_EARPHONES,
    DUSTYHAT_EYEGLASSES,
    DUSTYHAT_EYEPATCH,
    DUSTYHAT_FLOWER,
    DUSTYHAT_FROG_CROWN,
    DUSTYHAT_GRADUATION,
    DUSTYHAT_GREEN_FEATHER,
    DUSTYHAT_JESTER,
    DUSTYHAT_KARATE,
    DUSTYHAT_MONOCLE,
    DUSTYHAT_NURSE,
    DUSTYHAT_PARTY,
    DUSTYHAT_PINK_BOW,
    DUSTYHAT_PINK_SHADES,
    DUSTYHAT_PINK_TIARA,
    DUSTYHAT_PIRATE,
    DUSTYHAT_PURPLE_FEATHER,
    DUSTYHAT_SNORKEL,
    DUSTYHAT_SUNGLASSES,
    DUSTYHAT_TOPHAT,
    DUSTYHAT_TUTU,
    DUSTYHAT_WITCH,
    DUSTYHAT_YELLOW_TOPHAT,
    
    DUSTYHAT_COUNT
};


struct SDustyHatOffset
{
    float Angle;
    float X;
    float Y;
    float Scale;
};


extern SDustyHatOffset DustyHatOffsets[DUSTYSPRITE_COUNT];
extern SDustyHatOffset DustyHatAdjustments[DUSTYHAT_COUNT];
extern gxSprite* DustyHatSprites[DUSTYHAT_COUNT];

#endif
