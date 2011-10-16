//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Baby.h"
#include "chapter.h"
#include "Dusty.h"
#include "Vacuum.h"
#include "Tweak.h"
#include "Dust.h"


#define MAX_BABIES 50

#define MAX_DUSTY_HISTORY 30


enum EBabyState
{
    BABYSTATE_INACTIVE,
    BABYSTATE_WAIT_STAND,
    BABYSTATE_WAIT_HOP,
    BABYSTATE_DIE,
    BABYSTATE_HEAVEN,
    BABYSTATE_FOLLOW,
};

struct SBaby
{
    EBabyState State;
    EDustyDirection Direction;
    float FloatTimer;
    int Timer;
    float Size;
    float OriginalX;
	float X, Y;
    float VelocityX, VelocityY;
    float Alpha;
    float FollowOffset;
    int FollowOrder;
    EDustyState FollowState;
    EDustySprite FollowSprite;
};

struct SDustyHistory
{
    float X;
    float Y;
    float ScaleX;
    EDustyState State;
    EDustyDirection Direction;
    EDustySprite Sprite;
    float XAdj;
    float XMirrorAdj;
    float YAdj;
};


int NBabies;
SBaby Babies[MAX_BABIES];

SDustyHistory DustyHistory[MAX_DUSTY_HISTORY];
int DustyHistoryCount = 0;
int DustyHistoryFrame = 0;


static void RecordDustyHistory()
{
    DustyHistoryFrame++;
    
    for (int i = MAX_DUSTY_HISTORY-1; i >= 1; i--)
        DustyHistory[i] = DustyHistory[i-1];
    
    DustyHistory[0].Sprite = Dusty.LastSprite;
    DustyHistory[0].X = Dusty.FloatX;
    DustyHistory[0].Y = Dusty.FloatY;
    DustyHistory[0].ScaleX = Dusty.LastScaleX;
    DustyHistory[0].State = Dusty.State;
    DustyHistory[0].Direction = Dusty.Direction;
    DustyHistory[0].XAdj = Dusty.LastXAdj;
    DustyHistory[0].XMirrorAdj = Dusty.LastXMirrorAdj;
    DustyHistory[0].YAdj = Dusty.LastYAdj;
    
    if (DustyHistoryCount < MAX_DUSTY_HISTORY)
        DustyHistoryCount++;
}

static void ClearDustyHistory()
{
    DustyHistoryFrame = 0;
    DustyHistoryCount = 0;
}

void CreateBaby(int X, int Y, unsigned int Flags)
{
	if (NBabies >= MAX_BABIES)
		ReportError("Exceeded the maximum of %d total Babies.", MAX_BABIES);
    
	SBaby* Baby = &Babies[NBabies++];
    
    Baby->State = BABYSTATE_WAIT_STAND;
    Baby->FloatTimer = Random(60, 120);

    Baby->Direction = (Flags & SPECIALBLOCKID_FLIP_X) ? DIRECTION_RIGHT : DIRECTION_LEFT;

    Baby->Size = 0.6f; //Random(0.6f, 0.7f);
    
	Baby->X = (float)X*64 + 32;
	Baby->Y = (float)Y*64 + 64;
    
    Baby->OriginalX = Baby->X;
    
    Baby->VelocityX = 0;
    Baby->VelocityY = 0;
    
    Baby->Alpha = 1.0f;
    
    EraseBlock(X, Y);
}

void ClearBabies()
{
	NBabies = 0;
    
    ClearDustyHistory();
}

static void DisplayBabySprite(SBaby* Baby, EDustySprite Sprite, float XAdj, float XMirrorAdj, float YAdj)
{
	float ScaleX, OffsetX;
	if (Baby->Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 32;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 224;
	}
    
    float X = Baby->X + OffsetX + XAdj + XMirrorAdj*ScaleX;
    float Y = Baby->Y + YAdj * Baby->Size;
    
	AddLitSpriteScaledAlpha(LIGHTLIST_FOREGROUND, DustySprite[Sprite], X + ScrollX, Y + ScrollY, ScaleX * Baby->Size, Baby->Size, Baby->Alpha);
}

