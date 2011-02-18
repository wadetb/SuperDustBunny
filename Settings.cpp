//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Settings.h"


SSettings Settings;


void InitSettings()
{
    Settings.TiltSensitivity = 1;
    Settings.ContinuousJump = false;
    Settings.FallGracePeriod = true;
    Settings.DoubleJump = true;
    Settings.InfiniteLives = false;
    Settings.DisableVacuum = false;
}

void LoadSettings()
{
}

void SaveSettings()
{
}


extern "C"
{
void SetTiltSensitivity(int value)
{
    Settings.TiltSensitivity = value;
}

void SetContinuousJump(bool value)
{
    Settings.ContinuousJump = value;
}

void SetFallGracePeriod(bool value)
{
    Settings.FallGracePeriod = value;
}

void SetDoubleJump(bool value)
{
    Settings.DoubleJump = value;
}

void SetInfiniteLives(bool value)
{
    Settings.InfiniteLives = value;
}

void SetDisableVacuum(bool value)
{
    Settings.DisableVacuum = value;
}
}
