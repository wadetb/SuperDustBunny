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


SScore Score;

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

void DisplayScore()
{
    if (Chapter.PageProps.VacuumOff)
        return;
    
    DisplayTime(TimeX, TimeY, TimeScaleFactor, Score.CurrentPageTime);
}

void ResetScore()
{    
    Score.CurrentPageTime = 0;
    Score.CurrentBonus = 0;
    Score.CurrentBabies = 0;
    
    for (int i = 0; i < MAX_PAGE_TIMES; i++)
    {
        Score.PageTime[i] = 0;
        Score.PageBonus[i] = 0;
        
        Score.NewRecord[i] = false;
        Score.BestPageTime[i] = INT_MAX;
    }
    
    Score.ChapterTime = 0;
    Score.NewChapterRecord = false;
    Score.BestChapterTime = INT_MAX;
    
    Score.Medal = MEDAL_NONE;
    
    Score.Invalid = false;
    Score.DeathCount = 0;
}

#ifdef PLATFORM_IPHONE
bool authenticationComplete = false;
#endif

void UploadChapterScore()
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

#if 0
        GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:@"sdb"] autorelease];
        scoreReporter.value = score;
        
        [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
         if (error != nil)
         {
         // TODO: Write that score to a file.
         // handle the reporting error
         }
         }];
#endif
        
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
    NSString *name = [person valueForProperty:kABFirstNameProperty];
#endif

#ifdef PLATFORM_IPHONE_OR_MAC

    NSString* city = @"nocity";
    NSString* state = @"nostate";
    NSString* country = @"nocountry";

#ifdef PLATFORM_IPHONE
    // TODO- Request location data at an appropriate time in the application, instead of the beginning.
    if (theViewController.haveLocation)
    {
        city = theViewController.city;
        state = theViewController.state;
        country = theViewController.country;
    }
#endif
    
    NSString *post = [NSString stringWithFormat:@"build=%x&name=%@&time=%d&chapter=%s&city=%@&state=%@&country=%@", 
                      BuildNumber, name, Chapters[CurrentChapter].BestTime, Chapters[CurrentChapter].Name, city, state, country];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:
                                    [NSURL URLWithString:
                                     @"http://www.pluszerogames.com/sdb/postleaderboard.php"]];
    
    [request setHTTPMethod:@"POST"];
    [request setHTTPBody:[post dataUsingEncoding:NSUTF8StringEncoding]];
    
    [URLRequestStore submitRequest:request];
#endif
}

void RecordPageScore(int Page)
{
    if (Page < 0 || Page > MAX_PAGE_TIMES)
        ReportError("Invalid page number %d for scorekeeping.  Maximum is %d.", Page, MAX_PAGE_TIMES);

    // Uncomment to force ghost saving, e.g. when making race recordings.
    if (Tweak.RecordGhostRace && Chapter.PageProps.GhostRace && IsGhostRecordingActive())
        SaveGhost(Chapters[CurrentChapter].Name, Chapter.Pages[Chapter.PageNum].Name, true);

    if (Score.Invalid)
        return;
    
    if (Chapter.PageProps.VacuumOff)
    {
        Score.PageTime[Page] = 0;
        Score.PageBonus[Page] = 0;
        Score.BestPageTime[Page] = 0;
        Score.NewRecord[Page] = false;
        Score.CurrentPageTime = 0;
        Score.CurrentBonus = 0;
        Score.CurrentBabies = 0;
        Score.DeathCount = 0;
        return;
    }
    
    Score.CurrentPageTime -= Score.CurrentBonus;
    
    Score.PageTime[Page] = Score.CurrentPageTime;
    Score.PageBonus[Page] = Score.CurrentBonus;
    Score.PageBabies[Page] = Score.CurrentBabies;
    
    Score.ChapterTime += Score.CurrentPageTime;
    
    if (Score.CurrentPageTime < Score.BestPageTime[Page])
    {
        if (Score.BestPageTime[Page] != INT_MAX)
            Score.NewRecord[Page] = true;

        Score.BestPageTime[Page] = Score.CurrentPageTime;

        if (IsGhostRecordingActive())
            SaveGhost(Chapters[CurrentChapter].Name, Chapter.Pages[Chapter.PageNum].Name, false);
    }
    
    if (Page == Chapter.NPages - 1)
    {
        if (Score.ChapterTime < Score.BestChapterTime)
        {
            if (Score.BestChapterTime != INT_MAX)
                Score.NewChapterRecord = true;

            Score.BestChapterTime = Score.ChapterTime;
        }
    
        Chapters[CurrentChapter].Completed = true;
        Chapters[CurrentChapter].BestTime = Score.BestChapterTime;

        for (int i = 0; i < NChapters; i++)
        {
            if (Chapters[i].UnlockedBy)
            {
                if (strcmp(Chapters[i].UnlockedBy, Chapters[CurrentChapter].Name) == 0)
                {
                    if (Chapters[i].Unlocked == false)
                    {
                        Chapters[i].Unlocked = true;
                        SaveChapterUnlocks();
                    }
                }
            }
        }
        
        Score.Medal = MEDAL_NONE;
        if (Score.ChapterTime <= Chapters[CurrentChapter].BronzeTime)
            Score.Medal = MEDAL_BRONZE;
        if (Score.ChapterTime <= Chapters[CurrentChapter].SilverTime)
            Score.Medal = MEDAL_SILVER;
        if (Score.ChapterTime <= Chapters[CurrentChapter].GoldTime)
            Score.Medal = MEDAL_GOLD;
    }
    
    if (Chapters[CurrentChapter].Played == false)
    {
        Chapters[CurrentChapter].Played = true;
        SaveChapterUnlocks();
    }
    
    Score.CurrentPageTime = 0;
    Score.CurrentBonus = 0;
    Score.DeathCount = 0;
}

