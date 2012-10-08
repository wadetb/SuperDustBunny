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
	RESULT_RESTART_PAGE,
	RESULT_NEXT_PAGE,
	RESULT_CHAPTER_END,
	RESULT_DIE,
	RESULT_QUIT,
};

enum ERecordingEventType
{
    EVENT_TILT           = 0,
    
    EVENT_SWIPE_BEGIN    = 1,
    EVENT_SWIPE_POINT    = 2,
    EVENT_SWIPE_END      = 3,

    EVENT_GHOST          = 4,
};

struct SRecorder
{
	bool RecordingActive;

	bool PlaybackActive;

	bool MoveLeft;
	bool MoveRight;
	bool Jump;
    
    int SumFPS;
    int FPSCount;
};

#define BUILD_PRERELEASE (1<<31)

extern unsigned int BuildNumber;

extern SRecorder Recorder;

bool IsRecordingActive();

void StartRecordingSession();

void StartRecording();
void StopRecording(ERecordingEndType Result);

bool IsPlaybackActive();

void StartPlayback();
void StopPlayback();

void UpdateRecorder();

void RecordSwipeEvent(int Type, float X, float Y, float Time);
void RecordTiltEvents();
void RecordGhostEvent(float X, float Y, int Sprite);

void UploadRecording();
void DownloadRecording(int id);

char* GetRecordingChapterName();
char* GetRecordingPageName();
int GetRecordingStartLives();

enum EMinimapAction
{
    MINIMAP_NORMAL,
    MINIMAP_HEAD_BUMP,
    MINIMAP_WALL_JUMP,
    MINIMAP_CORNER_JUMP,
    MINIMAP_JUMP,
    MINIMAP_LAUNCH,
    MINIMAP_LAND,
    MINIMAP_FALL,
    MINIMAP_DIE,
    MINIMAP_WIN,
    MINIMAP_EXTRA_LIFE,
    MINIMAP_JAM,
    MINIMAP_HURT,
    MINIMAP_STUCK,
};

void InitMinimap();
void UpdateMinimap(EMinimapAction Action);

#endif