//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Chapter.h"
#include "Music.h"
#include "Dusty.h"
#include "Barrel.h"
#include "Coin.h"
#include "TennisBall.h"
#include "FireWorks.h"
#include "Gear.h"
#include "Dust.h"
#include "Flame.h"
#include "Flashlight.h"
#include "Vacuum.h"
#include "Smoke.h"
#include "Fan.h"
#include "Wipe.h"
#include "Debris.h"
#include "Lives.h"
#include "Recorder.h"
#include "Stapler.h"
#include "PowerUp.h"
#include "Tutorial.h"
#include "Ghost.h"
#include "Settings.h"
#include "GameScore.h"
#include "Hanger.h"
#include "Baby.h"
#include "Balloon.h"
#include "Text.h"

#ifdef PLATFORM_WINDOWS
#include <direct.h>
#endif

int NChapters;
SChapterListEntry Chapters[MAX_CHAPTERS];

int CurrentChapter = -1;

SChapter Chapter;

float ScrollX;
float ScrollY;

const char* CurrentChapterDir;

enum EPortfolioFlags
{
    PORTFOLIO_PRO = 0,
    PORTFOLIO_CON = 1,
    PORTFOLIO_TYPE_MASK = 1,
    
    PORTFOLIO_INITIAL = 2,
    PORTFOLIO_INITIAL_MASK = 2,
};

struct SPortfolioEntry
{
    const char* Name;
    int Flags;
    bool* Value;
    bool Dirty;
};

SPortfolioEntry PortfolioEntries[] =
{
    { "Coins", PORTFOLIO_PRO, &Portfolio.Coins },
    { "Gears", PORTFOLIO_PRO, &Portfolio.Gears },
    { "Fireworks", PORTFOLIO_PRO, &Portfolio.Fireworks },
    { "Babies", PORTFOLIO_PRO, &Portfolio.Babies },
    { "Barrels", PORTFOLIO_PRO, &Portfolio.Barrels },
    { "Fans", PORTFOLIO_PRO, &Portfolio.Fans },
    { "Staplers", PORTFOLIO_PRO, &Portfolio.Staplers },
    { "Powerups", PORTFOLIO_PRO, &Portfolio.Powerups },
    { "Balloons", PORTFOLIO_PRO, &Portfolio.Balloons },
    
    { "LightsOff", PORTFOLIO_CON, &Portfolio.LightsOff },
//    { "Sharp", PORTFOLIO_CON, &Portfolio.Sharp },
    { "Sticky", PORTFOLIO_CON, &Portfolio.Sticky },
    { "DustBuster", PORTFOLIO_CON, &Portfolio.DustBuster },
    { "UpsideDown", PORTFOLIO_CON, &Portfolio.UpsideDown },
    { "MirrorPage", PORTFOLIO_CON, &Portfolio.MirrorPage },
    { "VacuumCatchup", PORTFOLIO_CON, &Portfolio.VacuumCatchup },
};

SPortfolio Portfolio;
SPortfolio SavedPortfolio;

bool PortfolioDirty[ARRAY_COUNT(PortfolioEntries)];

int PortfolioLine;


static void ParseNailProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SNailProperties* NailProperties = (SNailProperties*)malloc(sizeof(SNailProperties));

	// Set default values.
	NailProperties->Dir = 0;

	// Scan properties for values.
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* Value = PropertyNode->first_attribute("value")->value();

		if (strcmp(Name, "dir") == 0)
		{
			NailProperties->Dir = atoi(Value);
		}

		PropertyNode = PropertyNode->next_sibling("property");
	}

	Block->Properties = NailProperties;
}

static void InitPageProperties(SPageProperties* Props)
{
	Props->LightsOff = false;
	Props->VacuumOff = false;
    Props->VacuumType = VACUUM_NORMAL;
	Props->VacuumDir = VACUUMDIR_UP;
	Props->VacuumStart = 100000000;
    Props->VacuumSpeed = 0.75f;
    Props->GhostRace = false;
    Props->GhostHat = DUSTYHAT_NONE;
    Props->ShadowOffsetX = 30;
    Props->ShadowOffsetY = 20;
    Props->ShadowScaleX = 1.0f;
    Props->ShadowScaleY = 1.0f;
    Props->LightOriginX = 384.0f;
    Props->LightOriginY = 512.0f;
}

static void ParsePageProperties(SPageProperties* Props, rapidxml::xml_node<char>* PropertiesNode)
{
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");

	while (PropertyNode != NULL)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* Value = PropertyNode->first_attribute("value")->value();

		if (strcmp(Name, "lights") == 0)
		{
			if (strcmp(Value, "off") == 0)
				Props->LightsOff = true;
			else if (strcmp(Value, "on") == 0)
				Props->LightsOff = false;
			else
				ReportError("'%s' is not a valid value for the 'lights' property.  The value must be 'on' or 'off'.  Fix this problem and re-save the TMX file.", Value);
		}
		else if (strcmp(Name, "vacuum") == 0)
		{
			if (strcmp(Value, "off") == 0)
				Props->VacuumOff = true;
			else if (strcmp(Value, "on") == 0)
				Props->VacuumOff = false;
			else
				ReportError("'%s' is not a valid value for the 'vacuum' property.  The value must be 'on' or 'off'.  Fix this problem and re-save the TMX file.", Value);
		}
		else if (strcmp(Name, "vacuum_type") == 0)
		{
			if (strcmp(Value, "normal") == 0)
				Props->VacuumType = VACUUM_NORMAL;
			else if (strcmp(Value, "dustbuster") == 0)
				Props->VacuumType = VACUUM_DUSTBUSTER;
			else
				ReportError("'%s' is not a valid value for the 'vacuum_type' property.  The value must be 'normal' or 'dustbuster'.  Fix this problem and re-save the TMX file.", Value);
		}
		else if (strcmp(Name, "vacuum_dir") == 0)
		{
			if (strcmp(Value, "up") == 0)
				Props->VacuumDir = VACUUMDIR_UP;
			else if (strcmp(Value, "down") == 0)
				Props->VacuumDir = VACUUMDIR_DOWN;
			else
				ReportError("'%s' is not a valid value for the 'vacuum_dir' property.  The value must be 'up' or 'down'.  Fix this problem and re-save the TMX file.", Value);
		}
		else if (strcmp(Name, "vacuum_start") == 0)
		{
            Props->VacuumStart = atoi(Value) * 64;
		}
		else if (strcmp(Name, "vacuum_speed") == 0)
		{
            Props->VacuumSpeed = (float)atof(Value);
		}
		else if (strcmp(Name, "ghost_race") == 0)
		{
			if (strcmp(Value, "on") == 0)
				Props->GhostRace = true;
			else if (strcmp(Value, "off") == 0)
				Props->GhostRace = false;
			else
				ReportError("'%s' is not a valid value for the 'ghost_race' property.  The value must be 'on' or 'off'.  Fix this problem and re-save the TMX file.", Value);
		}
		else if (strcmp(Name, "ghost_hat") == 0)
		{
			if (strcmp(Value, "girl") == 0)
				Props->GhostHat = DUSTYHAT_PINK_BOW;
			else
				ReportError("'%s' is not a valid value for the 'ghost_hat' property.  The value must be 'on' or 'off'.  Fix this problem and re-save the TMX file.", Value);
		}
		else if (strcmp(Name, "shadow_offset") == 0)
		{
            Props->ShadowOffsetX = strtof((char*)Value, (char**)&Value);
            Props->ShadowOffsetY = strtof((char*)Value, (char**)&Value);
		}
		else if (strcmp(Name, "shadow_scale") == 0)
		{
            Props->ShadowScaleX = strtof((char*)Value, (char**)&Value);
            Props->ShadowScaleY = strtof((char*)Value, (char**)&Value);
		}
		else if (strcmp(Name, "light_origin") == 0)
		{
            Props->LightOriginX = strtof((char*)Value, (char**)&Value);
            Props->LightOriginY = strtof((char*)Value, (char**)&Value);
		}
		else
			ReportError("'%s' is not a valid map property (value is '%s').  Fix this problem and re-save the TMX file.", Name, Value);

		PropertyNode = PropertyNode->next_sibling("property");
	}
}

static void OverridePagePropertiesFromPortfolio(SPageProperties* Props)
{
    Props->VacuumDir = Portfolio.UpsideDown ? VACUUMDIR_DOWN : VACUUMDIR_UP;
    Props->VacuumType = Portfolio.DustBuster ? VACUUM_DUSTBUSTER : VACUUM_NORMAL;
    Props->LightsOff = Portfolio.LightsOff;
}

