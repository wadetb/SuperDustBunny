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
@synthesize infiniteLives;
@synthesize disableVacuum;
@synthesize liveAssets;
@synthesize assetServer;

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
    [controlScheme release];
    [infiniteLives release];
    [disableVacuum release];
    [liveAssets release];
    [assetServer release];
    [assetServer release];
    [assetServer release];
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
    [assetServer release];
    assetServer = nil;
    [self setAssetServer:nil];
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

- (void)transferSettingsToView {
    controlScheme.selectedSegmentIndex = Settings.ControlStyle == CONTROL_TILT ? 0 : 1;
    infiniteLives.on = Settings.InfiniteLives;
    disableVacuum.on = Settings.DisableVacuum;
    liveAssets.on = Settings.LiveAssets;
    assetServer.text = [NSString stringWithUTF8String:Settings.AssetServer];
}

- (void)transferSettingsFromView {
    Settings.ControlStyle = controlScheme.selectedSegmentIndex == 0 ? CONTROL_TILT : CONTROL_SWIPE;
    Settings.InfiniteLives = infiniteLives.on;
    Settings.DisableVacuum = disableVacuum.on;
    Settings.LiveAssets = liveAssets.on;
    snprintf(Settings.AssetServer, sizeof(Settings.AssetServer), "%s", [[assetServer text] UTF8String]);
}

@end
