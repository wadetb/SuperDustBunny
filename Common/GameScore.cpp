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

SScore Score;

float TimeX = 40;
float TimeY = 10;
float TimeScaleFactor = 0.65f;

void DisplayTimeDigit(int Digit, float BaseX, float BaseY, float ScaleFactor, float X, float Y)
{
    X = BaseX + X * 64 * ScaleFactor;
    Y = BaseY + Y * 64 * ScaleFactor;
    switch (Digit)
    {
        case 0:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 1,   0, 54,  63, ScaleFactor, ScaleFactor); break;        
        case 1:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X+10, Y, 55,  0, 74,  63, ScaleFactor, ScaleFactor); break;
        case 2:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 76,  0, 131, 63, ScaleFactor, ScaleFactor); break;
        case 3:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 133, 0, 187, 63, ScaleFactor, ScaleFactor); break;
        case 4:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 189, 0, 242, 63, ScaleFactor, ScaleFactor); break;
        case 5:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 243, 0, 296, 63, ScaleFactor, ScaleFactor); break;
        case 6:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 298, 0, 353, 63, ScaleFactor, ScaleFactor); break;
        case 7:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 354, 0, 406, 63, ScaleFactor, ScaleFactor); break;
        case 8:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 408, 0, 462, 63, ScaleFactor, ScaleFactor); break;
        case 9:    AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 463, 0, 520, 63, ScaleFactor, ScaleFactor); break;
        case 10:   AddLitSubSpriteScaled(LIGHTLIST_WIPE, &FullScoreSprite, X,    Y, 55,  0, 74,  63, ScaleFactor, ScaleFactor/4.0f); break;
        default:   break;   
    }    
}

void DisplayTime(float X, float Y, float ScaleFactor, int Time)
{
    int Minutes = ( Time / 60 ) / 60;
    int Seconds = ( Time / 60 ) % 60;
    int Hundredths = ( Time % 60 ) * 60 / 100;
    
	DisplayTimeDigit(Minutes,         X, Y, ScaleFactor, 0,    0);
	DisplayTimeDigit(10,              X, Y, ScaleFactor, 1,    0.25f);
	DisplayTimeDigit(10,              X, Y, ScaleFactor, 1,    0.75f);
	DisplayTimeDigit(Seconds/10,      X, Y, ScaleFactor, 1.5f, 0);
	DisplayTimeDigit(Seconds%10,      X, Y, ScaleFactor, 2.5f, 0);
	DisplayTimeDigit(10,              X, Y, ScaleFactor, 3.5f, 0.75f);
    DisplayTimeDigit(Hundredths/10,   X, Y, ScaleFactor, 4,    0);
    DisplayTimeDigit(Hundredths%10,   X, Y, ScaleFactor, 5,    0);
}

void DisplayScore()
{
    DisplayTime(TimeX, TimeY, TimeScaleFactor, Score.CurrentPageTime);
}

void ResetScore()
{    
    Score.CurrentPageTime = 0;
    
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
}

void RecordPageScore(int Page)
{
    if (Page < 0 || Page > MAX_PAGE_TIMES)
        ReportError("Invalid page number %d for scorekeeping.  Maximum is %d.", Page, MAX_PAGE_TIMES);
    
    Score.CurrentPageTime -= Score.CurrentBonus;
    
    Score.PageTime[Page] = Score.CurrentPageTime;
    Score.PageBonus[Page] = Score.CurrentBonus;
    
    Score.ChapterTime += Score.CurrentPageTime;
    
    if ( Score.CurrentPageTime < Score.BestPageTime[Page] )
    {
        Score.BestPageTime[Page] = Score.CurrentPageTime;
        Score.NewRecord[Page] = true;
    }
    
    if ( Page == Chapter.NPages - 1 )
    {
        if ( Score.ChapterTime < Score.BestChapterTime )
        {
            Score.BestChapterTime = Score.ChapterTime;
            Score.NewChapterRecord = true;
        }
    }
    
    Score.CurrentPageTime = 0;
    Score.CurrentBonus = 0;
}

void AwardBonus()
{
    // TODO: On screen representation.
    Score.CurrentBonus++;
}

void LoadChapterScores(char* ChapterName)
{
    ResetScore();

#ifdef PLATFORM_IPHONE
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *fileName = [[NSString stringWithUTF8String:ChapterName] stringByAppendingPathExtension:@"scores"];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];

    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:filePath];
    if ( !dict )
        return;
    
    NSNumber *version = [dict objectForKey:@"version"];
    if ( [version intValue] != 1 )
        return;
    
    NSArray *pageTimes = [dict objectForKey:@"pageTimes"];
    if ( [pageTimes count] != Chapter.NPages )
        return;

    NSNumber *chapterTime = [dict objectForKey:@"chapterTime"];
    Score.BestChapterTime = [chapterTime intValue];
    
    for (int i = 0; i < [pageTimes count]; i++)
        Score.BestPageTime[i] = [[pageTimes objectAtIndex:i] intValue];
#endif
}

void SaveChapterScores(char* ChapterName)
{
#ifdef PLATFORM_IPHONE
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *fileName = [[NSString stringWithUTF8String:ChapterName] stringByAppendingPathExtension:@"scores"];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];
    
    NSMutableArray *array = [[NSMutableArray alloc] init];
    for (int i = 0; i < Chapter.NPages; i++)
        [array addObject:[NSNumber numberWithInt:Score.BestPageTime[i]]];
    
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInt:1], @"version",
                          [NSNumber numberWithInt:Score.BestChapterTime], @"chapterTime",
                          array, @"pageTimes", nil];
    
    [dict writeToFile:filePath atomically:YES];
#endif
}

void UpdateScore()
{    
    Score.CurrentPageTime++;
}