// Loads a <tileset> node from either an internal or external tileset.
// FileName is used to locate image files.
static void LoadTileSetNode(rapidxml::xml_node<char>* TileSetNode, const char* FileName)
{
	// Grr, Tiled doesn't currently put a TSX version number in the TSX file.
	//if (strcmp(TileSetNode->first_attribute("version")->value(), "1.0") != 0)
	//	ReportError("Unsupported TSX file version.  Re-save the TSX file with the right version of Tiled.");

	if (atoi(TileSetNode->first_attribute("tilewidth")->value()) != 64)
		ReportError("Tile width must be 64.  Fix this problem and re-save the TSX or TMX file.");

	if (atoi(TileSetNode->first_attribute("tileheight")->value()) != 64)
		ReportError("Tile height must be 64.  Fix this problem and re-save the TSX or TMX file.");

	rapidxml::xml_node<char>* ImageNode = TileSetNode->first_node("image");
	if (ImageNode == NULL)
		ReportError("Missing <image> node.  Re-saving the TSX file may help.");

	if (ImageNode->next_sibling("image"))
		ReportError("Only be one image per tileset is supported.  Fix this problem and re-save the TSX or TMX file.");

	rapidxml::xml_attribute<char>* ImageSourceAttr = ImageNode->first_attribute("source");
	if (ImageSourceAttr == NULL)
		ReportError("Missing image source attribute.  Re-saving the TSX or TMX file may help.");

	// Set up the tileset structure.
	if (Chapter.NTileSets >= MAX_TILESETS)
		ReportError("Exceeded the maximum of %d total tilesets.", MAX_TILESETS);

	STileSet* TileSet = &Chapter.TileSets[Chapter.NTileSets];

	TileSet->Name = strdup(FileName);

	TileSet->FirstBlock = Chapter.NBlocks;

	// Calculate the path to the tileset file.
	char CurrentDirectory[1024];
	snprintf(CurrentDirectory, sizeof(CurrentDirectory), "%s", FileName);
	char* Slash = strrchr(CurrentDirectory, '/');
	if (Slash == NULL) Slash = strrchr(CurrentDirectory, '\\');
	if (Slash == NULL) Slash = CurrentDirectory;
	*Slash = 0;

	// Load the tileset image sprite.
    // It hasn't been loaded previously, so load the external tileset.
	char ImageFileName[1024];
    
    if (strncmp(ImageSourceAttr->value(), "../../", 6) == 0)
        snprintf(ImageFileName, sizeof(ImageFileName), "%s", ImageSourceAttr->value()+6);
    else
        snprintf(ImageFileName, sizeof(ImageFileName), "%s/%s", CurrentDirectory, ImageSourceAttr->value());

	LoadSpriteAsset(ImageFileName, &TileSet->Sprite);

	if (TileSet->Sprite.width == 0 || TileSet->Sprite.height == 0)
		ReportError("Invalid tileset image '%s'.  Fix this problem and re-save the TSX or TMX file.", ImageFileName);

	if (TileSet->Sprite.width % 64 || TileSet->Sprite.height % 64)
		ReportError("Tileset image '%s' width and height must be multiples of 64.  Fix this problem and re-save the TSX or TMX file.", ImageFileName);

	// Slice the image up into blocks.
	int BlocksX = TileSet->Sprite.width/64;
	int BlocksY = TileSet->Sprite.height/64;

	TileSet->NBlocks = BlocksX * BlocksY;

	for (int y = 0; y < BlocksY; y++)
	{
		for (int x = 0; x < BlocksX; x++)
		{
			if (Chapter.NBlocks >= MAX_BLOCKS)
				ReportError("Exceeded the maximum of %d total blocks.", MAX_BLOCKS);

			SBlock* Block = &Chapter.Blocks[Chapter.NBlocks];

			Block->Type = BLOCKTYPE_NORMAL;

			Block->ID = Chapter.NBlocks;
			Block->TileSet = Chapter.NTileSets;

			Block->SubX = x*64;
			Block->SubY = y*64;

			Block->Properties = NULL;

			Chapter.NBlocks++;
		}
	}

	// Scan the tileset for optional <tile> nodes (which contain block types and properties) and add them to the blocks.
	rapidxml::xml_node<char>* TileNode = TileSetNode->first_node("tile");
	while (TileNode)
	{
		int ID = atoi(TileNode->first_attribute("id")->value());

		if (ID < 0 || ID >= TileSet->NBlocks)
			ReportError("Invalid block ID.  Re-saving the TSX file may help.");

		PushErrorContext("While processing tile %d:\n", ID);

		SBlock* Block = &Chapter.Blocks[TileSet->FirstBlock + ID];

		rapidxml::xml_node<char>* PropertiesNode = TileNode->first_node("properties");
		if (PropertiesNode)
		{
			// First, scan for the 'type' property.
			rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
			while (PropertyNode)
			{
				const char* Name = PropertyNode->first_attribute("name")->value();
				const char* Value = PropertyNode->first_attribute("value")->value();

				if (strcmp(Name, "type") == 0)
				{
					// This code converts "type=blah" values in the tileset into actual block types.
					// To add a new special kind of block to the game, you need to add a check here.
					if (strcmp(Value, "nocollision") == 0)
					{
						Block->Type = BLOCKTYPE_NOCOLLIDE;
					}
					else if (strcmp(Value, "start") == 0)
					{
						Block->Type = BLOCKTYPE_CHAPTERSTART;
					}
					else if (strcmp(Value, "end") == 0)
					{
						Block->Type = BLOCKTYPE_CHAPTEREND;
					}
					else if (strcmp(Value, "barrel") == 0)
					{
						Block->Type = BLOCKTYPE_BARREL;
					}
					else if (strcmp(Value, "firework") == 0)
					{
						Block->Type = BLOCKTYPE_FIREWORK;
					}
					else if (strcmp(Value, "ball") == 0)
					{
						Block->Type = BLOCKTYPE_BALL;
					}
					else if (strcmp(Value, "gear") == 0)
					{
						Block->Type = BLOCKTYPE_GEAR;
					}
					else if (strcmp(Value, "coin") == 0)
					{
						Block->Type = BLOCKTYPE_COIN;
					}
					else if (strcmp(Value, "nail") == 0)
					{
						Block->Type = BLOCKTYPE_NAIL;
					}
					else if (strcmp(Value, "fan") == 0)
					{
						Block->Type = BLOCKTYPE_FAN;
					}
					else if (strcmp(Value, "flashlight_waypoint") == 0)
					{
						Block->Type = BLOCKTYPE_FLASHLIGHT_WAYPOINT;
					}
					else if (strcmp(Value, "flame") == 0)
					{
						Block->Type = BLOCKTYPE_FLAME;
					}
                    else if (strcmp(Value, "stapler") == 0)
                    {
                        Block->Type = BLOCKTYPE_STAPLER;
                    }
                    else if (strcmp(Value, "jello") == 0)
                    {
                        Block->Type = BLOCKTYPE_JELLO;
                    }
                    else if (strcmp(Value, "clothespin") == 0)
                    {
                        Block->Type = BLOCKTYPE_CLOTHESPIN;
                    }
                    else if (strcmp(Value, "powerup") == 0)
                    {
                        Block->Type = BLOCKTYPE_POWERUP;
                    }
                    else if (strcmp(Value, "tutorial") == 0)
                    {
                        Block->Type = BLOCKTYPE_TUTORIAL;
                    }
                    else if (strcmp(Value, "vacuum_trigger") == 0)
                    {
                        Block->Type = BLOCKTYPE_VACUUM_TRIGGER;
                    }
                    else if (strcmp(Value, "hanger") == 0)
                    {
                        Block->Type = BLOCKTYPE_HANGER;
                    }
                    else if (strcmp(Value, "baby") == 0)
                    {
                        Block->Type = BLOCKTYPE_BABY;
                    }
                    else if (strcmp(Value, "balloon") == 0)
                    {
                        Block->Type = BLOCKTYPE_BALLOON;
                    }
                    else
                    {
#ifdef PLATFORM_IPHONE_OR_MAC
                        printf("WARNING: Unknown block type '%s'.\n", Value);
#endif
                    }
				}
				else if (strcmp(Name, "material") == 0)
				{
					if (strcmp(Value, "normal") == 0)
					{
						Block->Material = MATERIAL_NORMAL;
					}
					else if (strcmp(Value, "breakable") == 0)
					{
						Block->Material = MATERIAL_BREAKABLE;
					}
					else if (strcmp(Value, "ice") == 0)
					{
						Block->Material = MATERIAL_ICE;
					}
					else if (strcmp(Value, "sticky") == 0)
					{
						Block->Material = MATERIAL_STICKY;
					}
				}

				PropertyNode = PropertyNode->next_sibling("property");
			}

			// If the block type requires additional properties, parse them here.
			if (Block->Type == BLOCKTYPE_BARREL)
			{
				ParseBarrelProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_FIREWORK)
			{
				ParseFireWorkProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_NAIL)
			{
				ParseNailProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_FAN)
			{
				ParseFanProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_FLAME)
			{
				ParseFlameProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_GEAR)
			{
				ParseGearProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_FLASHLIGHT_WAYPOINT)
			{
				ParseFlashlightWaypointProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_TUTORIAL)
			{
				ParseTutorialProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_VACUUM_TRIGGER)
			{
				ParseVacuumTriggerProperties(Block, PropertiesNode);
			}
			else if (Block->Type == BLOCKTYPE_HANGER)
			{
				ParseHangerProperties(Block, PropertiesNode);
			}
			else
			{
				// Make sure there aren't any extra properties (only 'type' is allowed).
				for (PropertyNode = PropertiesNode->first_node("property"); PropertyNode; PropertyNode = PropertyNode->next_sibling("property"))
				{
					const char* Name = PropertyNode->first_attribute("name")->value();
					const char* Value = PropertyNode->first_attribute("value")->value();

					if (strcmp(Name, "type") != 0 && strcmp(Name, "material") != 0)
						ReportError("Unrecognized tile property '%s'='%s'.", Name, Value);
				}
			}

			PopErrorContext();
		}
		TileNode = TileNode->next_sibling("tile");
	}

	Chapter.NTileSets++;
}

static void LoadTileSet(const char* FileName)
{
	PushErrorContext("While loading tileset '%s':\n", FileName);

    char* XML = (char*)LoadAssetFile(FileName, NULL, NULL);
    if (!XML)
		ReportError("Unable to open TSX file.  Check that all required files are present, and re-save the TMX file to fix.");

	// Parse the XML text buffer into a Document hierarchy.
	rapidxml::xml_document<> Document;
    try
    {
        Document.parse<0>(XML);
    }
    catch (rapidxml::parse_error e)
    {
        ReportError("Failed to parse tileset file:\n%s", e.what());
    }

	// Get the <map> node and validate everything extensively :)
	rapidxml::xml_node<char>* TileSetNode = Document.first_node("tileset");
	if (TileSetNode == NULL)
		ReportError("Missing <tileset> node.  Re-saving the TSX file may help.");

	LoadTileSetNode(TileSetNode, FileName);

	PopErrorContext();
}

