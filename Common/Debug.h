//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef DEBUG_H
#define DEBUG_H

void AddDebugLine(float X1, float Y1, float X2, float Y2, unsigned int Color, float Time);

void DisplayDebugLine(float X1, float Y1, float X2, float Y2, float Width, unsigned int Color);

void DisplayDebug();

// Stack of "error contexts" which provide extra information about the state of the program when an error occurs.
void PushErrorContext(const char* ErrorContext, ...);
void PopErrorContext();

// Reports a fatal error and immediately exits the program.
#ifdef WIN32
__declspec(noreturn) void ReportError(const char* ErrorMessage, ...);
#else
void ReportError(const char* ErrorMessage, ...) __attribute__((__noreturn__));
#endif

// Print something to the application log.
void LogMessage(const char* LogMessage, ...);

#endif
