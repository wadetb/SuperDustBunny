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

@implementation SDBOpenGLView

- (void)awakeFromNib
{
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
        0
    };
    
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pf)
        NSLog(@"No OpenGL pixel format");

    [self setPixelFormat:pf];
    
    started = NO;
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
//    GLint swapInt = 1;
//    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
//    
//    CVDisplayLinkCreateWithCGDisplay(CGMainDisplayID(), &displayLink);
//    
//    CVDisplayLinkSetOutputCallback(displayLink, DisplayLinkCallback, self);
//
//    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
//    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
//    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    //CVDisplayLinkStart(displayLink);
    
    timer = [NSTimer scheduledTimerWithTimeInterval:0.0001 target:self selector:@selector(displayFrameForTimer) userInfo:nil repeats:YES];
    
    [super prepareOpenGL];
}

- (void)startup
{
    Init();
    started = YES;
}

- (void)displayFrameForTimer
{
    [[self openGLContext] makeCurrentContext];
    
    glClearColor(1, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //glViewport(0, 0, 576, 768);
    
    if (started)
    {
        Update();
        Display();
    }
    
    [[self openGLContext] flushBuffer];
}

- (void)displayFrameForDisplayLink:(const CVTimeStamp*)outputTime
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

- (void)drawRect:(NSRect)dirtyRect
{
    [self displayFrameForTimer];
}

@end
