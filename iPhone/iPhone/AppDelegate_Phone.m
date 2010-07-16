//
//  AppDelegate_Phone.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 7/13/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "AppDelegate_Phone.h"

@implementation AppDelegate_Phone

@synthesize window;
@synthesize view;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
	
    // Override point for customization after application launch
	
    [window makeKeyAndVisible];
	
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0f/100.0f)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:view];	

	return YES;
}

- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
