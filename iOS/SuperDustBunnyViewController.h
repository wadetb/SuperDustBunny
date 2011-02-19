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
    EAGLContext *context;
    BOOL paused;
}

@property (nonatomic, retain) IBOutlet SettingsViewController *settingsViewController;

@property BOOL paused;

- (void)showSettings;
- (void)hideSettings;

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

extern SuperDustBunnyViewController *theViewController;
