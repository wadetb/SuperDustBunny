//
//  EAGLView.m
//  sdm
//
//  Created by Wade Brainerd on 11/7/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "EAGLView.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

@implementation EAGLView

void Init();
void Exit();
void Update();
void Display();

extern int _msNewX;
extern int _msNewY;
extern int _msNewButton1;

extern float _msNewAccelX;
extern float _msNewAccelY;
extern float _msNewAccelZ;

extern int gxScreenWidth;
extern int gxScreenHeight;

// You must implement this method
+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id) initWithCoder:(NSCoder*)coder
{    
    if ((self = [super initWithCoder:coder]))
	{
		
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context])
		{
            [self release];
            return nil;
        }
		
		// Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
		glGenFramebuffersOES(1, &defaultFramebuffer);
		glGenRenderbuffersOES(1, &colorRenderbuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
        
		[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
		Init();
	
		//displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
		//[displayLink setFrameInterval:1];
		//[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		
		timer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1 / 60.0f) target:self selector:@selector(mainLoop:) userInfo:nil repeats:NO];
    }
	
    return self;
}

- (void) layoutSubviews
{
	// Allocate color buffer backing based on the current layer size
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    }
	
    [self drawView:nil];
}

- (void) drawView:(id)sender
{
    [EAGLContext setCurrentContext:context];
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(0, gxScreenWidth, gxScreenHeight, 0, 0, 100);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	Display();
	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	// This applies a pretty heavy smoothing function to the accelerometer.
	accel[0] = acceleration.x * 0.5f + accel[0] * (1.0f - 0.5f);
	accel[1] = acceleration.y * 0.5f + accel[1] * (1.0f - 0.5f);
	accel[2] = acceleration.z * 0.5f + accel[2] * (1.0f - 0.5f);
	_msNewAccelX = accel[0];
	_msNewAccelY = accel[1];
	_msNewAccelZ = accel[2];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchPoint = [touch locationInView:self];
	_msNewX = touchPoint.y*2;
	_msNewY = backingWidth*2 - touchPoint.x*2;
	_msNewButton1 = 1;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchPoint = [touch locationInView:self];
	_msNewX = touchPoint.y*2;
	_msNewY = backingWidth*2 - touchPoint.x*2;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchPoint = [touch locationInView:self];
	_msNewX = touchPoint.y*2;
	_msNewY = backingWidth*2 - touchPoint.x*2;
	_msNewButton1 = 0;
}

- (void) dealloc
{
	Exit();
	
	// Tear down GL
	if (defaultFramebuffer)
	{
		glDeleteFramebuffersOES(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}
	
	if (colorRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}
	
	// Tear down context
	if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	
	[context release];
	context = nil;
	
    [super dealloc];
}

// Runs the main loop. This will keep going as long as running is YES.
// Fires as fast as it possibly can, giving a delta that is based upon the
// fraction of the target FPS we're getting.
- (void) mainLoop:(id)sender
{
	//Get the resolution of the iPhone timer.
	mach_timebase_info_data_t timerInfo;
	mach_timebase_info(&timerInfo);
	
	//Store the ratio between the numberator and the denominator.
	const float TIMER_RATIO = ((float)timerInfo.numer / (float)timerInfo.denom);
	
	//The resolution of the iPhone is in nanoseconds.
	const uint64_t RESOLUTION = 1000000000;
	
	//Create a target time variable based upon the iPhone timer resolution and our FPS.
	const float TARGET_TIME = (float)RESOLUTION / 60.0f;
	
	//Start the frame average at the target time for a frame.
	float frameAverage = TARGET_TIME;
	
	//Create an artificial last update time that matches our target delta.
	float lastUpdateTime = mach_absolute_time() * TIMER_RATIO - TARGET_TIME;
	
	//It will act this many times before it draws.
	const unsigned int DRAW_FREQUENCY = 1;
	unsigned int timesUpdated = 0;
	
	//Start the game loop.
	bool running = YES;
	while (running)
	{
		//Create the autorelease pool.
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		
		//Get the current time and convert it to a useable standard value (ns).
		uint64_t now = mach_absolute_time() * TIMER_RATIO;
		
		//Adjust the frame average based upon how long this last update took us.
		frameAverage = (frameAverage * 10 + (now - lastUpdateTime)) / 11;
		
		//Create a delta out of the value for the current frame average.
		//float delta = frameAverage / TARGET_TIME;
		
		//Set the last delta so we can access it elsewhere.
		//[Globals setLastDelta:delta];
		
		//Refresh the last update time.
		lastUpdateTime = now;
		
		//Yield to system calls (touches, etc.) for one ms.
		while( CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.002, FALSE) == kCFRunLoopRunHandledSource);
		
		//Update the game logic.
		Update();
		
		//Draw the game.
		timesUpdated++;
		if (timesUpdated >= DRAW_FREQUENCY)
		{
			[self drawView:nil];
			timesUpdated = 0;
		}
		
		[pool release];
	}
}

@end
