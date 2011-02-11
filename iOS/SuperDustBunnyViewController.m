//
//  SuperDustBunnyViewController.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 1/20/11.
//  Copyright 2011 Self. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "SuperDustBunnyViewController.h"
#import "EAGLView.h"

#include <mach/mach.h>
#include <mach/mach_time.h>


void Init();
void Exit();
void Update();
void Display();

extern int gxScreenWidth;
extern int gxScreenHeight;


@interface SuperDustBunnyViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation SuperDustBunnyViewController

@synthesize animating, context, displayLink;

- (void)awakeFromNib
{
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
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0f/100.0f)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:(EAGLView *)self.view];	
    
    Init();
    
    [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1 / 60.0f) target:self selector:@selector(mainLoop:) userInfo:nil repeats:NO];
}

- (void)dealloc
{
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
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
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    return;
    
    if (!animating) {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    return;
    
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
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
        } while (result == kCFRunLoopRunHandledSource || now - lastUpdateTime < TARGET_TIME);
        
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
