//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef TEXT_H
#define TEXT_H

enum EStringFormatFlag
{
    FORMAT_CENTER_X = 1,
    FORMAT_CENTER_Y = 2,
};

float GetStringWidth(const char* String, float Scale);

void DisplayString(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale);
void DisplayStringAlpha(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale, float Alpha);

void GetMultilineStringDimensions(const char* String, float Scale, float* Width, float* Height);

void DisplayMultilineString(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale);
void DisplayMultilineStringAlpha(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale, float Alpha);

#endif
