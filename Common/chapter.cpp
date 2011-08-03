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
#include "Crumb.h"
#include "Gear.h"
#include "Dust.h"
#include "Flame.h"
#include "Flashlight.h"
#include "Vacuum.h"
#include "Fan.h"
#include "Wipe.h"
#include "Debris.h"
#include "Lives.h"
#include "Recorder.h"
#include "Stapler.h"
#include "PowerUp.h"

#ifdef PLATFORM_WINDOWS
#include <direct.h>
#endif

int NChapters;
SChapterListEntry Chapters[MAX_CHAPTERS];

int CurrentChapter = -1;

SChapter Chapter;

int ScrollY;
int ScrollX;


const char* CurrentChapterDir;


void ParseNailProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
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

void InitPageProperties(SPageProperties* Props)
{
	Props->LightsOff = false;
	Props->VacuumOff = false;
	Props->VacuumDir = VACUUMDIR_UP;
}

void ParsePageProperties(SPageProperties* Props, rapidxml::xml_node<char>* PropertiesNode)
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
		else if (strcmp(Name, "vacuum_dir") == 0)
		{
			if (strcmp(Value, "up") == 0)
				Props->VacuumDir = VACUUMDIR_UP;
			else if (strcmp(Value, "down") == 0)
				Props->VacuumDir = VACUUMDIR_DOWN;
			else
				ReportError("'%s' is not a valid value for the 'vacuum_dir' property.  The value must be 'up' or 'down'.  Fix this problem and re-save the TMX file.", Value);
		}
		else
			ReportError("'%s' is not a valid map property (value is '%s').  Fix this problem and re-save the TMX file.", Name, Value);

		PropertyNode = PropertyNode->next_sibling("property");
	}
}

// Loads a <tileset> node from either an internal or external tileset.
// FileName is used to locate image files.
void LoadTileSetNode(rapidxml::xml_node<char>* TileSetNode, const char* FileName)
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
                    else if (strcmp(Value, "powerup") == 0)
                    {
                        Block->Type = BLOCKTYPE_POWERUP;
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
			else if (Block->Type == BLOCKTYPE_FLASHLIGHT_WAYPOINT)
			{
				ParseFlashlightWaypointProperties(Block, PropertiesNode);
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

void LoadTileSet(const char* FileName)
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

void LoadPageFromTMX(const char* FileName)
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
			ReportError("Too many tilesets. The maximum number of tilesets per page is 10.  Re-saving the TMX file may help.");

		int TileSetIndex = -1;

		rapidxml::xml_attribute<char>* TileSetSourceAttr = TileSetNode->first_attribute("source");
		if (TileSetSourceAttr == NULL)
		{
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
	
	if (LayerNode->next_sibling("layer") != NULL)
		ReportError("Cannot have more than one layer.  Fix this problem and re-save the TMX file.");
	
	// Get the <data> node and validate.
	rapidxml::xml_node<char>* DataNode = LayerNode->first_node("data");
	
	if (DataNode == NULL)
		ReportError("Missing <data> node.");

	rapidxml::xml_attribute<char>* DataEncodingAttr = DataNode->first_attribute("encoding");
	if (DataEncodingAttr == NULL || strcmp(DataEncodingAttr->value(), "csv") != 0)
		ReportError("Wrong map encoding.  Set encoding to CSV in the Tiled preferences and re-save the TMX file.");

	// Build the SPage structure.
	if (Chapter.NPages >= MAX_PAGES)
		ReportError("Exceeded the maximum of %d total pages.", MAX_PAGES);

	SPage* Page = &Chapter.Pages[Chapter.NPages++];
	
	// Determine page name from the file name (no extension, no path).
	char Name[1024];
	snprintf(Name, sizeof(Name), "%s", FileName);
	
	char* Dot = strchr(Name, '.');
	if (Dot) *Dot = '\0';
	
	char* Slash = strrchr(Name, '/');
	if (!Slash) Slash = strrchr(Name, '\\');
	if (!Slash) Slash = Name;
	
	Page->Name = strdup(Slash);
	
	Page->Width = atoi(LayerNode->first_attribute("width")->value());
	Page->Height = atoi(LayerNode->first_attribute("height")->value());
	
	if (Page->Width < 12)
		ReportError("Layer width must be at least 12.  Fix this problem and re-save the TMX file.");

	if (Page->Height < 1)
		ReportError("Invalid layer height.  Fix this problem and re-save the TMX file.");

	// Allocate the Blocks buffer.
	Page->Blocks = (int*)malloc(Page->Width * Page->Height * sizeof(int));
	
	// Parse the CSV data into the Blocks buffer.
	char* Data = DataNode->value();
	char* DataEnd = Data + DataNode->value_size();
	
	for (int y = 0; y < Page->Height; y++)
	{
		for (int x = 0; x < Page->Width; x++)
		{
			if (Data >= DataEnd)
				ReportError("Unexpected end of tile data.  Re-saving the TMX file may help.");
			
			int ID = strtol(Data, &Data, 0);
			if (ID < 0)
				ReportError("Invalid tile data.  Re-saving the TMX file may help.");
			
			if (x < Page->Width-1 || y < Page->Height-1)
			{
				if (*Data != ',')
					ReportError("Tile data format is invalid.  Re-saving the TMX file may help.");
				Data++;
			}

			if (ID == 0)
				ID = SPECIALBLOCKID_BLANK;
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
			}

			Page->Blocks[y*Page->Width + x] = ID;
		}
		
		if (*Data != '\n')
			ReportError("Tile data format is invalid (but I'm being picky).  Re-saving the TMX file may help.");
		Data++;
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
			free(Chapter.Blocks[i].Properties);
	}
	Chapter.NBlocks = 0;

	if (Chapter.PageBlocks)
	{
		free(Chapter.PageBlocks);
		Chapter.PageBlocks = NULL;
	}
}

void ClearPageObjects()
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

	if (Chapter.PageBlocks)
	{
		free(Chapter.PageBlocks);
		Chapter.PageBlocks = NULL;
	}
}

