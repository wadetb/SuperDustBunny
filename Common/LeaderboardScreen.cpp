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

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


#define LEADERBOARD_COUNT 25


enum ELeaderboardRegion
{
    LEADERBOARDREGION_WORLD,
    LEADERBOARDREGION_STATE,
    LEADERBOARDREGION_CITY,
    LEADERBOARDREGION_COUNT
};

enum ELeaderboardMode
{
    LEADERBOARDMODE_ALLTIME,
    LEADERBOARDMODE_KINGOFTHEHILL,
    LEADERBOARDMODE_COUNT
};


struct SLeaderboardScreen
{
    float WiggleTime;
    float FadeInTime;
    
    float StartX;
    float StartY;
    
    bool Dragging;
    float DragStartX;
    float DragStartY;
    
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
    const char* RegionTag[LEADERBOARDREGION_COUNT] = { "world", "state", "city" };
    const char* ModeTag[LEADERBOARDMODE_COUNT] = { "alltime", "kingofthehill" };
    
    NSString* URLString;
#ifdef PLATFORM_IPHONE
    if (theViewController.haveLocation && (LeaderboardScreen.Region == LEADERBOARDREGION_CITY || LeaderboardScreen.Region == LEADERBOARDREGION_STATE))
    {
        NSString* region;
        if (LeaderboardScreen.Region == LEADERBOARDREGION_CITY)
            region = theViewController.city;
        else
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
            
            if (atoi([time UTF8String]))
            {
                LeaderboardScreen.Name[i] = strdup([[name lowercaseString] UTF8String]);
                LeaderboardScreen.Time[i] = atoi([time UTF8String]);
            }
        }
    }
#endif
}

void InitLeaderboardScreen()
{
    LeaderboardScreen.StartX = 0;
    LeaderboardScreen.StartY = 0;
    LeaderboardScreen.Dragging = false;
    
#ifdef PLATFORM_IPHONE
    [theViewController retrieveLocationAndDownloadLeaderboards];
#else
    DownloadLeaderboards();
#endif
}

void DisplayLeaderboardScreen()
{
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 0);
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 1024);   
    
    // Leaderboard buttons.
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &IconCredits1Sprite, 90 + sinf(LeaderboardScreen.WiggleTime)*4.0f, 90, 0.25f, 1.0f);

    const char* RegionName[LEADERBOARDREGION_COUNT] = { "worldwide", "state", "city" };
    const char* ModeName[LEADERBOARDMODE_COUNT] = { "top score", "king of the day" };

    int CurrentX = LeaderboardScreen.StartX;
    
    for (int RegionIndex = 0; RegionIndex < LeaderboardScreen.AvailableRegionCount; RegionIndex++)
    {
        ELeaderboardRegion Region = LeaderboardScreen.AvailableRegions[RegionIndex];
        
        for (int Mode = 0; Mode < LEADERBOARDMODE_COUNT; Mode++)
        {
            const char* LocalRegionName = RegionName[Region];
#ifdef PLATFORM_IPHONE
            if (theViewController.haveLocation && (Region == LEADERBOARDREGION_CITY || Region == LEADERBOARDREGION_STATE))
            {
                if (Region == LEADERBOARDREGION_CITY)
                    LocalRegionName = [[theViewController.city lowercaseString] UTF8String];
                else
                    LocalRegionName = [[theViewController.state lowercaseString] UTF8String];
            }
#endif
            DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, LocalRegionName, FORMAT_CENTER_X, 384 + CurrentX, -LeaderboardScreen.StartY + 50, 0.8f);

            DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, ModeName[Mode], FORMAT_CENTER_X, 384 + CurrentX, -LeaderboardScreen.StartY + 130, 1.0f);
            
            if (Region == LeaderboardScreen.Region && Mode == LeaderboardScreen.Mode)
            {
                int CurrentY = 250;
                
                for (int i = 0; i < LEADERBOARD_COUNT; i++)
                {
                    if (!LeaderboardScreen.Name[i])
                        continue;
                    
                    float Alpha = Remap((float)CurrentY - 300, 0.0f, LeaderboardScreen.FadeInTime * 5000.0f, 1.0f, 0.0f, true);
                    DisplayStringAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, LeaderboardScreen.Name[i], 0, 80 + CurrentX, CurrentY-LeaderboardScreen.StartY, 0.8f, Alpha);
                    DisplayTimeAlpha(450 + CurrentX, CurrentY-LeaderboardScreen.StartY, 0.6f, LeaderboardScreen.Time[i], Alpha);
                    
                    CurrentY += 75;
                }
            }
            
            CurrentX += 768;
        }
    }

}

void UpdateLeaderboardScreen()
{
    if (msY < 256 && msX < 384 && !msButton1 && msOldButton1)
    {
#ifdef PLATFORM_IPHONE
        [TestFlight passCheckpoint:[NSString stringWithFormat:@"Closed Leaderboards", Chapters[CurrentChapter].Name]];
#endif
        SetGameState_StartScreen();
        return;
    }

    LeaderboardScreen.AvailableRegionCount = 0;
    
    LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_WORLD;
#ifdef PLATFORM_IPHONE
    if (theViewController.haveLocation)
    {
        LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_STATE;
        LeaderboardScreen.AvailableRegions[LeaderboardScreen.AvailableRegionCount++] = LEADERBOARDREGION_CITY;
    }
#endif
    // TODO if (HaveFacebook), if (HaveGameCenter)

    if (LeaderboardScreen.Dragging)
    {
        if (!msButton1)
        {
            LeaderboardScreen.Dragging = false;
        }
        
        LeaderboardScreen.StartY -= msY - LeaderboardScreen.DragStartY;
        LeaderboardScreen.DragStartY = msY;
        
        LeaderboardScreen.StartX += (msX - LeaderboardScreen.DragStartX) * 2.5f;
        LeaderboardScreen.DragStartX = msX;
        
        if (LeaderboardScreen.StartX > 100)
            LeaderboardScreen.StartX = 100;
        if (LeaderboardScreen.StartX < -((LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount - 1) * 768) - 100 )
            LeaderboardScreen.StartX = -((LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount - 1) * 768) - 100;
    }
    else
    {
        if (msButton1)
        {
            LeaderboardScreen.Dragging = true;
            LeaderboardScreen.DragStartX = msX;
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
        
        float TargetX = -((LeaderboardScreen.Region * LEADERBOARDMODE_COUNT) + LeaderboardScreen.Mode) * 768;
        LeaderboardScreen.StartX  = LeaderboardScreen.StartX*0.9f + TargetX*0.1f;
    }
    
    int CurTab = (-LeaderboardScreen.StartX+384) / 768;

    if (CurTab < 0)
        CurTab = 0;
    if (CurTab >= LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount)
        CurTab = (LEADERBOARDMODE_COUNT * LeaderboardScreen.AvailableRegionCount) - 1;
    
    int CurRegion = CurTab / LEADERBOARDMODE_COUNT;
    int CurMode = CurTab % LEADERBOARDMODE_COUNT;

    if (CurRegion != LeaderboardScreen.Region || CurMode != LeaderboardScreen.Mode)
    {
        LeaderboardScreen.Region = (ELeaderboardRegion)CurRegion;
        LeaderboardScreen.Mode = (ELeaderboardMode)CurMode;
        NSLog(@"Region=%d Mode=%d\n", CurRegion, CurMode);
        DownloadLeaderboards();
    }
    
    LeaderboardScreen.WiggleTime += 1.0f/60.0f;
    LeaderboardScreen.FadeInTime += 1.0f/60.0f;
}
