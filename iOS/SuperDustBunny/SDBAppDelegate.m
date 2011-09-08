//
//  SDBAppDelegate.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 9/7/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import "SDBAppDelegate.h"

@implementation SDBAppDelegate

@synthesize window = _window;
@synthesize view = _view;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setTitle:@"Select the SuperDustBunny directory"];
    [openPanel setPrompt:@"Got it"];
    [openPanel setCanChooseFiles:NO];
    [openPanel setCanChooseDirectories:YES];
    [openPanel setAllowsMultipleSelection:NO];
    NSInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelOKButton)
    {
        NSString* path = [[[openPanel URLs] objectAtIndex:0] path];
        NSLog(@"Root directory: %@\n", path);
        
        extern char RootDirectory[1024];
        snprintf(RootDirectory, sizeof(RootDirectory), "%s", [path UTF8String]);
        
        [_view startup];
    }
    else
    {
        NSLog(@"Root directory selection cancelled, exiting.\n");

        exit(255);
    }
}

@end
