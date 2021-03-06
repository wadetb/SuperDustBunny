//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Debug.h"
#include "Text.h"
#include "Tweak.h"
#include "Settings.h"

#ifdef PLATFORM_IPHONE
#import "SuperDustBunnyViewController.h"
#endif


#define MAX_DEBUG_TEXT  10
#define MAX_DEBUG_LINES 10


struct SDebugText
{
    int EndFrame;
    char Text[128];
    int Flags;
    float X, Y;
    float Scale;
    unsigned int Color;
};

struct SDebugLine
{
    int EndFrame;
    float X1, Y1;
    float X2, Y2;
    unsigned int Color;
};


SDebugText DebugTexts[MAX_DEBUG_TEXT];
SDebugLine DebugLines[MAX_DEBUG_LINES];

int CurFrame;

void AddDebugText(const char* Text, int Flags, float X, float Y, float Scale, unsigned int Color, float Time)
{
    for (int i = 0; i < MAX_DEBUG_TEXT; i++)
    {
        if (DebugTexts[i].EndFrame <= CurFrame)
        {
            SDebugText* DebugText = &DebugTexts[i];
            snprintf(DebugText->Text, sizeof(DebugText->Text), "%s", Text);
            DebugText->Flags = Flags;
            DebugText->X = X;
            DebugText->Y = Y;
            DebugText->Scale = Scale;
            DebugText->Color = Color;
            DebugText->EndFrame = CurFrame + int(Time*60.0f);
            return;
        }
    }
}

void AddDebugLine(float X1, float Y1, float X2, float Y2, unsigned int Color, float Time)
{    
    for (int i = 0; i < MAX_DEBUG_LINES; i++)
    {
        if (DebugLines[i].EndFrame <= CurFrame)
        {
            SDebugLine* DebugLine = &DebugLines[i];
            DebugLine->X1 = X1;
            DebugLine->Y1 = Y1;
            DebugLine->X2 = X2;
            DebugLine->Y2 = Y2;
            DebugLine->Color = Color;
            DebugLine->EndFrame = CurFrame + int(Time*60.0f);
            return;
        }
    }
}

void DisplayDebugText(const char* Text, int Flags, float X, float Y, float Scale, unsigned int Color)
{
    DisplayStringColor(LIGHTLIST_WIPE, Text, Flags, X, Y, Scale, Color);
}

void DisplayDebugLine(float X1, float Y1, float X2, float Y2, float Width, unsigned int Color)
{
    float PerpX = Y2 - Y1;
    float PerpY = -(X2 - X1);
    float L = Length(PerpX, PerpY);
    if (Length > 0)
    {
        PerpX *= Width / L;
        PerpY *= Width / L;
    }
    else
    {
        PerpX = 0;
        PerpY = 0;
    }
    
    AddLitQuad(LIGHTLIST_WIPE, &WhiteSprite, Color,
               X1 - PerpX, Y1 - PerpY, 0, 0,
               X1 + PerpX, Y1 + PerpY, 0, 0,
               X2 + PerpX, Y2 + PerpY, 0, 0,
               X2 - PerpX, Y2 - PerpY, 0, 0);    
}

void DisplayDebug()
{
    if (!Settings.DeveloperMode)
        return;
    
    for (int i = 0; i < MAX_DEBUG_LINES; i++)
    {
        if (DebugLines[i].EndFrame > CurFrame)
        {
            SDebugLine* DebugLine = &DebugLines[i];
            
            float Width = 4.0f;
            
            DisplayDebugLine(DebugLine->X1, DebugLine->Y1, DebugLine->X2, DebugLine->Y2, Width, DebugLine->Color);
        }
    }

    for (int i = 0; i < MAX_DEBUG_TEXT; i++)
    {
        if (DebugTexts[i].EndFrame > CurFrame)
        {
            SDebugText* DebugText = &DebugTexts[i];
            
            DisplayDebugText(DebugText->Text, DebugText->Flags, DebugText->X, DebugText->Y, DebugText->Scale, DebugText->Color);
        }
    }

    CurFrame++;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Error handling (platform specific)                                                 //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

#define MAX_ERROR_CONTEXT 10

struct SErrorContext
{
	char Text[1024];
};

int NErrorContexts = 0;
SErrorContext ErrorContexts[MAX_ERROR_CONTEXT];

bool InErrorHandler = false;

void PushErrorContext(const char* ErrorContext, ...)
{
	if (NErrorContexts >= MAX_ERROR_CONTEXT)
		ReportError("Exceeded the maximum of %d error contexts.", MAX_ERROR_CONTEXT);
    
	SErrorContext* Context = &ErrorContexts[NErrorContexts];
    
	va_list args;
	va_start(args, ErrorContext);
	vsnprintf(Context->Text, sizeof(Context->Text), ErrorContext, args);
	va_end(args);
    
	NErrorContexts++;
}

void PopErrorContext()
{
	if (NErrorContexts <= 0)
		ReportError("Error context underflow.");
    
	NErrorContexts--;
}

void ReportError(const char* ErrorMessage, ...)
{
    InErrorHandler = true;

	char ErrorContext[8192];
	char TotalMessage[8192];
	int MessageSize = 0;
    
	// Append the error contexts.
	strcpy(ErrorContext, "");
	for (int i = 0; i < NErrorContexts; i++)
	{
		MessageSize += snprintf(ErrorContext + MessageSize, sizeof(ErrorContext) - MessageSize, "%s", ErrorContexts[i].Text);
	}
    
	// Append the error message.
	char Work[1024];
    
	va_list args;
	va_start(args, ErrorMessage);
	vsnprintf(Work, sizeof(Work), ErrorMessage, args);
	va_end(args);
    
	snprintf(TotalMessage, sizeof(TotalMessage), "%s%s", ErrorContext, Work);
    
#ifdef PLATFORM_WINDOWS
    OutputDebugString(TotalMessage);
	MessageBox(NULL, TotalMessage, "SuperDustBunny Error", MB_OK | MB_ICONSTOP);
	exit(1);
#endif
    
#ifdef PLATFORM_MAC
	NSLog(@"SuperDustBunny Error: %s\n", TotalMessage);
    
    NSWindow *window = [[NSApp delegate] window];
    [window setLevel:NSNormalWindowLevel];
    
    [NSCursor unhide];
    
    NSAlert* alert = [[NSAlert alloc] init];
    [alert setAlertStyle:NSCriticalAlertStyle];
    [alert setMessageText: @"Super Dust Bunny Error"];
    [alert setInformativeText:[NSString stringWithUTF8String:TotalMessage]];
    [alert runModal];
    
    exit(255);
#endif
    
#ifdef PLATFORM_IPHONE
	NSLog(@"SuperDustBunny Error: %s\n", TotalMessage);
    
    theViewController.paused = TRUE;
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"SuperDustBunny Error"
                                                    message:[NSString stringWithUTF8String:TotalMessage]
                                                   delegate:theViewController
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
    
    while (theViewController.paused)
    {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
    
    exit(255);
#endif

    InErrorHandler = false;
}

void LogMessage(const char* LogMessage, ...)
{
#ifdef PLATFORM_IPHONE_OR_MAC
	va_list args;
	va_start(args, LogMessage);
	vprintf(LogMessage, args);
	va_end(args);
#endif

#ifdef PLATFORM_WINDOWS
	char Work[1024];
	va_list args;
	va_start(args, LogMessage);
	vsnprintf(Work, sizeof(Work), LogMessage, args);
	OutputDebugString(Work);
	va_end(args);
#endif
}

