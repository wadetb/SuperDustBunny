//
//  SDBOpenGLView.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 9/7/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import "SDBOpenGLView.h"

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

extern GLuint _gxDefaultFrameBuffer;
extern GLuint _gxDefaultFrameBufferWidth;
extern GLuint _gxDefaultFrameBufferHeight;

void GetInput_BeginSwipe(float X, float Y, double Time);
void GetInput_AddToSwipe(float X, float Y, double Time);
void GetInput_EndSwipe(float X, float Y, double Time);

@implementation SDBOpenGLView

- (id)initWithFrame:(NSRect)frameRect
{
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
//        NSOpenGLPFAOpenGLProfile,
//        NSOpenGLProfileVersion3_2Core,
        0
    };
    
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pixelFormat)
        NSLog(@"Failed to create OpenGL pixel format");
        
    started = NO;
    currentTouch = nil;

    return [super initWithFrame:frameRect pixelFormat:pixelFormat];
}

- (void)dealloc
{
    Exit();
    
    //CVDisplayLinkRelease(displayLink);
}

static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, 
                                    CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    [(SDBOpenGLView*)displayLinkContext displayFrameForDisplayLink:outputTime];
    return kCVReturnSuccess;    
}

- (void)prepareOpenGL
{
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
//    
//    CVDisplayLinkCreateWithCGDisplay(CGMainDisplayID(), &displayLink);
//    
//    CVDisplayLinkSetOutputCallback(displayLink, DisplayLinkCallback, self);
//
//    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
//    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
//    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    //CVDisplayLinkStart(displayLink);
    
    [super prepareOpenGL];
}

- (void)reshape
{
}

- (void)startup
{    
    Init();
    
    timer = [NSTimer scheduledTimerWithTimeInterval:0.0001 target:self selector:@selector(displayFrameForTimer) userInfo:nil repeats:YES];
    started = YES;

    [self setAcceptsTouchEvents:YES];
}

- (void)displayFrameForTimer
{
    [[self openGLContext] makeCurrentContext];
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (started)
    {
        Update();
        Display();
    }
    
    [[self openGLContext] flushBuffer];
}

- (void)displayFrameForDisplayLink:(const CVTimeStamp*)outputTime
{
    [self displayFrameForTimer];
}

- (void)drawRect:(NSRect)dirtyRect
{
    [self displayFrameForTimer];
}

- (void)touchesBeganWithEvent:(NSEvent *)event
{
    NSSet* touches = [event touchesMatchingPhase:NSTouchPhaseTouching inView:nil];    
    if (currentTouch)
        return;
    
	currentTouch = [[touches anyObject] retain];
    
    if (currentTouch) {
        CGPoint touchPoint = currentTouch.normalizedPosition;
        _msNewX = touchPoint.x * 768;
        _msNewY = (1.0f-touchPoint.y) * 1024;
        _msNewButton1 = 1;
        
        GetInput_BeginSwipe(_msNewX, _msNewY, event.timestamp);
    }
}

- (void)touchesMovedWithEvent:(NSEvent *)event
{
    NSSet* touches = [event touchesMatchingPhase:NSTouchPhaseTouching inView:nil];  
    NSArray* array = [touches allObjects];    
    for (int i = 0; i < array.count; i++)
    {
        NSTouch *touch = [array objectAtIndex:i];
        if ([touch.identity isEqual:currentTouch.identity])
        {
            [currentTouch release];
            currentTouch = [touch retain];
            break;
        }
    }
    
    if (currentTouch) {
        CGPoint touchPoint = currentTouch.normalizedPosition;
        _msNewX = touchPoint.x * 768;
        _msNewY = (1.0f-touchPoint.y) * 1024;
        
        GetInput_AddToSwipe(_msNewX, _msNewY, event.timestamp);
    }
}

- (void)touchesEndedWithEvent:(NSEvent *)event
{
    NSSet* touches = [event touchesMatchingPhase:NSTouchPhaseTouching inView:nil];  
    NSArray* array = [touches allObjects];    
    for (int i = 0; i < array.count; i++)
    {
        NSTouch *touch = [array objectAtIndex:i];
        if ([touch.identity isEqual:currentTouch.identity])
        {
            [currentTouch release];
            currentTouch = [touch retain];
            break;
        }
    }
    
    if (currentTouch) {
        CGPoint touchPoint = currentTouch.normalizedPosition;
        _msNewX = touchPoint.x * 768;
        _msNewY = (1.0f-touchPoint.y) * 1024;
        _msNewButton1 = 0;
        
        GetInput_EndSwipe(_msNewX, _msNewY, event.timestamp);
        
        [currentTouch release];
        currentTouch = nil;
    }
}

- (void)touchesCancelledWithEvent:(NSEvent *)event
{
    NSSet* touches = [event touchesMatchingPhase:NSTouchPhaseTouching inView:nil];  
    NSArray* array = [touches allObjects];    
    for (int i = 0; i < array.count; i++)
    {
        NSTouch *touch = [array objectAtIndex:i];
        if ([touch.identity isEqual:currentTouch.identity])
        {
            [currentTouch release];
            currentTouch = [touch retain];
            break;
        }
    }
    
    if (currentTouch) {
        CGPoint touchPoint = currentTouch.normalizedPosition;
        _msNewX = touchPoint.x * 768;
        _msNewY = (1.0f-touchPoint.y) * 1024;
        _msNewButton1 = 0;
        
        GetInput_EndSwipe(_msNewX, _msNewY, event.timestamp);
        
        [currentTouch release];
        currentTouch = nil;
    }
}

@end
