//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "GameScore.h"
#include "Dusty.h"

#include "Barrel.h"
#include "Fan.h"
#include "Coin.h"
#include "TennisBall.h"
#include "FireWorks.h"
#include "Gear.h"
#include "Debris.h"
#include "Lives.h"
#include "Chapter.h"
#include "Smoke.h"
#include "Recorder.h"
#include "Ghost.h"
#include "Settings.h"
#include "Tweak.h"

#ifdef PLATFORM_MAC
#import <AddressBook/AddressBook.h>
#import "URLRequestStore.h"
#endif
#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#import <GameKit/GameKit.h>
#import "URLRequestStore.h"
#endif


float TimeX = 40;
float TimeY = 10;
float TimeScaleFactor = 0.65f;


void DisplayTimeDigitAlpha(int Digit, float BaseX, float BaseY, float ScaleFactor, float X, float Y, float Alpha)
{
    X = BaseX + X * 64 * ScaleFactor;
    Y = BaseY + Y * 64 * ScaleFactor;
    switch (Digit)
    {
        case 0:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 1,   0, 54,  63, ScaleFactor, ScaleFactor, Alpha); break;        
        case 1:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X+10, Y, 55,  0, 74,  63, ScaleFactor, ScaleFactor, Alpha); break;
        case 2:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 76,  0, 131, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 3:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 133, 0, 187, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 4:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 189, 0, 242, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 5:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 243, 0, 296, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 6:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 298, 0, 353, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 7:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 354, 0, 406, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 8:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 408, 0, 462, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 9:    AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 463, 0, 520, 63, ScaleFactor, ScaleFactor, Alpha); break;
        case 10:   AddLitSubSpriteScaledAlpha(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 520, 41, 542, 63, ScaleFactor, ScaleFactor, Alpha); break;
        default:   break;   
    }    
}

void DisplayTimeDigit(int Digit, float BaseX, float BaseY, float ScaleFactor, float X, float Y)
{
    DisplayTimeDigitAlpha(Digit, BaseX, BaseY, ScaleFactor, X, Y, 1.0f);
}

void DisplayTimeAlpha(float X, float Y, float ScaleFactor, int Time, float Alpha)
{
    int Minutes = ( Time / 60 ) / 60;
    int Seconds = ( Time / 60 ) % 60;
    int Hundredths = ( Time % 60 ) * 60 / 100;
    
	DisplayTimeDigitAlpha(Minutes,         X, Y, ScaleFactor, 0,    0,    Alpha);
	DisplayTimeDigitAlpha(10,              X, Y, ScaleFactor, 1,    0.2f, Alpha);
	DisplayTimeDigitAlpha(10,              X, Y, ScaleFactor, 1,    0.7f, Alpha);
	DisplayTimeDigitAlpha(Seconds/10,      X, Y, ScaleFactor, 1.5f, 0,    Alpha);
	DisplayTimeDigitAlpha(Seconds%10,      X, Y, ScaleFactor, 2.5f, 0,    Alpha);
	DisplayTimeDigitAlpha(10,              X, Y, ScaleFactor, 3.5f, 0.7f, Alpha);
    DisplayTimeDigitAlpha(Hundredths/10,   X, Y, ScaleFactor, 4,    0,    Alpha);
    DisplayTimeDigitAlpha(Hundredths%10,   X, Y, ScaleFactor, 5,    0,    Alpha);
}

void DisplayTime(float X, float Y, float ScaleFactor, int Time)
{
    DisplayTimeAlpha(X, Y, ScaleFactor, Time, 1.0f);
}


#ifdef PLATFORM_IPHONE
bool authenticationComplete = false;
#endif

void UploadScore()
{
#ifdef PLATFORM_IPHONE
    NSString *name = nil;
    
    if (isGameCenterAPIAvailable())
    {
        theViewController.paused = TRUE;

        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
            NSLog(@"Player authenticated: %s", ([localPlayer isAuthenticated] ? "YES" : "NO"));
            authenticationComplete = true;
         }];

        while (!authenticationComplete)
        {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        }

        theViewController.paused = FALSE;

        GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:@"pages_survived"] autorelease];
        scoreReporter.value = Portfolio.PageCount;
        scoreReporter.context = Dusty.Hat;
        
        [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
            if (error != nil)
                NSLog(@"game center score reporting error: %@\n", error);
            else
                NSLog(@"game center score reported\n");
         }];
        
        if ([localPlayer isAuthenticated])
            name = [localPlayer alias];    
    }
    
    if (name == nil)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Enter your name"
                                                        message:@"-------------------------------"
                                                       delegate:theViewController
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        
        UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(12.0, 45.0, 260.0, 31.0)]; 
        [textField setContentVerticalAlignment:UIControlContentVerticalAlignmentCenter];
        [textField setAutocapitalizationType:UITextAutocapitalizationTypeWords];
        [textField setBorderStyle:UITextBorderStyleRoundedRect];
        [textField setBackgroundColor:[UIColor whiteColor]];
        [textField setTextAlignment:UITextAlignmentCenter];

        if (Settings.LeaderboardName != nil)
            [textField setText:Settings.LeaderboardName];

        [textField becomeFirstResponder];
        
        [alert addSubview:textField];

        theViewController.paused = TRUE;
        
        [alert show];
        
        while (theViewController.paused)
        {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        }

        if (Settings.LeaderboardName != nil)
            [Settings.LeaderboardName release];
        Settings.LeaderboardName = [[textField text] retain];
        
        name = [textField text];
        [textField release];
        
        [alert release];
    }
#endif
    
#ifdef PLATFORM_MAC
    ABPerson *person = [[ABAddressBook sharedAddressBook] me];
    NSString *name;
    if (person)
        name = [person valueForProperty:kABFirstNameProperty];
    else
        name = NSUserName();
#endif

#ifdef PLATFORM_IPHONE_OR_MAC
    NSString* city = @"nocity";
    NSString* state = @"nostate";
    NSString* country = @"nocountry";

#ifdef PLATFORM_IPHONE
    if (theViewController.haveLocation)
    {
        city = theViewController.city;
        state = theViewController.state;
        country = theViewController.country;
    }
#endif
    
    NSString *post = [NSString stringWithFormat:@"build=%x&name=%@&pages=%d&hat=%d&city=%@&state=%@&country=%@",
                      BuildNumber, name, Portfolio.PageCount, Dusty.Hat, city, state, country];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:
                                    [NSURL URLWithString:
                                     @"http://www.pluszerogames.com/sdb/postleaderboard.php"]];
    
    [request setHTTPMethod:@"POST"];
    [request setHTTPBody:[post dataUsingEncoding:NSUTF8StringEncoding]];
    
    [URLRequestStore submitRequest:request];
#endif
}

//void RecordPageScore(int Page)
//{
//    // Uncomment to force ghost saving, e.g. when making race recordings.
//    if (Tweak.RecordGhostRace && Chapter.PageProps.GhostRace && IsGhostRecordingActive())
//        SaveGhost(Chapters[CurrentChapter].Name, Chapter.Pages[Chapter.PageNum].Name, true);
//
//    if (Score.Invalid)
//        return;
//}
