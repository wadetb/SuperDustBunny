//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef MUSIC_H
#define MUSIC_H

enum EMusicTrack
{
    MUSIC_CHAPTER,
    MUSIC_COUNT
};

void QueueMusicTrack(EMusicTrack Track, SMusicAsset* Asset);
void StopMusicTrack(EMusicTrack Track);

void UpdateMusic();

#endif