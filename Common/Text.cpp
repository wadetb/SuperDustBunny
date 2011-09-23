//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Text.h"


static void DisplayLetter(ELightList List, char Letter, float X, float Y, float Scale, float Alpha)
{
    switch (Letter)
    {
        case 'a': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  0*48,   0,  1*48, 60, Scale, Scale, Alpha); break;
        case 'b': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  1*48,   0,  2*48, 60, Scale, Scale, Alpha); break;
        case 'c': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  2*48,   0,  3*48, 60, Scale, Scale, Alpha); break;
        case 'd': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  3*48,   0,  4*48, 60, Scale, Scale, Alpha); break;
        case 'e': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  4*48,   0,  5*48, 60, Scale, Scale, Alpha); break;
        case 'f': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  5*48,   0,  6*48, 60, Scale, Scale, Alpha); break;
        case 'g': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  6*48,   0,  7*48, 60, Scale, Scale, Alpha); break;
        case 'h': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  7*48,   0,  8*48, 60, Scale, Scale, Alpha); break;
        case 'i': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  8*48,   0,  9*48, 60, Scale, Scale, Alpha); break;
        case 'j': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  9*48,   0, 10*48, 60, Scale, Scale, Alpha); break;
        case 'k': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 10*48,   0, 11*48, 60, Scale, Scale, Alpha); break;
        case 'l': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 11*48,   0, 12*48, 60, Scale, Scale, Alpha); break;
        case 'm': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 12*48,   0, 13*48, 60, Scale, Scale, Alpha); break;
        case 'n': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  0*48,  64,  1*48, 124, Scale, Scale, Alpha); break;
        case 'o': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  1*48,  64,  2*48, 124, Scale, Scale, Alpha); break;
        case 'p': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  2*48,  64,  3*48, 124, Scale, Scale, Alpha); break;
        case 'q': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  3*48,  64,  4*48, 124, Scale, Scale, Alpha); break;
        case 'r': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  4*48,  64,  5*48, 124, Scale, Scale, Alpha); break;
        case 's': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  5*48,  64,  6*48, 124, Scale, Scale, Alpha); break;
        case 't': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  6*48,  64,  7*48, 124, Scale, Scale, Alpha); break;
        case 'u': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  7*48,  64,  8*48, 124, Scale, Scale, Alpha); break;
        case 'v': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  8*48,  64,  9*48, 124, Scale, Scale, Alpha); break;
        case 'w': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  9*48,  64, 10*48, 124, Scale, Scale, Alpha); break;
        case 'x': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 10*48,  64, 11*48, 124, Scale, Scale, Alpha); break;
        case 'y': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 11*48,  64, 12*48, 124, Scale, Scale, Alpha); break;
        case 'z': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 12*48,  64, 13*48, 124, Scale, Scale, Alpha); break;
        case '1': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  0*48, 128,  1*48, 188, Scale, Scale, Alpha); break;
        case '2': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  1*48, 128,  2*48, 188, Scale, Scale, Alpha); break;
        case '3': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  2*48, 128,  3*48, 188, Scale, Scale, Alpha); break;
        case '4': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  3*48, 128,  4*48, 188, Scale, Scale, Alpha); break;
        case '5': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  4*48, 128,  5*48, 188, Scale, Scale, Alpha); break;
        case '6': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  5*48, 128,  6*48, 188, Scale, Scale, Alpha); break;
        case '7': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  6*48, 128,  7*48, 188, Scale, Scale, Alpha); break;
        case '8': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  7*48, 128,  8*48, 188, Scale, Scale, Alpha); break;
        case '9': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  8*48, 128,  9*48, 188, Scale, Scale, Alpha); break;
        case '0': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y,  9*48, 128, 10*48, 188, Scale, Scale, Alpha); break;
        case '!': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 10*48, 128, 11*48, 188, Scale, Scale, Alpha); break;
        case '?': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 11*48, 128, 12*48, 188, Scale, Scale, Alpha); break;
        case ',': AddLitSubSpriteScaledAlpha(List, &AlphabetSprite, X, Y, 12*48, 128, 13*48, 188, Scale, Scale, Alpha); break;
        default: break;
    }
}

