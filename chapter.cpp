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

SChapter Chapter;
SScore Score;

int ScrollY;

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

bool LoadBlocks(const char* FileName)
{
#ifdef PLATFORM_IPHONE
	FILE* BlocksFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* BlocksFile = fopen(FileName, "r");
#endif

	if (!BlocksFile)
		return false;

	while (!feof(BlocksFile))
	{
		char Line[1024];
		GetNextLine(BlocksFile, Line, sizeof(Line));

		SBlock* Block = &Chapter.Blocks[Chapter.NBlocks];

		sscanf(Line, "%c%c%c", &Block->Key[0][0], &Block->Key[0][1], &Block->Key[0][2] );

		GetNextLine(BlocksFile, Line, sizeof(Line)-1);
		sscanf(Line, "%c%c%c", &Block->Key[1][0], &Block->Key[1][1], &Block->Key[1][2]);

		Block->Desc = strdup(Line + 4);

		GetNextLine(BlocksFile, Line, sizeof(Line)-1);
		sscanf(Line, "%c%c%c", &Block->Key[2][0], &Block->Key[2][1], &Block->Key[2][2]);

        if (strcmp(Block->Desc, "delaydest") == 0)
		{
			Block->ID = SPECIALBLOCKID_DELAYDEST;
		}
				
		if (strcmp(Block->Desc, "blank") == 0)
		{
			Block->ID = SPECIALBLOCKID_BLANK;
		}
		else
		{
			Block->ID = Chapter.NBlocks;

			char* SpriteName = strdup(Block->Desc);
			SpriteName = strtok(SpriteName, " ");
			gxLoadSprite(SpriteName, &Block->Sprite);
			free(SpriteName);

#ifdef PLATFORM_WINDOWS
			// If unable to load the sprite, substitute an ASCII representation.
			if (!Block->Sprite.tex)
			{
				gxCreateASCIIBlockSprite(&Block->Sprite, &Block->Key[0][0]);
			}
#endif

			if (strstr(Block->Desc, "destructible"))
			{
				Block->Destructible = true;
			}
			
			if (strstr(Block->Desc, "delaydest"))
			{
				Block->DelayDest = true;
			}
						
			if (strstr(Block->Desc, "EndOfLevel.png"))
			{
				Block->EndOfLevel = true;
			}

			if (strstr(Block->Desc, "Gum.png"))
			{
				Block->Gum = true;
			}

			if (strstr(Block->Desc, "GumJump.png"))
			{
				Block->GumJump = true;
			}

			if (strstr(Block->Desc, "Jello.png"))
			{
				Block->Jello = true;
			}
			
			if (strstr(Block->Desc, "nowalljump"))
			{
				Block->NoWallJump = true;
			}	
		}

		Chapter.NBlocks++;
	}

	return true;
}

bool LoadPage(const char* FileName)
{
#ifdef PLATFORM_IPHONE
	FILE* PageFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* PageFile = fopen(FileName, "r");
#endif

	if (!PageFile)
		return false;

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

	// Allocate largest possible block array (will be adjusted later).
	Page->Blocks = (int*)malloc(MAX_PAGE_BLOCKS * sizeof(int));

	Page->Width = 0;
	Page->Height = 0;

	int NBlocks = 0;

	while (!feof(PageFile))
	{
		char Line0[1024];
		char Line1[1024];
		char Line2[1024];

		GetNextLine(PageFile, Line0, sizeof(Line0));
		GetNextLine(PageFile, Line1, sizeof(Line1));
		GetNextLine(PageFile, Line2, sizeof(Line2));

		if (feof(PageFile))
			break;

		int Col = 0;

		for (;;)
		{
			// Skip column if all spaces or ruler					
			while (Col < (int)strlen(Line0) && Col < (int)strlen(Line1) && Col < (int)strlen(Line2) &&
				   isspace(Line0[Col]) && isspace(Line1[Col]) && isspace(Line2[Col]))
			{
				Col++;
			}

			// Break if any line width is exceeded.
			if (Col >= (int)strlen(Line0) || Col >= (int)strlen(Line1) || Col >= (int)strlen(Line2)) 
			{
				break;
			}

			// Find the block which matches this ASCII pattern.
			// (If none is found the unknown block will be used)
			int BlockIndex = SPECIALBLOCKID_UNKNOWN;

			for (int i = 0; i < Chapter.NBlocks; i++)
			{
				SBlock* Block = &Chapter.Blocks[i];

				if (Block->Key[0][0] == Line0[Col+0] &&
					Block->Key[0][1] == Line0[Col+1] &&
					Block->Key[0][2] == Line0[Col+2])
				{
					if (Block->Key[1][0] == Line1[Col+0] &&
						Block->Key[1][1] == Line1[Col+1] &&
						Block->Key[1][2] == Line1[Col+2])
					{
						if (Block->Key[2][0] == Line2[Col+0] &&
							Block->Key[2][1] == Line2[Col+1] &&
							Block->Key[2][2] == Line2[Col+2])
						{
							BlockIndex = i;
							break;
						}
					}
				}
			}

			Page->Blocks[NBlocks++] = BlockIndex;
			Col -= -3;

			if (NBlocks >= MAX_PAGE_BLOCKS)
				break;
		}

		// The width of the page is set by the first row.
		if (Page->Width == 0)
		{
			Page->Width = NBlocks;
		}

		Page->Height++;

		// Fix missing or extra blocks.
		if (NBlocks < Page->Width * Page->Height)
		{
			while (NBlocks < Page->Width * Page->Height && NBlocks < MAX_PAGE_BLOCKS)
				Page->Blocks[NBlocks++] = SPECIALBLOCKID_UNKNOWN;
		}
		else if (NBlocks > Page->Width * Page->Height)
		{
			NBlocks = Page->Width * Page->Height;
		}

		if (NBlocks >= MAX_PAGE_BLOCKS)
			break;
	}

	// If no valid blocks were found, this page file is invalid.
	if (Page->Width == 0 && Page->Height == 0)
	{
		free(Page->Blocks);

		Chapter.NPages--;

		return false;
	}

	// Reduce page memory to what was actually used.
	Page->Blocks = (int*)realloc(Page->Blocks, Page->Width * Page->Height * sizeof(int));

	return true;
}

void LoadChapter(const char* ChapterDir)
{
	Chapter.NBlocks = 0;
	Chapter.NPages = 0;
	Chapter.NStitchedPages = 0;

	char FileName[1024];
	snprintf(FileName, sizeof(FileName), "%s/Blocks.txt", ChapterDir);
	if (!LoadBlocks(FileName))
	{
		// TODO: Display error message here.
		return;
	}

	snprintf(FileName, sizeof(FileName), "%s/Chapter.txt", ChapterDir);

#ifdef PLATFORM_IPHONE
	FILE* ChapFile = gxOpenFile(FileName, "r");
#endif
#ifdef PLATFORM_WINDOWS
	FILE* ChapFile = fopen(FileName, "r");
#endif

	if (!ChapFile)
	{
		// TODO: Display error message here.
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
						snprintf(FileName, sizeof(FileName), "%s/Pages/%s.txt", ChapterDir, PageName);
						
						if (LoadPage(FileName))
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
				SBlock* Block = &Chapter.Blocks[BlockID];

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

	for (int i = 0; i < Chapter.NBlocks; i++)
	{
		free(Chapter.Blocks[i].Desc);
		gxDestroySprite(&Chapter.Blocks[i].Sprite);
	}
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
				gxDrawSprite(x*64, y*64 + ScrollY, &Block->Sprite);
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