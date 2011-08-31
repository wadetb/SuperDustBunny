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
    Settings.ChapterSkip = true;
    
#ifdef PLATFORM_IPHONE
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary *appDefaults = [NSDictionary
                                 dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithInt: Settings.ControlStyle], @"ControlStyle", 
                                 [NSNumber numberWithInt: Settings.TiltSensitivity], @"TiltSensitivity", 
                                 [NSNumber numberWithBool: Settings.ContinuousJump], @"ContinuousJump", 
                                 [NSNumber numberWithBool: Settings.FallGracePeriod], @"FallGracePeriod", 
                                 [NSNumber numberWithBool: Settings.DoubleJump], @"DoubleJump", 
                                 [NSNumber numberWithBool: Settings.InfiniteLives], @"InfiniteLives", 
                                 [NSNumber numberWithBool: Settings.LiveAssets], @"LiveAssets", 
                                 [NSNumber numberWithInt: Settings.LiveAssetSlot], @"LiveAssetSlot", 
                                 [NSNumber numberWithBool: Settings.ChapterSkip], @"ChapterSkip", 
                                 nil];
    
    [defaults registerDefaults:appDefaults];
#endif
    
    LoadSettings();
}

void LoadSettings()
{
#ifdef PLATFORM_IPHONE
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    Settings.ControlStyle = (EControlStyle)[defaults integerForKey:@"ControlStyle"];
    Settings.TiltSensitivity = [defaults integerForKey:@"TiltSensitivity"];
    Settings.ContinuousJump = [defaults boolForKey:@"ContinuousJump"];
    Settings.FallGracePeriod = [defaults boolForKey:@"FallGracePeriod"];
    Settings.DoubleJump = [defaults boolForKey:@"DoubleJump"];
    Settings.InfiniteLives = [defaults boolForKey:@"InfiniteLives"];
    Settings.LiveAssets = [defaults boolForKey:@"LiveAssets"];
    Settings.LiveAssetSlot = [defaults integerForKey:@"LiveAssetSlot"];
    Settings.ChapterSkip = [defaults boolForKey:@"ChapterSkip"];
#endif
}

void SaveSettings()
{
#ifdef PLATFORM_IPHONE
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:Settings.ControlStyle] forKey:@"ControlStyle"];
    [defaults setObject:[NSNumber numberWithInt:Settings.TiltSensitivity] forKey:@"TiltSensitivity"];
    [defaults setObject:[NSNumber numberWithBool:Settings.ContinuousJump] forKey:@"ContinuousJump"];
    [defaults setObject:[NSNumber numberWithBool:Settings.FallGracePeriod] forKey:@"FallGracePeriod"];
    [defaults setObject:[NSNumber numberWithBool:Settings.DoubleJump] forKey:@"DoubleJump"];
    [defaults setObject:[NSNumber numberWithBool:Settings.InfiniteLives] forKey:@"InfiniteLives"];
    [defaults setObject:[NSNumber numberWithBool:Settings.LiveAssets] forKey:@"LiveAssets"];
    [defaults setObject:[NSNumber numberWithInt:Settings.LiveAssetSlot] forKey:@"LiveAssetSlot"];
    [defaults setObject:[NSNumber numberWithBool:Settings.ChapterSkip] forKey:@"ChapterSkip"];
#endif
}

