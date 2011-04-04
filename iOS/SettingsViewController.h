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
    UISegmentedControl *tiltSensitivity;
    UISwitch *continuousJump;
    UISwitch *fallGracePeriod;
    UISwitch *doubleJump;
    UISwitch *infiniteLives;
    UISwitch *disableVacuum;
}

@property (nonatomic, retain) IBOutlet SuperDustBunnyViewController *mainViewController;

@property (nonatomic, retain) IBOutlet UISegmentedControl *controlScheme;
@property (nonatomic, retain) IBOutlet UISegmentedControl *tiltSensitivity;
@property (nonatomic, retain) IBOutlet UISwitch *continuousJump;
@property (nonatomic, retain) IBOutlet UISwitch *fallGracePeriod;
@property (nonatomic, retain) IBOutlet UISwitch *doubleJump;
@property (nonatomic, retain) IBOutlet UISwitch *infiniteLives;
@property (nonatomic, retain) IBOutlet UISwitch *disableVacuum;

- (IBAction)done:(id)sender;
- (IBAction)controlSchemeChanged:(id)sender;
- (IBAction)tiltSensitivityChanged:(id)sender;
- (IBAction)continuousJumpChanged:(id)sender;
- (IBAction)fallGracePeriodChanged:(id)sender;
- (IBAction)doubleJumpChanged:(id)sender;
- (IBAction)infiniteLivesChanged:(id)sender;
- (IBAction)disableVacuumChanged:(id)sender;

- (void)transferSettingsFromView;
- (void)transferSettingsToView;

@end
