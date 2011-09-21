//
//  SuperDustBunnyAppDelegate.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 1/20/11.
//  Copyright 2011 Self. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SuperDustBunnyViewController;
@class SettingsViewController;

@interface SuperDustBunnyAppDelegate : NSObject <UIApplicationDelegate> {
    BOOL gameCenterEnabled;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet SuperDustBunnyViewController *viewController;
@property (nonatomic, retain) IBOutlet SettingsViewController *settingsViewController;

@end
