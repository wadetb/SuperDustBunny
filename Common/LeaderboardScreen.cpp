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
    
    float StartX;
    float StartY;
    
    bool Dragging;
    float DragStartX;
    float DragStartY;
    
    ELeaderboardRegion Region;
    ELeaderboardMode Mode;

    char* Name[LEADERBOARD_COUNT];
    int Time[LEADERBOARD_COUNT];
};


SLeaderboardScreen LeaderboardScreen;


static void DownloadLeaderboards()
{
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
    
    NSString* URLString = [NSString stringWithFormat:@"http://pluszerogames.com/sdb/getleaderboard.php?zone=%s_%s&chapter=%s", 
                           RegionTag[LeaderboardScreen.Region], ModeTag[LeaderboardScreen.Mode], Chapters[CurrentChapter].Name];
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
    
    DownloadLeaderboards();
}

void DisplayLeaderboardScreen()
{
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 0);
    AddLitSprite(LIGHTLIST_BACKGROUND, &BackgroundFridgeSprite, 0, 1024);   
    
    // Leaderboard buttons.
    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_VACUUM, &IconCredits1Sprite, 90 + sinf(LeaderboardScreen.WiggleTime)*4.0f, 90, 0.25f, 1.0f);

    int CurrentY = LeaderboardScreen.StartY;
    
    const char* RegionName[LEADERBOARDREGION_COUNT] = { "worldwide", "state", "city" };
    const char* ModeName[LEADERBOARDMODE_COUNT] = { "top score", "king of the hill" };

    CurrentY += 50;

    DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, RegionName[LeaderboardScreen.Region], FORMAT_CENTER_X, 384 + LeaderboardScreen.StartX, CurrentY, 0.9f);
    CurrentY += 100;
    DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, ModeName[LeaderboardScreen.Mode], FORMAT_CENTER_X, 384 + LeaderboardScreen.StartX, CurrentY, 0.8f);
    CurrentY += 150;
    
    for (int i = 0; i < LEADERBOARD_COUNT; i++)
    {
        if (!LeaderboardScreen.Name[i])
            continue;
        
        DisplayString(LIGHTLIST_FOREGROUND_NO_SHADOW, LeaderboardScreen.Name[i], 0, 80 + LeaderboardScreen.StartX, CurrentY, 0.8f);
        DisplayTime(450 + LeaderboardScreen.StartX, CurrentY, 0.6f, LeaderboardScreen.Time[i]);
        
        CurrentY += 75;
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

    if (LeaderboardScreen.Dragging)
    {
        if (!msButton1)
        {
            LeaderboardScreen.Dragging = false;
        }
        
        LeaderboardScreen.StartY += msY - LeaderboardScreen.DragStartY;
        LeaderboardScreen.DragStartY = msY;
        
        if (!LeaderboardScreen.Dragging && fabsf(msX - LeaderboardScreen.DragStartX) > 100)
        {
            if (msX < LeaderboardScreen.DragStartX)
            {
                if (LeaderboardScreen.Region > 0)
                    LeaderboardScreen.Region = (ELeaderboardRegion)(LeaderboardScreen.Region-1);
/*                else
                if (LeaderboardScreen.Mode > 0)
                {
                    LeaderboardScreen.Mode = (ELeaderboardMode)(LeaderboardScreen.Mode-1);
                    LeaderboardScreen.Region = (ELeaderboardRegion)(LEADERBOARDREGION_COUNT - 1);
                }*/
                
                LeaderboardScreen.StartX = -384;
            }
            else
            {
                if (LeaderboardScreen.Region < LEADERBOARDREGION_COUNT - 1)
                    LeaderboardScreen.Region = (ELeaderboardRegion)(LeaderboardScreen.Region+1);
/*                else
                if (LeaderboardScreen.Mode < LEADERBOARDMODE_COUNT - 1)
                {
                    LeaderboardScreen.Mode = (ELeaderboardMode)(LeaderboardScreen.Mode+1);
                    LeaderboardScreen.Region = (ELeaderboardRegion)(0);
                }*/
                
                LeaderboardScreen.StartX = 384;
            }
            
            DownloadLeaderboards();
        }
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
            LeaderboardScreen.StartY += ((TotalHeight - LitScreenHeight) - LeaderboardScreen.StartY) * 0.1f;
        
        LeaderboardScreen.StartX *= 0.95f;
    }
    
    LeaderboardScreen.WiggleTime += 1.0f/60.0f;
}
