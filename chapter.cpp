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
#include "Flashlight.h"
#include "Vacuum.h"
#include "Fan.h"
#include "Wipe.h"
#include "Debris.h"
#include "Lives.h"
#include "Recorder.h"

#include <direct.h>

SChapter Chapter;

int ScrollY;

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

void GetNextLine(FILE* File, char* Line, int LineSize)
{
	*Line = '\0';
	
	while (!feof(File))
	{
		fgets(Line, LineSize-1, File);

		// Remove comments up to the end of the line.
		char* Comment = Line;
		while (*Comment)
		{
			if (Comment[0] == '/' && Comment[1] == '/')
			{
				*Comment = '\0';
				break;
			}
			Comment++;
		}
		
		// Remove whitespace from the end of the line.
		char* End = Line + strlen(Line)-1;
		while (End > Line && isspace(*End))
		{
			End--;
		}
		End[1] = '\0';
		
		// Remove whitespace from the beginning of the line.
		char* Begin = Line;
		while (isspace(*Begin))
		{
			Begin++;
		}
		memmove(Line, Begin, strlen(Begin)+1);
		
		// If the resulting line is not empty, return it.
		// Otherwise, loop around and get another one.
		if (*Line)
			return;
	}
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
	char ImageFileName[1024];
	snprintf(ImageFileName, sizeof(ImageFileName), "%s/%s", CurrentDirectory, ImageSourceAttr->value());

	gxLoadSprite(ImageFileName, &TileSet->Sprite);

	if (TileSet->Sprite.width == 0 || TileSet->Sprite.height == 0)
		ReportError("Invalid tileset image '%s'.  Fix this problem and re-save the TSX or TMX file.", FileName);

	if (TileSet->Sprite.width % 64 || TileSet->Sprite.height % 64)
		ReportError("Tileset image '%s' width and height must be multiples of 64.  Fix this problem and re-save the TSX or TMX file.", FileName);

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

#ifdef PLATFORM_IPHONE
	FILE* BlocksFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* BlocksFile = fopen(FileName, "r");
#endif

	if (!BlocksFile)
		ReportError("Unable to open TSX file.  Check that all required files are present, and re-save the TMX file to fix.");

	// Read the entire XML file into a text buffer.
	fseek(BlocksFile, 0, SEEK_END);
	int FileSize = ftell(BlocksFile);
	rewind(BlocksFile);

	char* XML = (char*)malloc(FileSize + 1);
	fread(XML, FileSize, 1, BlocksFile);
	fclose(BlocksFile);
	XML[FileSize] = '\0';

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
#ifdef PLATFORM_IPHONE
	FILE* PageFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* PageFile = fopen(FileName, "r");
#endif

	if (!PageFile)
		ReportError("Unable to open TMX file.  Check that all required files are present, and re-save the TMX file to fix.");

	// Read the entire XML file into a text buffer.
	fseek(PageFile, 0, SEEK_END);
	int FileSize = ftell(PageFile);
	rewind(PageFile);

	char* XML = (char*)malloc(FileSize + 1);
	fread(XML, FileSize, 1, PageFile);
	fclose(PageFile);
	XML[FileSize] = '\0';

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
	
	if (atoi(MapNode->first_attribute("width")->value()) != 12)
		ReportError("Map width must be 12.  Fix this problem and re-save the TMX file.");
	
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
			// If the external tileset has been loaded already, just reference it.
			for (int i = 0; i < Chapter.NTileSets; i++)
			{
				if (strcmp(Chapter.TileSets[i].Name, TileSetSourceAttr->value()) == 0)
				{
					TileSetIndex = i;
					break;
				}
			}

			if (TileSetIndex == -1)
			{
				// It hasn't been loaded previously, so load the external tileset.
				char TileSetName[1024];
				snprintf(TileSetName, sizeof(TileSetName), "%s/%s", CurrentChapterDir, TileSetSourceAttr->value());

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
	
	if (Page->Width != 12)
		ReportError("Layer width must be 12.  Fix this problem and re-save the TMX file.");

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
	PushErrorContext("While loading chapter '%s':\n", ChapterDir);

	if (Chapter.NBlocks > 0 || Chapter.NPages > 0 || Chapter.NTileSets > 0)
		ReportError("Attempted to load a chapter without clearing it first.");

	Chapter.EndX = 0;
	Chapter.EndY = 0;

	Chapter.NBlocks = 0;
	Chapter.NPages = 0;
	Chapter.NTileSets = 0;

	Chapter.Name = strdup(ChapterDir);

	CurrentChapterDir = ChapterDir;

	char FileName[1024];
	snprintf(FileName, sizeof(FileName), "%s/Chapter.txt", ChapterDir);

#ifdef PLATFORM_IPHONE
	FILE* ChapFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* ChapFile = fopen(FileName, "r");
#endif

	if (!ChapFile)
		ReportError("Chapter.txt is missing.");

	while (!feof(ChapFile))
	{
		char Line[1024];
		
		GetNextLine(ChapFile, Line, sizeof(Line)-1);

		if (strstr(Line, "PAGES") != NULL)
		{
			while (!feof(ChapFile))
			{
				GetNextLine(ChapFile, Line, sizeof(Line));

				if (strstr(Line, "END OF PAGES") != NULL)
					break;

				char FileName[1024];
				snprintf(FileName, sizeof(FileName), "%s/%s.tmx", CurrentChapterDir, Line);
				
				LoadPageFromTMX(FileName);
			}

			continue;
		}		
	}

	fclose(ChapFile);

	if (!Chapter.NPages)
		ReportError("Chapter contains no pages.");

	if (!Chapter.NBlocks)
		ReportError("Chapter contains no blocks.");

	SetCurrentPage(0);

	PopErrorContext();
}

void ClearChapter()
{
	free((void*)Chapter.Name);
	Chapter.Name = NULL;

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
	ClearFlashlightWaypoints();

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
				case BLOCKTYPE_FLASHLIGHT_WAYPOINT:
					CreateFlashlightWaypoint(x * 64, y * 64, (SFlashlightWaypointProperties*)Block->Properties);
					EraseBlock(x, y);
					break;
				}
			}
		}
	}

	// Set initial ScrollY.
	ScrollY = -(Chapter.PageHeight * 64 - gxScreenHeight);

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

	CreatePageObjects();

	PopErrorContext();
}

void CalculateScrollY()
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
	if (Dusty.FloatY + ScrollY > gxScreenHeight - 200)
	{
		ScrollY = (gxScreenHeight - 200) - (int)Dusty.FloatY;
	}

	// Prevent scrolling off bottom of map.
	if (ScrollY < -(Chapter.PageHeight * 64 - gxScreenHeight))
	{
		ScrollY = -(Chapter.PageHeight * 64 - gxScreenHeight);
	} 

	// Prevent scrolling off top of map.
	if (ScrollY > 0)
	{
		ScrollY = 0;
	}
}

void DisplayChapter()
{
	for (int y = 0; y < Chapter.PageHeight; y++)
	{
		// Skip rows of tiles that cannot be on screen.
		if (y*64 + ScrollY > gxScreenHeight || (y+1)*64 + ScrollY < 0)
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

				AddLitSubSprite(LIGHTLIST_FOREGROUND, &TileSet->Sprite, (float)x*64, (float)y*64 + ScrollY, (float)SubX, (float)SubY, (float)SubX + 64, (float)SubY + 64);
			}
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