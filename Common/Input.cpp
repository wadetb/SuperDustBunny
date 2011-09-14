//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Input.h"
#include "Settings.h"
#include "Recorder.h"


#define MAX_SWIPE_POINTS 256


struct SSwipe
{
    int Count;
    double StartTime;
    double Duration;
    double Current;
    bool Used;
};

struct SSwipePoint
{
    float X, Y;
    float Time;
};


SRemoteControl RemoteControl;

float AccelThreshold[3] = { 0.20f, 0.15f, 0.10f };

SSwipe Swipe;

SSwipePoint SwipePoints[MAX_SWIPE_POINTS];


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                         Tilt controls                                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //


bool GetInput_MoveLeft()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried when a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.MoveLeft;
    
#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_A);
#endif
#ifdef PLATFORM_IPHONE_OR_MAC
	return msAccelX < -AccelThreshold[0];
#endif
}

bool GetInput_MoveRight()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried while a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.MoveRight;
    
#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_D);
#endif
#ifdef PLATFORM_IPHONE_OR_MAC
	return msAccelX > AccelThreshold[0];
#endif
}

bool GetInput_Jump()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried while a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.Jump;
        
#ifdef PLATFORM_WINDOWS
    return kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE_OR_MAC
    return msButton1 && !msOldButton1;
#endif	
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                        Swipe controls                                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //

void GetInput_BeginSwipe(float X, float Y, double Time)
{
    Swipe.Count = 1;
    SwipePoints[0].X = X;
    SwipePoints[0].Y = Y;
    SwipePoints[0].Time = 0;
    Swipe.StartTime = Time;
    Swipe.Current = 0;
    Swipe.Used = false;
    
    if ( Recorder.RecordingActive )
        RecordSwipeEvent(EVENT_SWIPE_BEGIN, X, Y, 0);
    
#ifdef SWIPE_DEBUG
    printf("Begin swipe- %f %f\n", X, Y);
#endif
}

void GetInput_AddToSwipe(float X, float Y, double Time)
{
    // This occurs when the swipe is cleared in the middle.
    if ( Swipe.Count == 0 )
    {
        GetInput_BeginSwipe(X, Y, Time);
        return;
    }
    
    if ( Swipe.Count < MAX_SWIPE_POINTS )
    {
        SwipePoints[Swipe.Count].X = X;
        SwipePoints[Swipe.Count].Y = Y;
        SwipePoints[Swipe.Count].Time = (float)(Time - Swipe.StartTime);
        Swipe.Count++;
        Swipe.Duration = Time - Swipe.StartTime;

        if ( Recorder.RecordingActive )
            RecordSwipeEvent(EVENT_SWIPE_POINT, X, Y, (float)(Time - Swipe.StartTime));

#ifdef SWIPE_DEBUG
        printf("Add swipe- %f %f\n", X, Y);
#endif
    }
}

void GetInput_EndSwipe(float X, float Y, double Time)
{
    // This occurs when the swipe is cleared in the middle.
    if ( Swipe.Count == 0 )
        return;
    
    if ( Swipe.Count < MAX_SWIPE_POINTS )
    {
        SwipePoints[Swipe.Count].X = X;
        SwipePoints[Swipe.Count].Y = Y;
        SwipePoints[Swipe.Count].Time = (float)(Time - Swipe.StartTime);
        Swipe.Count++;    
        Swipe.Duration = Time - Swipe.StartTime;
        
        if ( Recorder.RecordingActive )
            RecordSwipeEvent(EVENT_SWIPE_END, X, Y, (float)(Time - Swipe.StartTime));

#ifdef SWIPE_DEBUG
        printf("End swipe- %f %f\n", X, Y);
#endif
    }
}

float GetInput_GetSwipeTimeLeft()
{
    return (float)( Swipe.Duration - Swipe.Current );
}

float GetInput_GetSwipeCurrent()
{
    return (float)Swipe.Current;
}

void GetInput_GetSwipePosAtTime(float* X, float* Y, double Time)
{
    if (Settings.ControlStyle != CONTROL_SWIPE)
        ReportError("Swipe controls queried while a different control style was in use.");
    
    if (Swipe.Count < 1)
    {
        ReportError("Swipe controls queried when no valid swipe existed.");
        return;
    }
    
    if (Swipe.Count < 2)
    {
        *X = SwipePoints[0].X;
        *Y = SwipePoints[0].X;
        return;
    }

    int Index = 0;
    while (Index < Swipe.Count-1)
    {
        if (SwipePoints[Index+1].Time > Time)
            break;
        Index++;
    }
    
    double Start = SwipePoints[Index].Time;
    double Duration = SwipePoints[Index+1].Time - Start;
    
    double Ratio = (Time - Start) / Duration;
    *X = (float)( SwipePoints[Index].X + (SwipePoints[Index+1].X - SwipePoints[Index].X) * Ratio );
    *Y = (float)( SwipePoints[Index].Y + (SwipePoints[Index+1].Y - SwipePoints[Index].Y) * Ratio );
}

