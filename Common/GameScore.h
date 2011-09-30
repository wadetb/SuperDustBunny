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

#define MAX_PAGE_TIMES 20


enum EMedalType
{
    MEDAL_NONE,
    MEDAL_BRONZE,
    MEDAL_SILVER,
    MEDAL_GOLD
};

struct SScore
{
    int CurrentPageTime;
    int CurrentBonus;
    
    int PageTime[MAX_PAGE_TIMES];
    int PageBonus[MAX_PAGE_TIMES];

    bool NewRecord[MAX_PAGE_TIMES];
    int BestPageTime[MAX_PAGE_TIMES];

    int ChapterTime;

    bool NewChapterRecord;
    int BestChapterTime;
    
    EMedalType Medal;
    
    bool Invalid;
};

extern SScore Score;

void DisplayScore();
void UpdateScore();

void ResetScore();

void AwardBonus(float X, float Y);

void LoadChapterScores(char* ChapterName);
void SaveChapterScores(char* ChapterName);
void RecordPageScore(int Page);
void UploadChapterScore();

void DisplayTimeDigit(int Digit, float BaseX, float BaseY, float ScaleFactor, float X, float Y);
void DisplayTimeDigitAlpha(int Digit, float BaseX, float BaseY, float ScaleFactor, float X, float Y, float Alpha);

void DisplayTime(float X, float Y, float ScaleFactor, int Time);
void DisplayTimeAlpha(float X, float Y, float ScaleFactor, int Time, float Alpha);

#endif