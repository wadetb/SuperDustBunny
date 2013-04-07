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
#include "StoreScreen.h"
#include "Hat.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#import <GameKit/GameKit.h>
#import "URLRequestStore.h"
#endif


#define LEADERBOARD_COUNT 25


enum ELeaderboardRegion
{
    LEADERBOARDREGION_GAMECENTER,
    LEADERBOARDREGION_GAMECENTER_FRIENDS,
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
    bool FirstInit;
    
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
    int Pages[LEADERBOARD_COUNT];
    int Hat[LEADERBOARD_COUNT];
};


SLeaderboardScreen LeaderboardScreen;


#ifdef PLATFORM_IPHONE
static bool authenticationComplete = false;
#endif


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
    
    if ((LeaderboardScreen.Region == LEADERBOARDREGION_GAMECENTER) || (LeaderboardScreen.Region == LEADERBOARDREGION_GAMECENTER_FRIENDS))
    {
#ifdef PLATFORM_IPHONE
        theViewController.paused = TRUE;
        
        authenticationComplete = false;
        
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
         if (error != nil)
            NSLog(@"Leaderboard authenticate error: %@\n", error);
         
         NSLog(@"Player authenticated: %s", ([localPlayer isAuthenticated] ? "YES" : "NO"));
         
         GKLeaderboard *leaderboardRequest = [[GKLeaderboard alloc] init];
         
         if (LeaderboardScreen.Region == LEADERBOARDREGION_GAMECENTER_FRIENDS)
            leaderboardRequest.playerScope = GKLeaderboardPlayerScopeFriendsOnly;
         else
            leaderboardRequest.playerScope = GKLeaderboardPlayerScopeGlobal;
         
         if (LeaderboardScreen.Mode == LEADERBOARDMODE_KINGOFTHEHILL)
            leaderboardRequest.timeScope = GKLeaderboardTimeScopeToday;
         else
            leaderboardRequest.timeScope = GKLeaderboardTimeScopeAllTime;
         
         leaderboardRequest.category = @"pages_survived";
         leaderboardRequest.range = NSMakeRange(1,LEADERBOARD_COUNT);
         
         [leaderboardRequest loadScoresWithCompletionHandler: ^(NSArray *scores, NSError *error) {
          if (error != nil)
            NSLog(@"Leaderboard request error: %@\n", error);
          if (scores == nil)
            return;

          NSLog(@"Leaderboard returned %d scores\n", scores.count);
          
          NSMutableArray *playerIDs = [[NSMutableArray alloc] init];
          for (int i = 0; i < scores.count; i++)
          {
            GKScore *score = [scores objectAtIndex:i];
            [playerIDs addObject:score.playerID];
          }
          
          [GKPlayer loadPlayersForIdentifiers:playerIDs withCompletionHandler:^(NSArray *players, NSError *error) {
           if (error != nil)
            NSLog(@"Players request error: %@\n", error);
           if (players.count != scores.count)
            return;

           for (int i = 0; i < scores.count; i++)
           {
               if (i >= LEADERBOARD_COUNT)
                break;
           
               GKScore *score = [scores objectAtIndex:i];
               GKPlayer *player = [players objectAtIndex:i];
               
               LeaderboardScreen.Name[i] = strdup([[player.alias lowercaseString] UTF8String]);
               
               if (strlen(LeaderboardScreen.Name[i]) > 10)
                LeaderboardScreen.Name[i][10] = 0;
               
               LeaderboardScreen.Pages[i] = score.value;
               LeaderboardScreen.Hat[i] = score.context;
           }
           authenticationComplete = true;
           }];
          }];
         }];
        
        while (!authenticationComplete)
        {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        }
        
        theViewController.paused = FALSE;
#endif
    }
    else
    {
#ifdef PLATFORM_IPHONE_OR_MAC
        const char* RegionTag[LEADERBOARDREGION_COUNT] = { "gamecenter", "gamecenter_friends", "world", "state" };
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
            
            URLString = [NSString stringWithFormat:@"http://pluszerogames.com/sdb/getleaderboard.php?mode=%s&region=%s&region_value=%@",
                         ModeTag[LeaderboardScreen.Mode],
                         RegionTag[LeaderboardScreen.Region],
                         [region stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
        }
#endif
        else
        {
            URLString = [NSString stringWithFormat:@"http://pluszerogames.com/sdb/getleaderboard.php?mode=%s&region=%s",
                         ModeTag[LeaderboardScreen.Mode],
                         RegionTag[LeaderboardScreen.Region]];
        }
        NSURL *URL = [NSURL URLWithString:URLString];
        
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData timeoutInterval:5];
        
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
                
                NSString *name = [entries objectAtIndex:i*3+0];
                NSString *pages = [entries objectAtIndex:i*3+1];
                NSString *hat = [entries objectAtIndex:i*3+2];
                
                if ([name isEqualToString:@"--"])
                    break;
                
                if (atoi([pages UTF8String]))
                {
                    LeaderboardScreen.Name[i] = strdup([[name lowercaseString] UTF8String]);
                    
                    if (strlen(LeaderboardScreen.Name[i]) > 10)
                        LeaderboardScreen.Name[i][10] = 0;
                    
                    LeaderboardScreen.Pages[i] = atoi([pages UTF8String]);
                    LeaderboardScreen.Hat[i] = atoi([hat UTF8String]);
                }
            }
        }
#endif        
    }
}

