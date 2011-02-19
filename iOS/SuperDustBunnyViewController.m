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

extern int gxScreenWidth;
extern int gxScreenHeight;

SuperDustBunnyViewController *theViewController;


@interface SuperDustBunnyViewController ()
@property (nonatomic, retain) EAGLContext *context;
@end

@implementation SuperDustBunnyViewController

@synthesize context;
@synthesize settingsViewController;
@synthesize paused;

- (void)awakeFromNib
{
    theViewController = self;
    
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!aContext) {
        aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    }
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
    
    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0f/60.0f)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:(EAGLView *)self.view];	
    
    paused = FALSE;
    
    Init();
    
    // Likely cause of white flash:
    [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1 / 60.0f) target:self selector:@selector(mainLoop:) userInfo:nil repeats:NO];
}

- (void)dealloc
{
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
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
    [(EAGLView *)self.view setFramebuffer];
   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	Display();

    [(EAGLView *)self.view presentFramebuffer];
}

- (void) mainLoop:(id)sender
{
	mach_timebase_info_data_t timerInfo;
	mach_timebase_info(&timerInfo);
	
	const double TIMER_RATIO = ((double)timerInfo.numer / (double)timerInfo.denom);
	const double RESOLUTION = 1000000000.0;	
	const double TARGET_TIME = RESOLUTION / 60.0f;
	
	double lastUpdateTime = mach_absolute_time() * TIMER_RATIO - TARGET_TIME;
	
	for (;;)
	{
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		
        double now;
        int result;
        do 
        {
            result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.001, TRUE);
            now = mach_absolute_time() * TIMER_RATIO;
        } while (paused || result == kCFRunLoopRunHandledSource /*|| now - lastUpdateTime < TARGET_TIME*/);
        
        if (result == kCFRunLoopRunStopped)
        {
            NSLog(@"Run loop stopped.\n");
            [pool release];
            break;
        }
            
        if (now - lastUpdateTime >= 5 * TARGET_TIME)
        {
            lastUpdateTime = now;
        }
        
        do 
        {
            Update();
            lastUpdateTime += TARGET_TIME;
        } while ( lastUpdateTime < now );
		
		[self drawFrame];

		[pool release];
	}
}

@end

void ShowSettings()
{
    [theViewController showSettings];
}