static void ParseCSVData(rapidxml::xml_node<char>* DataNode, int Width, int Height, int NTileSetInfos, STileSetInfo* TileSetInfo, int* Blocks)
{
	rapidxml::xml_attribute<char>* DataEncodingAttr = DataNode->first_attribute("encoding");
	if (DataEncodingAttr == NULL || strcmp(DataEncodingAttr->value(), "csv") != 0)
		ReportError("Wrong map encoding.  Set encoding to CSV in the Tiled preferences and re-save the TMX file.");

	char* Data = DataNode->value();
	char* DataEnd = Data + DataNode->value_size();
	
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			if (Data >= DataEnd)
				ReportError("Unexpected end of tile data.  Re-saving the TMX file may help.");
			
			unsigned int ID = (unsigned int)strtoul(Data, &Data, 0);
			
			if (x < Width-1 || y < Height-1)
			{
				if (*Data != ',')
					ReportError("Tile data format is invalid.  Re-saving the TMX file may help.");
				Data++;
			}

            unsigned int FlipX = ID & SPECIALBLOCKID_FLIP_X;
            unsigned int FlipY = ID & SPECIALBLOCKID_FLIP_Y;
            unsigned int FlipDiagonal = ID & SPECIALBLOCKID_FLIP_DIAGONAL;
            ID &= SPECIALBLOCKID_MASK;

			if (ID == 0)
            {
                Blocks[y*Width + x] = SPECIALBLOCKID_BLANK;
            }
			else
			{
				int TileSetInfoIndex = -1;
				for (int i = NTileSetInfos - 1; i >= 0; i--)
				{
					if (ID >= TileSetInfo[i].FirstGID)
					{
						TileSetInfoIndex = i;
						break;
					}
				}

				if (TileSetInfoIndex == -1)
					ReportError("Tile data format is invalid.  Re-saving the TMX file may help.");

				ID = ID - TileSetInfo[TileSetInfoIndex].FirstGID + TileSetInfo[TileSetInfoIndex].TileSet->FirstBlock;

                Blocks[y*Width + x] = ID | FlipX | FlipY | FlipDiagonal;
			}
		}
		
		if (*Data != '\n')
			ReportError("Tile data format is invalid (but I'm being picky).  Re-saving the TMX file may help.");
		Data++;
	}
}

static void LoadPageFromTMX(const char* FileName)
{
	PushErrorContext("While loading page '%s':\n", FileName);

	// Open the TMX file.
    char* XML = (char*)LoadAssetFile(FileName, NULL, NULL);
    if (!XML)
		ReportError("Unable to open TMX file.  Check that all required files are present, and re-save the TMX file to fix.");
    
	// Parse the XML text buffer into a Document hierarchy.
	rapidxml::xml_document<> Document;
    try
    {
        Document.parse<0>(XML);
    }
    catch (rapidxml::parse_error e)
    {
        ReportError("Failed to parse page file:\n%s", e.what());
    }

	// Get the <map> node and validate everything extensively :)
	rapidxml::xml_node<char>* MapNode = Document.first_node("map");
	if (MapNode == NULL)
		ReportError("Missing <map> node.  Re-saving the TMX file may help.");

	if (strcmp(MapNode->first_attribute("version")->value(), "1.0") != 0)
		ReportError("Unsupported TMX file version.  Re-save the TMX file with the right version of Tiled.");
	
	if (strcmp(MapNode->first_attribute("orientation")->value(), "orthogonal") != 0)
		ReportError("Map orientation must be orthogonal.  Fix this problem and re-save the TMX file.");
	
	if (atoi(MapNode->first_attribute("width")->value()) < 12)
		ReportError("Map width must be at least 12.  Fix this problem and re-save the TMX file.");
	
	if (atoi(MapNode->first_attribute("height")->value()) < 1)
		ReportError("Invalid map height.  Fix this problem and re-save the TMX file.");
	
	// Scan the tilesets.  Load the tilesets that haven't already been loaded, and record information about each one into a local structure.
	// Specifically, we need to know which GID range in the layer data maps to each tileset.
	rapidxml::xml_node<char>* TileSetNode = MapNode->first_node("tileset");
	if (TileSetNode == NULL)
		ReportError("Missing <tileset> node.  Re-saving the TMX file may help.");

	STileSetInfo TileSetInfo[MAX_PAGE_TILESETS];
	int NTileSetInfos = 0;

	while (TileSetNode)
	{
		if (NTileSetInfos >= MAX_PAGE_TILESETS)
			ReportError("Too many tilesets. The maximum number of tilesets per page is %d.  Re-saving the TMX file may help.", MAX_PAGE_TILESETS);

		int TileSetIndex = -1;

		rapidxml::xml_attribute<char>* TileSetSourceAttr = TileSetNode->first_attribute("source");
		if (TileSetSourceAttr == NULL)
		{
            rapidxml::xml_attribute<char>* TileSetNameAttr = TileSetNode->first_attribute("name");
            ReportError("Detected internal tileset '%s'.  For consistency purposes, internal tilesets are no longer supported.  Fix this problem and re-save the TMX file.", TileSetNameAttr->value());
            
			// Load the internal tileset.
			LoadTileSetNode(TileSetNode, FileName);
			TileSetIndex = Chapter.NTileSets - 1;
		}
		else
		{
            // It hasn't been loaded previously, so load the external tileset.
            char TileSetName[1024];
            
            if (strncmp(TileSetSourceAttr->value(), "../../", 6) == 0)
                snprintf(TileSetName, sizeof(TileSetName), "%s", TileSetSourceAttr->value()+6);
            else
                snprintf(TileSetName, sizeof(TileSetName), "Chapters/%s/%s", CurrentChapterDir, TileSetSourceAttr->value());
            
			// If the external tileset has been loaded already, just reference it.
			for (int i = 0; i < Chapter.NTileSets; i++)
			{
				if (strcmp(Chapter.TileSets[i].Name, TileSetName) == 0)
				{
					TileSetIndex = i;
					break;
				}
			}

			if (TileSetIndex == -1)
			{
				LoadTileSet(TileSetName);
				TileSetIndex = Chapter.NTileSets - 1;
			}
		}

		TileSetInfo[NTileSetInfos].TileSet = &Chapter.TileSets[TileSetIndex];
		TileSetInfo[NTileSetInfos].FirstGID = atoi(TileSetNode->first_attribute("firstgid")->value());
		NTileSetInfos++;

		TileSetNode = TileSetNode->next_sibling("tileset");
	}

	// Get the <layer> node and validate.
	rapidxml::xml_node<char>* LayerNode = MapNode->first_node("layer");
	
	if (LayerNode == NULL)
		ReportError("Missing <layer> node.");
	
	// Get the <data> node and validate.
	rapidxml::xml_node<char>* DataNode = LayerNode->first_node("data");
	
	if (DataNode == NULL)
		ReportError("Missing <data> node.");

	// Build the SPage structure.
	if (Chapter.NPages >= MAX_PAGES)
		ReportError("Exceeded the maximum of %d total pages per chapter.", MAX_PAGES);

	SPage* Page = &Chapter.Pages[Chapter.NPages++];
	
	// Determine page name from the file name (no extension, no path).
	char Name[1024];
	snprintf(Name, sizeof(Name), "%s", FileName);
	
	char* Dot = strchr(Name, '.');
	if (Dot) *Dot = '\0';
	
	char* Slash = strrchr(Name, '/');
    if (Slash)
        Page->Name = strdup(Slash+1);
    else
    {
        Slash = strrchr(Name, '\\');
        if (Slash)
            Page->Name = strdup(Slash+1);
        else
            Page->Name = strdup(Name);
    }
	
	Page->Width = atoi(LayerNode->first_attribute("width")->value());
	Page->Height = atoi(LayerNode->first_attribute("height")->value());
	
	if (Page->Width < 12)
		ReportError("Page width is %d, must be at least %d.  Fix this problem and re-save the TMX file.", Page->Width, MIN_PAGE_WIDTH);

	if (Page->Height < 1)
		ReportError("Page height is %d, must be at least %d.  Fix this problem and re-save the TMX file.", Page->Height, MIN_PAGE_HEIGHT);

	// Allocate and fill the Blocks buffer.
	Page->Blocks = (int*)malloc(Page->Width * Page->Height * sizeof(int));

    ParseCSVData(DataNode, Page->Width, Page->Height, NTileSetInfos, TileSetInfo, Page->Blocks);

    // Load additional layers.
    Page->NLayers = 0;

	while (LayerNode->next_sibling("layer") != NULL)
    {
        LayerNode = LayerNode->next_sibling("layer");

        DataNode = LayerNode->first_node("data");
	    if (DataNode == NULL)
		    ReportError("Missing <data> node.");

	    if (atoi(LayerNode->first_attribute("width")->value()) != Page->Width)
            ReportError("Layer width does not match page width.  Fix this problem and re-save the TMX file.");

	    if (atoi(LayerNode->first_attribute("height")->value()) != Page->Height)
            ReportError("Layer height does not match page height.  Fix this problem and re-save the TMX file.");

        if (Page->NLayers >= MAX_PAGE_LAYERS)
            ReportError("Exceeded the maximum of %d extra layers per page.  Fix this problem and re-save the TMX file.", MAX_PAGE_LAYERS);

        SPageLayer* Layer = &Page->Layers[Page->NLayers++];

        Layer->Name = strdup(LayerNode->first_attribute("name")->value());
        
        Layer->LightList = LIGHTLIST_FOREGROUND_NO_SHADOW;
        Layer->Alpha = 1.0f;

		rapidxml::xml_node<char>* PropertiesNode = LayerNode->first_node("properties");
		if (PropertiesNode)
		{
			rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
			while (PropertyNode)
			{
				const char* Name = PropertyNode->first_attribute("name")->value();
				const char* Value = PropertyNode->first_attribute("value")->value();
                
				if (strcmp(Name, "list") == 0)
				{
                    if (strcmp(Value, "background") == 0)
                        Layer->LightList = LIGHTLIST_BACKGROUND;
                    else
                    if (strcmp(Value, "midground") == 0)
                        Layer->LightList = LIGHTLIST_MIDGROUND;
                    else
                    if (strcmp(Value, "foreground") == 0)
                        Layer->LightList = LIGHTLIST_FOREGROUND;
                    else
                        ReportError("Invalid light list '%s' for layer.", Value);
                }
                
                PropertyNode = PropertyNode->next_sibling("property");
            }
        }

        Layer->Blocks = (int*)malloc(Page->Width * Page->Height * sizeof(int));

        ParseCSVData(DataNode, Page->Width, Page->Height, NTileSetInfos, TileSetInfo, Layer->Blocks);
    }
	
	// Scan the properties.
	InitPageProperties(&Page->Props);

	rapidxml::xml_node<char>* PropertiesNode = MapNode->first_node("properties");
	if (PropertiesNode)
	{
		ParsePageProperties(&Page->Props, PropertiesNode);
	}

	PopErrorContext();
}

