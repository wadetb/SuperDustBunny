//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//
#ifndef __SETTINGS_H__
#define __SETTINGS_H__


struct SSettings
{
    float TiltSensitivity;
    bool ContinuousJump;
    bool FallGracePeriod;
    bool DoubleJump;
    
    bool InfiniteLives;
    bool DisableVacuum;
};


extern SSettings Settings;


void InitSettings();

void LoadSettings();
void SaveSettings();


#endif
