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

#include <direct.h>

SChapter Chapter;
SScore Score;

int ScrollY;

const char* CurrentChapterDir;

void InitScore()
{
    Score.RaiseScore = false;
    Score.ScoreCounter = 0;
    Score.X = 600;
    Score.Y = 0;
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

void LoadTileSetNode(rapidxml::xml_node<char>* TileSetNode)
{
	// Grr, Tiled doesn't currently put a TSX version number in the TSX file.
	//if (strcmp(TileSetNode->first_attribute("version")->value(), "1.0") != 0)
	//	ReportError("Unsupported TSX file version.  Re-save the TSX file with the right version of Tiled.");

	if (atoi(TileSetNode->first_attribute("tilewidth")->value()) != 64)
		ReportError("Tile width must be 64.  Fix this problem and re-save the TSX file.");

	if (atoi(TileSetNode->first_attribute("tileheight")->value()) != 64)
		ReportError("Tile height must be 64.  Fix this problem and re-save the TSX file.");

	rapidxml::xml_node<char>* ImageNode = TileSetNode->first_node("image");
	if (ImageNode == NULL)
		ReportError("Missing <image> node.  Re-saving the TSX file may help.");

	if (ImageNode->next_sibling("image"))
		ReportError("Only be one image per tileset is supported.  Fix this problem and re-save the TSX file.");

	rapidxml::xml_attribute<char>* ImageSourceAttr = ImageNode->first_attribute("source");
	if (ImageSourceAttr == NULL)
		ReportError("Missing image source attribute.  Re-saving the TSX file may help.");

	// Set up the tileset structure.
	STileSet* TileSet = &Chapter.TileSets[Chapter.NTileSets];

	TileSet->FirstBlock = Chapter.NBlocks;

	// Load the tileset image sprite.
	char FileName[1024];
	snprintf(FileName, sizeof(FileName), "%s/%s", CurrentChapterDir, ImageSourceAttr->value());

	gxLoadSprite(FileName, &TileSet->Sprite);

	if (TileSet->Sprite.width == 0 || TileSet->Sprite.height == 0)
		ReportError("Invalid tileset image.  Fix this problem and re-save the TSX file.");

	if (TileSet->Sprite.width % 64 || TileSet->Sprite.height % 64)
		ReportError("Tileset image width and height must be multiples of 64.  Fix this problem and re-save the TSX file.");

	// Slice the image up into blocks.
	int BlocksX = TileSet->Sprite.width/64;
	int BlocksY = TileSet->Sprite.height/64;

	for (int y = 0; y < BlocksY; y++)
	{
		for (int x = 0; x < BlocksX; x++)
		{
			SBlock* Block = &Chapter.Blocks[Chapter.NBlocks];

			Block->ID = TileSet->FirstBlock + y * BlocksY + x;
			Block->TileSet = Chapter.NTileSets;

			Block->SubX = x*64;
			Block->SubY = y*64;

			Chapter.NBlocks++;
		}
	}

	Chapter.NTileSets++;
}

void LoadTileSet(const char* FileName)
{
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

	LoadTileSetNode(TileSetNode);
}

bool LoadPageFromTMX(const char* FileName)
{
	// Open the TMX file.
#ifdef PLATFORM_IPHONE
	FILE* PageFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* PageFile = fopen(FileName, "r");
#endif

	if (!PageFile)
		return false;

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
	
	// Scan the tilesets.  Load each one into a local structure.
	rapidxml::xml_node<char>* TileSetNode = MapNode->first_node("tileset");
	if (TileSetNode == NULL)
		ReportError("Missing <tileset> node.  Re-saving the TMX file may help.");

	STileSetInfo TileSetInfo[MAX_PAGE_TILESETS];
	int NTileSetInfos = 0;

	while (TileSetNode)
	{
		if (NTileSetInfos >= MAX_PAGE_TILESETS)
			ReportError("Too many tilesets; the maximum per page is 10.  Re-saving the TMX file may help.");

		int TileSetIndex = -1;

		rapidxml::xml_attribute<char>* TileSetSourceAttr = TileSetNode->first_attribute("source");
		if (TileSetSourceAttr == NULL)
		{
			// Load the internal tileset.
			LoadTileSetNode(TileSetNode);
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
				// Not loaded yet, load the external tileset.
				char FileName[1024];
				snprintf(FileName, sizeof(FileName), "%s/%s", CurrentChapterDir, TileSetSourceAttr->value());

				LoadTileSet(FileName);
				TileSetIndex = Chapter.NTileSets - 1;
			}
		}

		TileSetInfo[NTileSetInfos].TileSet = &Chapter.TileSets[TileSetIndex];
		TileSetInfo[NTileSetInfos].FirstGID = atoi(TileSetNode->first_attribute("firstgid")->value());
		NTileSetInfos++;

		TileSetNode = MapNode->next_sibling("tileset");
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
	// TODO: Check MAX_PAGES
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
	
	return true;
}

void LoadChapter(const char* ChapterDir)
{
	Chapter.NBlocks = 0;
	Chapter.NPages = 0;
	Chapter.NStitchedPages = 0;

	CurrentChapterDir = ChapterDir;

	char FileName[1024];
	snprintf(FileName, sizeof(FileName), "Chapter.txt", ChapterDir);

#ifdef PLATFORM_IPHONE
	FILE* ChapFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* ChapFile = fopen(FileName, "r");
#endif

	if (!ChapFile)
	{
		ReportError("Chapter.txt is missing.");
		return;
	}

	while (!feof(ChapFile))
	{
		char Line[1024];
		
		GetNextLine(ChapFile, Line, sizeof(Line)-1);

		if (strstr(Line, "STITCHING") != NULL)
		{
			while (!feof(ChapFile))
			{
				GetNextLine(ChapFile, Line, sizeof(Line));

				if (strstr(Line, "END OF STITCHING") != NULL)
					break;

				int NRandomPages = 0;
				int RandomPages[MAX_PAGES];

				char* PageName = strtok(Line, " ,");
				while (PageName)
				{
					bool Found = false;
					for (int i = 0; i < Chapter.NPages; i++)
					{
						if (strcasecmp(Chapter.Pages[i].Name, PageName) == 0)
						{
							Found = true;
							RandomPages[NRandomPages++] = i;
							break;
						}
					}
					if (!Found)
					{
						// Page not already loaded, attempt to load it.
						char FileName[1024];
						snprintf(FileName, sizeof(FileName), "%s/%s.tmx", CurrentChapterDir, PageName);
						
						if (LoadPageFromTMX(FileName))
						{
							RandomPages[NRandomPages++] = Chapter.NPages-1;
						}
					}

					PageName = strtok(NULL, " ");
				}

				if (NRandomPages > 0)
				{
					int Choice = RandomPages[rand() % NRandomPages];
					Chapter.StitchedPages[Chapter.NStitchedPages++] = Choice;
				}
			}

			continue;
		}		
	}

	fclose(ChapFile);

	if (!Chapter.NPages)
		return;

	if (!Chapter.NBlocks)
		return;

	// If no stitching was found, use the first page.
	if (Chapter.NStitchedPages == 0)
	{
		Chapter.NStitchedPages = 1;
		Chapter.StitchedPages[0] = 0;
	}

	// Stitch pages
	Chapter.StitchedWidth = Chapter.Pages[0].Width;

	Chapter.StitchedHeight = 0;
	for (int i = 0; i < Chapter.NStitchedPages; i++)
		Chapter.StitchedHeight += Chapter.Pages[Chapter.StitchedPages[i]].Height;

	Chapter.StitchedBlocks = (int*)malloc(Chapter.StitchedWidth * Chapter.StitchedHeight * sizeof(int));
	int PageY = 0;
	for (int i = 0; i < Chapter.NStitchedPages; i++)
	{
		SPage* Page = &Chapter.Pages[Chapter.StitchedPages[i]];

		for (int y = 0; y < Page->Height; y++)
		{
			for (int x = 0; x < Chapter.StitchedWidth; x++)
			{
				Chapter.StitchedBlocks[(PageY + y) * Chapter.StitchedWidth + x] = Page->Blocks[y * Page->Width + x];
			}
		}

		PageY += Page->Height;
	}

	// Process any special blocks that need to create dynamic objects.
	for (int y = 0; y < Chapter.StitchedHeight; y++)
	{
		for (int x = 0; x < Chapter.StitchedWidth; x++)
		{
			int BlockID = Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x];

			if (BlockID < SPECIALBLOCKID_FIRST)
			{
				if (BlockID < 0 || BlockID > Chapter.NBlocks)
					ReportError("Invalid block ID encountered when stitching.");
				
				SBlock* Block = &Chapter.Blocks[BlockID];

				/*
				if (strcasecmp(Block->Desc, "blank") == 0)
				{
					Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
				}

				if (strstr(Block->Desc, "barrel") != NULL)
				{
					CreateBarrel(x * 64, y * 64, Block->Desc);
					Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
				}

				if (strstr(Block->Desc, "coin") != NULL)
				{
					CreateCoin(x * 64, y * 64, Block->Desc);
					Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
				}

				if (strstr(Block->Desc, "ball") != NULL)
				{
					CreateBall(x * 64, y * 64, Block->Desc);
					Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
				}
				
                if (strstr(Block->Desc, "gear") != NULL)
                {
                    CreateGear(x * 64, y * 64, Block->Desc);
                    Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
                }

				if (strstr(Block->Desc, "firework") != NULL)
				{
					CreateFireWork(x * 64, y * 64, Block->Desc);
					Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
				}
												                
				if (strcasecmp(Block->Desc, "dusty") == 0)
				{
					Dusty.FloatX = (float)x * 64;
					Dusty.FloatY = (float)y * 64 + 64;
					Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
				}
				*/
			}
		}
	}

	// Set initial ScrollY.
	ScrollY = -(Chapter.StitchedHeight * 64 - gxScreenHeight);
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
		gxDestroySprite(&Chapter.TileSets[i].Sprite);
	}
	Chapter.NTileSets = 0;

	Chapter.NBlocks = 0;

	Chapter.NStitchedPages = 0;
	free(Chapter.StitchedBlocks);
	Chapter.StitchedBlocks = NULL;

	Chapter.StitchedWidth = 0;
	Chapter.StitchedHeight = 0;
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
	if (ScrollY < -(Chapter.StitchedHeight * 64 - gxScreenHeight))
	{
		ScrollY = -(Chapter.StitchedHeight * 64 - gxScreenHeight);
	}
}