bool GetInput_CheckSwipeStraightness(float StartTime, float EndTime, float MaxAngle)
{
    int StartIndex = 0;
    while (StartIndex < Swipe.Count-1)
    {
        if (SwipePoints[StartIndex+1].Time > StartTime)
            break;
        StartIndex++;
    }
    
    int EndIndex = StartIndex;
    while (EndIndex < Swipe.Count-1)
    {
        if (SwipePoints[EndIndex+1].Time > EndTime)
            break;
        EndIndex++;
    }
    
    float CosMaxAngle = cosf(DegreesToRadians(MaxAngle));
    
    if (EndIndex - StartIndex < 2)
        return true;
    
    for (int i = StartIndex; i <= EndIndex-2; i++)
    {
        float DX1 = SwipePoints[i+1].X - SwipePoints[i].X;
        float DY1 = SwipePoints[i+1].Y - SwipePoints[i].Y;
        float L1 = sqrtf(DX1*DX1 + DY1*DY1);
        if (L1 >= 0.0001f)
        {
            DX1 /= L1;
            DY1 /= L1;
        }

        float DX2 = SwipePoints[i+2].X - SwipePoints[i+1].X;
        float DY2 = SwipePoints[i+2].Y - SwipePoints[i+1].Y;
        float L2 = sqrtf(DX2*DX2 + DY2*DY2);
        if (L1 >= 0.0001f)
        {
            DX2 /= L2;
            DY2 /= L2;
        }
        
        float CosAngle = DX1*DX2 + DY1*DY2;
        
        if (CosAngle < CosMaxAngle)
            return false;
    }
    
    return true;
}

void GetInput_ConsumeSwipe(float Time)
{
    Swipe.Current += Time;
}

void GetInput_ConsumeAllSwipe()
{
    Swipe.Current = Swipe.Duration;
}

void GetInput_ClearSwipe()
{
    // Swipe clear on landing is currently disabled.  
    // Probably need to reenable this and remove the calls from Dusty instead, since this will be needed by menus etc.
#if 0
    Swipe.Count = 0;
    Swipe.StartTime = 0;
    Swipe.Current = 0;
    Swipe.Duration = 0;
#endif
}

void GetInput_SetSwipeUsed()
{
    Swipe.Used = true;
}

bool GetInput_IsSwipedUsed()
{
    return Swipe.Used;
}

void DisplaySwipe()
{
#ifdef SWIPE_DEBUG
    for (int i = 0; i < Swipe.Count-1; i++)
    {        
        if ( Swipe.Current < SwipePoints[i].Time )
        {
            DisplayDebugLine(SwipePoints[i].X, SwipePoints[i].Y, SwipePoints[i+1].X, SwipePoints[i+1].Y, 4.0f, gxRGBA32(128, 128, 192, 255));
        }
        else if ( Swipe.Current < SwipePoints[i+1].Time )
        {
            float Duration = SwipePoints[i+1].Time - SwipePoints[i].Time;
            float Ratio = ( Swipe.Current - SwipePoints[i].Time ) / Duration;
            
            DisplayDebugLine(SwipePoints[i].X, SwipePoints[i].Y,
                             SwipePoints[i].X + ( SwipePoints[i+1].X - SwipePoints[i].X ) * Ratio, 
                             SwipePoints[i].Y + ( SwipePoints[i+1].Y - SwipePoints[i].Y ) * Ratio, 
                             4.0f, gxRGBA32(128, 192, 128, 255));            

            DisplayDebugLine(SwipePoints[i].X + ( SwipePoints[i+1].X - SwipePoints[i].X ) * Ratio, 
                             SwipePoints[i].Y + ( SwipePoints[i+1].Y - SwipePoints[i].Y ) * Ratio, 
                             SwipePoints[i+1].X, SwipePoints[i+1].Y,
                             4.0f, gxRGBA32(128, 128, 192, 255));            
        }
        else
        {
            DisplayDebugLine(SwipePoints[i].X, SwipePoints[i].Y, SwipePoints[i+1].X, SwipePoints[i+1].Y, 4.0f, gxRGBA32(128, 192, 128, 255));            
        }
    }
    
	//gxDrawString(5, 37, 16, gxRGB32(255, 255, 255), "Active:%d Valid:%d Count:%d", Swipe.Active, Swipe.Valid, Swipe.Count);
#endif
}

