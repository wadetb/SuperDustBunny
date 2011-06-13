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
#ifdef PLATFORM_WINDOWS
	Settings.ControlStyle = CONTROL_TILT;
#endif
#ifdef PLATFORM_IPHONE
    Settings.ControlStyle = CONTROL_SWIPE;
#endif
    Settings.TiltSensitivity = 1;
    Settings.ContinuousJump = false;
    Settings.FallGracePeriod = true;
    Settings.DoubleJump = false;
    Settings.InfiniteLives = false;
    Settings.DisableVacuum = false;
    Settings.LiveAssets = false;
    Settings.LiveAssetSlot = 1;
}

void LoadSettings()
{
}

void SaveSettings()
{
}

