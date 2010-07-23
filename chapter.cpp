#include "Common.h"
#include "chapter.h"
#include "Dusty.h"
#include "Barrel.h"
#include "Coin.h"

SChapter Chapter;

extern gxSprite UnknownBlock;

int ScrollY;

void GetNextLine(FILE* File, char* Line, int LineSize)
{
	for (;;)
	{
		fgets(Line, LineSize-1, File);

		char* p = Line;

		bool AllWhitespace = true;
		for (; *p; p++) 
		{
			if (!isspace(*p))
			{
				AllWhitespace = false;
				break;
			}
		}

		if (!AllWhitespace)
			break;
	}
}

void LoadChapter(const char* filename)
{
#if defined(PLATFORM_IPHONE)
	FILE* ChapFile = gxOpenFile(filename, "r");
#else
	FILE* ChapFile = fopen(filename, "r");
#endif
	
	Chapter.NBlocks = 0;
	Chapter.NPages = 0;

	while (!feof(ChapFile))
	{
		char Line[1024];
		
		fgets(Line, sizeof(Line)-1, ChapFile);
		while (strcmp(Line, "\n") == 0)
		{
			fgets(Line, sizeof(Line)-1, ChapFile);
		}

		if (strstr(Line, "BLOCKS") != NULL)
		{

			while (!feof(ChapFile))
			{
				GetNextLine(ChapFile, Line, sizeof(Line));

				if (strstr(Line, "END OF BLOCKS") != NULL)
					break;

				SBlock* Block = &Chapter.Blocks[Chapter.NBlocks];

				sscanf(Line, "%c%c%c", &Block->Key[0][0], &Block->Key[0][1], &Block->Key[0][2] );

				fgets(Line, sizeof(Line)-1, ChapFile);
				sscanf(Line, "%c%c%c", &Block->Key[1][0], &Block->Key[1][1], &Block->Key[1][2]);

				Block->Desc = _strdup(Line + 4);
				*strrchr(Block->Desc, '\n') = 0;

				fgets(Line, sizeof(Line)-1, ChapFile);
				sscanf(Line, "%c%c%c", &Block->Key[2][0], &Block->Key[2][1], &Block->Key[2][2]);
				
				if (_stricmp(Block->Desc, "blank") == 0)
				{
					Block->ID = SPECIALBLOCKID_BLANK;
				}
				else
				{
					Block->ID = Chapter.NBlocks;

					gxLoadSprite(Block->Desc, &Block->Sprite);

#ifdef PLATFORM_WINDOWS
					// If unable to load the sprite, substitute an ASCII representation.
					if (!Block->Sprite.tex)
					{
						gxCreateASCIIBlockSprite(&Block->Sprite, &Block->Key[0][0]);
					}
#endif
				}

				Chapter.NBlocks++;
			}

			continue;
		}

		if (strstr(Line, "PAGE") != NULL)
		{
			SPage* Page = &Chapter.Pages[Chapter.NPages++];
				
			Page->Blocks = (int*)malloc(MAX_PAGE_BLOCKS * sizeof(int));
			
			Page->Width = 0;
			Page->Height = 0;

			int NBlocks = 0;

			while (!feof(ChapFile))
			{
				GetNextLine(ChapFile, Line, sizeof(Line));

				if (strstr(Line, "END OF PAGE") != NULL)
					break;

				char Line0[1024];
				char Line1[1024];
				char Line2[1024];

				strcpy(Line0, Line);
				fgets(Line1, sizeof(Line1)-1, ChapFile);
				fgets(Line2, sizeof(Line2)-1, ChapFile);

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
						Page->Blocks[NBlocks++] = -1;
				}
				else if (NBlocks > Page->Width * Page->Height)
				{
					NBlocks = Page->Width * Page->Height;
				}

				if (NBlocks >= MAX_PAGE_BLOCKS)
					break;
			}

			// Reduce page memory to what was actually used.
			Page->Blocks = (int*)realloc(Page->Blocks, Page->Width * Page->Height * sizeof(int));

			// Process any special blocks that need to create dynamic objects.
			for (int y = 0; y < Page->Height; y++)
			{
				for (int x = 0; x < Page->Width; x++)
				{
					int BlockID = Page->Blocks[y * Page->Width + x];

					if (BlockID < SPECIALBLOCKID_FIRST)
					{
						SBlock* Block = &Chapter.Blocks[BlockID];

						if (_stricmp(Block->Desc, "blank") == 0)
						{
							Page->Blocks[y * Page->Width + x] = SPECIALBLOCKID_BLANK;
						}

						if (strstr(Block->Desc, "barrel") != NULL)
						{
							CreateBarrel(x * 64, y * 64, Block->Desc);
							Page->Blocks[y * Page->Width + x] = SPECIALBLOCKID_BLANK;
						}
						
                        if (strstr(Block->Desc, "coin") != NULL)
                        {
                            CreateCoin(x * 64, y * 64, Block->Desc);
                            Page->Blocks[y * Page->Width + x] = SPECIALBLOCKID_BLANK;
                        }

						if (_stricmp(Block->Desc, "dusty") == 0)
						{
							Dusty.FloatX = x * 64;
							Dusty.FloatY = y * 64;
							Page->Blocks[y * Page->Width + x] = SPECIALBLOCKID_BLANK;
						}
					}
				}
			}

			continue;
		}		
	}

	// Set initial ScrollY.
	ScrollY = -(Chapter.Pages[0].Height * 64 - gxScreenHeight);
}