void CreatePageObjects()
{
	// Process any special blocks that need to create dynamic objects.
	for (int y = 0; y < Chapter.PageHeight; y++)
	{
		for (int x = 0; x < Chapter.PageWidth; x++)
		{
			int BlockID = Chapter.PageBlocks[y * Chapter.PageWidth + x];

			if (BlockID < SPECIALBLOCKID_FIRST)
			{
				if (BlockID < 0 || BlockID > Chapter.NBlocks)
					ReportError("Invalid block ID encountered when creating page objects.");
				
				SBlock* Block = &Chapter.Blocks[BlockID];

				switch (Block->Type)
				{
				case BLOCKTYPE_CHAPTERSTART:
					Chapter.StartX = (float)x * 64 + 32;
					Chapter.StartY = (float)y * 64 + 64;
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
					CreateGear(x * 64, y * 64);
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
                    CreateFlame(x * 64, y * 64);
                    EraseBlock(x, y);
                    break;
				case BLOCKTYPE_FLASHLIGHT_WAYPOINT:
					CreateFlashlightWaypoint(x * 64, y * 64, (SFlashlightWaypointProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				case BLOCKTYPE_STAPLER:
					CreateStapler(x * 64, y * 64);
					EraseBlock(x, y);
					break;
                case BLOCKTYPE_POWERUP:
                    CreatePowerUp(x * 64, y * 64);
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
	ScrollY = -(Chapter.PageHeight * 64 - LitScreenHeight);

	// Initialize global stuff for the page.
	InitDusty();
	SetDustyPosition(Chapter.StartX, Chapter.StartY);

	InitDust();
	InitDebris();
	InitFlashlight();

	InitVacuum();
	TurnOnVacuum();
}

void SetCurrentPage(int PageNum)
{
	if (PageNum < 0 || PageNum >= Chapter.NPages)
		ReportError("%d is not a valid page number.", PageNum);

	// Clear out objects from previous page.
	ClearPageObjects();

	PushErrorContext("While setting the current page to '%s':\n", Chapter.Pages[PageNum].Name);

	Chapter.PageNum = PageNum;
	Chapter.PageWidth = Chapter.Pages[PageNum].Width;
	Chapter.PageHeight = Chapter.Pages[PageNum].Height;

	Chapter.PageBlocks = (int*)malloc(Chapter.PageWidth * Chapter.PageHeight * sizeof(unsigned int));
	memcpy(Chapter.PageBlocks, Chapter.Pages[PageNum].Blocks, Chapter.PageWidth * Chapter.PageHeight * sizeof(unsigned int));

	Chapter.PageProps = Chapter.Pages[PageNum].Props;

	Chapter.PortalAngle = 0;

	CreatePageObjects();

	PopErrorContext();
}

void CalculateScroll()
{
	// If Dusty is dead, freeze scrolling while he falls off the screen.
	if (Dusty.State == DUSTYSTATE_DIE)
		return;

	// Screen tracks Dusty upwards.
	if (Dusty.FloatY + ScrollY < 500)
	{
		ScrollY = 500 - (int)Dusty.FloatY;
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

	if (Dusty.FloatX + ScrollX < 400)
	{
		ScrollX = 400 - (int)Dusty.FloatX;
	}
    
	if (Dusty.FloatX + ScrollX > 768 - 400)
	{
		ScrollX = (768 - 400) - (int)Dusty.FloatX;
	}
    
	if (ScrollX < -(Chapter.PageWidth * 64 - 768))
	{
		ScrollX = -(Chapter.PageWidth * 64 - 768);
	} 
    
	if (ScrollX > 0)
	{
		ScrollX = 0;
	}
}

void DisplayChapter()
{
	for (int y = 0; y < Chapter.PageHeight; y++)
	{
		// Skip rows of tiles that cannot be on screen.
		if (y*64 + ScrollY > LitScreenHeight || (y+1)*64 + ScrollY < 0)
			continue;

		for (int x = 0; x < Chapter.PageWidth; x++)
		{
			int BlockID = Chapter.PageBlocks[y * Chapter.PageWidth + x];

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

				int SubX = Block->SubX;
				int SubY = Block->SubY;

				AddLitSubSprite(LIGHTLIST_FOREGROUND, &TileSet->Sprite, (float)x*64 + ScrollX, (float)y*64 + ScrollY, (float)SubX, (float)SubY, (float)SubX + 64, (float)SubY + 64);
			}
		}
	}
    
    if (Chapter.EndX + 256 + ScrollY >= 0 && 
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

int GetBlockID(int x, int y)
{
	// Requests for blocks outside the map return a special value.
	if (x < 0 || x >= Chapter.PageWidth)
		return SPECIALBLOCKID_OUTOFBOUNDS;
	if (y < 0 || y >= Chapter.PageHeight)
		return SPECIALBLOCKID_OUTOFBOUNDS;

	return Chapter.PageBlocks[y * Chapter.PageWidth + x];
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
            Chapter->BronzeTime = atoi(BronzeTimeAttr->value());
        else
            Chapter->BronzeTime = INT_MAX;
        
        rapidxml::xml_attribute<char>* SilverTimeAttr = ChapterNode->first_attribute("SilverTime");
        if (SilverTimeAttr)
            Chapter->SilverTime = atoi(SilverTimeAttr->value());
        else
            Chapter->SilverTime = INT_MAX;
        
        rapidxml::xml_attribute<char>* GoldTimeAttr = ChapterNode->first_attribute("GoldTime");
        if (GoldTimeAttr)
            Chapter->GoldTime = atoi(GoldTimeAttr->value());
        else
            Chapter->GoldTime = INT_MAX;

        rapidxml::xml_attribute<char>* UnlockedByAttr = ChapterNode->first_attribute("UnlockedBy");
        if (UnlockedByAttr)
            Chapter->UnlockedBy = strdup(UnlockedByAttr->value());
        else
            Chapter->UnlockedBy = NULL;
        
        rapidxml::xml_attribute<char>* EndOfGameAttr = ChapterNode->first_attribute("EndOfGame");
        if (EndOfGameAttr)
            Chapter->EndOfGame = atoi(EndOfGameAttr->value());
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
    
#ifdef PLATFORM_IPHONE
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
    
#ifdef PLATFORM_IPHONE
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
#endif
    
    PopErrorContext();

}
