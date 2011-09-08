//
//  SDBAppDelegate.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 9/7/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class SDBWindow;

@interface SDBAppDelegate : NSObject <NSApplicationDelegate>
{
    SDBWindow *window;
}

@property (retain) SDBWindow *window;

@end
