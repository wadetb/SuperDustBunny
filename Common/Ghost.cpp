//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Ghost.h"
#include "Lighting.h"
#include "Dusty.h"
#include "Chapter.h"


#define MAX_GHOST_EVENTS 20000


struct SGhostEvent
{
    float X;
    float Y;
    float ScaleX;
    int Sprite;
};

struct SGhost
{
    bool RecordingActive;
    bool PlaybackActive;
    
    int RecordingEventCount;
    int PlaybackEventCount;
    int PlaybackTime;
};


SGhost Ghost;

SGhostEvent GhostRecordingEvents[MAX_GHOST_EVENTS];
SGhostEvent GhostPlaybackEvents[MAX_GHOST_EVENTS];


void AddGhostEvent(float X, float Y, float ScaleX, int Sprite)
{
    if (Ghost.RecordingEventCount >= MAX_GHOST_EVENTS)
        return;
    
    SGhostEvent* Event = &GhostRecordingEvents[Ghost.RecordingEventCount++];
    
    Event->X = X;
    Event->Y = Y;
    Event->ScaleX = ScaleX;
    Event->Sprite = Sprite;
}

void LoadGhost(const char* ChapterName, int Page, bool Race)
{
    Ghost.PlaybackEventCount = 0;
    
#ifdef PLATFORM_IPHONE_OR_MAC
    @try
    {
        NSString *filePath;
        if (Race)
        {
#ifdef PLATFORM_MAC
            extern char RootDirectory[1024];
            filePath = [NSString stringWithFormat:@"%s/Chapters/%s/%s_%d.ghost", RootDirectory, ChapterName, ChapterName, Page];
#endif
#ifdef PLATFORM_IPHONE
            filePath =  [NSString stringWithFormat:@"%s/Chapters~%s~%s_%d.ghost", [[NSBundle mainBundle] bundlePath], ChapterName, ChapterName, Page];
#endif            
        }
        else
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString *documentsDirectory = [paths objectAtIndex:0];
            NSString *fileName = [NSString stringWithFormat:@"%s_%d.ghost", ChapterName, Page];
            filePath = [documentsDirectory stringByAppendingPathComponent:fileName];            
        }
        
        NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:filePath];
        if ( !dict )
        {
            NSLog(@"Failed to load dictionary from ghost data file.\n");
            return;
        }
        
        NSNumber *version = [dict objectForKey:@"version"];
        if ( [version intValue] != 1 )
        {
            NSLog(@"Ghost dictionary file is wrong version.  Got %d, expected %d.\n", [version intValue], 1);
            return;
        }
        
        NSData* data = [dict objectForKey:@"events"];
        int eventCount = [[dict objectForKey:@"eventCount"] intValue];

        if (eventCount > MAX_GHOST_EVENTS)
        {
            NSLog(@"Ghost file contains too many events.  Got %d, maximum is %d.\n", eventCount, MAX_GHOST_EVENTS);
            return;
        }

        if ([data length] != sizeof(SGhostEvent)*eventCount)
        {
            NSLog(@"Ghost file appears truncated.\n");
            return;
        }
        
        Ghost.PlaybackEventCount = eventCount;
        memcpy(GhostPlaybackEvents, [data bytes], sizeof(SGhostEvent)*eventCount);
    }
    @catch (NSException *exception) 
    {
        Ghost.PlaybackEventCount = 0;
    }
#endif
}

void SaveGhost(const char* ChapterName, int Page)
{
#ifdef PLATFORM_IPHONE_OR_MAC
    @try
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *fileName = [NSString stringWithFormat:@"%s_%d.ghost", ChapterName, Page];
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];
        
        NSData* events = [NSData dataWithBytes:GhostRecordingEvents length:sizeof(SGhostEvent)*Ghost.RecordingEventCount];
        
        NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithInt:1], @"version",
                              [NSNumber numberWithInt:Ghost.RecordingEventCount], @"eventCount",
                              events, @"events", nil];
        
        [dict writeToFile:filePath atomically:YES];
    }
    @catch (NSException *e)
    {
    }
#endif
}

bool IsGhostRecordingActive()
{
    return Ghost.RecordingActive;
}

bool IsGhostPlaybackActive()
{
    return Ghost.PlaybackActive;
}

void StartGhostRecording()
{
    Ghost.RecordingEventCount = 0;
    
    Ghost.RecordingActive = true;
}

void StartGhostPlayback()
{
    Ghost.PlaybackTime = 0;

    Ghost.PlaybackActive = true;
}

void InitGhost()
{
    Ghost.PlaybackTime = 0;
    Ghost.PlaybackEventCount = 0;
    Ghost.RecordingEventCount = 0;
    
    Ghost.RecordingActive = false;
    Ghost.PlaybackActive = false;
}

void DisplayGhost()
{
    if (Ghost.PlaybackActive && Ghost.PlaybackTime < Ghost.PlaybackEventCount)
    {
        SGhostEvent* Event = &GhostPlaybackEvents[Ghost.PlaybackTime];
        
        unsigned int Color = gxRGBA32(0x80, 0xc0, 0xff, 0xc0);
        AddLitSpriteScaledColor(LIGHTLIST_FOREGROUND, DustySprite[Event->Sprite], Event->X + ScrollX, Event->Y + ScrollY, Event->ScaleX, 1.0f, Color);
    }
}

void UpdateGhost()
{
    if (Ghost.PlaybackActive)
    {
        Ghost.PlaybackTime++;
        if (Ghost.PlaybackTime >= Ghost.PlaybackEventCount)
        {
            Ghost.PlaybackTime = 0;
            Ghost.PlaybackActive = false;
            
            if (Chapter.PageProps.GhostRace)
            {
                SetDustyState_Die(DEATH_GHOST);
            }
        }
    }
}
