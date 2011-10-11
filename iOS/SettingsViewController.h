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
    
    UISegmentedControl *controlScheme;
    UISwitch *infiniteLives;
    UISwitch *disableVacuum;
    UISwitch *liveAssets;
    UITextField *assetServer;
}

@property (nonatomic, retain) IBOutlet UISegmentedControl *controlScheme;
@property (nonatomic, retain) IBOutlet UISwitch *infiniteLives;
@property (nonatomic, retain) IBOutlet UISwitch *disableVacuum;
@property (nonatomic, retain) IBOutlet UISwitch *liveAssets;
@property (nonatomic, retain) IBOutlet UITextField *assetServer;

- (IBAction)done:(id)sender;
- (IBAction)clearCache:(id)sender;

- (void)transferSettingsFromView;
- (void)transferSettingsToView;

@end
