//
  //  SuperDustBunnyViewController.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 1/20/11.
//  Copyright 2011 Self. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "SuperDustBunnyViewController.h"
#import "SettingsViewController.h"
#import "EAGLView.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

void Init();
void Exit();
void Update();
void Display();

extern GLuint gxOpenGLESVersion;

SuperDustBunnyViewController *theViewController;


@implementation SuperDustBunnyViewController

@synthesize context;
@synthesize settingsViewController;

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
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(volumeChanged:) 
                                                 name:@"AVSystemController_SystemVolumeDidChangeNotification" 
                                               object:nil];
    
    paused = FALSE;
    wasPaused = TRUE;
    
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

    paused = FALSE;
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
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [self setPaused:FALSE];
    
    if (buttonIndex == 1) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://pluszerogames.com/sdb/feedback.html"]];
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

void ShowSettings()
{
    [theViewController showSettings];
}