void LoadChapter(const char* ChapterDir)
{
    double StartTime = (float)GetCurrentTime();

	PushErrorContext("While loading chapter '%s':\n", ChapterDir);

	if (Chapter.NBlocks > 0 || Chapter.NPages > 0 || Chapter.NTileSets > 0)
		ReportError("Attempted to load a chapter without clearing it first.");

    Chapter.EndX = 0;
	Chapter.EndY = 0;

    Chapter.HasBackground = false;
    
	Chapter.NBlocks = 0;
	Chapter.NPages = 0;
	Chapter.NTileSets = 0;

	Chapter.Name = strdup(ChapterDir);

	CurrentChapterDir = ChapterDir;

	char FileName[1024];
	snprintf(FileName, sizeof(FileName), "Chapters/%s/Chapter.xml", ChapterDir);

    char* XML = (char*)LoadAssetFile(FileName, NULL, NULL);
    if (!XML)
		ReportError("Unable to open chapter file %s.  Check that all required files are present.", FileName);
    
    // Parse the XML text buffer into a Document hierarchy.
	rapidxml::xml_document<> Document;
    try
    {
        Document.parse<0>(XML);
    }
    catch (rapidxml::parse_error e)
    {
        ReportError("Failed to parse chapter file:\n%s", e.what());
    }
    
  	// Get the <Chapter> node and valiate.
	rapidxml::xml_node<char>* ChapterNode = Document.first_node("Chapter");
	if (ChapterNode == NULL)
		ReportError("Missing <Chapter> node.  Check for errors in the XML.");

    rapidxml::xml_attribute<char>* TitleAttr = ChapterNode->first_attribute("Title");
    if (TitleAttr)
        Chapter.Title = strdup(TitleAttr->value());
    else
        Chapter.Title = strdup("Untitled");

    rapidxml::xml_attribute<char>* BackgroundAttr = ChapterNode->first_attribute("Background");
    if (BackgroundAttr)
    {
        Chapter.HasBackground = true;
        LoadSpriteAsset(BackgroundAttr->value(), &Chapter.BackgroundSprite);
    }
    else
        Chapter.HasBackground = false;
    
    Chapter.Theme = THEME_STUDY;
    rapidxml::xml_attribute<char>* ThemeAttr = ChapterNode->first_attribute("Theme");
    if (ThemeAttr)
    {
        if (strcmp(ThemeAttr->value(), "study") == 0)
            Chapter.Theme = THEME_STUDY;
        else if (strcmp(ThemeAttr->value(), "pantry") == 0)
            Chapter.Theme = THEME_PANTRY;
        else if (strcmp(ThemeAttr->value(), "fridge") == 0)
            Chapter.Theme = THEME_FRIDGE;
        else if (strcmp(ThemeAttr->value(), "workshop") == 0)
            Chapter.Theme = THEME_WORKSHOP;
    }
    
    rapidxml::xml_attribute<char>* MusicAttr = ChapterNode->first_attribute("Music");
    if (MusicAttr)
    {
        LoadMusicAsset(MusicAttr->value(), &Chapter.Music, true);
        Chapter.HasMusic = true;
    }
    else
    {
        Chapter.HasMusic = false;
    }
    
    rapidxml::xml_attribute<char>* IntroMusicAttr = ChapterNode->first_attribute("IntroMusic");
    if (IntroMusicAttr)
    {
        LoadMusicAsset(IntroMusicAttr->value(), &Chapter.IntroMusic, false);
        Chapter.HasIntroMusic = true;
    }
    else
    {
        Chapter.HasIntroMusic = false;
    }
    
   	rapidxml::xml_node<char>* PageNode = ChapterNode->first_node("Page");

    while (PageNode != NULL)
	{
        rapidxml::xml_attribute<char>* NameAttr = PageNode->first_attribute("Name");
        if (NameAttr == NULL)
            ReportError("Page is missing the Name attribute.  Check for errors in the XML.");
     
		char FileName[1024];
		snprintf(FileName, sizeof(FileName), "Chapters/%s/%s.tmx", CurrentChapterDir, NameAttr->value());

        LoadPageFromTMX(FileName);

     	PageNode = PageNode->next_sibling("Page");
    }

	if (!Chapter.NPages)
		ReportError("Chapter contains no pages.");

	if (!Chapter.NBlocks)
		ReportError("Chapter contains no blocks.");

	PopErrorContext();
    
    double EndTime = GetCurrentTime();
    LogMessage("Loading chapter '%s' took %.1f seconds.\n", ChapterDir, EndTime-StartTime);
}

void ClearChapter()
{
	free((void*)Chapter.Name);
	Chapter.Name = NULL;

    free((void*)Chapter.Title);
	Chapter.Title = NULL;

    if (Chapter.HasIntroMusic)
        DestroyMusicAsset(&Chapter.IntroMusic);
    
    if (Chapter.HasMusic)
        DestroyMusicAsset(&Chapter.Music);
    
    if (Chapter.HasBackground)
        gxDestroySprite(&Chapter.BackgroundSprite);
    
	for (int i = 0; i < Chapter.NPages; i++)
	{
		free(Chapter.Pages[i].Name);
		free(Chapter.Pages[i].Blocks);

        for (int j = 0; j < Chapter.Pages[i].NLayers; j++)
        {
            free(Chapter.Pages[i].Layers[j].Name);
            free(Chapter.Pages[i].Layers[j].Blocks);
        }
	}
	Chapter.NPages = 0;

	for (int i = 0; i < Chapter.NTileSets; i++)
	{
		free(Chapter.TileSets[i].Name);
		gxDestroySprite(&Chapter.TileSets[i].Sprite);
	}
	Chapter.NTileSets = 0;

	for (int i = 0; i < Chapter.NBlocks; i++)
	{
		if (Chapter.Blocks[i].Properties)
        {
            // TODO FLAME!!!!
//            if (Chapter.Blocks[i].Type == BLOCKTYPE_GEAR)
//                FreeGearProperties(&Chapter.Blocks[i]);
//            else
                free(Chapter.Blocks[i].Properties);
            Chapter.Blocks[i].Properties = NULL;
        }
	}
	Chapter.NBlocks = 0;
    
    ClearFlamePropertiesList();

	if (Chapter.PageBlocks)
	{
		free(Chapter.PageBlocks);
		Chapter.PageBlocks = NULL;
	}
}

void StartChapterMusic()
{
    StopMusicTrack(MUSIC_CHAPTER);
    
    if (Chapter.HasIntroMusic)
        QueueMusicTrack(MUSIC_CHAPTER, &Chapter.IntroMusic);
    if (Chapter.HasMusic)
        QueueMusicTrack(MUSIC_CHAPTER, &Chapter.Music);
}

void RestartChapterMusic()
{
    StopMusicTrack(MUSIC_CHAPTER);
    
    if (Chapter.HasMusic)
        QueueMusicTrack(MUSIC_CHAPTER, &Chapter.Music);
}

static void ClearPageObjects()
{
	ClearBarrels();
	ClearFans();
	ClearCoins();
	ClearFireWorks();
	ClearBalls();
	ClearGears();
	ClearStaplers();
	ClearFlames();
	ClearFlashlightWaypoints();
	ClearPowerUps();
    ClearTutorials();
    ClearVacuumTriggers();
    ClearHangers();
    ClearBabies();
    ClearBalloons();

	if (Chapter.PageBlocks)
	{
		free(Chapter.PageBlocks);
		Chapter.PageBlocks = NULL;
	}
}

