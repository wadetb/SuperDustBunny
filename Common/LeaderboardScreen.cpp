//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "common.h"
#include "LeaderboardScreen.h"
#include "Text.h"
#include "Chapter.h"
#include "GameScore.h"
#include "Settings.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


#define LEADERBOARD_COUNT 25


enum ELeaderboardRegion
{
    LEADERBOARDREGION_WORLD,
    LEADERBOARDREGION_STATE,
    LEADERBOARDREGION_COUNT
};

enum ELeaderboardMode
{
    LEADERBOARDMODE_KINGOFTHEHILL,
    LEADERBOARDMODE_ALLTIME,
    LEADERBOARDMODE_COUNT
};


struct SLeaderboardScreen
{
    float WiggleTime;
    float FadeInTime;
    
    float BaseY;
    
    float StartY;
    
    bool Dragging;
    float DragStartY;
    
    int CurTab;
    
    ELeaderboardRegion Region;
    ELeaderboardMode Mode;

    int AvailableRegionCount;
    ELeaderboardRegion AvailableRegions[LEADERBOARDREGION_COUNT];
    

    char* Name[LEADERBOARD_COUNT];
    int Time[LEADERBOARD_COUNT];
};


SLeaderboardScreen LeaderboardScreen;


void DownloadLeaderboards()
{
    LeaderboardScreen.FadeInTime = 0.0f;
    
    for (int i = 0; i < LEADERBOARD_COUNT; i++)
    {
        if (LeaderboardScreen.Name[i])
        {
            free(LeaderboardScreen.Name[i]);
            LeaderboardScreen.Name[i] = NULL;
        }
    }
    

#ifdef PLATFORM_IPHONE_OR_MAC
    const char* RegionTag[LEADERBOARDREGION_COUNT] = { "world", "state" };
    const char* ModeTag[LEADERBOARDMODE_COUNT] = { "kingofthehill", "alltime" };
    
    
    NSString* URLString;
#ifdef PLATFORM_IPHONE
    
    if (theViewController.haveLocation && (/*LeaderboardScreen.Region == LEADERBOARDREGION_CITY ||*/ LeaderboardScreen.Region == LEADERBOARDREGION_STATE))
    {
        NSString* region;
//        if (LeaderboardScreen.Region == LEADERBOARDREGION_CITY)
//            region = theViewController.city;
//        else
            region = theViewController.state;
        
        URLString = [NSString stringWithFormat:@"http://pluszerogames.com/sdb/getleaderboard.php?mode=%s&region=%s&chapter=%s&region_value=%@", 
                     ModeTag[LeaderboardScreen.Mode], 
                     RegionTag[LeaderboardScreen.Region], 
                     Chapters[CurrentChapter].Name, 
                     [region stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    }
    else
#endif
    {
        URLString = [NSString stringWithFormat:@"http://pluszerogames.com/sdb/getleaderboard.php?mode=%s&region=%s&chapter=%s", 
                     ModeTag[LeaderboardScreen.Mode], 
                     RegionTag[LeaderboardScreen.Region], 
                     Chapters[CurrentChapter].Name];        
    }
    NSURL *URL = [NSURL URLWithString:URLString];
    
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData timeoutInterval:30];
    
    NSURLResponse *response;
    NSError *error = [[NSError alloc] init];
    NSData* result = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    
    if (result)
    {
        NSString *text = [[NSString alloc] initWithData:result encoding:NSUTF8StringEncoding];
        NSLog(@"Leaderboard results:\n%@\n", text);
        
        NSArray *entries = [text componentsSeparatedByString:@"\n"];
        
        for (int i = 0; i < entries.count/2; i++)
        {
            if (i >= LEADERBOARD_COUNT)
                break;
            
            NSString *name = [entries objectAtIndex:i*2+0];
            NSString *time = [entries objectAtIndex:i*2+1];
            
            if ([name isEqualToString:@"--"])
                break;
            
            if (atoi([time UTF8String]))
            {
                LeaderboardScreen.Name[i] = strdup([[name lowercaseString] UTF8String]);
                
                if (strlen(LeaderboardScreen.Name[i]) > 10)
                    LeaderboardScreen.Name[i][10] = 0;
                
                LeaderboardScreen.Time[i] = atoi([time UTF8String]);
            }
        }
    }
#endif
}

void InitLeaderboardScreen()
{
    LeaderboardScreen.BaseY = -LitScreenHeight * 0.75;
    
    LeaderboardScreen.StartY = 0;
    LeaderboardScreen.Dragging = false;
    
#ifdef PLATFORM_IPHONE
    [theViewController retrieveLocationAndDownloadLeaderboards];
#else
    DownloadLeaderboards();
#endif
}

static void DisplayLeaderboardScrollString(const char* String, unsigned int Flags, float X, float Y, float Scale, float Alpha)
{
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 30.0f, 50.0f, 0.0f, 1.0f, true);
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 700.0f, 800.0f, 1.0f, 0.0f, true);
    if (Alpha < 0)
        return;
    DisplayStringAlpha(LIGHTLIST_VACUUM, String, Flags, X, Y - LeaderboardScreen.StartY + LeaderboardScreen.BaseY, Scale, Alpha);
}