void DisplayBabies()
{
    for (int i = 0; i < NBabies; i++)
    {
        SBaby* Baby = &Babies[i];
        
        if (Baby->State == BABYSTATE_WAIT_STAND)
        {
			if (Baby->Timer % 45 < 10)
				DisplayBabySprite(Baby, DUSTYSPRITE_IDLE_1, -124, 5, -221);
			else if (Baby->Timer % 45 < 40)
				DisplayBabySprite(Baby, DUSTYSPRITE_IDLE_2, -124, 5, -221);
			else
				DisplayBabySprite(Baby, DUSTYSPRITE_IDLE_3, -124, 5, -221);
        }
        else if (Baby->State == BABYSTATE_WAIT_HOP)
        {
            switch (Baby->Timer)
            {
                case 0:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_2, -122,  0, -201); break;
                case 1:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_2, -122,  0, -212); break;
                case 2:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_2, -122,  0, -218); break;
                case 3:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_2, -122,  0, -224); break;
                case 4:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_2, -122,  0, -228); break;
                case 5:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_2, -122,  0, -236); break;
                case 6:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_3, -122,  0, -226); break;
                case 7:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_3, -122,  0, -226); break;
                case 8:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_3, -122,  0, -226); break;
                case 9:    DisplayBabySprite(Baby, DUSTYSPRITE_HOP_3, -122,  0, -226); break;
                case 10:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_3, -122,  0, -226); break;
                case 11:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_3, -122,  0, -226); break;
                case 12:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_4, -119,  0, -220); break;
                case 13:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_4, -119,  0, -215); break;
                case 14:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_4, -119,  0, -210); break;
                case 15:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_4, -119,  0, -205); break;
                case 16:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_4, -119,  0, -200); break;
                case 17:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_4, -119,  0, -195); break;
                case 18:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_5, -119, 18, -217); break;
                case 19:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_5, -119, 18, -217); break;
                case 20:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_5, -119, 18, -217); break;
                case 21:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_5, -119, 18, -217); break;
                case 22:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_5, -119, 18, -217); break;
                case 23:   DisplayBabySprite(Baby, DUSTYSPRITE_HOP_5, -119, 18, -217); break;
                default:   break;
            }
        }
        else if (Baby->State == BABYSTATE_DIE)
        {
            DisplayBabySprite(Baby, DUSTYSPRITE_DEATH, -125, 0, -181);
        }
        else if (Baby->State == BABYSTATE_HEAVEN)
        {
            float ScaleX, OffsetX;
            if (Baby->Direction == DIRECTION_RIGHT)
            {
                ScaleX = 1.0f;
                OffsetX = 0.0f;
            }
            else
            {
                ScaleX = -1.0f;
                OffsetX = 256.0f * Baby->Size;
            }
            
            float t = Baby->FloatTimer * 6.0f;
            float dx = cos(t/5)*200 + cos(1+t/7)*200 + cos(1-t/9)*200;
            float dy = sin(t/5)*200 + sin(1+t/7)*200 + sin(1-t/9)*200;
            
            AddLitSpriteScaledAlpha(LIGHTLIST_FOREGROUND, &ScreenLoseGhostSprite, 
                                    Baby->X + dx + OffsetX + ScrollX, 
                                    Baby->Y + dy + ScrollY, 
                                    ScaleX * 0.35f * Baby->Size, 0.35f * Baby->Size, Baby->Alpha);            
        }
        else if (Baby->State == BABYSTATE_FOLLOW)
        {
            SDustyHistory* History = &DustyHistory[Baby->FollowOrder < DustyHistoryCount-1 ? Baby->FollowOrder : DustyHistoryCount-1];

            DisplayBabySprite(Baby, History->Sprite, History->XAdj, History->XMirrorAdj, History->YAdj);
        }
    }
}

static void SendBabyToHeaven(SBaby* Baby)
{
    Baby->State = BABYSTATE_HEAVEN;
    Baby->VelocityY = -6;
    Baby->Timer = 0;
    Baby->FloatTimer = Random(0, 100);
    Baby->Alpha = 0.3f;
    
    for (int i = 0; i < 25; i++)
        MakeDustMote(Baby->X + Random(-25, 25), Baby->Y - 30 + Random(-25, 25)); 
}

static void VacuumUpBaby(SBaby* Baby)
{
#if 1
    Baby->State = BABYSTATE_INACTIVE;
#elif 1
    SendBabyToHeaven(Baby);
#else
    Baby->State = BABYSTATE_DIE;
    Baby->Timer = 0;
    
    Baby->VelocityX = Baby->X > Vacuum.X ? -10.0f : 10.0f;
    Baby->VelocityY = -20;
#endif
}