static void CreatePageObjects()
{
	// Process any special blocks that need to create dynamic objects.
	for (int y = 0; y < Chapter.PageHeight; y++)
	{
		for (int x = 0; x < Chapter.PageWidth; x++)
		{
			unsigned int BlockID = Chapter.PageBlocks[y * Chapter.PageWidth + x];
            unsigned int Flags = BlockID & ~SPECIALBLOCKID_MASK;
            BlockID &= SPECIALBLOCKID_MASK;

			if (BlockID < SPECIALBLOCKID_FIRST)
			{
				if (BlockID > Chapter.NBlocks)
					ReportError("Invalid block ID encountered when creating page objects.");
				
				SBlock* Block = &Chapter.Blocks[BlockID];

				switch (Block->Type)
				{
				case BLOCKTYPE_CHAPTERSTART:
                    if (Portfolio.UpsideDown)
                    {
                        Chapter.EndX = (float)x * 64 + 32;
                        Chapter.EndY = (float)y * 64 + 32;
                    }
                    else
                    {
                        Chapter.StartX = (float)x * 64 + 32;
                        Chapter.StartY = (float)y * 64 + 64;
                        Chapter.StartDirection = (Flags & SPECIALBLOCKID_FLIP_X) ? DIRECTION_LEFT : DIRECTION_RIGHT;
                    }
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_CHAPTEREND:
                    if (Portfolio.UpsideDown)
                    {
                        Chapter.StartX = (float)x * 64 + 32;
                        Chapter.StartY = (float)y * 64 + 64;
                        Chapter.StartDirection = (Flags & SPECIALBLOCKID_FLIP_X) ? DIRECTION_LEFT : DIRECTION_RIGHT;
                    }
                    else
                    {
                        Chapter.EndX = (float)x * 64 + 32;
                        Chapter.EndY = (float)y * 64 + 32;
                    }
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_BARREL:
					if (Portfolio.Barrels) CreateBarrel(x * 64, y * 64, (SBarrelProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_FIREWORK:
					if (Portfolio.Fireworks) CreateFireWork(x * 64, y * 64, (SFireWorkProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_BALL:
					if (Portfolio.Gears) CreateBall(x * 64, y * 64);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_GEAR:
					if (Portfolio.Gears) CreateGear(x * 64, y * 64, (SGearProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_COIN:
					if (Portfolio.Coins) CreateCoin(x * 64, y * 64);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_FAN:
					if (Portfolio.Fans) CreateFan(x * 64, y * 64, (SFanProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
                case BLOCKTYPE_FLAME:
                    CreateFlame(x * 64, y * 64, Flags, (SFlameProperties*)Block->Properties);
                    break;
				case BLOCKTYPE_FLASHLIGHT_WAYPOINT:
					if (Portfolio.LightsOff) CreateFlashlightWaypoint(x * 64, y * 64, (SFlashlightWaypointProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
                case BLOCKTYPE_STAPLER:
                    if (Portfolio.Staplers) CreateStapler(x * 64, y * 64, STAPLER_STAPLER);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_JELLO:
                    if (Portfolio.Staplers) CreateStapler(x * 64, y * 64, STAPLER_JELLO);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_CLOTHESPIN:
                    if (Portfolio.Staplers) CreateStapler(x * 64, y * 64, STAPLER_CLOTHESPIN);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_POWERUP:
                    if (Portfolio.Powerups) CreatePowerUp(x * 64, y * 64);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_TUTORIAL:
                    CreateTutorial(x * 64, y * 64, (STutorialProperties*)Block->Properties);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_VACUUM_TRIGGER:
                    // Vacuum triggers made obsolete by portfolio system.
                    //CreateVacuumTrigger(x * 64, y * 64, (SVacuumTriggerProperties*)Block->Properties);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_HANGER:
                    CreateHanger(x, y, Flags, (SHangerProperties*)Block->Properties);
                    break;
                case BLOCKTYPE_BABY:
                    if (Portfolio.Babies) CreateBaby(x, y, Flags, Random(0, 2) == 0 ? DUSTYHAT_NONE : DUSTYHAT_PINK_BOW, false);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_BALLOON:
                    if (Portfolio.Balloons) CreateBalloon(x, y);
                    EraseBlock(x, y);
                    break;
                default:
                    break;
				}
			}
		}
	}

	// Set initial scrolling.
    ScrollX = 0;
	ScrollY = 768 - Chapter.StartY;

	// Initialize global stuff for the page.
	InitDusty();
	SetDustyPosition(Chapter.StartX, Chapter.StartY, (EDustyDirection)Chapter.StartDirection);

	InitDust();
	InitDebris();
	InitFlashlight();
    InitTutorial();
    
	InitVacuum();
    InitSmoke();
    
    if (!Chapter.PageProps.VacuumOff)
        TurnOnVacuum(Portfolio.VacuumDistance, 2.0f, false);
    
    for (int i = 0; i < Portfolio.BabyCount; i++)
        CreateBaby(Chapter.StartX/64, (Chapter.StartY-32)/64, 0, Portfolio.BabyHats[i], true);
    
    ResetLightState();
    
    LightState.LightsOff = Chapter.PageProps.LightsOff;
    
	if (Chapter.PageProps.LightsOff)
		LightState.AmbientColor = gxRGBA32(16, 16, 16, 255);
	else
		LightState.AmbientColor = gxRGBA32(128, 128, 128, 255);
}

static bool IsPortfolioChapter()
{
    if (strcmp(Chapters[CurrentChapter].Name, "Training") == 0)
        return false;
    
    if (strcmp(Chapters[CurrentChapter].Name, "Credits") == 0)
        return false;
    
    return true;
}

void SetCurrentPage(int PageNum)
{
	if (PageNum < 0 || PageNum >= Chapter.NPages)
		ReportError("%d is not a valid page number.", PageNum);

	// Clear out objects from previous page.
	ClearPageObjects();

#ifdef PLATFORM_IPHONE
    [TestFlight passCheckpoint:[NSString stringWithFormat:@"Entered page %s", Chapter.Pages[PageNum].Name]];
#endif
    
	PushErrorContext("While setting the current page to '%s':\n", Chapter.Pages[PageNum].Name);

	Chapter.PageNum = PageNum;
	Chapter.PageWidth = Chapter.Pages[PageNum].Width;
	Chapter.PageHeight = Chapter.Pages[PageNum].Height;

	Chapter.PageBlocks = (int*)malloc(Chapter.PageWidth * Chapter.PageHeight * sizeof(unsigned int));

    if (Portfolio.MirrorPage)
    {
        for (int y = 0; y < Chapter.PageHeight; y++)
            for (int x = 0; x < Chapter.PageWidth; x++)
                Chapter.PageBlocks[y*Chapter.PageWidth + x] = Chapter.Pages[PageNum].Blocks[y*Chapter.PageWidth + (Chapter.PageWidth - 1 - x)] ^ SPECIALBLOCKID_FLIP_X;
    }
    else
    {
        memcpy(Chapter.PageBlocks, Chapter.Pages[PageNum].Blocks, Chapter.PageWidth * Chapter.PageHeight * sizeof(unsigned int));        
    }
    
    for (int i = 0; i < Chapter.Pages[PageNum].NLayers; i++)
    {
        SPageLayer* Layer = &Chapter.Pages[PageNum].Layers[i];
        
        if (strncasecmp(Layer->Name, "sticky", 6) == 0)
        {
            if (Portfolio.Sticky)
            {
                if (Portfolio.MirrorPage)
                {
                    for (int y = 0; y < Chapter.PageHeight; y++)
                    {
                        for (int x = 0; x < Chapter.PageWidth; x++)
                        {
                            if (Layer->Blocks[y*Chapter.PageWidth + x] != SPECIALBLOCKID_BLANK)
                                Chapter.PageBlocks[y*Chapter.PageWidth + (Chapter.PageWidth-1-x)] |= SPECIALBLOCKID_STICKY;
                        }
                    }
                }
                else
                {
                    for (int y = 0; y < Chapter.PageHeight; y++)
                    {
                        for (int x = 0; x < Chapter.PageWidth; x++)
                        {
                            if (Layer->Blocks[y*Chapter.PageWidth + x] != SPECIALBLOCKID_BLANK)
                                Chapter.PageBlocks[y*Chapter.PageWidth + x] |= SPECIALBLOCKID_STICKY;
                        }
                    }
                }
                Layer->Alpha = 1.0f;
                    
            }
            else
            {
                Layer->Alpha = 0.0f;
            }
        }
        else
        {
            for (int j = 0; j < ARRAY_COUNT(PortfolioEntries); j++)
            {
                bool IsPositiveMergeLayer = strncasecmp(Layer->Name, PortfolioEntries[j].Name, strlen(PortfolioEntries[j].Name)) == 0;
                bool IsNegativeMergeLayer = Layer->Name[0] == '!' && strncasecmp(Layer->Name+1, PortfolioEntries[j].Name, strlen(PortfolioEntries[j].Name)) == 0;
                
                if ((IsPositiveMergeLayer && *PortfolioEntries[j].Value) || (IsNegativeMergeLayer && !*PortfolioEntries[j].Value))
                {
                    for (int y = 0; y < Chapter.PageHeight; y++)
                    {
                        for (int x = 0; x < Chapter.PageWidth; x++)
                        {
                            if (Layer->Blocks[y*Chapter.PageWidth + x] != SPECIALBLOCKID_BLANK)
                                Chapter.PageBlocks[y*Chapter.PageWidth + x] = Layer->Blocks[y*Chapter.PageWidth + x];
                        }
                    }                    
                    Layer->Alpha = 1.0f;
                }
                else
                {
                    if (IsPositiveMergeLayer || IsNegativeMergeLayer)
                        Layer->Alpha = 0.0f;
                }
            }
        }
    }
    
	Chapter.PageProps = Chapter.Pages[PageNum].Props;
    
    if (IsPortfolioChapter())
        OverridePagePropertiesFromPortfolio(&Chapter.PageProps);
    
	Chapter.PortalAngle = 0;

	CreatePageObjects();

    StartRecording();
    InitMinimap();

    StartGhostRecording();
    
//    if (Settings.GhostActive)
//    {
//        LoadGhost(Chapters[CurrentChapter].Name, Chapter.Pages[Chapter.PageNum].Name, false);
//        StartGhostPlayback();
//    }
//    else
//    {
//        if (Chapter.PageProps.GhostRace)
//        {
//            LoadGhost(Chapters[CurrentChapter].Name, Chapter.Pages[Chapter.PageNum].Name, true);
//            StartGhostPlayback();        
//        }        
//    }

	PopErrorContext();
}

void CalculateScroll()
{
	// If Dusty is dead, freeze scrolling while he falls off the screen.
	if (Dusty.State == DUSTYSTATE_DIE)
		return;

    if (TutorialOverrides.FocusOnVacuum)
    {
//        ScrollX = ScrollX*0.9f + (LitScreenHeight/2-Vacuum.X)*0.1f;
//        ScrollY = ScrollY*0.95f + (LitScreenHeight*0.75f-Vacuum.Y)*0.05f;
        ScrollY = Remap(SmoothStep(TutorialOverrides.Timer), 0.0f, 1.0f, TutorialOverrides.SavedScrollY, (LitScreenHeight*0.65f-Vacuum.Y), true);
    }
    else
    {
        //float TopY = Vacuum.Charging ? 500 : 600;
        if (Portfolio.UpsideDown)
        {
            float TopY = 200;
            
            // Screen tracks Dusty upwards.
            if (Dusty.FloatY + ScrollY < TopY)
            {
                ScrollY = TopY - Dusty.FloatY;
            }
            
            // Screen also tracks Dusty downwards.
            if (Dusty.FloatY + ScrollY > LitScreenHeight - 600)
            {
                ScrollY = (LitScreenHeight - 600) - (int)Dusty.FloatY;
            }
        }
        else
        {
            float TopY = 600;
            
            // Screen tracks Dusty upwards.
            if (Dusty.FloatY + ScrollY < TopY)
            {
                ScrollY = TopY - Dusty.FloatY;
            }

            // Screen also tracks Dusty downwards.
            if (Dusty.FloatY + ScrollY > LitScreenHeight - 200)
            {
                ScrollY = (LitScreenHeight - 200) - (int)Dusty.FloatY;
            }
        }

        // Prevent scrolling off bottom of map.
        if (ScrollY < -(Chapter.PageHeight * 64 - LitScreenHeight))
        {
            ScrollY = -(Chapter.PageHeight * 64 - LitScreenHeight);
        } 

        // Prevent scrolling off top of map.
        if (ScrollY > 0)
        {
            ScrollY = 0;
        }
        
        //float XRatio = Dusty.FloatX/(Chapter.PageWidth*64);
        //    XRatio = powf(XRatio*2.0f-1.0f, 1.0f) *0.5f+0.5f;
        
        //ScrollX = -XRatio * (Chapter.PageWidth*64 - 768);
        ScrollX = -Dusty.FloatX + 384;
        
        if (ScrollX < -(Chapter.PageWidth * 64 - 768))
        {
            ScrollX = (float)-(Chapter.PageWidth * 64 - 768);
        } 
        
        if (ScrollX > 0)
        {
            ScrollX = 0;
        }
    }
}

void DisplayPortal()
{
    if (Chapter.EndY + 256 + ScrollY >= 0 && 
        Chapter.EndY - 256 + ScrollY <= LitScreenHeight)
    {
        Chapter.PortalAngle += 2.0f*PI / 60.0f * 0.1f; 
        for (int i = 5; i > 4; i--)
        {
            float Alpha = (float)i / 5.0f;
            float Angle = Chapter.PortalAngle - (1.0f-Alpha);
            AddLitSpriteCenteredScaledRotatedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &PortalSprite, Chapter.EndX + ScrollX, Chapter.EndY - 50 + ScrollY, 1.0f, Angle, Alpha);
        }
    }
    
    if (Chapter.PageProps.LightsOff)
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_LIGHTING, &LightFlashlightSprite, Chapter.EndX + ScrollX, Chapter.EndY - 50 + ScrollY, 2.0f, 1.0f);
}

static void DisplayChapterLayer(ELightList LightList, int* Blocks, float Alpha, bool BaseLayer)
{
    unsigned int Color = gxRGBA32(255, 255, 255, (int)(255*Alpha));
    
    float CurY = ScrollY;
	for (int y = 0; y < Chapter.PageHeight; y++, CurY += 64)
	{
		// Skip rows of tiles that cannot be on screen.
		if (CurY > LitScreenHeight || CurY+64 < 0)
			continue;

        float CurX = ScrollX;
		for (int x = 0; x < Chapter.PageWidth; x++, CurX += 64)
		{
            int SrcX;
            int ToggleFlag;
            if (!BaseLayer && Portfolio.MirrorPage)
            {
                SrcX = Chapter.PageWidth - 1 - x;
                ToggleFlag = SPECIALBLOCKID_FLIP_X;
            }
            else
            {
                SrcX = x;
                ToggleFlag = 0;
            }
            
		    //if (CurX > gxScreenWidth || CurX+64 < 0)
			   // continue;

            unsigned int ID = Blocks[y * Chapter.PageWidth + SrcX] ^ ToggleFlag;
            int BlockID = ID & SPECIALBLOCKID_MASK;

			if (BlockID >= SPECIALBLOCKID_FIRST)
			{
				switch (BlockID)
				{
				case SPECIALBLOCKID_BLANK: 
					// Nothing to draw.
					break;
				}
			}
			else
			{
				SBlock* Block = &Chapter.Blocks[BlockID];

				STileSet* TileSet = &Chapter.TileSets[Block->TileSet];

				float U0 = (float)(Block->SubX) / TileSet->Sprite.width;
				float U1 = (float)(Block->SubX + 64) / TileSet->Sprite.width;
				float U2 = (float)(Block->SubX + 64) / TileSet->Sprite.width;
				float U3 = (float)(Block->SubX) / TileSet->Sprite.width;

				float V0 = (float)(Block->SubY) / TileSet->Sprite.height;
				float V1 = (float)(Block->SubY) / TileSet->Sprite.height;
				float V2 = (float)(Block->SubY + 64) / TileSet->Sprite.height;
				float V3 = (float)(Block->SubY + 64) / TileSet->Sprite.height;
                
                float m11 = 64, m12 = 0, m21 = 0, m22 = 64;
                float dx = CurX, dy = CurY;
                
                if (ID & SPECIALBLOCKID_FLIP_DIAGONAL) 
                {
                    Swap(&m11, &m12);
                    Swap(&m21, &m22);
                }
                if (ID & SPECIALBLOCKID_FLIP_X) 
                {
                    m11 = -m11;
                    m21 = -m21;    
                    dx += 64;
                }
                if (ID & SPECIALBLOCKID_FLIP_Y) 
                {
                    m12 = -m12;
                    m22 = -m22;
                    dy += 64;
                }
                
                float X0 = 0.0f*m11 + 0.0f*m21 + dx;
                float Y0 = 0.0f*m12 + 0.0f*m22 + dy;
                
                float X1 = 1.0f*m11 + 0.0f*m21 + dx;
                float Y1 = 1.0f*m12 + 0.0f*m22 + dy;
                
                float X2 = 1.0f*m11 + 1.0f*m21 + dx;
                float Y2 = 1.0f*m12 + 1.0f*m22 + dy;
                
                float X3 = 0.0f*m11 + 1.0f*m21 + dx;
                float Y3 = 0.0f*m12 + 1.0f*m22 + dy;
                
                AddLitQuad(LightList, &TileSet->Sprite, Color,
                           X0, Y0, U0, V0, 
                           X1, Y1, U1, V1,
                           X2, Y2, U2, V2, 
                           X3, Y3, U3, V3);
			}
		}
	}
}

void DisplayChapterBaseLayer()
{
    DisplayChapterLayer(LIGHTLIST_FOREGROUND, Chapter.PageBlocks, 1.0f, true);
}

void DisplayChapterExtraLayers()
{
    SPage* Page = &Chapter.Pages[Chapter.PageNum];

    for (int i = 0; i < Page->NLayers; i++)
    {
        SPageLayer* Layer = &Page->Layers[i];
        if (Layer->Alpha == 0.0f)
            continue;
        DisplayChapterLayer(Layer->LightList, Layer->Blocks, Layer->Alpha, false);
    }
}

int GetBlockID(int x, int y)
{
	// Requests for blocks outside the map return a special value.
	if (x < 0 || x >= Chapter.PageWidth)
		return SPECIALBLOCKID_OUTOFBOUNDS;
	if (y < 0 || y >= Chapter.PageHeight)
		return SPECIALBLOCKID_OUTOFBOUNDS;
    
	return Chapter.PageBlocks[y * Chapter.PageWidth + x] & SPECIALBLOCKID_MASK;
}

int GetBlockType(int x, int y)
{
    int ID = GetBlockID(x, y);
    if (ID < SPECIALBLOCKID_FIRST)
        return Chapter.Blocks[ID].Type;
    else
        return BLOCKTYPE_NORMAL;
}

int GetBlockFlags(int x, int y)
{
	// Requests for blocks outside the map return a special value.
	if (x < 0 || x >= Chapter.PageWidth)
		return SPECIALBLOCKID_OUTOFBOUNDS;
	if (y < 0 || y >= Chapter.PageHeight)
		return SPECIALBLOCKID_OUTOFBOUNDS;
    
	return Chapter.PageBlocks[y * Chapter.PageWidth + x] & ~SPECIALBLOCKID_MASK;
}

bool IsBlockEmpty(int x, int y)
{
	// Requests for blocks outside the map return solid.
	if (x < 0 || x >= Chapter.PageWidth)
		return false;
	if (y < 0 || y >= Chapter.PageHeight)
		return false;
    
    if (GetBlockType(x, y) == BLOCKTYPE_NOCOLLIDE)
        return true;

	if ((Chapter.PageBlocks[y * Chapter.PageWidth + x] & SPECIALBLOCKID_MASK) == SPECIALBLOCKID_BLANK)
		return true;
	else
		return false;
}

bool IsBlockSolid(int x, int y)
{
	return !IsBlockEmpty(x, y);
}

void EraseBlock(int x, int y)
{
	if (x < 0 || x >= Chapter.PageWidth)
		return;
	if (y < 0 || y >= Chapter.PageHeight)
		return;

	Chapter.PageBlocks[y * Chapter.PageWidth + x] = SPECIALBLOCKID_BLANK;
}

void ClearChapterList()
{
    for (int i = 0; i < NChapters; i++)
    {
        free(Chapters[i].Name);
        
        if (Chapters[i].HasBackground)
            gxDestroySprite(&Chapters[i].BackgroundSprite);
    }
    
    NChapters = 0;
}

void LoadChapterList()
{
	PushErrorContext("While loading the list of chapters:\n");
    
    char* XML = (char*)LoadAssetFile("Chapters/ChapterList.xml", NULL, NULL);
    if (!XML)
		ReportError("Unable to open chapter list file Chapters/ChapterList.xml.  Check that all required files are present.");
    
	// Parse the XML text buffer into a Document hierarchy.
	rapidxml::xml_document<> Document;
    try
    {
        Document.parse<0>(XML);
    }
    catch (rapidxml::parse_error e)
    {
        ReportError("Failed to parse chapter list file:\n%s", e.what());
    }
    
	// Get the <ChapterList> node.
	rapidxml::xml_node<char>* ChapterListNode = Document.first_node("ChapterList");
	if (ChapterListNode == NULL)
		ReportError("Missing <ChapterList> node.  Check for errors in the XML.");
    
	rapidxml::xml_node<char>* ChapterNode = ChapterListNode->first_node("Chapter");
    
	while (ChapterNode != NULL)
	{
        rapidxml::xml_attribute<char>* NameAttr = ChapterNode->first_attribute("Name");
        if (NameAttr == NULL)
            ReportError("Chapter is missing the Name attribute.  Check for errors in the XML.");
        
        if (NChapters >= MAX_CHAPTERS)
            ReportError("Exceeded the maximum of %d chapters.", MAX_CHAPTERS);

        SChapterListEntry* Chapter = &Chapters[NChapters];
        
        Chapter->Name = strdup(NameAttr->value());
        
        rapidxml::xml_attribute<char>* BackgroundAttr = ChapterNode->first_attribute("Background");
        if (BackgroundAttr)
        {
            LoadSpriteAsset(BackgroundAttr->value(), &Chapter->BackgroundSprite);
            Chapter->HasBackground = true;
        }
        else
            Chapter->HasBackground = false;
        
        NChapters++;
        
		ChapterNode = ChapterNode->next_sibling("Chapter");
	}
    
	PopErrorContext();
}

static void ReportPortfolio(const char* Message, ...)
{
    char Work[256];
    
	va_list args;
	va_start(args, Message);
	vsnprintf(Work, sizeof(Work), Message, args);
	va_end(args);

    AddDebugText(Work, FORMAT_CENTER_X, 384, 200 + 64*PortfolioLine, 0.7f, gxRGB32(255, 192, 128), 5.0f);
    PortfolioLine = PortfolioLine >= 5 ? 0 : PortfolioLine + 1;
}

void ResetPortfolio()
{
    memset(&Portfolio, 0, sizeof(Portfolio));
}

static void PreparePortfolioForChanges()
{
    for (int i = 0; i < ARRAY_COUNT(PortfolioEntries); i++)
        PortfolioEntries[i].Dirty = false;
}

static void EnableRandomPortfolio(int Flags, int FlagsMask, bool Value)
{
    SPortfolioEntry* EnabledElements[ARRAY_COUNT(PortfolioEntries)];
    int EnabledElementCount = 0;
    
    for (int i = 0; i < ARRAY_COUNT(PortfolioEntries); i++)
        if ((PortfolioEntries[i].Flags & FlagsMask) == Flags && *PortfolioEntries[i].Value != Value)
            if (!PortfolioEntries[i].Dirty)
                EnabledElements[EnabledElementCount++] = &PortfolioEntries[i];
    
    if (EnabledElementCount)
    {
        int ElementIndex = Random(0, EnabledElementCount);
        SPortfolioEntry* Element = EnabledElements[ElementIndex];
        *Element->Value = Value;
        Element->Dirty = true;
        
        ReportPortfolio("%s %s", Element->Name, Value ? "enabled" : "disabled");
    }
}

static void EnableAllPortfolio(int Flags, int FlagsMask, bool Value)
{
    SPortfolioEntry* EnabledElements[ARRAY_COUNT(PortfolioEntries)];
    int EnabledElementCount = 0;
    
    for (int i = 0; i < ARRAY_COUNT(PortfolioEntries); i++)
        if ((PortfolioEntries[i].Flags & FlagsMask) == Flags && *PortfolioEntries[i].Value != Value)
            EnabledElements[EnabledElementCount++] = &PortfolioEntries[i];
    
    if (EnabledElementCount)
    {
        for (int ElementIndex = 0; ElementIndex < EnabledElementCount; ElementIndex++)
        {
            *EnabledElements[ElementIndex]->Value = Value;
            
            ReportPortfolio("%s %s", EnabledElements[ElementIndex]->Name, Value ? "enabled" : "disabled");
        }
    }
}

void LoadPortfolio()
{
    PushErrorContext("While loading portfolio:\n");
    
    char* XML = (char*)LoadAssetFile("Portfolio.xml", NULL, NULL);
    if (!XML)
        ReportError("Failed to open portfolio file 'Portfolio.xml'.  Check that the file exists.");
    
    // Parse the XML text buffer into a Document hierarchy.
    rapidxml::xml_document<> Document;
    try
    {
        Document.parse<0>(XML);
    }
    catch (rapidxml::parse_error e)
    {
        ReportError("Failed to parse portfolio file:\n%s", e.what());
    }
    
    // Get the <Portfolio> node and validate.
    rapidxml::xml_node<char>* PortfolioNode = Document.first_node("Portfolio");
    if (PortfolioNode == NULL)
        ReportError("Missing <Portfolio> node.  Check for errors in the XML.");

    rapidxml::xml_node<char>* InitialNode = PortfolioNode->first_node("Initial");
    if (InitialNode == NULL)
        ReportError("Missing <Initial> node.  Check for errors in the XML.");
    
    for (int i = 0; i < ARRAY_COUNT(PortfolioEntries); i++)
        if (InitialNode->first_attribute(PortfolioEntries[i].Name) && atoi(InitialNode->first_attribute(PortfolioEntries[i].Name)->value()) != 0)
            PortfolioEntries[i].Flags |= PORTFOLIO_INITIAL;

    rapidxml::xml_node<char>* ChangeElementsNode = PortfolioNode->first_node("ChangeElements");
    if (ChangeElementsNode == NULL)
        ReportError("Missing <ChangeElements> node.  Check for errors in the XML.");
    Portfolio.ElementChangeFrequency = ChangeElementsNode->first_attribute("frequency") ? atoi(ChangeElementsNode->first_attribute("frequency")->value()) : 1;

    rapidxml::xml_node<char>* ChangeChapterNode = PortfolioNode->first_node("ChangeChapter");
    if (ChangeChapterNode == NULL)
        ReportError("Missing <ChangeChapter> node.  Check for errors in the XML.");
    Portfolio.ChapterChangeFrequency = ChangeChapterNode->first_attribute("frequency") ? atoi(ChangeChapterNode->first_attribute("frequency")->value()) : 1;
    Portfolio.FirstStoryPageFrequency = ChangeChapterNode->first_attribute("firstStoryFrequency") ? atoi(ChangeChapterNode->first_attribute("firstStoryFrequency")->value()) : 1;
    Portfolio.StoryPageFrequency = ChangeChapterNode->first_attribute("storyFrequency") ? atoi(ChangeChapterNode->first_attribute("storyFrequency")->value()) : 1;

    rapidxml::xml_node<char>* VacuumNode = PortfolioNode->first_node("Vacuum");
    if (VacuumNode == NULL)
        ReportError("Missing <Vacuum> node.  Check for errors in the XML.");
    Portfolio.InitialVacuumSpeed = VacuumNode->first_attribute("initialSpeed") ? atof(VacuumNode->first_attribute("initialSpeed")->value()) : 1;
    Portfolio.VacuumSpeedChange = VacuumNode->first_attribute("speedChange") ? atof(VacuumNode->first_attribute("speedChange")->value()) : 1;
    Portfolio.VacuumSpeedChangeFrequency = VacuumNode->first_attribute("frequency") ? atoi(VacuumNode->first_attribute("frequency")->value()) : 1;
    Portfolio.VacuumDamageSpeedModifier = VacuumNode->first_attribute("damageSpeedModifier") ? atoi(VacuumNode->first_attribute("damageSpeedModifier")->value()) : 1;
    Portfolio.InitialVacuumDistance = VacuumNode->first_attribute("initialDistance") ? atoi(VacuumNode->first_attribute("initialDistance")->value()) : 1;

    rapidxml::xml_node<char>* DebugNode = PortfolioNode->first_node("Debug");
    if (DebugNode)
    {
        PortfolioLine = 0;
        
        Portfolio.VacuumDistance = Portfolio.InitialVacuumDistance;

        Portfolio.VacuumSpeed = Portfolio.InitialVacuumSpeed;
        ReportPortfolio("Vacuum speed %.1f", Portfolio.VacuumSpeed);
        
        for (int i = 0; i < ARRAY_COUNT(PortfolioEntries); i++)
            *PortfolioEntries[i].Value = DebugNode->first_attribute(PortfolioEntries[i].Name) && atoi(DebugNode->first_attribute(PortfolioEntries[i].Name)->value()) != 0;

        if (Portfolio.DustBuster)
        {
            Vacuum.Side = VACUUMSIDE_TOP; // Force intro
            Vacuum.Dir = VACUUMDIR_DOWN;
            Portfolio.VacuumSide = Vacuum.Side;
            Portfolio.VacuumDir = Vacuum.Dir;
        }
        else
        {
            Portfolio.VacuumDir = Portfolio.UpsideDown ? VACUUMDIR_DOWN : VACUUMDIR_UP;
        }

        Portfolio.Chapter = DebugNode->first_attribute("Chapter") ? atoi(DebugNode->first_attribute("Chapter")->value()) : 0;
        CurrentChapter = Portfolio.Chapter;
        LoadCurrentChapter();

        Portfolio.Page = DebugNode->first_attribute("Page") ? atoi(DebugNode->first_attribute("Page")->value()) : 0;
        SetCurrentPage(Portfolio.Page);
    }
    else
    {
        SetupInitialPortfolio();
    }

    PopErrorContext();
}

void SetupInitialPortfolio()
{
    PortfolioLine = 0;
    
    // Start with initials + extra pro.
    PreparePortfolioForChanges();
    EnableAllPortfolio(PORTFOLIO_INITIAL, PORTFOLIO_INITIAL_MASK, true);
    EnableRandomPortfolio(PORTFOLIO_PRO, PORTFOLIO_TYPE_MASK, true);
    
    if (Portfolio.DustBuster)
    {
        Vacuum.Side = VACUUMSIDE_TOP; // Force intro
        Vacuum.Dir = VACUUMDIR_DOWN;
        Portfolio.VacuumSide = Vacuum.Side;
        Portfolio.VacuumDir = Vacuum.Dir;
    }
    else
    {
        Portfolio.VacuumDir = Portfolio.UpsideDown ? VACUUMDIR_DOWN : VACUUMDIR_UP;
    }

    Portfolio.VacuumDistance = Portfolio.InitialVacuumDistance;
    
    Portfolio.VacuumSpeed = Portfolio.InitialVacuumSpeed;
    ReportPortfolio("Vacuum speed %.1f", Portfolio.VacuumSpeed);

    Portfolio.Chapter = CurrentChapter;
    LoadCurrentChapter();
    
    Portfolio.Page = Random(0, Chapter.NPages - 1);
    SetCurrentPage(Portfolio.Page);
    
    Portfolio.ChaptersUntilNextStory = Portfolio.FirstStoryPageFrequency;
}

void SetupTutorialPortfolio()
{
    PortfolioLine = 0;
    
    Portfolio.Gears = true;
    Portfolio.Staplers = true;
    Portfolio.Balloons = true;
    Portfolio.Fireworks = true;
    Portfolio.Coins = true;
    
    Portfolio.VacuumDistance = 1000;
    Portfolio.VacuumSpeed = 0.5f;
    
    Portfolio.VacuumSpeedChangeFrequency = 1;
    Portfolio.ElementChangeFrequency = 1;
    Portfolio.ChapterChangeFrequency = 1;
    
    Portfolio.Chapter = 0;
    CurrentChapter = Portfolio.Chapter;
    LoadCurrentChapter();
    
    Portfolio.Page = 0;
    SetCurrentPage(Portfolio.Page);
}

static void SetupStoryPortfolio(int NewChapter)
{
    PortfolioLine = 0;
    
    Portfolio.Gears = true;
    Portfolio.Staplers = true;
    Portfolio.Balloons = true;
    Portfolio.Fireworks = true;
    Portfolio.Coins = true;
    
    Portfolio.VacuumDistance = 1000;
    Portfolio.VacuumSpeed = 0.5f;
    
    Portfolio.VacuumSpeedChangeFrequency = 1;
    Portfolio.ElementChangeFrequency = 1;
    Portfolio.ChapterChangeFrequency = 1;
    
    Portfolio.Chapter = NewChapter;
    CurrentChapter = Portfolio.Chapter;
    LoadCurrentChapter();
    
    Portfolio.Page = 0;
    SetCurrentPage(Portfolio.Page);
}

void AdvancePortfolio()
{
    bool WasDustbuster = Portfolio.DustBuster;
    
    PreparePortfolioForChanges();

    if (Portfolio.Chapter >= FIRST_STORY_CHAPTER && Portfolio.Chapter <= LAST_STORY_CHAPTER)
    {
        Portfolio = SavedPortfolio;
        Portfolio.ChaptersUntilNextStory = Portfolio.StoryPageFrequency;
    }

    PortfolioLine = 0;

    Portfolio.PageCount++;

    if (Portfolio.PageCount % Portfolio.VacuumSpeedChangeFrequency == 0)
    {
        Portfolio.VacuumSpeed += Portfolio.VacuumSpeedChange * (1.0f - Portfolio.VacuumDamage * Portfolio.VacuumDamageSpeedModifier);
        if (Portfolio.VacuumSpeed < 0.1f)
            Portfolio.VacuumSpeed = 0.1f;
    }
    ReportPortfolio("Vacuum speed %.1f", Portfolio.VacuumSpeed);
    
    if (Portfolio.PageCount % Portfolio.ElementChangeFrequency == 0)
    {
        // Coins always appear after one turn.
        if (!Portfolio.Coins)
            Portfolio.Coins = true;
        
        // Upside down and lights off can only persist for one round.  (This currently prevents them from being combined)
        if (Portfolio.UpsideDown)
            Portfolio.UpsideDown = false;
        
        if (Portfolio.LightsOff)
            Portfolio.LightsOff = false;
        
        // Randomly add one pro or con.
        if (Random(0.0f, 1.0f) >= 0.5f)
            EnableRandomPortfolio(PORTFOLIO_PRO, PORTFOLIO_TYPE_MASK, true);
        else
            EnableRandomPortfolio(PORTFOLIO_CON, PORTFOLIO_TYPE_MASK, true);
        
        EnableRandomPortfolio(0, PORTFOLIO_INITIAL_MASK, false);
        
        if (Portfolio.LightsOff)
            Portfolio.Fireworks = true;
    }

    if (Portfolio.PageCount % Portfolio.ChapterChangeFrequency == 0)
    {
        Portfolio.ChapterCount++;
        
        Portfolio.VacuumDistance = Portfolio.InitialVacuumDistance;
        Portfolio.VacuumSpeed = Portfolio.InitialVacuumSpeed + Portfolio.ChapterCount * Portfolio.VacuumSpeedChange * 3;

        Portfolio.VacuumDamage = 0;
        
#if 0
        Portfolio.ChaptersUntilNextStory--;
        if (Portfolio.ChaptersUntilNextStory == 0)
        {
            Portfolio.ChaptersUntilNextStory = Portfolio.StoryPageFrequency;

            SavedPortfolio = Portfolio;

            int NewChapter = Random(FIRST_STORY_CHAPTER, LAST_STORY_CHAPTER);
            
            int ChapterTestCount = 0;
            for (;;)
            {
                if (!Portfolio.StorySeen[NewChapter])
                {
                    Portfolio.Chapter = NewChapter;
                    Portfolio.StorySeen[NewChapter] = true;
                    SavedPortfolio = Portfolio;
                    SetupStoryPortfolio(NewChapter);
                    return;
                }
                ChapterTestCount++;
                if (ChapterTestCount > (LAST_STORY_CHAPTER - FIRST_STORY_CHAPTER + 1))
                {
                    Portfolio.Chapter++;
                    if (Portfolio.Chapter > LAST_NORMAL_CHAPTER)
                        Portfolio.Chapter = FIRST_NORMAL_CHAPTER;
                    break;
                }
                NewChapter++;
                if (NewChapter > LAST_STORY_CHAPTER)
                    NewChapter = FIRST_STORY_CHAPTER;
            }
        }
        else
#endif
        {
            Portfolio.Chapter++;
            if (Portfolio.Chapter > LAST_NORMAL_CHAPTER)
                Portfolio.Chapter = FIRST_NORMAL_CHAPTER;
        }
        
        CurrentChapter = Portfolio.Chapter;
        LoadCurrentChapter();
        
        Portfolio.NewChapter = true;
    }
    
    if (Portfolio.VacuumCatchup)
    {
        Portfolio.VacuumDistance = Portfolio.InitialVacuumDistance / 2;
        Portfolio.VacuumCatchup = false;
    }
    
    if (Portfolio.DustBuster)
    {
        if (!WasDustbuster)
        {
            Vacuum.Side = VACUUMSIDE_TOP; // Force intro
            Vacuum.Dir = VACUUMDIR_DOWN;
            Portfolio.VacuumSide = Vacuum.Side;
            Portfolio.VacuumDir = Vacuum.Dir;            
        }
        else
        {
            Vacuum.Side = (EVacuumSide)Portfolio.VacuumSide;
            Vacuum.Dir = (EVacuumDir)Vacuum.Dir;
            AdvanceDustBusterSideAndDir();
            Portfolio.VacuumSide = Vacuum.Side;
            Portfolio.VacuumDir = Vacuum.Dir;
        }
    }
    else
    {
        Portfolio.VacuumDir = Portfolio.UpsideDown ? VACUUMDIR_DOWN : VACUUMDIR_UP;
    }
    
TryAgain:
    int NewPage = Random(0, Chapter.NPages-1);
    
    if (Chapter.NPages > 1 && NewPage == Portfolio.Page)
        goto TryAgain;
    
    for (int HistIndex = 0; HistIndex < ARRAY_COUNT(Portfolio.PageHistory); HistIndex++)
        if (NewPage == Portfolio.PageHistory[HistIndex])
            goto TryAgain;
    
    for (int HistIndex = 1; HistIndex < ARRAY_COUNT(Portfolio.PageHistory); HistIndex++)
        Portfolio.PageHistory[HistIndex] = Portfolio.PageHistory[HistIndex-1];
    Portfolio.PageHistory[0] = Portfolio.Page;
    
    Portfolio.Page = NewPage;
    SetCurrentPage(Portfolio.Page);
}


void AwardBaby(int Hat)
{
    if (Portfolio.BabyCount >= MAX_SCORE_BABIES)
        ReportError("Exceeded maximum of %d babies carried over between pages.", MAX_SCORE_BABIES);
    
    Portfolio.BabyHats[Portfolio.BabyCount] = Hat;
    Portfolio.BabyCount++;
}

void RemoveBaby(int Hat)
{
    for (int i = 0; i < Portfolio.BabyCount; i++)
    {
        if (Portfolio.BabyHats[i] == Hat)
        {
            for (int j = i + 1; j < Portfolio.BabyCount; j++)
                Portfolio.BabyHats[j - 1] = Portfolio.BabyHats[j];
            Portfolio.BabyCount--;
            return;
        }
    }
}

