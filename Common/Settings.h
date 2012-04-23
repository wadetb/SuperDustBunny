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

typedef enum
{
    CONTROL_TILT,
    CONTROL_SWIPE
} EControlStyle;

typedef struct
{
    EControlStyle ControlStyle;
    
    bool GhostActive;
    
    bool InfiniteLives;
    bool DisableVacuum;
    
    bool LiveAssets;
    char AssetServer[256];
    
    bool ChapterSkip;
    
    bool LeaderboardToolTipTap;
    
#ifdef PLATFORM_IPHONE_OR_MAC
    NSString* LeaderboardName;
#endif
} SSettings;


extern SSettings Settings;


void InitSettings();

void LoadSettings();
void SaveSettings();

#endif