void AwardBonus(float X, float Y)
{
    //CreateBonus(X, Y);
    
    Score.CurrentBonus++;
}

void AwardBaby(int Hat)
{
    if (Score.CurrentBabies > MAX_SCORE_BABIES)
        ReportError("Exceeded maximum of %d babies carried over between pages.", MAX_SCORE_BABIES);
    
    Score.BabyHats[Score.CurrentBabies] = Hat;
    Score.CurrentBabies++;
}

void LoadChapterScores(char* ChapterName)
{
    PushErrorContext("While loading scores for chapter '%s':\n", ChapterName);
    
    ResetScore();

#ifdef PLATFORM_IPHONE_OR_MAC
    @try
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *fileName = [[NSString stringWithUTF8String:ChapterName] stringByAppendingPathExtension:@"scores"];
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];

        NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:filePath];
        if ( !dict )
        {
            NSLog(@"Failed to load dictionary from chapter scores file for chapter '%s'.\n", ChapterName);
            PopErrorContext();
            return;
        }
        
        NSNumber *version = [dict objectForKey:@"version"];
        if ( [version intValue] != 1 )
        {
            NSLog(@"Chapter scores dictionary file is wrong version.  Got %d, expected %d.\n", [version intValue], 1);
            PopErrorContext();
            return;
        }
        
        NSArray *pages = [dict objectForKey:@"pageTimes"];

        for (int i = 0; i < [pages count]; i++)
        {
            NSDictionary *page = [pages objectAtIndex:i];
            
            NSString *name = [page objectForKey:@"name"];
            
            for (int j = 0; j < Chapter.NPages; j++)
            {
                if ([name isEqualToString:[NSString stringWithUTF8String:Chapter.Pages[j].Name]])
                {
                    Score.BestPageTime[j] = [[page objectForKey:@"time"] intValue];
                    break;
                }
            }
        }

        Score.BestChapterTime = [[dict objectForKey:@"chapterTime"] intValue];
    }
    @catch (NSException *exception) 
    {
        for (int i = 0; i < Chapter.NPages; i++)
            Score.BestPageTime[i] = INT_MAX;
        
        Score.BestChapterTime = INT_MAX;
    }
#endif
    
    PopErrorContext();
}

void SaveChapterScores(char* ChapterName)
{
    PushErrorContext("While saving scores for chapter '%s':\n", ChapterName);

#ifdef PLATFORM_IPHONE_OR_MAC
    @try
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *fileName = [[NSString stringWithUTF8String:ChapterName] stringByAppendingPathExtension:@"scores"];
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];
        
        NSMutableArray *pages = [[NSMutableArray alloc] init];
        for (int i = 0; i < Chapter.NPages; i++)
        {
            NSDictionary *page = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSString stringWithUTF8String:Chapter.Pages[i].Name], @"name",
                                  [NSNumber numberWithInt:Score.BestPageTime[i]], @"time", nil];
            [pages addObject:page];
        }
        
        NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithInt:1], @"version",
                              [NSNumber numberWithInt:Score.BestChapterTime], @"chapterTime",
                              pages, @"pageTimes", nil];
        
        [dict writeToFile:filePath atomically:YES];
    }
    @catch (NSException *e)
    {
    }
#endif
    
    PopErrorContext();
}

void UpdateScore()
{
    if (!Chapter.PageProps.VacuumOff)
        Score.CurrentPageTime++;
}