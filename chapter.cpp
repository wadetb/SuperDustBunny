#include "chapter.h"


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

void LoadChapter(const char* filename, SChapter* Chap)
{
	FILE* ChapFile = fopen(filename, "r");

	Chap->NBlocks = 0;
	Chap->NPages = 0;

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

				SBlock* Block = &Chap->Blocks[Chap->NBlocks++];

				sscanf(Line, "%c%c%c\n", &Block->Key[0][0], &Block->Key[0][1], &Block->Key[0][2] );
				
				fgets(Line, sizeof(Line)-1, ChapFile);
				sscanf(Line, "%c%c%c %s\n", &Block->Key[1][0], &Block->Key[1][1], &Block->Key[1][2], &Block->Desc);

				fgets(Line, sizeof(Line)-1, ChapFile);
				sscanf(Line, "%c%c%c\n", &Block->Key[2][0], &Block->Key[2][1], &Block->Key[2][2]);
				
				gxLoadSprite(Block->Desc, &Block->Sprite);
			}

			continue;
		}

		if (strstr(Line, "PAGE") != NULL)
		{
			SPage* Page = &Chap->Pages[Chap->NPages++];
				
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

					if (Col >= (int)strlen(Line0) || Col >= (int)strlen(Line1) || Col >= (int)strlen(Line2)) 
					{
						break;
					}

					int BlockIndex = -1;

					for (int i = 0; i < Chap->NBlocks; i++)
					{
						SBlock* Block = &Chap->Blocks[i];

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

			continue;
		}		
	}

}


void RenderChapter(SChapter* Chap)
{
	for (int y = 0; y < Chap->Pages[0].Height; y++)
	{
		for (int x = 0; x < Chap->Pages[0].Width; x++)
		{

			int CurBlockNum = Chap->Pages[0].Blocks[y * Chap->Pages[0].Width + x];
			SBlock* Block = &Chap->Blocks[CurBlockNum];

			gxDrawSprite(x*64, y*64, &Block->Sprite);

			
		}
	}
}
