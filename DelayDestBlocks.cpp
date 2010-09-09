//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "DelayDestBlocks.h"
#include "Common.h"
#include "Dusty.h"
#include "chapter.h"

#define MAX_DBLOCKS 100

int NDBlocks = 0;
SDBlock DBlocks[MAX_DBLOCKS];

void CreateDBlock(int X, int Y, const char* Desc)
{
    SDBlock* DBlock = &DBlocks[NDBlocks++];

	DBlock->State = DBLOCKSTATE_INACTIVE;

    DBlock->X = (float)X + 32;
    DBlock->Y = (float)Y + 32;
    DBlock->FloatVelocityY = 0.0f;
    
    DBlock->Transition = 80;
	DBlock->Sprite = 1;
}

void ClearDBlocks()
{
	NDBlocks = 0;
}

void DisplayDBlocks()
{
    for (int i = 0; i < NDBlocks; i++)
    {
        SDBlock* DBlock = &DBlocks[i];
        
        if (DBlock->Sprite == 1) 
        {
            gxDrawSpriteCenteredRotated( (int)DBlock->X, (int)(DBlock->Y + ScrollY), 0, &TileDelayDest );
        } 
		
        if (DBlock->Sprite == 2)
        {
            gxDrawSpriteCenteredRotated( (int)DBlock->X, (int)(DBlock->Y + ScrollY), 0, &TileGreenDelayDest );
        } 

        if (DBlock->Sprite == 3)
        {
            gxDrawSpriteCenteredRotated( (int)DBlock->X, (int)(DBlock->Y + ScrollY), 0, &TileYellowDelayDest );
        }

        if (DBlock->Sprite == 4)
        {
            gxDrawSpriteCenteredRotated( (int)DBlock->X, (int)(DBlock->Y + ScrollY), 0, &TileRedDelayDest );
        } 	
    }    
}

void UpdateDBlocks()
{
    for (int i = 0; i < NDBlocks; i++)
    {
        SDBlock* DBlock = &DBlocks[i];
        
		if (DBlock->State == DBLOCKSTATE_INACTIVE)
		{
			float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, DBlock->X, DBlock->Y);
			
			if (Dist < 50)
			{
				DBlock->State = DBLOCKSTATE_ACTIVE;
			}      
		}
		
		if (DBlock->State == DBLOCKSTATE_ACTIVE)
		{		    
		    if (DBlock->Transition == 80)
		    {
		        DBlock->Sprite = 1;
		    }    
		    
		    if (DBlock->Transition == 79)
		    {
		        DBlock->Sprite = 2;
		    }
		    
		    if (DBlock->Transition == 60)
		    {
		        DBlock->Sprite = 3;
		    }
		    
		    if (DBlock->Transition == 40)
		    {
		        DBlock->Sprite = 4;
		    }
		    
		    if (DBlock->Transition == 0)
		    {
		        DBlock->Sprite = 5;
		    }

		    if (DBlock->Transition == 0)
		    {
		    	//DBlock->Transition = 30;

		        for (int y = 0; y < Chapter.StitchedHeight; y++)
			    {
				    for (int x = 0; x < Chapter.StitchedWidth; x++)
				    {
					    int BlockID = GetBlockID(x, y);
					    if (BlockID < SPECIALBLOCKID_FIRST)
					    {
						    SBlock* Block = &Chapter.Blocks[BlockID];						
						    if (Block->DelayDest)
						    {
							    Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
						    }
					    }
				    }
			    }
		    }
		    DBlock->Transition -= 1;	      
		}	
    }  
}