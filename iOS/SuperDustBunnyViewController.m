//
  //  SuperDustBunnyViewController.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 1/20/11.
//  Copyright 2011 Self. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <GameKit/GameKit.h>

#import "SuperDustBunnyViewController.h"
#import "SettingsViewController.h"
#import "EAGLView.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

void Init();
void Exit();
void Update();
void Display();
void SaveSettings();
void DownloadLeaderboards();

extern GLuint gxOpenGLESVersion;

SuperDustBunnyViewController *theViewController;

@implementation SuperDustBunnyViewController

@synthesize context;
@synthesize settingsViewController;
@synthesize haveLocation, city, state, country;

BOOL isGameCenterAPIAvailable()
{
    // Check for presence of GKLocalPlayer class.
    BOOL localPlayerClassAvailable = (NSClassFromString(@"GKLocalPlayer")) != nil;
    
    // The device must be running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (localPlayerClassAvailable && osVersionSupported);
}

//- (BOOL) isGameCenterEnabled
//{
//    if ( !isGameCenterAPIAvailable() )
//        return NO;
//    
//    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
//    return [localPlayer isAuthenticated];
//}

- (void) authenticateLocalPlayer
{
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
        NSLog(@"Player authenticated: %s", ([localPlayer isAuthenticated] ? "YES" : "NO"));
    }];
}

- (void)retrieveLocationAndDownloadLeaderboards
{
    if (locationManager == nil)
        locationManager = [[CLLocationManager alloc] init];

    haveLocation = NO;
    city = nil;
    state = nil;
    country = nil;

    CLAuthorizationStatus status = [CLLocationManager authorizationStatus];
    if (status != kCLAuthorizationStatusNotDetermined && status != kCLAuthorizationStatusAuthorized)
    {
        DownloadLeaderboards();
        return;
    }
    
    locationManager.delegate = self;
    locationManager.desiredAccuracy = kCLLocationAccuracyKilometer;
    locationManager.distanceFilter = 500;
    
    [locationManager startUpdatingLocation];
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
    [locationManager stopUpdatingLocation];

    MKReverseGeocoder* geo = [[MKReverseGeocoder alloc] initWithCoordinate:newLocation.coordinate];
    geo.delegate = self;
    [geo start];
}

- (void)reverseGeocoder:(MKReverseGeocoder*)geocoder didFindPlacemark:(MKPlacemark*)place
{
    haveLocation = YES;
    city = place.locality;
    state = place.administrativeArea;
    country = place.country;
    
    NSLog(@"city:%@ state:%@ country:%@\n", city, state, country);
    
    DownloadLeaderboards();
}

- (void)reverseGeocoder:(MKReverseGeocoder*)geocoder didFailWithError:(NSError*)error
{
    haveLocation = NO;
    city = nil;
    state = nil;
    country = nil;

    NSLog(@"Could not retrieve the specified place information.\n");

    DownloadLeaderboards();
}

- (void)awakeFromNib
{
    theViewController = self;
    
    EAGLContext *aContext = NULL;
    
    aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    gxOpenGLESVersion = 2;
    
    if (!aContext) {
        aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        gxOpenGLESVersion = 1;
    }
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
    
    slowFactor = 1;
    
    Init();
    
    displayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(displayLinkFrame)];
    [displayLink setFrameInterval:slowFactor];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];

    /*
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(volumeChanged:) 
                                                 name:@"AVSystemController_SystemVolumeDidChangeNotification" 
                                               object:nil];
    */
    
    spinnerPanel = [[UIView alloc] initWithFrame:CGRectMake(75, 155, 170, 140)];
    spinnerPanel.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.5];
    spinnerPanel.clipsToBounds = YES;
    spinnerPanel.layer.cornerRadius = 10.0;
    
    UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    [spinner setCenter:CGPointMake(85, 45)];
    [spinnerPanel addSubview:spinner]; 
    [spinner startAnimating];
    [spinner release];

    UILabel *spinnerCaption = [[UILabel alloc] initWithFrame:CGRectMake(20, 90, 130, 22)];
    spinnerCaption.backgroundColor = [UIColor clearColor];
    spinnerCaption.textColor = [UIColor whiteColor];
    spinnerCaption.adjustsFontSizeToFitWidth = YES;
    spinnerCaption.textAlignment = UITextAlignmentCenter;
    spinnerCaption.text = @"Syncing Live Assets";
    [spinnerPanel addSubview:spinnerCaption]; 
    [spinnerCaption release];
    
    [self.view addSubview:spinnerPanel];
    
    spinnerPanel.hidden = YES;

    paused = FALSE;
    wasPaused = TRUE;
    
    locationManager = nil;
    haveLocation = NO;
    city = nil;
    state = nil;
    country = nil;
    
    [self drawFrame]; 
}

- (void)dealloc
{
    [displayLink invalidate];

    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void) volumeChanged:(NSNotification *)notify
{  
    NSLog(@"Volume changed.\n");
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    printf("viewWillAppear\n");
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    printf("viewWillDisappear\n");
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (void)showSettings
{
    paused = TRUE;
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:1.0];
    [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromRight forView:[self view] cache:YES];
    [[self view] addSubview:[settingsViewController view]];
    [UIView commitAnimations];

    [settingsViewController transferSettingsToView];    
}

- (void)hideSettings
{
    [settingsViewController transferSettingsFromView];

    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:1.0];
    [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromLeft forView:[self view] cache:YES];
    [[settingsViewController view] removeFromSuperview];
    [UIView commitAnimations];

    SaveSettings();
    
    paused = FALSE;
}

- (void)startSpinner
{
    spinnerPanel.hidden = NO;
    [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
}

- (void)stopSpinner
{
    spinnerPanel.hidden = YES;
    [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
}

- (void)drawFrame
{
    if (paused)
        return;
    
    [(EAGLView *)self.view setFramebuffer];
   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	Display();

    [(EAGLView *)self.view presentFramebuffer];
}

- (void)displayLinkFrame
{
    if (paused)
        return;
    
    static bool recursing = false;
    if (recursing)
        return;
    recursing = true;
    
    if (wasPaused)
    {
        lastTimeStamp = displayLink.timestamp;
        wasPaused = FALSE;
    }
    
    CFTimeInterval t = lastTimeStamp;
    int count = 0;
    
    while (t < displayLink.timestamp + displayLink.duration*slowFactor)
    {
        if (count < 3)
            Update();
        count++;
        t += displayLink.duration*slowFactor;
    }
    lastTimeStamp = t;
    
    [self drawFrame];
    recursing = false;
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [self setPaused:FALSE];
    
    if (buttonIndex == 1) {
        [TestFlight openFeedbackView];
    }
}

- (void)setPaused:(BOOL)pause
{
    printf("setPaused:%d\n", pause);
    
    paused = pause;
    displayLink.paused = pause;
    if (pause)
        wasPaused = TRUE;
}

- (BOOL)paused
{
    return paused;
}

@end
