//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef GHOST_H
#define GHOST_H

void AddGhostEvent(float X, float Y, float ScaleX, int Sprite);

void LoadGhost(const char* ChapterName, const char* PageName, bool Race);
void SaveGhost(const char* ChapterName, const char* PageName, bool Race);

bool IsGhostRecordingActive();
bool IsGhostPlaybackActive();

void StartGhostRecording();
void StartGhostPlayback();

void InitGhost();
void DisplayGhost();
void UpdateGhost();

#endif