static void DisplayLeaderboardScrollTime(int Time, float X, float Y, float Scale, float Alpha)
{
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 30.0f, 50.0f, 0.0f, 1.0f, true);
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 700.0f, 800.0f, 1.0f, 0.0f, true);
    if (Alpha < 0)
        return;
    DisplayTimeAlpha(X, Y - LeaderboardScreen.StartY + LeaderboardScreen.BaseY, Scale, Time, Alpha);
}

void DisplayLeaderboardScreen()
{
    AddLitSprite(LIGHTLIST_VACUUM, &LeaderboardBackgroundSprite, 0, LeaderboardScreen.BaseY);
    
    // Leaderboard buttons.
    //AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonLeaderboardSprite, 768-100, LeaderboardScreen.BaseY + 60, 1.0f, 1.0f);
 
    float PrevAlpha = LeaderboardScreen.CurTab > 0 ? 1.0f : 0.5f;
    float NextAlpha = LeaderboardScreen.CurTab < LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount - 1 ? 1.0f : 0.5f;
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonFastForwardSprite, 192+16, LeaderboardScreen.BaseY + 920, -1.0f, PrevAlpha);
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonFastForwardSprite, 768-192-16, LeaderboardScreen.BaseY + 920, 1.0f, NextAlpha);        

    float GhostAlpha = Settings.GhostActive ? 1.0f : 0.5f;
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonGhostSprite, 384, LeaderboardScreen.BaseY + 920, 1.0f, GhostAlpha);

    const char* RegionName[LEADERBOARDREGION_COUNT] = { "worldwide", "state" };
    const char* ModeName[LEADERBOARDMODE_COUNT] = { "king of the day", "top score" };
    
    ELeaderboardRegion Region = LeaderboardScreen.Region;
    
    int Mode = LeaderboardScreen.Mode;

    const char* LocalRegionName = RegionName[Region];
#ifdef PLATFORM_IPHONE
    if (theViewController.haveLocation && (Region == LEADERBOARDREGION_STATE))
    {
//        if (Region == LEADERBOARDREGION_CITY)
//            LocalRegionName = [[theViewController.city lowercaseString] UTF8String];
//        else
            LocalRegionName = [[theViewController.state lowercaseString] UTF8String];
    }
#endif
    
    DisplayLeaderboardScrollString(LocalRegionName, FORMAT_CENTER_X, 384, 50, 0.8f, 1.0f);

    DisplayLeaderboardScrollString(ModeName[Mode], FORMAT_CENTER_X, 384, 130, 1.0f, 1.0f);
    
    int CurrentY = 250;
    
    for (int i = 0; i < LEADERBOARD_COUNT; i++)
    {
        if (!LeaderboardScreen.Name[i])
            continue;
        
        float Alpha = Remap((float)CurrentY - 300, 0.0f, LeaderboardScreen.FadeInTime * 5000.0f, 1.0f, 0.0f, true);
        
        DisplayLeaderboardScrollString(LeaderboardScreen.Name[i], 0, 80, CurrentY, 0.8f, Alpha);
        DisplayLeaderboardScrollTime(LeaderboardScreen.Time[i], 450, CurrentY, 0.6f, Alpha);
        
        CurrentY += 75;
    }
}

