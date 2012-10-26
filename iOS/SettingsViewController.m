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

void ClearLiveAssetCache();

@implementation SettingsViewController

@synthesize developerMode;
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
    [developerMode release];
    [disableVacuum release];
    [liveAssets release];
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
    [developerMode release];
    developerMode = nil;
    [disableVacuum release];
    disableVacuum = nil;
    [assetServer release];
    assetServer = nil;
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
    [theViewController hideSettings];
}

- (IBAction)clearCache:(id)sender {
    ClearLiveAssetCache();
}

- (void)transferSettingsToView {
    developerMode.on = Settings.DeveloperMode;
    disableVacuum.on = Settings.DisableVacuum;
    liveAssets.on = Settings.LiveAssets;
    assetServer.text = [NSString stringWithUTF8String:Settings.AssetServer];
}

- (void)transferSettingsFromView {
#ifdef PLATFORM_IPHONE
    if (developerMode.on != Settings.DeveloperMode)
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Turned Developer Mode %s", developerMode.on ? "on" : "off"]];
#endif
    Settings.DeveloperMode = developerMode.on;

#ifdef PLATFORM_IPHONE
    if (disableVacuum.on != Settings.DisableVacuum)
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Turned Disable Vacuum %s", disableVacuum.on ? "on" : "off"]];
#endif
    Settings.DisableVacuum = disableVacuum.on;

#ifdef PLATFORM_IPHONE
    if (liveAssets.on != Settings.LiveAssets)
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Turned Live Assets %s", liveAssets.on ? "on" : "off"]];
#endif
    Settings.LiveAssets = liveAssets.on;
    snprintf(Settings.AssetServer, sizeof(Settings.AssetServer), "%s", [[assetServer text] UTF8String]);
}

@end
