//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef CHAPTER_H
#define CHAPTER_H

#include "common.h"

#define MAX_PAGES			20
#define MAX_BLOCKS			100
#define MAX_PAGE_BLOCKS		(256*1024)

enum ESpecialBlockID
{
	SPECIALBLOCKID_FIRST			= 10000,
	SPECIALBLOCKID_UNKNOWN			= SPECIALBLOCKID_FIRST,
	SPECIALBLOCKID_OUTOFBOUNDS,
	SPECIALBLOCKID_BLANK,
};

struct SBlock
{
	int ID;
	char Key[3][3];
	char* Desc;
	gxSprite Sprite;
	bool Destructible;
	bool EndOfLevel;
};

struct SPage
{
	int* Blocks;
	int Width;
	int Height;
};

struct SChapter
{
	SPage Pages[MAX_PAGES];
	const char* Name;
	int NPages;
	int NBlocks;
	SBlock Blocks[MAX_BLOCKS];
};


extern SChapter Chapter;

extern int ScrollY;

void LoadChapter(const char* filename);
void DisplayChapter();

void CalculateScrollY();

int GetBlockID(int x, int y);

bool IsBlockEmpty(int x, int y);
bool IsBlockSolid(int x, int y);

#endif
