//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Wipe.h"


SWipe Wipe;


void StartWipe(EWipeType Type)
{
	Wipe.Type = Type;
	Wipe.T = 0.0f;
	Wipe.Middle = false;
	Wipe.Finished = false;
}

void UpdateWipe()
{
	if (!Wipe.Finished)
	{
		Wipe.T += 2.0f/60.0f;

		if (Wipe.T == 0.5f)
		{
			Wipe.Middle = true;
		}
		if (Wipe.T >= 1.0f)
		{
			Wipe.Finished = true;
		}
	}
}

void DisplayWipe()
{
	if (Wipe.Type != WIPE_NONE)
	{
		//       |       | \          \/
		//       |       |  \          \/
		//       |       |   \          \/
		//       |       |    \          \/
		float TotalWidth = 2*gxScreenWidth + 400.0f;

		float TopX1 = gxScreenWidth - TotalWidth*Wipe.T;
		float TopX2 = TopX1 + gxScreenWidth + 200.0f;
		float BotX1 = gxScreenWidth + 200.0f - TotalWidth*Wipe.T;
		float BotX2 = BotX1 + gxScreenWidth + 200.0f;

		AddLitQuad(LIGHTLIST_VACUUM, &WhiteSprite, gxRGBA32(0,0,0,255),
			TopX1, 0,                       0.0f, 0.0f, 
			TopX2, 0,                       1.0f, 0.0f,
			BotX2, (float)gxScreenHeight,   1.0f, 1.0f, 
			BotX1, (float)gxScreenHeight,   0.0f, 1.0f);
	}
}