void UpdateBabies()
{
    RecordDustyHistory();
    
    for (int i = 0; i < NBabies; i++)
    {
        SBaby* Baby = &Babies[i];
        
        bool CloseToVacuum = GetDistanceToVacuum(Baby->X, Baby->Y) < 600.0f;
        
        bool CloseToDusty = Distance(Baby->X, Baby->Y, Dusty.FloatX, Dusty.FloatY) < 400;
        
        if (Baby->State == BABYSTATE_WAIT_STAND)
        {
            if (IsInVacuum(Baby->X, Baby->Y))
            {
                VacuumUpBaby(Baby);
                return;
            }
            
            if (Dusty.FloatY <= Baby->Y && Distance(Baby->X, Baby->Y, Dusty.FloatX, Dusty.FloatY) < 200)
            {
                Baby->State = BABYSTATE_FOLLOW;
                Baby->FollowOffset = Random(-80.0f, 80.0f);
                Baby->FollowOrder = Random(10, MAX_DUSTY_HISTORY);
                continue;
            }
            
            Baby->FloatTimer -= 1.0f;
            
            if (CloseToVacuum)
                Baby->FloatTimer = 0;
            
            Baby->Timer = (int)Baby->FloatTimer;

            if (Baby->Timer <= 0)
            {
                Baby->State = BABYSTATE_WAIT_HOP;
                Baby->FloatTimer = 0;

                if (Baby->Direction == DIRECTION_LEFT && (IsBlockSolid((Baby->X - 96)/64, (Baby->Y - 32)/64) || !IsBlockSolid((Baby->X - 96)/64, (Baby->Y + 32)/64)))
                {
                    Baby->Direction = DIRECTION_RIGHT;
                }
                else if (Baby->Direction == DIRECTION_RIGHT && (IsBlockSolid((Baby->X + 96)/64, (Baby->Y - 32)/64) || !IsBlockSolid((Baby->X + 96)/64, (Baby->Y + 32)/64)))
                {
                    Baby->Direction = DIRECTION_LEFT;
                }
                else if (CloseToDusty && Baby->X > Dusty.FloatX)
                {
                    Baby->Direction = DIRECTION_LEFT;
                }
                else if (CloseToDusty && Baby->X < Dusty.FloatX)
                {
                    Baby->Direction = DIRECTION_RIGHT;
                }
                else if (Baby->Direction == DIRECTION_LEFT && Baby->X < Baby->OriginalX - 192)
                {
                    Baby->Direction = DIRECTION_RIGHT;
                }
                else if (Baby->Direction == DIRECTION_RIGHT && Baby->X > Baby->OriginalX + 192)
                {
                    Baby->Direction = DIRECTION_LEFT;
                }
                else if (Random(0.0f, 1.0f) > 0.95f)
                {
                    if (Baby->Direction == DIRECTION_LEFT)
                        Baby->Direction = DIRECTION_RIGHT;
                    else
                        Baby->Direction = DIRECTION_LEFT;
                }
                
                continue;
            }
        }
        else if (Baby->State == BABYSTATE_WAIT_HOP)
        {
            if (IsInVacuum(Baby->X, Baby->Y))
            {
                VacuumUpBaby(Baby);
                return;
            }

            if (Baby->Direction == DIRECTION_LEFT)
                Baby->X -= 5 * Baby->Size;
            else
                Baby->X += 5 * Baby->Size;
            
            if (CloseToVacuum)
                Baby->FloatTimer += 1.25f;
            else
                Baby->FloatTimer += 1.0f;
            
            Baby->Timer = (int)Baby->FloatTimer;
            
            if (Baby->Timer >= 24)
            {
                Baby->State = BABYSTATE_WAIT_STAND;
                
                if (CloseToVacuum)
                    Baby->FloatTimer = Random(0, 10);
                else
                    Baby->FloatTimer = Random(60, 120);
            
                continue;
            }
        }
        else if (Baby->State == BABYSTATE_DIE)
        {
            Baby->Timer += 1;
            
            if (Baby->Alpha > 0.0f)
                Baby->Alpha -= 0.01f;
            
            Baby->X += Baby->VelocityX;
            Baby->Y += Baby->VelocityY;
            
            Baby->VelocityY += Tweak.DustyGravity;
            
            float DirX, DirY;
            GetVacuumForce(Baby->X, Baby->Y, &DirX, &DirY, (float)Baby->Timer/400.0f, false);
            Baby->VelocityX += DirX;
            Baby->VelocityY += DirY;
            
            Baby->VelocityX *= 0.99f;
            Baby->VelocityY *= 0.99f;
            
            if (Baby->Timer > 60 && Distance(Baby->X, Baby->Y, Vacuum.X, Vacuum.Y) < 40.0f)
            {
                Baby->State = BABYSTATE_HEAVEN;
                Baby->Timer = 0;
            }
        }
        else if (Baby->State == BABYSTATE_HEAVEN)
        {
            if (Baby->VelocityY >= -10)
                Baby->VelocityY -= 0.125f;
            
            Baby->FloatTimer += 1.0f/60.0f;
            
            Baby->Y += Baby->VelocityY;
            if (Baby->Y < -1000)
            {
                Baby->State = BABYSTATE_INACTIVE;
                continue;
            }
        }
        else if (Baby->State == BABYSTATE_FOLLOW)
        {
            SDustyHistory* History = &DustyHistory[Baby->FollowOrder < DustyHistoryCount-1 ? Baby->FollowOrder : DustyHistoryCount-1];
            if (History->State == DUSTYSTATE_WALLJUMP)
                Baby->X = History->X;
            else
                Baby->X = History->X+Baby->FollowOffset;
            Baby->Y = History->Y;
            Baby->FollowState = History->State;
            Baby->Direction = History->Direction;
            Baby->Timer++;
        }
    }
}
