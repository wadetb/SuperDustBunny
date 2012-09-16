//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//
#include "Common.h"
#include "Music.h"


#define PLAYLIST_MAX 2


struct SMusicTrack
{
    SMusicAsset* Playlist[PLAYLIST_MAX];
};


SMusicTrack MusicTracks[MUSIC_COUNT];


void QueueMusicTrack(EMusicTrack Track, SMusicAsset* Asset)
{
    SMusicTrack* Music = &MusicTracks[Track];
    
#ifdef PLATFORM_IPHONE_OR_MAC
    NSTimeInterval totalDurationLeft = 0;
    
    for (int i = 0; i < PLAYLIST_MAX; i++)
    {
        if (Music->Playlist[i])
        {
            AVAudioPlayer* existingPlayer = Music->Playlist[i]->Player;
            if ([existingPlayer numberOfLoops] < 0)
                ReportError("Cannot queue music track '%s' while a looping sound '%s' is still playing.  Stop all other music first.",
                            Asset->FileName, Music->Playlist[i]->FileName);
            NSTimeInterval durationLeft = ( [existingPlayer duration] * ([existingPlayer numberOfLoops]+1) ) - [existingPlayer currentTime];
            if (durationLeft <= 0)
                Music->Playlist[i] = NULL;
            totalDurationLeft += durationLeft;
        }
    }
    
    AVAudioPlayer* player = Asset->Player;
    [player setCurrentTime:0];
    [player playAtTime:[player deviceCurrentTime] + totalDurationLeft];
#endif
    
    int PlaylistIndex;
    for (PlaylistIndex = 0; PlaylistIndex < PLAYLIST_MAX; PlaylistIndex++)
    {
        if (Music->Playlist[PlaylistIndex] == NULL)
        {
            Music->Playlist[PlaylistIndex] = Asset;
            break;
        }
    }
    if (PlaylistIndex == PLAYLIST_MAX)
        ReportError("Tried to queue more than %d songs on music channel %d, when queueing track '%s'.", PLAYLIST_MAX, Track, Asset->FileName);
}

void StopMusicTrack(EMusicTrack Track)
{
    SMusicTrack* Music = &MusicTracks[Track];
    
    int PlaylistIndex;
    for (PlaylistIndex = 0; PlaylistIndex < PLAYLIST_MAX; PlaylistIndex++)
    {
        if (Music->Playlist[PlaylistIndex])
        {
#ifdef PLATFORM_IPHONE_OR_MAC
            AVAudioPlayer* existingPlayer = Music->Playlist[PlaylistIndex]->Player;
            [existingPlayer stop];
#endif
            Music->Playlist[PlaylistIndex] = NULL;
        }
    }
}

void UpdateMusic()
{
}