void CalculateScrollY()
{
	// If Dusty is dead, freeze scrolling while he falls off the screen.
	if (Dusty.State == DUSTYSTATE_DIE)
		return;

	// Screen tracks Dusty upwards.
	if (Dusty.FloatY + ScrollY < 300)
	{
		ScrollY = 300 - Dusty.FloatY;
	}

	// Screen also tracks Dusty downwards.
	if (Dusty.FloatY + ScrollY > gxScreenHeight - 100)
	{
		ScrollY = (gxScreenHeight - 100) - Dusty.FloatY;
	}

	// Prevent scrolling off bottom of map.
	if (ScrollY < -(Chapter.Pages[0].Height * 64 - gxScreenHeight))
	{
		ScrollY = -(Chapter.Pages[0].Height * 64 - gxScreenHeight);
	}
}

void DisplayChapter()
{
	for (int y = 0; y < Chapter.Pages[0].Height; y++)
	{
		// Skip rows of tiles that cannot be on screen.
		if (y*64 + ScrollY > gxScreenHeight || (y+1)*64 + ScrollY < 0)
			continue;

		for (int x = 0; x < Chapter.Pages[0].Width; x++)
		{
			int BlockID = Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x];

			if (BlockID >= SPECIALBLOCKID_FIRST)
			{
				switch (BlockID)
				{
				case SPECIALBLOCKID_BLANK: 
					// Nothing to draw.
					break; 

				case SPECIALBLOCKID_UNKNOWN:
					gxDrawSprite(x*64, y*64 + ScrollY, &UnknownBlock);
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
	if (x < 0 || x >= Chapter.Pages[0].Width)
		return SPECIALBLOCKID_OUTOFBOUNDS;
	if (y < 0 || y >= Chapter.Pages[0].Height)
		return SPECIALBLOCKID_OUTOFBOUNDS;

	return Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x];
}

bool IsBlockEmpty(int x, int y)
{
	// Requests for blocks outside the map return solid.
	if (x < 0 || x >= Chapter.Pages[0].Width)
		return false;
	if (y < 0 || y >= Chapter.Pages[0].Height)
		return false;

	if (Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x] == SPECIALBLOCKID_BLANK)
		return true;
	else
		return false;
}

bool IsBlockSolid(int x, int y)
{
	return !IsBlockEmpty(x, y);
}

