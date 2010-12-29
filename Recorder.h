//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef RECORDER_H
#define RECORDER_H

enum ERecordingEndType
{
	RESULT_NONE = 0,
	RESULT_NEXT_PAGE,
	RESULT_CHAPTER_END,
	RESULT_DIE,
	RESULT_QUIT,
};

struct SRecorder
{
	bool RecordingActive;

	bool PlaybackActive;
	bool MoveLeft;
	bool MoveRight;
	bool Jump;
};

extern SRecorder Recorder;

bool IsRecordingActive();

void StartRecording();
void StopRecording(ERecordingEndType Result);

bool IsPlaybackActive();

void StartPlayback();
void StopPlayback();

void UpdateRecorder();

#endif