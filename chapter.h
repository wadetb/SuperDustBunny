#ifndef CHAPTER_H
#define CHAPTER_H
#include "common.h"
#define MAX_PAGES 20
#define MAX_BLOCKS 100
#define MAX_DESC 1024
#define MAX_PAGE_BLOCKS (256*1024*1024)

struct SBlock
{
	char Key[3][3];
	char Desc[MAX_DESC];
	gxSprite Sprite;
};

struct SPage
{
	int* Blocks;
	int Width;
	int Height;
};

// SPage Methods

struct SChapter
{
	SPage Pages[MAX_PAGES];
	const char* Name;
	int NPages;
	int NBlocks;
	SBlock Blocks[MAX_BLOCKS];

};

// SChapter Methods
void LoadChapter(const char* filename, SChapter* Chap);
void RenderChapter(SChapter* Chap);







#endif