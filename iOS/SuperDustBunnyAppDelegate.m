//
//  SuperDustBunnyAppDelegate.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 1/20/11.
//  Copyright 2011 Self. All rights reserved.
//

#import "SuperDustBunnyAppDelegate.h"

#import "EAGLView.h"

#import "SuperDustBunnyViewController.h"

@implementation SuperDustBunnyAppDelegate


@synthesize window;

@synthesize viewController, settingsViewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.window.rootViewController = self.viewController;
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    //[self.viewController stopAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Save data if appropriate.
    //[self.viewController stopAnimation];
}

- (void)dealloc {

    [window release];
    [viewController release];
    [super dealloc];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    //[self.viewController startAnimation];
}

@end
