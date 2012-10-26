//
//  SettingsViewController.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 2/15/11.
//  Copyright 2011 Self. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SuperDustBunnyViewController;


@interface SettingsViewController : UIViewController {
@private
    
    UISwitch *developerMode;
    UISwitch *disableVacuum;
    UISwitch *liveAssets;
    UITextField *assetServer;
}

@property (nonatomic, retain) IBOutlet UISwitch *developerMode;
@property (nonatomic, retain) IBOutlet UISwitch *disableVacuum;
@property (nonatomic, retain) IBOutlet UISwitch *liveAssets;
@property (nonatomic, retain) IBOutlet UITextField *assetServer;

- (IBAction)done:(id)sender;
- (IBAction)clearCache:(id)sender;

- (void)transferSettingsFromView;
- (void)transferSettingsToView;

@end
