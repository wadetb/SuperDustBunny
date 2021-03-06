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

#define GOLD_PAGE_COUNT     20
#define SILVER_PAGE_COUNT   10
#define BRONZE_PAGE_COUNT   5

#define FIRST_NORMAL_CHAPTER    1
#define LAST_NORMAL_CHAPTER     4

#define FIRST_STARTING_CHAPTER  1
#define LAST_STARTING_CHAPTER   2

#define FIRST_STORY_CHAPTER     5
#define LAST_STORY_CHAPTER      5


enum ESpecialBlockID
{
	SPECIALBLOCKID_FIRST			= 10000,
	SPECIALBLOCKID_OUTOFBOUNDS      = SPECIALBLOCKID_FIRST,
	SPECIALBLOCKID_SOLID,
	SPECIALBLOCKID_BLANK,

    SPECIALBLOCKID_STICKY           = 1 << 28,
    SPECIALBLOCKID_FLIP_DIAGONAL    = 1 << 29,
    SPECIALBLOCKID_FLIP_Y           = 1 << 30,
    SPECIALBLOCKID_FLIP_X           = 1 << 31,
    SPECIALBLOCKID_MASK             = 0xffffff, // One byte reserved for flags.
};

inline float BlockFlagsToAngle(unsigned int Flags)
{
    if (Flags == (SPECIALBLOCKID_FLIP_X | SPECIALBLOCKID_FLIP_DIAGONAL)) return PI/2;
    if (Flags == (SPECIALBLOCKID_FLIP_X | SPECIALBLOCKID_FLIP_Y)) return PI;
    return 0;
}

inline unsigned int ApplyBlockFlagsToDir(unsigned int Dir, unsigned int Flags)
{
    float a = DirectionToAngle(Dir);
    float dx = cosf(a);
    float dy = -sinf(a);
    
    if (Flags & SPECIALBLOCKID_FLIP_DIAGONAL)
    {
        Swap(&dx, &dy);
    }
    if (Flags & SPECIALBLOCKID_FLIP_X)
    {
        dx = -dx;
    }
    if (Flags & SPECIALBLOCKID_FLIP_Y)
    {
        dy = -dy;
    }
    
    float a2 = atan2f(-dy, dx);
    int d2 = (int)(AngleToDirection(a2) + 360) % 360;
    
    return d2;
}

enum EBlockType
{
	BLOCKTYPE_NORMAL,
	BLOCKTYPE_NOCOLLIDE,
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
    char* Name;
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
    const char* Title;
    
    EChapterTheme Theme;
    
    bool HasBackground;
    gxSprite BackgroundSprite;
    
    bool HasMusic;
    SMusicAsset Music;
    
    bool HasIntroMusic;
    SMusicAsset IntroMusic;
    
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
};

#define MAX_SCORE_BABIES 50

struct SPortfolio
{
    bool Coins;
    bool Gears;
    bool Fireworks;
    bool Babies;
    bool LightsOff;
    bool Barrels;
    bool Fans;
    bool Staplers;
    bool Powerups;
    bool Balloons;
    bool Sharp;
    bool Sticky;
    bool DustBuster;
    bool UpsideDown;
    bool MirrorPage;
    bool VacuumCatchup;
    
    int Chapter;
    int Page;
    
    bool NewChapter;
    int PageHistory[3];
    
    int ElementChangeFrequency;
    int ChapterChangeFrequency;
    int FirstStoryPageFrequency;
    int StoryPageFrequency;
    
    int ChapterCount;
    int PageCount;
    
    int ChaptersUntilNextStory;
    bool StorySeen[MAX_CHAPTERS];
    
    float InitialVacuumSpeed;
    float VacuumSpeedChange;
    int VacuumSpeedChangeFrequency;
    float VacuumDamageSpeedModifier;
    float VacuumSpeed;
    float InitialVacuumDistance;
    float VacuumDistance;
    int VacuumDamage;
    int VacuumDir;
    int VacuumSide;
    
    int BabyCount;
    int BabyHats[MAX_SCORE_BABIES];
    
    int TotalLives;
};


extern int NChapters;
extern SChapterListEntry Chapters[MAX_CHAPTERS];
extern int CurrentChapter;

extern SChapter Chapter;

extern SPageProperties Props;

extern float ScrollX;
extern float ScrollY;

extern SPortfolio Portfolio;

void LoadChapter(const char* filename);
void ClearChapter();

void StartChapterMusic();
void RestartChapterMusic();

void SetCurrentPage(int PageNum);

void DisplayPortal();

void DisplayChapterBaseLayer();
void DisplayChapterExtraLayers();

void CalculateScroll();

int GetBlockType(int x, int y);
int GetBlockID(int x, int y);
int GetBlockFlags(int x, int y);

bool IsBlockEmpty(int x, int y);
bool IsBlockSolid(int x, int y);

void EraseBlock(int x, int y);

void ClearChapterList();
void LoadChapterList();

void ResetPortfolio();
void LoadPortfolio();
void SetupTutorialPortfolio();
void SetupInitialPortfolio();
void AdvancePortfolio();

void AwardBaby(int Hat);
void RemoveBaby(int Hat);

#endif
