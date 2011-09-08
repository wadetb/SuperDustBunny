//
//  SDBAppDelegate.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 9/7/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SDBOpenGLView.h"

@interface SDBAppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet SDBOpenGLView *view;

@end
