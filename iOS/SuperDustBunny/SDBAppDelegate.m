//
//  SDBAppDelegate.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 9/7/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import "SDBAppDelegate.h"

#import "SDBWindow.h"
#import "SDBOpenGLView.h"

@implementation SDBAppDelegate

- (void)getRootDirectory
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setTitle:@"Select the SuperDustBunny directory"];
    [openPanel setPrompt:@"Got it"];
    [openPanel setCanChooseFiles:NO];
    [openPanel setCanChooseDirectories:YES];
    [openPanel setAllowsMultipleSelection:NO];
    
    NSInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
    {
        NSLog(@"Root directory selection cancelled, exiting.\n");
        
        exit(255);
    }    

    NSString* path = [[[openPanel URLs] objectAtIndex:0] path];
    NSLog(@"Root directory: %@\n", path);
    
    extern char RootDirectory[1024];
    snprintf(RootDirectory, sizeof(RootDirectory), "%s", [path UTF8String]);
}

- (void)startupFullScreen
{
    NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
    SDBWindow *fullScreenWindow = [[SDBWindow alloc] initWithContentRect:mainDisplayRect 
                                                             styleMask:NSBorderlessWindowMask 
                                                               backing:NSBackingStoreBuffered 
                                                                 defer:YES];
    [fullScreenWindow setLevel:NSMainMenuWindowLevel+1];
    [fullScreenWindow setOpaque:YES];
    [fullScreenWindow setHidesOnDeactivate:YES];
    
    NSRect viewRect = NSMakeRect(0, 0, mainDisplayRect.size.width, mainDisplayRect.size.height);
    SDBOpenGLView *fullScreenView = [[SDBOpenGLView alloc] initWithFrame:viewRect];
    [fullScreenWindow setContentView:fullScreenView];
    
    [fullScreenWindow makeKeyAndOrderFront:self];

    [NSCursor hide];

    [fullScreenView startup];
}

- (void)startupWindowed
{
    NSRect mainDisplayRect = NSMakeRect(0, 0, 576, 768);
    SDBWindow *fullScreenWindow = [[SDBWindow alloc] initWithContentRect:mainDisplayRect 
                                                             styleMask:NSTitledWindowMask 
                                                               backing:NSBackingStoreBuffered 
                                                                 defer:YES];
    [fullScreenWindow setOpaque:YES];
    
    NSRect viewRect = NSMakeRect(0, 0, mainDisplayRect.size.width, mainDisplayRect.size.height);
    SDBOpenGLView *fullScreenView = [[SDBOpenGLView alloc] initWithFrame:viewRect];
    [fullScreenWindow setContentView:fullScreenView];
    
    [fullScreenWindow makeKeyAndOrderFront:self];
    
    [fullScreenView startup];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self getRootDirectory];
    
    //[self startupWindowed];
    [self startupFullScreen];
}

@end
