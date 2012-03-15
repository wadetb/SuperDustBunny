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

void LoadGhost(const char* ChapterName, const char* PageName, bool Race)
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
            filePath = [NSString stringWithFormat:@"%s/Chapters/%s/%s.ghost", RootDirectory, ChapterName, PageName];
#endif
#ifdef PLATFORM_IPHONE
            filePath =  [NSString stringWithFormat:@"%@/Converted/Chapters~%s~%s.ghost", [[NSBundle mainBundle] bundlePath], ChapterName, PageName];
#endif            
        }
        else
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString *documentsDirectory = [paths objectAtIndex:0];
            NSString *fileName = [NSString stringWithFormat:@"%s_%s.ghost", ChapterName, PageName];
            filePath = [documentsDirectory stringByAppendingPathComponent:fileName];            
        }
        
        NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:filePath];
        if ( !dict )
        {
            NSLog(@"Failed to load dictionary from ghost data file %@.\n", filePath);
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

void SaveGhost(const char* ChapterName, const char* PageName, bool Race)
{
#ifdef PLATFORM_IPHONE_OR_MAC
    @try
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *fileName;
        if (Race)
            fileName = [NSString stringWithFormat:@"%s.ghost", PageName];
        else
            fileName = [NSString stringWithFormat:@"%s_%s.ghost", ChapterName, PageName];
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
        
        unsigned int Color;
        
        if ( Chapter.PageProps.GhostRace)
            Color = gxRGBA32(255, 255, 255, 255);
        else
            Color = gxRGBA32(0x80, 0x80, 0x80, 0xc0);
            
        AddLitSpriteScaledColor(LIGHTLIST_FOREGROUND, DustySprite[Event->Sprite], Event->X + ScrollX, Event->Y + ScrollY, Event->ScaleX, 1.0f, Color);
        
        if (Chapter.PageProps.GhostHat != DUSTYHAT_NONE)
        {
            gxSprite* HatSprite = DustyHatSprites[Chapter.PageProps.GhostHat];
            SDustyHatOffset* HatOffset = &DustyHatOffsets[Event->Sprite];
            
            AddLitSpriteCenteredScaled2Rotated(LIGHTLIST_FOREGROUND, HatSprite, 
                                               Event->X + ScrollX + HatOffset->X*Event->ScaleX, Event->Y + ScrollY + HatOffset->Y, 
                                               Event->ScaleX, 1.0f, DegreesToRadians(HatOffset->Angle)*Event->ScaleX);
        }
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
//                if (Ghost.PlaybackEventCount > 0)
//                    SetDustyState_Die(DEATH_GHOST);
            }
        }

        // Catch the ghost up automatically.
        if (Chapter.PageProps.GhostRace)
        {
            SGhostEvent* Event = &GhostPlaybackEvents[Ghost.PlaybackTime];
            if (Ghost.PlaybackTime < Ghost.PlaybackEventCount && Event->Y >= Dusty.FloatY + 200)
            {
                Ghost.PlaybackTime++;
                //Event = &GhostPlaybackEvents[Ghost.PlaybackTime];
            }
        }
    }
}