void DisplayChapter()
{
	for (int y = 0; y < Chapter.StitchedHeight; y++)
	{
		// Skip rows of tiles that cannot be on screen.
		if (y*64 + ScrollY > gxScreenHeight || (y+1)*64 + ScrollY < 0)
			continue;

		for (int x = 0; x < Chapter.StitchedWidth; x++)
		{
			int BlockID = Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x];

			if (BlockID >= SPECIALBLOCKID_FIRST)
			{
				switch (BlockID)
				{
				case SPECIALBLOCKID_BLANK: 
					// Nothing to draw.
					break;
												   
				case SPECIALBLOCKID_UNKNOWN:
					gxDrawSprite(x*64, y*64 + ScrollY, &TileUnknownSprite);
					break;
				}
			}
			else
			{
				SBlock* Block = &Chapter.Blocks[BlockID];

				STileSet* TileSet = &Chapter.TileSets[Block->TileSet];

				int SubX = Block->SubX;
				int SubY = Block->SubY;

				gxDrawSpriteSubRect(x*64, y*64 + ScrollY, SubX, SubY, SubX + 64, SubY + 64, &TileSet->Sprite);
			}
		}
	}
}

int GetBlockID(int x, int y)
{
	// Requests for blocks outside the map return a special value.
	if (x < 0 || x >= Chapter.StitchedWidth)
		return SPECIALBLOCKID_OUTOFBOUNDS;
	if (y < 0 || y >= Chapter.StitchedHeight)
		return SPECIALBLOCKID_OUTOFBOUNDS;

	return Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x];
}

