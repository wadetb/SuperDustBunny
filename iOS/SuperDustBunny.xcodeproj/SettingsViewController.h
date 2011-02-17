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
    
}

@property (nonatomic, retain) IBOutlet SuperDustBunnyViewController *mainViewController;


- (IBAction)done:(id)sender;
- (IBAction)tiltSensitivityChanged:(id)sender;
- (IBAction)continuousJumpChanged:(id)sender;
- (IBAction)fallGracePeriodChanged:(id)sender;
- (IBAction)doubleJumpChanged:(id)sender;
- (IBAction)infiniteLivesChanged:(id)sender;
- (IBAction)disableVacuumChanged:(id)sender;

@end
