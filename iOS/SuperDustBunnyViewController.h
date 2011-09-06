//
//  SuperDustBunnyViewController.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 1/20/11.
//  Copyright 2011 Self. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@class SettingsViewController;

@interface SuperDustBunnyViewController : UIViewController <UIAlertViewDelegate> {
@private
    CADisplayLink *displayLink;
    CFTimeInterval lastTimeStamp;
    EAGLContext *context;
    BOOL paused;
    BOOL wasPaused;
    int slowFactor;
    UIView *spinnerPanel;
}

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, retain) IBOutlet SettingsViewController *settingsViewController;
@property BOOL paused;

- (void)drawFrame;

- (void)showSettings;
- (void)hideSettings;

- (void)startSpinner;
- (void)stopSpinner;

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

extern SuperDustBunnyViewController *theViewController;
