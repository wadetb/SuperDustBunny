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
		else if (strcmp(Name, "shadow_offset") == 0)
		{
            Props->ShadowOffsetX = strtof(Value, (char**)&Value);
            Props->ShadowOffsetY = strtof(Value, (char**)&Value);
		}
		else if (strcmp(Name, "shadow_scale") == 0)
		{
            Props->ShadowScaleX = strtof(Value, (char**)&Value);
            Props->ShadowScaleY = strtof(Value, (char**)&Value);
		}
		else if (strcmp(Name, "light_origin") == 0)
		{
            Props->LightOriginX = strtof(Value, (char**)&Value);
            Props->LightOriginY = strtof(Value, (char**)&Value);
		}
		else
			ReportError("'%s' is not a valid map property (value is '%s').  Fix this problem and re-save the TMX file.", Name, Value);

		PropertyNode = PropertyNode->next_sibling("property");
	}
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
					if (strcmp(Value, "start") == 0)
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
	Document.parse<0>(XML);

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
	Document.parse<0>(XML);

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
	Document.parse<0>(XML);
    
  	// Get the <Chapter> node and valiate.
	rapidxml::xml_node<char>* ChapterNode = Document.first_node("Chapter");
	if (ChapterNode == NULL)
		ReportError("Missing <Chapter> node.  Check for errors in the XML.");
    
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

	SetCurrentPage(0);

	PopErrorContext();
    
    double EndTime = GetCurrentTime();
    LogMessage("Loading chapter '%s' took %.1f seconds.\n", ChapterDir, EndTime-StartTime);
}

