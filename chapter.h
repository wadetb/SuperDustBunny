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

#define MAX_PAGES			100
#define MAX_BLOCKS			10000
#define MAX_PAGE_BLOCKS		(256*1024)
#define MAX_STITCHED_PAGES	100
#define MAX_TILESETS		100
#define MAX_PAGE_TILESETS	10


enum ESpecialBlockID
{
	SPECIALBLOCKID_FIRST			= 10000,
	SPECIALBLOCKID_UNKNOWN			= SPECIALBLOCKID_FIRST,
	SPECIALBLOCKID_OUTOFBOUNDS,
	SPECIALBLOCKID_BLANK,
	SPECIALBLOCKID_DELAYDEST,
	SPECIALBLOCKID_GREEN,
	SPECIALBLOCKID_YELLOW,
	SPECIALBLOCKID_RED,
};

enum EBlockType
{
	BLOCKTYPE_NORMAL,
	BLOCKTYPE_CHAPTERSTART,
	BLOCKTYPE_CHAPTEREND,
	BLOCKTYPE_BARREL,
	BLOCKTYPE_FIREWORK,
	BLOCKTYPE_BALL,
	BLOCKTYPE_GEAR,
	BLOCKTYPE_COIN, 
};

struct STileSet
{
	char* Name;
	int FirstBlock;
	int NBlocks;
	gxSprite Sprite;
};

struct STileSetInfo
{
	STileSet* TileSet;
	int FirstGID;
};

struct SBlock
{
	EBlockType Type;

	int ID;
	int TileSet;

	int SubX, SubY;

	void* Properties;

	bool Destructible;
	bool DelayDest;
	bool GreenDelayDest;
	bool YellowDelayDest;
	bool RedDelayDest;
	bool Gum;
	bool GumJump;
	bool HasCrumbTriggered;
	bool Jello;
	bool NoWallJump;
};

struct SPage
{
	char* Name;
	int* Blocks;
	int Width;
	int Height;
};

struct SChapter
{
	const char* Name;

	float EndX, EndY;

	int NPages;
	SPage Pages[MAX_PAGES];

	int NTileSets;
	STileSet TileSets[MAX_TILESETS];

	int NBlocks;
	SBlock Blocks[MAX_BLOCKS];

	int NStitchedPages;
	int StitchedPages[MAX_STITCHED_PAGES];

	int StitchedWidth;
	int StitchedHeight;
	int* StitchedBlocks;
};

struct SScore
{
    bool RaiseScore;
    int ScoreCounter;
    int X, Y;  
};


extern SChapter Chapter;
extern SScore Score;

extern int ScrollY;

void LoadChapter(const char* filename);
void ClearChapter();

void DisplayChapter();

void CalculateScrollY();

void InitScore();
void DisplayScore();
void UpdateScore();

void DisplayDelayDestructible();

void InitCrumb();

int GetBlockID(int x, int y);

bool IsBlockEmpty(int x, int y);
bool IsBlockSolid(int x, int y);

void EraseBlock(int x, int y);

#endif
