//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Crumb.h"
#include "Dusty.h"
#include "chapter.h"

#define MAX_CRUMBS 100

int NCrumbs = 0;
SCrumb Crumbs[MAX_CRUMBS];

void CreateCrumb(int X, int Y, const char* Desc)
{
    SCrumb* Crumb = &Crumbs[NCrumbs++];

    Crumb->X = (float)X + 32;
    Crumb->Y = (float)Y + 32;

    sscanf(Desc, "Crumb dir=%d", &Crumb->Dir);

    Crumb->State = CRUMBSTATE_WAIT;
}

extern gxSprite ColonelCrumb;

extern int ScrollY;

void DisplayCrumbs()
{
    for (int i = 0; i < NCrumbs; i++)
    {
        SCrumb* Crumb = &Crumbs[i];

        if (Crumb->State == CRUMBSTATE_WAIT || Crumb->State == CRUMBSTATE_LAUNCH)
        {
            gxDrawSpriteCenteredRotated((int)Crumb->X, (int)Crumb->Y + ScrollY, Crumb->Dir * 3.14159f / 180.0f, &Crumb01);
        }

        if (Crumb->State == CRUMBSTATE_EXPLODE)
        {
            gxDrawSpriteCenteredRotated( (int)Crumb->X, (int)Crumb->Y + ScrollY, 0, &Crumb02);				
        }
    }
}

void UpdateCrumbs()
{
    for (int i = 0; i < NCrumbs; i++)
    {
        SCrumb* Crumb = &Crumbs[i];

        if (Crumb->State == CRUMBSTATE_WAIT)
        {
            float XDist = (float)(Dusty.FloatX - Crumb->X);
            float YDist = (float)((Dusty.FloatY-50) - (Crumb->Y));
            float Dist = sqrtf(XDist*XDist + YDist*YDist);

            if (Dist < 70)
            {
                Crumb->Timer = 10;
                Crumb->State = CRUMBSTATE_LAUNCH;
            }
        }

        if (Crumb->State == CRUMBSTATE_LAUNCH)
        {
            float Angle = (float)((90 - Crumb->Dir)) * 3.14159f/180.0f;
            float Velocity = 20.0f;
            Crumb->X += Velocity*cosf(Angle);
            Crumb->Y += -Velocity*sinf(Angle);

            Crumb->Timer--;

            if (Crumb->Timer == 0)
            {
                Crumb->Timer = 10;
                Crumb->State = CRUMBSTATE_EXPLODE;
            }

            for (int y = 0; y < Chapter.Pages[0].Height; y++)
            {
                for (int x = 0; x < Chapter.Pages[0].Width; x++)
                {
                    // Skip empty blocks.
                    if (IsBlockEmpty(x, y))
                    {
                        continue;
                    }

                    // Determine the bounds of the block.
                    float BlockLeft   = (float)x*64;
                    float BlockRight  = (float)x*64 + 64;
                    float BlockTop    = (float)y*64;
                    float BlockBottom = (float)y*64 + 64;

                    SBlock* Block = &Chapter.Blocks[GetBlockID(x, y)];

                    float XDist = (float)(Crumb->X - x*64);
                    float YDist = (float)(Crumb->Y - y*64);
                    float Dist = sqrtf(XDist*XDist + YDist*YDist);

                    if (Dist < 100)
                    { 
                        if (Crumb ->Y && Block->Destructible)
                        {
                            Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x] = SPECIALBLOCKID_BLANK;
                        }
                    }
                }
            }
        }

        if (Crumb->State == CRUMBSTATE_EXPLODE)
        {
            Crumb->Timer--;
            if (Crumb->Timer == 0)
            {
                Crumb->State = CRUMBSTATE_DONE;
            }
        }
    }
}
