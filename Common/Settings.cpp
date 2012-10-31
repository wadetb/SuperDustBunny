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
    Settings.Lives = 10;
    Settings.ControlStyle = CONTROL_SWIPE;
    Settings.GhostActive = false;
    Settings.DisableVacuum = false;
    Settings.DeveloperMode = false;
    Settings.LiveAssets = false;
    strcpy(Settings.AssetServer, "http://pluszerogames.com/sdb/live/1/");
    Settings.ChapterSkip = true;
    Settings.LeaderboardToolTipTap = false;
#ifdef PLATFORM_IPHONE_OR_MAC
    Settings.LeaderboardName = @"";
#endif
        
#ifdef PLATFORM_IPHONE_OR_MAC
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary *appDefaults = [NSDictionary
                                 dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithInt: Settings.Lives], @"Lives",
                                 [NSNumber numberWithInt: Settings.ControlStyle], @"ControlStyle",
                                 [NSNumber numberWithBool: Settings.GhostActive], @"GhostActive",
                                 [NSNumber numberWithBool: Settings.DeveloperMode], @"DeveloperMode",
                                 [NSNumber numberWithBool: Settings.DisableVacuum], @"DisableVacuum",
                                 [NSNumber numberWithBool: Settings.LiveAssets], @"LiveAssets",
                                 [NSString stringWithUTF8String:Settings.AssetServer], @"AssetServer",
                                 [NSNumber numberWithBool: Settings.ChapterSkip], @"ChapterSkip",
                                 [NSNumber numberWithBool: Settings.LeaderboardToolTipTap], @"LeaderboardToolTipTap",
                                 Settings.LeaderboardName, @"LeaderboardName",
                                 nil];
    
    [defaults registerDefaults:appDefaults];
#endif
    
    LoadSettings();
}

void LoadSettings()
{
#ifdef PLATFORM_IPHONE_OR_MAC
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    Settings.Lives = (int)[defaults integerForKey:@"Lives"];
    Settings.ControlStyle = (EControlStyle)[defaults integerForKey:@"ControlStyle"];
    Settings.GhostActive = [defaults boolForKey:@"GhostActive"];
    Settings.DeveloperMode = true; //[defaults boolForKey:@"DeveloperMode"];
    Settings.DisableVacuum = [defaults boolForKey:@"DisableVacuum"];
    Settings.LiveAssets = [defaults boolForKey:@"LiveAssets"];
    snprintf(Settings.AssetServer, sizeof(Settings.AssetServer), "%s", [[defaults stringForKey:@"AssetServer"] UTF8String]);
    Settings.ChapterSkip = [defaults boolForKey:@"ChapterSkip"];
    Settings.LeaderboardToolTipTap = [defaults boolForKey:@"LeaderboardToolTipTap"];
    if (Settings.LeaderboardName) 
        [Settings.LeaderboardName release];
    Settings.LeaderboardName = [[defaults stringForKey:@"LeaderboardName"] retain];
#endif
    
#ifdef PLATFORM_IPHONE
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

void SaveSettings()
{
#ifdef PLATFORM_IPHONE_OR_MAC
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:Settings.Lives] forKey:@"Lives"];
    [defaults setObject:[NSNumber numberWithInt:Settings.ControlStyle] forKey:@"ControlStyle"];
    [defaults setObject:[NSNumber numberWithBool:Settings.GhostActive] forKey:@"GhostActive"];
    [defaults setObject:[NSNumber numberWithBool:Settings.DeveloperMode] forKey:@"DeveloperMode"];
    [defaults setObject:[NSNumber numberWithBool:Settings.DisableVacuum] forKey:@"DisableVacuum"];
    [defaults setObject:[NSNumber numberWithBool:Settings.LiveAssets] forKey:@"LiveAssets"];
    [defaults setObject:[NSString stringWithUTF8String:Settings.AssetServer] forKey:@"AssetServer"];
    [defaults setObject:[NSNumber numberWithBool:Settings.ChapterSkip] forKey:@"ChapterSkip"];
    [defaults setObject:[NSNumber numberWithBool:Settings.LeaderboardToolTipTap] forKey:@"LeaderboardToolTipTap"];
    [defaults setObject:Settings.LeaderboardName forKey:@"LeaderboardName"];
#endif
    
#ifdef PLATFORM_IPHONE
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

