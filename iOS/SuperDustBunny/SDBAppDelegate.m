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

#import "URLRequestStore.h"

@implementation SDBAppDelegate

@synthesize window;

- (NSString *)searchForRootDirectory
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    NSString *path = [[NSBundle mainBundle] resourcePath];
    while ([path isNotEqualTo:@"/"])
    {
        NSLog(@"Searching '%@'...\n", path);
        if ([fileManager fileExistsAtPath:[path stringByAppendingPathComponent:@"Assets"]])
            return path;
        path = [[path stringByAppendingPathComponent:@"../"] stringByStandardizingPath];
    }
    
    return nil;
}

- (NSString *)askForRootDirectory
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setTitle:@"Select the SuperDustBunny directory"];
    [openPanel setPrompt:@"Got it"];
    [openPanel setCanChooseFiles:NO];
    [openPanel setCanChooseDirectories:YES];
    [openPanel setAllowsMultipleSelection:NO];
    
    NSInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return nil;

    return [[[openPanel URLs] objectAtIndex:0] path];
}

- (void)startupFullScreen
{
    NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
    window = [[SDBWindow alloc] initWithContentRect:mainDisplayRect 
                                          styleMask:NSBorderlessWindowMask 
                                            backing:NSBackingStoreBuffered 
                                              defer:YES];
    [window setLevel:NSMainMenuWindowLevel+1];
    [window setOpaque:YES];
    [window setHidesOnDeactivate:YES];
    
    NSRect viewRect = NSMakeRect(0, 0, mainDisplayRect.size.width, mainDisplayRect.size.height);
    SDBOpenGLView *view = [[SDBOpenGLView alloc] initWithFrame:viewRect];
    [window setContentView:view];
    [view release];
    
    [window makeKeyAndOrderFront:self];

    [NSCursor hide];
    CGAssociateMouseAndMouseCursorPosition(false);
    CGWarpMouseCursorPosition(NSMakePoint(mainDisplayRect.size.width/2, mainDisplayRect.size.height/2));
    
    [view startup];
}

- (void)startupWindowed
{
    NSRect mainDisplayRect = NSMakeRect(0, 0, 576, 768);
    window = [[SDBWindow alloc] initWithContentRect:mainDisplayRect 
                                          styleMask:NSTitledWindowMask 
                                            backing:NSBackingStoreBuffered 
                                              defer:YES];
    [window setOpaque:YES];
    
    NSRect viewRect = NSMakeRect(0, 0, mainDisplayRect.size.width, mainDisplayRect.size.height);
    SDBOpenGLView *view = [[SDBOpenGLView alloc] initWithFrame:viewRect];
    [window setContentView:view];
    [view release];
    
    [window makeKeyAndOrderFront:self];
    
    [view startup];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
    NSString *path = [self searchForRootDirectory];
    if (path == nil)
        path = [self askForRootDirectory];
    
    if (path)
    {
        NSLog(@"Root directory: %@\n", path);
        extern char RootDirectory[1024];
        snprintf(RootDirectory, sizeof(RootDirectory), "%s", [path UTF8String]);
    }    
    else
    {
        NSLog(@"Root directory selection cancelled, exiting.\n");
        
        exit(255);        
    }
    
    //[self startupWindowed];
    [self startupFullScreen];
    
    [URLRequestStore processStoredRequests];
}

@end