void SwitchToNextLeaderboard()
{
    LeaderboardScreen.CurTab++;
    
    if (LeaderboardScreen.CurTab < 0)
        LeaderboardScreen.CurTab = 0;
    if (LeaderboardScreen.CurTab >= LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount)
        LeaderboardScreen.CurTab = (LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount) - 1;
    
    int CurRegion = LeaderboardScreen.CurTab / LEADERBOARDMODE_COUNT;
    int CurMode = LeaderboardScreen.CurTab % LEADERBOARDMODE_COUNT;
    
    if (CurRegion != LeaderboardScreen.Region || CurMode != LeaderboardScreen.Mode)
    {
        LeaderboardScreen.Region = (ELeaderboardRegion)CurRegion;
        LeaderboardScreen.Mode = (ELeaderboardMode)CurMode;
        DownloadLeaderboards();
    }
}

void SwitchToPreviousLeaderboard()
{
    LeaderboardScreen.CurTab--;
    
    if (LeaderboardScreen.CurTab < 0)
        LeaderboardScreen.CurTab = 0;
    if (LeaderboardScreen.CurTab >= LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount)
        LeaderboardScreen.CurTab = (LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount) - 1;
    
    int CurRegion = LeaderboardScreen.CurTab / LEADERBOARDMODE_COUNT;
    int CurMode = LeaderboardScreen.CurTab % LEADERBOARDMODE_COUNT;
    
    if (CurRegion != LeaderboardScreen.Region || CurMode != LeaderboardScreen.Mode)
    {
        LeaderboardScreen.Region = (ELeaderboardRegion)CurRegion;
        LeaderboardScreen.Mode = (ELeaderboardMode)CurMode;
        DownloadLeaderboards();
    }    
}

void UpdateLeaderboardScreen()
{
    LeaderboardScreen.BaseY *= 0.85f;
    
    LeaderboardScreen.AvailableRegionCount = 0;
    
#ifdef PLATFORM_IPHONE
    if (theViewController.haveLocation)
    {
        LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_STATE;
//        LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_CITY;
    }
#endif
    // TODO if (HaveFacebook), if (HaveGameCenter)

    LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_WORLD;

    if (LeaderboardScreen.Dragging)
    {
        if (!msButton1)
        {
            LeaderboardScreen.Dragging = false;
        }
        
        LeaderboardScreen.StartY -= msY - LeaderboardScreen.DragStartY;
        LeaderboardScreen.DragStartY = msY;
    }
    else
    {
        if (msButton1)
        {
            LeaderboardScreen.Dragging = true;
            LeaderboardScreen.DragStartY = msY;
        }
        
        if (LeaderboardScreen.StartY < 0)
            LeaderboardScreen.StartY += (0 - LeaderboardScreen.StartY) * 0.1f;

        int Count = 0;
        for (int i = 0; i < LEADERBOARD_COUNT; i++)
        {
            if (!LeaderboardScreen.Name[i])
                break;
            Count++;
        }
        
        int TotalHeight = 450 + Count * 75;
        if (TotalHeight < LitScreenHeight)
            TotalHeight = LitScreenHeight;
        
        if (LeaderboardScreen.StartY > TotalHeight - LitScreenHeight)
            LeaderboardScreen.StartY = LeaderboardScreen.StartY*0.9f + (TotalHeight - LitScreenHeight)*0.1f;
    }
    
    LeaderboardScreen.WiggleTime += 1.0f/60.0f;
    LeaderboardScreen.FadeInTime += 1.0f/60.0f;
}
