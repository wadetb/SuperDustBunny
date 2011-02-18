//
//  SettingsViewController.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 2/15/11.
//  Copyright 2011 Self. All rights reserved.
//

#import "SettingsViewController.h"
#import "SuperDustBunnyViewController.h"

#include "Settings.h"

void SetTiltSensitivity(int value);
void SetContinuousJump(bool value);
void SetFallGracePeriod(bool value);
void SetDoubleJump(bool value);
void SetInfiniteLives(bool value);
void SetDisableVacuum(bool value);


@implementation SettingsViewController

@synthesize mainViewController;
@synthesize tiltSensitivity;
@synthesize continuousJump;
@synthesize fallGracePeriod;
@synthesize doubleJump;
@synthesize infiniteLives;
@synthesize disableVacuum;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)dealloc
{
    [tiltSensitivity release];
    [continuousJump release];
    [continuousJump release];
    [fallGracePeriod release];
    [doubleJump release];
    [infiniteLives release];
    [disableVacuum release];
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (IBAction)done:(id)sender {
    [[self mainViewController] hideSettings];
}

- (IBAction)tiltSensitivityChanged:(id)sender {
    UISegmentedControl* control = (UISegmentedControl*)sender;
    Settings.TiltSensitivity = [control selectedSegmentIndex];
}

- (IBAction)continuousJumpChanged:(id)sender {
    UISwitch* control = (UISwitch*)sender;
    SetContinuousJump(control.on);
}

- (IBAction)fallGracePeriodChanged:(id)sender {
    UISwitch* control = (UISwitch*)sender;
    SetFallGracePeriod(control.on);
}

- (IBAction)doubleJumpChanged:(id)sender {
    UISwitch* control = (UISwitch*)sender;
    SetDoubleJump(control.on);
}

- (IBAction)infiniteLivesChanged:(id)sender {
    UISwitch* control = (UISwitch*)sender;
    SetInfiniteLives(control.on);
}

- (IBAction)disableVacuumChanged:(id)sender {
    UISwitch* control = (UISwitch*)sender;
    SetDisableVacuum(control.on);
}

- (void)transferSettingsToView
{
    tiltSensitivity.selectedSegmentIndex = Settings.TiltSensitivity;
    continuousJump.on = Settings.ContinuousJump;
    fallGracePeriod.on = Settings.FallGracePeriod;
    doubleJump.on = Settings.DoubleJump;
    infiniteLives.on = Settings.InfiniteLives;
    disableVacuum.on = Settings.DisableVacuum;
}

- (void)transferSettingsFromView
{
    Settings.TiltSensitivity = tiltSensitivity.selectedSegmentIndex;
    Settings.ContinuousJump = continuousJump.on;
    Settings.FallGracePeriod = fallGracePeriod.on;
    Settings.DoubleJump = doubleJump.on;
    Settings.InfiniteLives = infiniteLives.on;
    Settings.DisableVacuum = disableVacuum.on;
}

@end