static int GetLetterWidth(char Letter)
{
    switch (Letter)
    {
        case 'a': return 44;
        case 'b': return 44;
        case 'c': return 44;
        case 'd': return 46;
        case 'e': return 44;
        case 'f': return 44;
        case 'g': return 46;
        case 'h': return 44;
        case 'i': return 44;
        case 'j': return 46;
        case 'k': return 44;
        case 'l': return 40;
        case 'm': return 44;
        case 'n': return 44;
        case 'o': return 44;
        case 'p': return 44;
        case 'q': return 44;
        case 'r': return 44;
        case 's': return 44;
        case 't': return 46;
        case 'u': return 44;
        case 'v': return 44;
        case 'w': return 46;
        case 'x': return 44;
        case 'y': return 40;
        case 'z': return 46;
        case '1': return 44;
        case '2': return 44;
        case '3': return 44;
        case '4': return 44;
        case '5': return 44;
        case '6': return 44;
        case '7': return 44;
        case '8': return 44;
        case '9': return 44;
        case '0': return 44;
        case '!': return 30;
        case ' ': return 30;
        default: return 46;
    }
}

float GetStringWidth(const char* String, float Scale)
{
    float CurWidth = 0;
    
    const char* P = String;
    while (*P)
    {
        char C = *P;
        CurWidth += GetLetterWidth(C) * Scale;
        P++;
    }
    
    return CurWidth;
}

void DisplayStringAlpha(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale, float Alpha)
{
    float CurX = X;
    float CurY = Y;
    
    if (FormatFlags & FORMAT_CENTER_X)
        CurX = X - GetStringWidth(String, Scale)/2;
    
    const char* P = String;
    while (*P)
    {
        char C = *P;
        DisplayLetter(List, C, CurX, CurY, Scale, Alpha);
        CurX += GetLetterWidth(C) * Scale;
        P++;
    }
}

void DisplayString(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale)
{
    DisplayStringAlpha(List, String, FormatFlags, X, Y, Scale, 1.0f);
}

static float GetLineWidth(const char* Line, float Scale)
{
    float CurWidth = 0;
    
    const char* P = Line;
    while (*P && *P != '\n' && *P != '|')
    {
        char C = *P;
        CurWidth += GetLetterWidth(C) * Scale;
        P++;
    }
    
    return CurWidth;
}

void GetMultilineStringDimensions(const char* String, float Scale, float* Width, float* Height)
{
    float CurWidth = 0;
    float CurHeight = 0;
    
    float LineWidth = 0;
    
    const char* P = String;
    while (*P)
    {
        char C = *P++;        
        if (C == '\n' || C == '|')
        {
            CurHeight += 64*Scale;
            if (LineWidth > CurWidth)
                CurWidth = LineWidth;
            LineWidth = 0;
        }
        else
        {
            LineWidth += GetLetterWidth(C) * Scale;
        }
    }    

    CurHeight += 64*Scale;
    if (LineWidth > CurWidth)
        CurWidth = LineWidth;
    
    *Width = CurWidth;
    *Height = CurHeight;
}

void DisplayMultilineStringAlpha(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale, float Alpha)
{
    float CurX, CurY;
    
    if (FormatFlags & FORMAT_CENTER_X)
        CurX = X - GetLineWidth(String, Scale)/2;
    else
        CurX = X;
    
    if (FormatFlags & FORMAT_CENTER_Y)
    {
        float Width, Height;
        GetMultilineStringDimensions(String, Scale, &Width, &Height);
        CurY = Y - Height/2;
    }
    else
        CurY = Y;
    
    const char* P = String;
    while (*P)
    {
        char C = *P++;
        if (C == '\n' || C == '|')
        {
            CurY += 64*Scale;
            CurX = X - GetLineWidth(P, Scale)/2;
        }
        else
        {
            DisplayLetter(List, C, CurX, CurY, Scale, Alpha);
            CurX += GetLetterWidth(C) * Scale;
        }
    }
}

void DisplayMultilineString(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale)
{
    DisplayMultilineStringAlpha(List, String, FormatFlags, X, Y, Scale, 1.0f);
}

