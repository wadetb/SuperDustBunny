//
//  SDBOpenGLView.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 9/7/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CoreVideo/CoreVideo.h"

@interface SDBOpenGLView : NSOpenGLView
{
    NSTimer* timer;
    CVDisplayLinkRef displayLink;
    BOOL started;
    NSTouch* currentTouch;
}

- (void)prepareOpenGL;

- (void)startup;
- (void)displayFrameForTimer;
- (void)displayFrameForDisplayLink:(const CVTimeStamp*)outputTime;

@end
