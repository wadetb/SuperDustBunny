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
#include "Tutorial.h"


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                             Input functions
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

SRemoteControl RemoteControl;


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                         Tilt controls                                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //


float AccelThreshold[3] = { 0.20f, 0.15f, 0.10f };


bool GetInput_MoveLeft()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried when a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.MoveLeft;
    
	if (RemoteControl.Enabled)
		return RemoteControl.MoveLeft;
    
#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_A);
#endif
#ifdef PLATFORM_IPHONE
	return msAccelX < -AccelThreshold[Settings.TiltSensitivity];
#endif
}

bool GetInput_MoveRight()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried while a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.MoveRight;
    
	if (RemoteControl.Enabled)
		return RemoteControl.MoveRight;
    
#ifdef PLATFORM_WINDOWS
	return kbIsKeyDown(KB_D);
#endif
#ifdef PLATFORM_IPHONE
	return msAccelX > AccelThreshold[Settings.TiltSensitivity];
#endif
}

bool GetInput_Jump()
{
    if (Settings.ControlStyle != CONTROL_TILT)
        ReportError("Tilt controls queried while a different control style was in use.");
    
	if (IsPlaybackActive())
		return Recorder.Jump;
    
	if (RemoteControl.Enabled)
		return RemoteControl.Jump;
    
	// Jump inhibitor is used to make the game ignore the jump key until it's released.
	if (Tutorial.JumpInhibit)
	{
#ifdef PLATFORM_WINDOWS
		if (!kbIsKeyDown(KB_SPACE))
			Tutorial.JumpInhibit = false;
#endif
#ifdef PLATFORM_IPHONE
		if (!msButton1)
			Tutorial.JumpInhibit = false;
#endif
	}
	if (Tutorial.JumpInhibit)
		return false;
    
    if (Settings.ContinuousJump)
    {
#ifdef PLATFORM_WINDOWS
        return kbIsKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
        return msButton1;
#endif	
    }
    else
    {
#ifdef PLATFORM_WINDOWS
        return kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
        return msButton1 && !msOldButton1;
#endif	
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                        Swipe controls                                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   //

#define SWIPE_DEBUG


SSwipe Swipe;

SSwipePoint SwipePoints[MAX_SWIPE_POINTS];


void GetInput_BeginSwipe(float X, float Y, double Time)
{
    Swipe.Active = true;
    Swipe.Valid = false;
    Swipe.Count = 1;
    SwipePoints[0].X = X;
    SwipePoints[0].Y = Y;
    SwipePoints[0].Time = 0;
    Swipe.StartTime = Time;
    Swipe.ValidCount = 0;
}

void GetInput_AddToSwipe(float X, float Y, double Time)
{
    SwipePoints[Swipe.Count].X = (float)msX;
    SwipePoints[Swipe.Count].Y = (float)msY;
    SwipePoints[Swipe.Count].Time = (float)(Time - Swipe.StartTime);
    Swipe.Count++;
    Swipe.Duration = Time - Swipe.StartTime;
}

void GetInput_EndSwipe(float X, float Y, double Time)
{
    SwipePoints[Swipe.Count].X = (float)msX;
    SwipePoints[Swipe.Count].Y = (float)msY;
    SwipePoints[Swipe.Count].Time = (float)(Time - Swipe.StartTime);
    Swipe.Count++;    
    Swipe.Duration = Time - Swipe.StartTime;
    Swipe.Valid = true;
}

void GetInput_NextSwipeDir(float* dX, float* dY)
{
    if (Settings.ControlStyle != CONTROL_SWIPE)
        ReportError("Swipe controls queried while a different control style was in use.");
    
    if (!Swipe.Valid)
    {
        *dX = 0;
        *dY = 0;
        return;
    }
    
    *dX = SwipePoints[1].X - SwipePoints[0].X;
    *dY = SwipePoints[1].Y - SwipePoints[0].Y;
}

void GetInput_AverageSwipeDir(float* dX, float* dY)
{
    if (Settings.ControlStyle != CONTROL_SWIPE)
        ReportError("Swipe controls queried while a different control style was in use.");
    
    if (!Swipe.Valid)
    {
        *dX = 0;
        *dY = 0;
        return;
    }
    
    *dX = SwipePoints[Swipe.Count-1].X - SwipePoints[0].X;
    *dY = SwipePoints[Swipe.Count-1].Y - SwipePoints[0].Y;
}

bool GetInput_CheckSwipeDir(float Angle, float Range)
{
    return false;
    
    if (Settings.ControlStyle != CONTROL_SWIPE)
        ReportError("Swipe controls queried while a different control style was in use.");
    
    if (!Swipe.Valid)
        return false;
    
    float dX, dY;
    GetInput_NextSwipeDir(&dX, &dY);
    
    float L = sqrtf(dX*dX + dY*dY);
    if (L < 10.0f)
        return false;
    
    dX /= L;
    dY /= L;
    
    float aX = cosf(DegreesToRadians(Angle));
    float aY = -sinf(DegreesToRadians(Angle));
    
    float Dot = aX*dX + aY*dY;
    float cR = cosf(Range);
    
    return Dot >= cR;
}

void GetInput_ConsumeSwipe(float Dist)
{
    while (Dist > 0 && Swipe.Count > 1)
    {
        float Length = Distance(SwipePoints[0].X, SwipePoints[0].Y, SwipePoints[1].X, SwipePoints[1].Y);
        
        if (Dist >= Length)
        {
            for (int i = 0; i < Swipe.Count-1; i++)
                SwipePoints[i] = SwipePoints[i+1];
            
            Swipe.Count--;
            
            Dist -= Length;
            
            if (Swipe.Count <= 1)
            {
                Swipe.Count = 0;
                Swipe.Valid = false;
                break;
            }                
        }
        else
        {
            SwipePoints[0].X = SwipePoints[0].X + (SwipePoints[1].X - SwipePoints[0].X) * Dist / Length;
            SwipePoints[0].Y = SwipePoints[0].Y + (SwipePoints[1].Y - SwipePoints[0].Y) * Dist / Length;
            
            Dist = 0;
        }
    }
}

void GetInput_ConsumeAllSwipe()
{
    Swipe.Count = 0;
    Swipe.Valid = false;
}

bool GetInput_SwipeValid()
{
    return Swipe.Valid;
}

void DisplayDebugLine(float X1, float Y1, float X2, float Y2, float Width, unsigned int Color);

void DisplaySwipe()
{
#ifdef SWIPE_DEBUG
    for (int i = 1; i < Swipe.Count; i++)
        DisplayDebugLine(SwipePoints[i-1].X, SwipePoints[i-1].Y, SwipePoints[i].X, SwipePoints[i].Y, 4.0f, 
                         i < Swipe.ValidCount ? gxRGBA32(192, 128, 128, 255) : gxRGBA32(128, 192, 128, 255));
    
	//gxDrawString(5, 37, 16, gxRGB32(255, 255, 255), "Active:%d Valid:%d Count:%d", Swipe.Active, Swipe.Valid, Swipe.Count);
#endif
}

