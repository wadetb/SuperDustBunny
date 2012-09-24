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
#include "GameScore.h"
#include "Tutorial.h"
#include "Coin.h"
#include "Gear.h"
#include "FireWorks.h"
#include "TennisBall.h"
#include "Smoke.h"


#define MAX_BABIES 50

#define MAX_DUSTY_HISTORY 50


enum EBabyState
{
    BABYSTATE_INACTIVE,
    BABYSTATE_WAIT_STAND,
    BABYSTATE_WAIT_HOP,
    BABYSTATE_DIE,
    BABYSTATE_HEAVEN,
    BABYSTATE_JUMP_TO_FOLLOW,
    BABYSTATE_FOLLOW,
    BABYSTATE_GATHER,
    BABYSTATE_JUMP_TO_ATTACK,
    BABYSTATE_ATTACK,
};

struct SBaby
{
    EBabyState State;
    EDustyDirection Direction;
    EDustyHat Hat;
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
    float GatherX;
    float GatherY;
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

void CreateBaby(float X, int Y, unsigned int Flags, int Hat, bool StartFollowing)
{
	if (NBabies >= MAX_BABIES)
		ReportError("Exceeded the maximum of %d total Babies.", MAX_BABIES);
    
	SBaby* Baby = &Babies[NBabies++];
    
    if (StartFollowing)
    {
        Baby->State = BABYSTATE_JUMP_TO_FOLLOW;
        Baby->FollowOffset = Random(-10.0f, 10.0f);
        Baby->FollowOrder = Random(10, MAX_DUSTY_HISTORY);
    }
    else
    {
        Baby->State = BABYSTATE_WAIT_STAND;
        Baby->FloatTimer = Random(60, 120);
    }
    
    Baby->Hat = (EDustyHat)Hat;
    
    Baby->Direction = (Flags & SPECIALBLOCKID_FLIP_X) ? DIRECTION_RIGHT : DIRECTION_LEFT;

    Baby->Size = 0.6f; //Random(0.6f, 0.7f);
    
	Baby->X = (float)X*64 + 32;
	Baby->Y = (float)Y*64 + 64;
    
    Baby->OriginalX = Baby->X;
    
    Baby->VelocityX = 0;
    Baby->VelocityY = 0;
    
    Baby->Alpha = 1.0f;
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
    
    if (Baby->Hat != DUSTYHAT_NONE)
    {
        gxSprite* HatSprite = DustyHatSprites[Baby->Hat];
        SDustyHatOffset* HatOffset = &DustyHatOffsets[Sprite];
        
        AddLitSpriteCenteredScaled2Rotated(LIGHTLIST_FOREGROUND, HatSprite, 
                                           X + ScrollX + HatOffset->X*ScaleX*Baby->Size, 
                                           Y + ScrollY + HatOffset->Y*Baby->Size, 
                                           ScaleX*Baby->Size, Baby->Size, DegreesToRadians(HatOffset->Angle)*ScaleX);
    }
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
        else if (Baby->State == BABYSTATE_JUMP_TO_FOLLOW || Baby->State == BABYSTATE_GATHER || Baby->State == BABYSTATE_JUMP_TO_ATTACK)
        {
            EDustySprite Hop2Sprites[3] =
            {
                DUSTYSPRITE_HOP_2,
                DUSTYSPRITE_HOP_2B,
                DUSTYSPRITE_HOP_2C,
            };
            
            EDustySprite Hop3Sprites[3] =
            {
                DUSTYSPRITE_HOP_3,
                DUSTYSPRITE_HOP_3B,
            };
            
            EDustySprite Hop4Sprites[3] =
            {
                DUSTYSPRITE_HOP_4,
                DUSTYSPRITE_HOP_4B,
                DUSTYSPRITE_HOP_4C,
            };
            
            if (Baby->VelocityY >= 5)
                DisplayBabySprite(Baby, Hop4Sprites[(Dusty.SpriteTransition/5) % 3], -124, -18, -150);
            else if (Baby->VelocityY <= -5)
                DisplayBabySprite(Baby, Hop2Sprites[(Dusty.SpriteTransition/5) % 3], -124, -18, -150);
            else
                DisplayBabySprite(Baby, Hop3Sprites[(Dusty.SpriteTransition/10) % 2], -124, -18, -150);
        }
        else if (Baby->State == BABYSTATE_FOLLOW)
        {
            SDustyHistory* History = &DustyHistory[Baby->FollowOrder < DustyHistoryCount-1 ? Baby->FollowOrder : DustyHistoryCount-1];

            DisplayBabySprite(Baby, History->Sprite, History->XAdj, History->XMirrorAdj, History->YAdj);
        }
        else if (Baby->State == BABYSTATE_ATTACK)
        {            
            DisplayBabySprite(Baby, DUSTYSPRITE_KICK, -124, -18, -150);
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
            
            if (!TutorialOverrides.NoBabyPickup && Dusty.FloatY <= Baby->Y && Distance(Baby->X, Baby->Y, Dusty.FloatX, Dusty.FloatY) < 200)
            {
                AwardBaby(Baby->Hat);
                Baby->State = BABYSTATE_JUMP_TO_FOLLOW;
                Baby->FollowOffset = Random(-10.0f, 10.0f);
                Baby->FollowOrder = Random(10, MAX_DUSTY_HISTORY);
                continue;
            }
            
            Baby->FloatTimer -= 1.0f;
            
            if (CloseToVacuum)
                Baby->FloatTimer = 0;
            
            Baby->Timer = (int)Baby->FloatTimer;

            if (Baby->Timer <= 0 && !TutorialOverrides.NoBabyHop)
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
                else if (!TutorialOverrides.NoBabyPickup && CloseToDusty && Baby->X > Dusty.FloatX)
                {
                    Baby->Direction = DIRECTION_LEFT;
                }
                else if (!TutorialOverrides.NoBabyPickup && CloseToDusty && Baby->X < Dusty.FloatX)
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
        else if (Baby->State == BABYSTATE_JUMP_TO_FOLLOW)
        {
            SDustyHistory* History = &DustyHistory[Baby->FollowOrder < DustyHistoryCount-1 ? Baby->FollowOrder : DustyHistoryCount-1];
            
            float TargetX;
            if (History->State == DUSTYSTATE_WALLJUMP)
                TargetX = History->X;
            else
                TargetX = History->X + Baby->FollowOffset;
            
            Baby->X = Baby->X*0.93f + TargetX*0.07f;
            
            float OldY = Baby->Y;
            Baby->Y = Baby->Y*0.95f + History->Y*0.05f;
            Baby->VelocityY = Baby->Y - OldY;
            
            if (Baby->X > Dusty.FloatX)
                Baby->Direction = DIRECTION_LEFT;
            else
                Baby->Direction = DIRECTION_RIGHT;
            
            Baby->Timer++;
            
            if (Distance(Baby->X, Baby->Y, TargetX, History->Y) < 30)
            {
                Baby->State = BABYSTATE_FOLLOW;
                Baby->Timer = 0;
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
        else if (Baby->State == BABYSTATE_GATHER)
        {
            Baby->X = Baby->X*0.9f + Baby->GatherX*0.1f;
            
            float OldY = Baby->Y;
            Baby->Y = Baby->Y*0.9f + Baby->GatherY*0.1f;
            Baby->VelocityY = Baby->Y - OldY;
            
            if (Baby->X > Baby->GatherX)
                Baby->Direction = DIRECTION_LEFT;
            else
                Baby->Direction = DIRECTION_RIGHT;
            
            Baby->Timer++;
            
            if (Distance(Baby->X, Baby->Y, Baby->GatherX, Baby->GatherY) < 30)
            {
                float X = Baby->X;
                float Y = Baby->Y;
                float Size = 3;
                
                bool GotSomething = false;
                
                for (int i = 0; i < NCoins; i++)//Need extern in header file to accomplish
                {
                    SCoin* Coin = &Coins[i];
                    float Dist =(Distance(X, Y, Coin->X, Coin->Y));
                    if (Dist < Size*64)
                    {                    
                        Coin->State = COINSTATE_COLLECTED;
                        GotSomething = true;
                    }        
                }
                
                for (int i = 0; i < NGears; i++)
                {
                    SGear* Gear = &Gears[i];
                    float Dist = (Distance(X, Y, Gear->X, Gear->Y));
                    if (Dist < Size*64)
                    {
                        Gear->State = GEARSTATE_FALLING;
                        GotSomething = true;
                    }
                }
                
                for (int i = 0; i < NBalls; i++)
                {
                    SBall* Ball = &Balls[i];
                    float Dist = (Distance(X, Y, Ball->X, Ball->Y));
                    if (Dist < Size*64)
                    {
                        Ball->State = BALLSTATE_FALLING;
                        GotSomething = true;
                    }
                }
                
                for (int i = 0; i < NFireWorks; i++)
                {
                    SFireWork* FireWork = &FireWorks[i];
                    
                    float Dist = (Distance(X, Y, FireWork->X, FireWork->Y));
                    if (Dist < Size*64 && Dist != 0 && FireWork->State == FIREWORKSTATE_WAIT)
                    {
                        FireWork->State = FIREWORKSTATE_FUSE;
                        GotSomething = true;
                    }
                }

                if (GotSomething)
                {
                    CreateWhiteSmoke(Baby->X, Baby->Y);
                    Baby->State = BABYSTATE_INACTIVE;
                    Baby->Timer = 0;
                }
                else
                {
                    Baby->State = BABYSTATE_JUMP_TO_FOLLOW;
                    Baby->Timer = 0;
                }
            }
        }
        else if (Baby->State == BABYSTATE_JUMP_TO_ATTACK)
        {
            Baby->X = Baby->X*0.9f + Baby->GatherX*0.1f;
            
            float OldY = Baby->Y;
            Baby->Y = Baby->Y*0.9f + Baby->GatherY*0.1f;
            Baby->VelocityY = Baby->Y - OldY;
            
            if (Baby->X > Baby->GatherX)
                Baby->Direction = DIRECTION_LEFT;
            else
                Baby->Direction = DIRECTION_RIGHT;
            
            Baby->Timer++;
            
            if (Distance(Baby->X, Baby->Y, Baby->GatherX, Baby->GatherY) < 30)
            {
                Baby->State = BABYSTATE_ATTACK;
                Baby->Timer = 0;
                Baby->GatherY = Vacuum.Y;
            }
        }
        else if (Baby->State == BABYSTATE_ATTACK)
        {
            Baby->X = Baby->X*0.9f + Baby->GatherX*0.1f;
            
            float OldY = Baby->Y;
            Baby->Y = Baby->Y*0.9f + Baby->GatherY*0.1f;
            Baby->VelocityY = Baby->Y - OldY;
            
            if (Baby->X > Baby->GatherX)
                Baby->Direction = DIRECTION_LEFT;
            else
                Baby->Direction = DIRECTION_RIGHT;
            
            Baby->Timer++;
            
            if (Distance(Baby->X, Baby->Y, Baby->GatherX, Baby->GatherY) < 30)
            {
                CreateWhiteSmoke(Baby->X, Baby->Y);
                JamVacuum();
                Baby->State = BABYSTATE_INACTIVE; // BABYSTATE_JUMP_TO_FOLLOW?
                Baby->Timer = 0;
            }
        }
    }
}

void SendBabyToGather(float X, float Y)
{
    if (NBabies == 0)
        return;
    
    for (int i = NBabies-1; i >= 0; i--)
    {
        SBaby* Baby = &Babies[i];
        if (Baby->State != BABYSTATE_FOLLOW)
            continue;
        
        if ((Y + ScrollY >= LitScreenHeight - 200 || Y >= Vacuum.Y) && Vacuum.State != VACUUMSTATE_RETREAT)
        {
            Baby->State = BABYSTATE_JUMP_TO_ATTACK;
            Baby->GatherX = X;
            Baby->GatherY = Baby->Y - 200;
        }
        else
        {
            Baby->State = BABYSTATE_GATHER;
            Baby->GatherX = X;
            Baby->GatherY = Y;
        }
        
        break;
    }
}

bool UseBabyProtection()
{
    if (NBabies == 0)
        return false;
    
    for (int i = NBabies-1; i >= 0; i--)
    {
        SBaby* Baby = &Babies[i];
        if (Baby->State != BABYSTATE_FOLLOW)
            continue;
        
        Baby->State = BABYSTATE_JUMP_TO_ATTACK;
        Baby->GatherX = 384 - ScrollX;
        Baby->GatherY = Baby->Y - 500;
        
        Vacuum.Paused = true;
        
        return true;
    }
    
    return false;
}
