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

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#import "EAGLView.h"
#endif


SSettings Settings;


void InitSettings()
{
#ifdef PLATFORM_WINDOWS
	Settings.ControlStyle = CONTROL_TILT;
#endif
#ifdef PLATFORM_IPHONE
    Settings.ControlStyle = CONTROL_SWIPE;
#endif
    Settings.InfiniteLives = false;
    Settings.DisableVacuum = false;
    Settings.LiveAssets = false;
    strcpy(Settings.AssetServer, "http://pluszerogames.com/sdb/live/1/");
    Settings.ChapterSkip = true;
    
#ifdef PLATFORM_IPHONE
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary *appDefaults = [NSDictionary
                                 dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithInt: Settings.ControlStyle], @"ControlStyle", 
                                 [NSNumber numberWithBool: Settings.InfiniteLives], @"InfiniteLives", 
                                 [NSNumber numberWithBool: Settings.LiveAssets], @"LiveAssets", 
                                 [NSString stringWithUTF8String:Settings.AssetServer], @"AssetServer", 
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
    Settings.InfiniteLives = [defaults boolForKey:@"InfiniteLives"];
    Settings.LiveAssets = [defaults boolForKey:@"LiveAssets"];
    snprintf(Settings.AssetServer, sizeof(Settings.AssetServer), "%s", [[defaults stringForKey:@"AssetServer"] UTF8String]);
    Settings.ChapterSkip = [defaults boolForKey:@"ChapterSkip"];
#endif
}

void SaveSettings()
{
#ifdef PLATFORM_IPHONE
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:Settings.ControlStyle] forKey:@"ControlStyle"];
    [defaults setObject:[NSNumber numberWithBool:Settings.InfiniteLives] forKey:@"InfiniteLives"];
    [defaults setObject:[NSNumber numberWithBool:Settings.LiveAssets] forKey:@"LiveAssets"];
    [defaults setObject:[NSString stringWithUTF8String:Settings.AssetServer] forKey:@"AssetServer"];
    [defaults setObject:[NSNumber numberWithBool:Settings.ChapterSkip] forKey:@"ChapterSkip"];
    
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:(EAGLView*)theViewController.view];	
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0f/30.0f)];
    }
    else
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:nil]; 
    }
#endif
}