bool IsBlockEmpty(int x, int y)
{
	// Requests for blocks outside the map return solid.
	if (x < 0 || x >= Chapter.StitchedWidth)
		return false;
	if (y < 0 || y >= Chapter.StitchedHeight)
		return false;

	if (Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] == SPECIALBLOCKID_BLANK)
		return true;
	else
		return false;
}

bool IsBlockSolid(int x, int y)
{
	return !IsBlockEmpty(x, y);
}

void DisplayScore()
{
    gxDrawString(600, 0, 16, gxRGB32(255, 255, 255), "Score: %d", Score.ScoreCounter);
}

void UpdateScore()
{
    if (Dusty.State == DUSTYSTATE_JUMP || Dusty.State == DUSTYSTATE_LAUNCH)
    {
        Score.RaiseScore = true;
    }
    else
    {   
        Score.RaiseScore = false;
    }
    
    if (Score.RaiseScore == true)
    {
        Score.ScoreCounter += 1;
    }
    else
    {
        Score.ScoreCounter += 0;
    }
}

void DisplayDelayDestructible()//Sorry about that, I must have been tired when I put it in here... I'll fiddle with it... :)
{
	for (int y = 0; y < Chapter.StitchedHeight; y++)
	{
		for (int x = 0; x < Chapter.StitchedWidth; x++)
		{  					
			int BlockID = GetBlockID(x, y);
			if (BlockID < SPECIALBLOCKID_FIRST)
			{			
				SBlock* Block = &Chapter.Blocks[BlockID];
				if ((Dusty.CollideWithBottomSide || Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide) && Block->DelayDest)//Need to setup a better trigger for this.
                {
                    if (Dusty.BlockSprite == 1)
                    {
                        gxDrawSprite(x*64, y*64 + ScrollY, &TileGreenDelayDest);					        
                    }

                    if (Dusty.BlockSprite == 2)
                    {
                        gxDrawSprite(x*64, y*64 + ScrollY, &TileYellowDelayDest);	    
                    }

                    if (Dusty.BlockSprite == 3)
                    {
                        gxDrawSprite(x*64, y*64 + ScrollY, &TileRedDelayDest);
                    }
                }							        															          					
			}			
		}
	}
}