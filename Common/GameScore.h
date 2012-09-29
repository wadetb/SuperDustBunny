//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef GAMESCORE_H
#define GAMESCORE_H

void UploadScore();

void DisplayTimeDigit(int Digit, float BaseX, float BaseY, float ScaleFactor, float X, float Y);
void DisplayTimeDigitAlpha(int Digit, float BaseX, float BaseY, float ScaleFactor, float X, float Y, float Alpha);

void DisplayTime(float X, float Y, float ScaleFactor, int Time);
void DisplayTimeAlpha(float X, float Y, float ScaleFactor, int Time, float Alpha);

#endif