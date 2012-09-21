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


static void DisplayLetter(ELightList List, char Letter, float X, float Y, float Scale, unsigned int Color)
{
    switch (tolower(Letter))
    {
        case 'a': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  0*48,   0,  1*48, 60, Scale, Scale, Color); break;
        case 'b': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  1*48,   0,  2*48, 60, Scale, Scale, Color); break;
        case 'c': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  2*48,   0,  3*48, 60, Scale, Scale, Color); break;
        case 'd': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  3*48,   0,  4*48, 60, Scale, Scale, Color); break;
        case 'e': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  4*48,   0,  5*48, 60, Scale, Scale, Color); break;
        case 'f': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  5*48,   0,  6*48, 60, Scale, Scale, Color); break;
        case 'g': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  6*48,   0,  7*48, 60, Scale, Scale, Color); break;
        case 'h': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  7*48,   0,  8*48, 60, Scale, Scale, Color); break;
        case 'i': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  8*48,   0,  9*48, 60, Scale, Scale, Color); break;
        case 'j': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  9*48,   0, 10*48, 60, Scale, Scale, Color); break;
        case 'k': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 10*48,   0, 11*48, 60, Scale, Scale, Color); break;
        case 'l': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 11*48,   0, 12*48, 60, Scale, Scale, Color); break;
        case 'm': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 12*48,   0, 13*48, 60, Scale, Scale, Color); break;
        case 'n': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  0*48,  64,  1*48, 124, Scale, Scale, Color); break;
        case 'o': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  1*48,  64,  2*48, 124, Scale, Scale, Color); break;
        case 'p': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  2*48,  64,  3*48, 124, Scale, Scale, Color); break;
        case 'q': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  3*48,  64,  4*48, 124, Scale, Scale, Color); break;
        case 'r': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  4*48,  64,  5*48, 124, Scale, Scale, Color); break;
        case 's': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  5*48,  64,  6*48, 124, Scale, Scale, Color); break;
        case 't': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  6*48,  64,  7*48, 124, Scale, Scale, Color); break;
        case 'u': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  7*48,  64,  8*48, 124, Scale, Scale, Color); break;
        case 'v': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  8*48,  64,  9*48, 124, Scale, Scale, Color); break;
        case 'w': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  9*48,  64, 10*48, 124, Scale, Scale, Color); break;
        case 'x': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 10*48,  64, 11*48, 124, Scale, Scale, Color); break;
        case 'y': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 11*48,  64, 12*48, 124, Scale, Scale, Color); break;
        case 'z': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 12*48,  64, 13*48, 124, Scale, Scale, Color); break;
        case '1': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  0*48, 128,  1*48, 188, Scale, Scale, Color); break;
        case '2': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  1*48, 128,  2*48, 188, Scale, Scale, Color); break;
        case '3': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  2*48, 128,  3*48, 188, Scale, Scale, Color); break;
        case '4': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  3*48, 128,  4*48, 188, Scale, Scale, Color); break;
        case '5': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  4*48, 128,  5*48, 188, Scale, Scale, Color); break;
        case '6': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  5*48, 128,  6*48, 188, Scale, Scale, Color); break;
        case '7': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  6*48, 128,  7*48, 188, Scale, Scale, Color); break;
        case '8': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  7*48, 128,  8*48, 188, Scale, Scale, Color); break;
        case '9': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  8*48, 128,  9*48, 188, Scale, Scale, Color); break;
        case '0': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y,  9*48, 128, 10*48, 188, Scale, Scale, Color); break;
        case '!': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 10*48, 128, 11*48, 188, Scale, Scale, Color); break;
        case '?': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 11*48, 128, 12*48, 188, Scale, Scale, Color); break;
        case ',': AddLitSubSpriteScaledColor(List, &AlphabetSprite, X, Y, 12*48, 128, 13*48, 188, Scale, Scale, Color); break;
        default: break;
    }
}

static int GetLetterWidth(char Letter)
{
    switch (tolower(Letter))
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

void DisplayStringColor(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale, unsigned int Color)
{
    float CurX = X;
    float CurY = Y;
    
    if (FormatFlags & FORMAT_CENTER_X)
        CurX = X - GetStringWidth(String, Scale)/2;
    
    const char* P = String;
    while (*P)
    {
        char C = *P;
        DisplayLetter(List, C, CurX, CurY, Scale, Color);
        CurX += GetLetterWidth(C) * Scale;
        P++;
    }
}

void DisplayStringAlpha(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale, float Alpha)
{
    DisplayStringColor(List, String, FormatFlags, X, Y, Scale, gxRGBA32(255, 255, 255, (int)(255*Alpha)));
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
            DisplayLetter(List, C, CurX, CurY, Scale, gxRGBA32(255, 255, 255, (int)(255*Alpha)));
            CurX += GetLetterWidth(C) * Scale;
        }
    }
}

void DisplayMultilineString(ELightList List, const char* String, unsigned int FormatFlags, float X, float Y, float Scale)
{
    DisplayMultilineStringAlpha(List, String, FormatFlags, X, Y, Scale, 1.0f);
}