void ClearChapter()
{
	free((void*)Chapter.Name);
	Chapter.Name = NULL;

    if (Chapter.HasBackground)
        gxDestroySprite(&Chapter.BackgroundSprite);
    
	for (int i = 0; i < Chapter.NPages; i++)
	{
		free(Chapter.Pages[i].Name);
		free(Chapter.Pages[i].Blocks);

        for (int j = 0; j < Chapter.Pages[i].NLayers; j++)
            free(Chapter.Pages[i].Layers[j].Blocks);
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
            if (Chapter.Blocks[i].Type == BLOCKTYPE_GEAR)
                FreeGearProperties(&Chapter.Blocks[i]);
            else
                free(Chapter.Blocks[i].Properties);
        }
	}
	Chapter.NBlocks = 0;

	if (Chapter.PageBlocks)
	{
		free(Chapter.PageBlocks);
		Chapter.PageBlocks = NULL;
	}
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
					Chapter.StartX = (float)x * 64 + 32;
					Chapter.StartY = (float)y * 64 + 64;
                    Chapter.StartDirection = (Flags & SPECIALBLOCKID_FLIP_X) ? DIRECTION_LEFT : DIRECTION_RIGHT;
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_CHAPTEREND:
					Chapter.EndX = (float)x * 64 + 32;
					Chapter.EndY = (float)y * 64 + 32;
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_BARREL:
					CreateBarrel(x * 64, y * 64, (SBarrelProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_FIREWORK:
					CreateFireWork(x * 64, y * 64, (SFireWorkProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_BALL:
					CreateBall(x * 64, y * 64);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_GEAR:
					CreateGear(x * 64, y * 64, (SGearProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_COIN:
					CreateCoin(x * 64, y * 64);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_FAN:
					CreateFan(x * 64, y * 64, (SFanProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
                case BLOCKTYPE_FLAME:
                    CreateFlame(x * 64, y * 64, Flags, (SFlameProperties*)Block->Properties);
                    break;
				case BLOCKTYPE_FLASHLIGHT_WAYPOINT:
					CreateFlashlightWaypoint(x * 64, y * 64, (SFlashlightWaypointProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
                case BLOCKTYPE_STAPLER:
                    CreateStapler(x * 64, y * 64, STAPLER_STAPLER);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_JELLO:
                    CreateStapler(x * 64, y * 64, STAPLER_JELLO);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_CLOTHESPIN:
                    CreateStapler(x * 64, y * 64, STAPLER_CLOTHESPIN);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_POWERUP:
                    CreatePowerUp(x * 64, y * 64);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_TUTORIAL:
                    CreateTutorial(x * 64, y * 64, (STutorialProperties*)Block->Properties);
                    EraseBlock(x, y);
                    break;
                case BLOCKTYPE_VACUUM_TRIGGER:
                    CreateVacuumTrigger(x * 64, y * 64, (SVacuumTriggerProperties*)Block->Properties);
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
    
    if (Score.DeathCount >= 2)
        CreatePowerUp(Dusty.FloatX + 192, Dusty.FloatY - 320);

    if (!Chapter.PageProps.VacuumOff)
        TurnOnVacuum(500, 2.0f, false);
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
	memcpy(Chapter.PageBlocks, Chapter.Pages[PageNum].Blocks, Chapter.PageWidth * Chapter.PageHeight * sizeof(unsigned int));

	Chapter.PageProps = Chapter.Pages[PageNum].Props;

	Chapter.PortalAngle = 0;

	CreatePageObjects();

    StartRecording();
    InitMinimap();

    StartGhostRecording();
    if (Settings.GhostActive)
    {
        LoadGhost(Chapters[CurrentChapter].Name, PageNum, false);
        StartGhostPlayback();
    }
    else
    {
        if (Chapter.PageProps.GhostRace)
        {
            LoadGhost(Chapters[CurrentChapter].Name, PageNum, true);
            StartGhostPlayback();        
        }        
    }

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
        ScrollY = Remap(SmoothStep(TutorialOverrides.Timer), 0.0f, 1.0f, TutorialOverrides.SavedScrollY, (LitScreenHeight*0.75f-Vacuum.Y), true);
    }
    else
    {        
        // Screen tracks Dusty upwards.
        if (Dusty.FloatY + ScrollY < 600)
        {
            ScrollY = 600 - Dusty.FloatY;
        }

        // Screen also tracks Dusty downwards.
        if (Dusty.FloatY + ScrollY > LitScreenHeight - 200)
        {
            ScrollY = (LitScreenHeight - 200) - (int)Dusty.FloatY;
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
        
        float XRatio = Dusty.FloatX/(Chapter.PageWidth*64);
        //    XRatio = powf(XRatio*2.0f-1.0f, 1.0f) *0.5f+0.5f;
        
        ScrollX = -XRatio * (Chapter.PageWidth*64 - 768);
        
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
        for (int i = 5; i > 0; i--)
        {
            float Alpha = (float)i / 5.0f;
            float Angle = Chapter.PortalAngle - (1.0f-Alpha);
            AddLitSpriteCenteredScaledRotatedAlpha(LIGHTLIST_FOREGROUND_NO_SHADOW, &PortalSprite, Chapter.EndX + ScrollX, Chapter.EndY - 50 + ScrollY, 1.0f, Angle, Alpha);
        }
    }
}

static void DisplayChapterLayer(ELightList LightList, int* Blocks)
{
    float CurY = ScrollY;
	for (int y = 0; y < Chapter.PageHeight; y++, CurY += 64)
	{
		// Skip rows of tiles that cannot be on screen.
		if (CurY > LitScreenHeight || CurY+64 < 0)
			continue;

        float CurX = ScrollX;
		for (int x = 0; x < Chapter.PageWidth; x++, CurX += 64)
		{
		    //if (CurX > gxScreenWidth || CurX+64 < 0)
			   // continue;

            unsigned int ID = Blocks[y * Chapter.PageWidth + x];
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
                
                AddLitQuad(LightList, &TileSet->Sprite, gxRGBA32(255,255,255,255),
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
    DisplayChapterLayer(LIGHTLIST_FOREGROUND, Chapter.PageBlocks);
}

void DisplayChapterExtraLayers()
{
    SPage* Page = &Chapter.Pages[Chapter.PageNum];

    for (int i = 0; i < Page->NLayers; i++)
        DisplayChapterLayer(LIGHTLIST_FOREGROUND_NO_SHADOW, Page->Layers[i].Blocks);
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

bool IsBlockEmpty(int x, int y)
{
	// Requests for blocks outside the map return solid.
	if (x < 0 || x >= Chapter.PageWidth)
		return false;
	if (y < 0 || y >= Chapter.PageHeight)
		return false;

	if (Chapter.PageBlocks[y * Chapter.PageWidth + x] == SPECIALBLOCKID_BLANK)
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
        
        if (Chapters[i].HasIcon)
            gxDestroySprite(&Chapters[i].IconSprite);
        
        if (Chapters[i].UnlockedBy)
            free(Chapters[i].UnlockedBy);
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
	Document.parse<0>(XML);
    
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
        
        rapidxml::xml_attribute<char>* IconAttr = ChapterNode->first_attribute("Icon");
        if (IconAttr)
        {
            LoadSpriteAsset(IconAttr->value(), &Chapter->IconSprite);
            Chapter->HasIcon = true;
        }
        else
            Chapter->HasIcon = false;
        
        rapidxml::xml_attribute<char>* BronzeTimeAttr = ChapterNode->first_attribute("BronzeTime");
        if (BronzeTimeAttr)
            Chapter->BronzeTime = (int)(atof(BronzeTimeAttr->value())*60.0f);
        else
            Chapter->BronzeTime = 0;
        
        rapidxml::xml_attribute<char>* SilverTimeAttr = ChapterNode->first_attribute("SilverTime");
        if (SilverTimeAttr)
            Chapter->SilverTime = (int)(atof(SilverTimeAttr->value())*60.0f);
        else
            Chapter->SilverTime = 0;
        
        rapidxml::xml_attribute<char>* GoldTimeAttr = ChapterNode->first_attribute("GoldTime");
        if (GoldTimeAttr)
            Chapter->GoldTime = (int)(atof(GoldTimeAttr->value())*60.0f);
        else
            Chapter->GoldTime = 0;

        rapidxml::xml_attribute<char>* UnlockedByAttr = ChapterNode->first_attribute("UnlockedBy");
        if (UnlockedByAttr)
            Chapter->UnlockedBy = strdup(UnlockedByAttr->value());
        else
            Chapter->UnlockedBy = NULL;
        
        rapidxml::xml_attribute<char>* EndOfGameAttr = ChapterNode->first_attribute("EndOfGame");
        if (EndOfGameAttr)
            Chapter->EndOfGame = atoi(EndOfGameAttr->value()) != 0;
        else
            Chapter->EndOfGame = false;

        // Chapters are locked by default if another chapter unlocks them.
        if (Chapter->UnlockedBy == NULL)
            Chapter->Unlocked = true;
        else
            Chapter->Unlocked = false;
        
        Chapter->Completed = false;
        Chapter->BestTime = INT_MAX;
        
        NChapters++;
        
		ChapterNode = ChapterNode->next_sibling("Chapter");
	}
    
	PopErrorContext();
}
                           
void LoadChapterUnlocks()
{
    PushErrorContext("While loading chapter unlocks:");
    
#ifdef PLATFORM_IPHONE_OR_MAC
    @try
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:@"chapter.unlocks"];
        
        NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:filePath];
        if ( !dict )
        {
            PopErrorContext();
            return;
        }
        
        NSNumber *version = [dict objectForKey:@"version"];
        if ( [version intValue] != 1 )
        {
            PopErrorContext();
            return;
        }
        
        NSArray *chapters = [dict objectForKey:@"chapters"];
        
        for (int i = 0; i < [chapters count]; i++)
        {
            NSDictionary *savedChapter = [chapters objectAtIndex:i];
            
            NSString *savedName = [savedChapter objectForKey:@"name"];
            
            SChapterListEntry* ChapterList = NULL;
            for (int j = 0; j < NChapters; j++)
            {
                if ([savedName isEqualToString:[NSString stringWithUTF8String:Chapters[j].Name]])
                {
                    ChapterList = &Chapters[j];
                    break;
                }
            }
            
            if (ChapterList)
            {
                ChapterList->Unlocked = [[savedChapter objectForKey:@"unlocked"] boolValue];
                ChapterList->Played = [[savedChapter objectForKey:@"played"] boolValue];
                ChapterList->Completed = [[savedChapter objectForKey:@"completed"] boolValue];
                ChapterList->BestTime = [[savedChapter objectForKey:@"bestTime"] intValue];
            }
        }
    }
    @catch (NSException *e)
    {
        NSLog(@"Caught exception while loading chapter unlocks: %@\n", e);
    }
#endif
    
    PopErrorContext();

}

void SaveChapterUnlocks()
{
    PushErrorContext("While saving chapter unlocks:\n");
    
#ifdef PLATFORM_IPHONE_OR_MAC
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent:@"chapter.unlocks"];
    
    NSMutableArray *chapters = [[NSMutableArray alloc] init];
    for (int i = 0; i < NChapters; i++)
    {
        SChapterListEntry* ChapterList = &Chapters[i];
        
        NSDictionary *chapter = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSString stringWithUTF8String:ChapterList->Name], @"name",
                                 [NSNumber numberWithBool:ChapterList->Unlocked], @"unlocked",
                                 [NSNumber numberWithBool:ChapterList->Played], @"played",
                                 [NSNumber numberWithBool:ChapterList->Completed], @"completed",
                                 [NSNumber numberWithInt:ChapterList->BestTime], @"bestTime", nil];
        [chapters addObject:chapter];
    }
    
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInt:1], @"version",
                          chapters, @"chapters", nil];
    
    [dict writeToFile:filePath atomically:YES];
    
    [chapters release];
#endif
    
    PopErrorContext();

}