void InitLeaderboardScreen()
{
    LeaderboardScreen.BaseY = -LitScreenHeight * 0.75;
    
    LeaderboardScreen.StartY = 0;
    LeaderboardScreen.Dragging = false;
    
    LeaderboardScreen.AvailableRegionCount = 0;
    
#ifdef PLATFORM_IPHONE
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
        
        if ([localPlayer isAuthenticated])
        {
            //LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_GAMECENTER;
            LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_GAMECENTER_FRIENDS;
        }
    }

    if (theViewController.haveLocation)
    {
        LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_STATE;
        //        LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_CITY;
    }
#endif
    
    LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_WORLD;

    if (!LeaderboardScreen.FirstInit)
    {
        LeaderboardScreen.Region = LeaderboardScreen.AvailableRegions[0];
        LeaderboardScreen.Mode = LEADERBOARDMODE_KINGOFTHEHILL;
    }
    
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

static void DisplayLeaderboardScrollNumber(int N, unsigned int Flags, float X, float Y, float Scale, float Alpha)
{
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 30.0f, 50.0f, 0.0f, 1.0f, true);
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 700.0f, 800.0f, 1.0f, 0.0f, true);
    if (Alpha < 0)
        return;
    char Work[40];
    snprintf(Work, sizeof(Work), "%d", N);
    DisplayStringAlpha(LIGHTLIST_VACUUM, Work, Flags, X, Y - LeaderboardScreen.StartY + LeaderboardScreen.BaseY, Scale, Alpha);
}

static void DisplayLeaderboardScrollTime(int Time, float X, float Y, float Scale, float Alpha)
{
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 30.0f, 50.0f, 0.0f, 1.0f, true);
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 700.0f, 800.0f, 1.0f, 0.0f, true);
    if (Alpha < 0)
        return;
    DisplayTimeAlpha(X, Y - LeaderboardScreen.StartY + LeaderboardScreen.BaseY, Scale, Time, Alpha);
}

static void DisplayLeaderboardScrollSprite(gxSprite* Sprite, float X, float Y, float Scale, float Alpha)
{
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 30.0f, 50.0f, 0.0f, 1.0f, true);
    Alpha *= Remap(Y - LeaderboardScreen.StartY, 700.0f, 800.0f, 1.0f, 0.0f, true);
    if (Alpha < 0)
        return;
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, Sprite, X, Y - LeaderboardScreen.StartY + LeaderboardScreen.BaseY, Scale, Alpha);
}

void DisplayLeaderboardScreen()
{
    AddLitSpriteScaled(LIGHTLIST_VACUUM, &LeaderboardBackgroundSprite, LeaderboardBackgroundSprite.width, LeaderboardScreen.BaseY, -1.0f, 1.0f);
    
    // Leaderboard buttons.
    //AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonLeaderboardSprite, 768-100, LeaderboardScreen.BaseY + 60, 1.0f, 1.0f);
 
    float PrevAlpha = LeaderboardScreen.CurTab > 0 ? 1.0f : 0.5f;
    float NextAlpha = LeaderboardScreen.CurTab < LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount - 1 ? 1.0f : 0.5f;
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonFastForwardSprite, 192+16, LeaderboardScreen.BaseY + 920, -1.0f, PrevAlpha);
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonFastForwardSprite, 768-192-16, LeaderboardScreen.BaseY + 920, 1.0f, NextAlpha);        

//    float GhostAlpha = Settings.GhostActive ? 1.0f : 0.5f;
//    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &ButtonGhostSprite, 384, LeaderboardScreen.BaseY + 920, 1.0f, GhostAlpha);

    const char* RegionName[LEADERBOARDREGION_COUNT] = { "game center", "friends", "worldwide", "state" };
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

    DisplayLeaderboardScrollString("name", 0, 80, CurrentY, 0.6f, 1.0f);
    DisplayLeaderboardScrollString("pages", 0, 450, CurrentY, 0.6f, 1.0f);
    DisplayLeaderboardScrollString("hat", 0, 600, CurrentY, 0.6f, 1.0f);
    CurrentY += 55;

    for (int i = 0; i < LEADERBOARD_COUNT; i++)
    {
        if (!LeaderboardScreen.Name[i])
            continue;
        
        float Alpha = Remap((float)CurrentY - 300, 0.0f, LeaderboardScreen.FadeInTime * 5000.0f, 1.0f, 0.0f, true);
        
        DisplayLeaderboardScrollString(LeaderboardScreen.Name[i], 0, 80, CurrentY, 0.8f, Alpha);
        DisplayLeaderboardScrollNumber(LeaderboardScreen.Pages[i], FORMAT_ALIGN_RIGHT, 580, CurrentY, 1.0f, Alpha);
        
        if (LeaderboardScreen.Hat[i] != DUSTYHAT_NONE)
        {
            gxSprite* HatSprite = GetInventoryHatSprite(LeaderboardScreen.Hat[i]);
            if (HatSprite)
                DisplayLeaderboardScrollSprite(HatSprite, 640, CurrentY+30, 1.0f, Alpha);
        }
        
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
    
    int CurRegion = LeaderboardScreen.AvailableRegions[LeaderboardScreen.CurTab / LEADERBOARDMODE_COUNT];
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
    
    int CurRegion = LeaderboardScreen.AvailableRegions[LeaderboardScreen.CurTab / LEADERBOARDMODE_COUNT];
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
