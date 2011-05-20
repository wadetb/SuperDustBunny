//
//  SettingsViewController.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 2/15/11.
//  Copyright 2011 Self. All rights reserved.
//

#import "SettingsViewController.h"
#import "SuperDustBunnyViewController.h"
#import "EAGLView.h"

#include "../Common/Settings.h"

@implementation SettingsViewController

@synthesize mainViewController;
@synthesize controlScheme;
@synthesize tiltSensitivity;
@synthesize continuousJump;
@synthesize fallGracePeriod;
@synthesize doubleJump;
@synthesize infiniteLives;
@synthesize disableVacuum;
@synthesize liveAssets;

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
    [fallGracePeriod release];
    [doubleJump release];
    [infiniteLives release];
    [disableVacuum release];
    [controlScheme release];
    [liveAssets release];
    [liveAssets release];
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

- (IBAction)controlSchemeChanged:(id)sender {
    tiltSensitivity.enabled = controlScheme.selectedSegmentIndex == 0;
    
    EAGLView *view = (EAGLView*)theViewController.view;
    
    if (controlScheme.selectedSegmentIndex == 0)
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:view];	
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0f/30.0f)];
    }
    else
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:nil]; 
    }
}

- (IBAction)tiltSensitivityChanged:(id)sender {
}

- (IBAction)continuousJumpChanged:(id)sender {
    if (continuousJump.on) {
        doubleJump.on = FALSE;
    }
    doubleJump.enabled = !continuousJump.on;
}

- (IBAction)fallGracePeriodChanged:(id)sender {
}

- (IBAction)doubleJumpChanged:(id)sender {
}

- (IBAction)infiniteLivesChanged:(id)sender {
}

- (IBAction)disableVacuumChanged:(id)sender {
}

- (void)transferSettingsToView {
    controlScheme.selectedSegmentIndex = Settings.ControlStyle == CONTROL_TILT ? 0 : 1;
    tiltSensitivity.selectedSegmentIndex = Settings.TiltSensitivity;
    continuousJump.on = Settings.ContinuousJump;
    fallGracePeriod.on = Settings.FallGracePeriod;
    doubleJump.on = Settings.DoubleJump;
    infiniteLives.on = Settings.InfiniteLives;
    disableVacuum.on = Settings.DisableVacuum;
    liveAssets.on = Settings.LiveAssets;
    
    doubleJump.enabled = !continuousJump.on;
}

- (void)transferSettingsFromView {
    Settings.ControlStyle = controlScheme.selectedSegmentIndex == 0 ? CONTROL_TILT : CONTROL_SWIPE;
    Settings.TiltSensitivity = tiltSensitivity.selectedSegmentIndex;
    Settings.ContinuousJump = continuousJump.on;
    Settings.FallGracePeriod = fallGracePeriod.on;
    Settings.DoubleJump = doubleJump.on;
    Settings.InfiniteLives = infiniteLives.on;
    Settings.DisableVacuum = disableVacuum.on;
    Settings.LiveAssets = liveAssets.on;
}

@end
