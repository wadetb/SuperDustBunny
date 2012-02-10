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


#define MAX_CHAPTERS        10
#define MAX_PAGES			25
#define MAX_BLOCKS			10000
#define MAX_TILESETS		100
#define MAX_PAGE_TILESETS	20
#define MAX_PAGE_LAYERS     10
#define MIN_PAGE_WIDTH      12
#define MIN_PAGE_HEIGHT     10

enum ESpecialBlockID
{
	SPECIALBLOCKID_FIRST			= 10000,
	SPECIALBLOCKID_OUTOFBOUNDS      = SPECIALBLOCKID_FIRST,
	SPECIALBLOCKID_SOLID,
	SPECIALBLOCKID_BLANK,

    SPECIALBLOCKID_FLIP_DIAGONAL    = 1 << 29,
    SPECIALBLOCKID_FLIP_Y           = 1 << 30,
    SPECIALBLOCKID_FLIP_X           = 1 << 31,
    SPECIALBLOCKID_MASK             = 0xffffff, // One byte reserved for flags.
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
	BLOCKTYPE_NAIL, 
	BLOCKTYPE_FAN, 
    BLOCKTYPE_FLAME,
	BLOCKTYPE_FLASHLIGHT_WAYPOINT,
	BLOCKTYPE_STAPLER, 
	BLOCKTYPE_JELLO, 
	BLOCKTYPE_CLOTHESPIN, 
	BLOCKTYPE_POWERUP,
    BLOCKTYPE_TUTORIAL,
    BLOCKTYPE_VACUUM_TRIGGER,
    BLOCKTYPE_HANGER,
    BLOCKTYPE_BABY,
    BLOCKTYPE_BALLOON,
};

enum EBlockMaterial
{
	MATERIAL_NORMAL,
	MATERIAL_BREAKABLE,
	MATERIAL_ICE,
	MATERIAL_STICKY,
};

enum EChapterTheme
{
    THEME_STUDY,
    THEME_PANTRY,
    THEME_FRIDGE,
    THEME_WORKSHOP
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
	unsigned int FirstGID;
};

struct SNailProperties
{
	int Dir;
};

struct SBlock
{
	EBlockType Type;
	EBlockMaterial Material;

	int ID;
	int TileSet;

	int SubX, SubY;

	void* Properties;
};

struct SPageProperties
{
	bool LightsOff;
	bool VacuumOff;
    int VacuumType;
	int VacuumDir;
    int VacuumStart;
    float VacuumSpeed;
    bool GhostRace;
    int GhostHat;
    float ShadowOffsetX;
    float ShadowOffsetY;
    float ShadowScaleX;
    float ShadowScaleY;
    float LightOriginX;
    float LightOriginY;
};

struct SPageLayer
{
    ELightList LightList;
    float Alpha;
    int* Blocks;
};

struct SPage
{
	char* Name;

	int* Blocks;

	int Width;
	int Height;

    int NLayers;
    SPageLayer Layers[MAX_PAGE_LAYERS];

	SPageProperties Props;
};

struct SChapter
{
	const char* Name;

    EChapterTheme Theme;
    
    bool HasBackground;
    gxSprite BackgroundSprite;
    
	float StartX, StartY;
    int StartDirection;
	float EndX, EndY;
    
    float PortalAngle;

	int NPages;
	SPage Pages[MAX_PAGES];

	int NTileSets;
	STileSet TileSets[MAX_TILESETS];

	int NBlocks;
	SBlock Blocks[MAX_BLOCKS];

	int PageNum;
	SPageProperties PageProps;
	int* PageBlocks;
	int PageWidth;
	int PageHeight;
};

struct SChapterListEntry
{
    char* Name;
    
    bool HasBackground;
    gxSprite BackgroundSprite;
    
    bool HasIcon;
    gxSprite IconSprite;

    int BronzeTime;
    int SilverTime;
    int GoldTime;
    
    char* UnlockedBy;
    bool EndOfGame;
    
    bool Unlocked;
    bool Played;
    bool Completed;
    int BestTime;
};


extern int NChapters;
extern SChapterListEntry Chapters[MAX_CHAPTERS];
extern int CurrentChapter;

extern SChapter Chapter;

extern float ScrollX;
extern float ScrollY;

void LoadChapter(const char* filename);
void ClearChapter();

void SetCurrentPage(int PageNum);

void DisplayPortal();

void DisplayChapterBaseLayer();
void DisplayChapterExtraLayers();

void CalculateScroll();

int GetBlockID(int x, int y);

bool IsBlockEmpty(int x, int y);
bool IsBlockSolid(int x, int y);

void EraseBlock(int x, int y);

void ClearChapterList();
void LoadChapterList();

void LoadChapterUnlocks();
void SaveChapterUnlocks();

#endif
