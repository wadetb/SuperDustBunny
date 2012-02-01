//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Vacuum.h"
#include "Dusty.h"
#include "Chapter.h"
#include "Settings.h"
#include "Smoke.h"
#include "Tutorial.h"


#define MAX_VACUUM_QUEUE_NODES 1000
#define MAX_VACUUM_TRIGGERS 50


struct SVacuumQueueNode
{
    int X, Y;
    int Force;
    int Next;
};

struct SVacuumTriggerProperties
{
    EVacuumDir Direction;
    EVacuumSide Side;
};

struct SVacuumTrigger
{
    float X, Y;
    SVacuumTriggerProperties* Props;
    bool Active;
};


SVacuumQueueNode VacuumQueue[MAX_VACUUM_QUEUE_NODES];
int VacuumQueueHead;
int VacuumQueueTail;
int VacuumFreeHead;

SVacuum Vacuum;

const int VACUUM_INITIAL_TIME = 5*60;
const int VACUUM_RETREAT_TIME = 5*60;
const int VACUUM_UNJAM_TIME   = 1*60;
const int VACUUM_CHARGE_DELAY = 5*60;

const float VacuumYOffset = 100;

int NVacuumTriggers;
SVacuumTrigger VacuumTriggers[MAX_VACUUM_TRIGGERS];


static void RestartVacuumForceMap();
static void UpdateVacuumForceMap();


void InitVacuum()
{
    Vacuum.Type = (EVacuumType)Chapter.PageProps.VacuumType;
	Vacuum.Dir = (EVacuumDir)Chapter.PageProps.VacuumDir;
    Vacuum.Side = VACUUMSIDE_LEFT;
    
    Vacuum.X = 384;
    if (Vacuum.Dir == VACUUMDIR_UP)
        Vacuum.Y = Chapter.PageHeight*64 + LitScreenHeight*2;
    else
        Vacuum.Y = -LitScreenHeight*2;
    
	Vacuum.State = VACUUMSTATE_OFF;
	Vacuum.Volume = 0.5f;
    
    Vacuum.Damage = 0;

    Vacuum.Charging = false;
    
    if (Vacuum.ForceMap)
        free(Vacuum.ForceMap);
    if (Vacuum.NextForceMap)
        free(Vacuum.NextForceMap);

    Vacuum.ForceMapWidth = Chapter.PageWidth;
    Vacuum.ForceMapHeight = Chapter.PageHeight;
    Vacuum.ForceMap = (int*)malloc(Chapter.PageWidth * Chapter.PageHeight * sizeof(int));
    Vacuum.NextForceMap = (int*)malloc(Chapter.PageWidth * Chapter.PageHeight * sizeof(int));

    for (int Offset = 0; Offset < Vacuum.ForceMapWidth * Vacuum.ForceMapHeight; Offset++)
        Vacuum.ForceMap[Offset] = (Chapter.PageBlocks[Offset] == SPECIALBLOCKID_BLANK) ? 0 : 0xffffffff;

    for (int i = 0; i < MAX_VACUUM_QUEUE_NODES; i++)
        VacuumQueue[i].Next = i+1;
    VacuumQueue[MAX_VACUUM_QUEUE_NODES-1].Next = -1;
    VacuumFreeHead = 0;

    Vacuum.ChargeTimer = VACUUM_CHARGE_DELAY * Chapter.PageProps.VacuumSpeed;
    
    RestartVacuumForceMap();
    
	// These two sounds loop continuously.
	sxSetSoundVolume(&VacuumOnSound, 0);
	sxPlaySoundLooping(&VacuumOnSound);
	sxSetSoundVolume(&VacuumJamSound, 0);
	sxPlaySoundLooping(&VacuumJamSound);
}

void DisplayVacuum()
{
	// If the vacuum is disabled for this page, don't display at all.
	if (Chapter.PageProps.VacuumOff || Settings.DisableVacuum)
		return;

 	if (Vacuum.State == VACUUMSTATE_RETREAT || Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
        gxSprite* BlinkSprite = NULL;
        float LightsAlpha = 1.0f;

        if (Vacuum.Type == VACUUM_NORMAL)
        {
            if (Vacuum.Dir == VACUUMDIR_UP)
                AddLitSpriteScaled(LIGHTLIST_VACUUM, &Vacuum1Sprite, Vacuum.X - Vacuum1Sprite.width/2 + ScrollX, Vacuum.Y + ScrollY - VacuumYOffset, 1.0f, 1.0f);
            else
                AddLitSpriteScaled(LIGHTLIST_VACUUM, &Vacuum1Sprite, Vacuum.X - Vacuum1Sprite.width/2 + ScrollX, Vacuum.Y + ScrollY + VacuumYOffset, 1.0f, -1.0f);

            if (Chapter.PageProps.LightsOff)
            {
                if (Vacuum.Dir == VACUUMDIR_UP)
                {
                    AddLitSpriteCenteredScaledAlpha(LIGHTLIST_LIGHTING, &LightVacuumSprite, Vacuum.X + ScrollX, Vacuum.Y + ScrollY - 580, 1.0f, LightsAlpha);
                }
            }
            
            // Damage overlay
            gxSprite* DamageSprite = NULL;
            switch (Vacuum.Damage)
            {
                case 1: DamageSprite = &VacuumHurt1Sprite; break;
                case 2: DamageSprite = &VacuumHurt2Sprite; break;
                case 3: DamageSprite = &VacuumHurt3Sprite; break;
                case 4: DamageSprite = &VacuumHurt4Sprite; break;
                case 5: DamageSprite = &VacuumHurt5Sprite; break;
                default: break;
            }
            if (DamageSprite)
                AddLitSpriteScaled(LIGHTLIST_VACUUM, DamageSprite, Vacuum.X - DamageSprite->width/2 + ScrollX, Vacuum.Y + ScrollY - VacuumYOffset, 1.0f, 1.0f);

            // Blinking eyes overlay
            if (Vacuum.State == VACUUMSTATE_RETREAT)
                BlinkSprite = &VacuumBlink2Sprite;
            else
            {
                switch (Vacuum.BlinkTimer/6)
                {
                    case 1: BlinkSprite = &VacuumBlink1Sprite; LightsAlpha = 0.75f; break;
                    case 2: BlinkSprite = &VacuumBlink2Sprite; LightsAlpha = 0.5f; break;
                    case 3: BlinkSprite = &VacuumBlink1Sprite; LightsAlpha = 0.75f; break;
                    default: break;
                }
            }
            if (BlinkSprite)
            {
                if (Vacuum.Dir == VACUUMDIR_UP)
                    AddLitSpriteScaled(LIGHTLIST_VACUUM, BlinkSprite, Vacuum.X - BlinkSprite->width/2 + ScrollX, Vacuum.Y + ScrollY - VacuumYOffset, 1.0f, 1.0f);
                else
                    AddLitSpriteScaled(LIGHTLIST_VACUUM, BlinkSprite, Vacuum.X - BlinkSprite->width/2  + ScrollX, Vacuum.Y + ScrollY + VacuumYOffset, 1.0f, -1.0f);
            }
        }
        else if (Vacuum.Type == VACUUM_DUSTBUSTER)
        {
            gxSprite* Sprite = &DustBusterSprite;
            
            if (Vacuum.Side == VACUUMSIDE_LEFT)
            {
                if (Vacuum.Dir == VACUUMDIR_UP)
                    AddLitSpriteScaled(LIGHTLIST_VACUUM, Sprite, Vacuum.X+140 + ScrollX, Vacuum.Y+560 + ScrollY, -1.0f, -1.0f);
                else
                    AddLitSpriteScaled(LIGHTLIST_VACUUM, Sprite, Vacuum.X+140 + ScrollX, Vacuum.Y-560 + ScrollY, -1.0f, 1.0f);
            }
            else
            {
                if (Vacuum.Dir == VACUUMDIR_UP)
                    AddLitSpriteScaled(LIGHTLIST_VACUUM, Sprite, Vacuum.X-140 + ScrollX, Vacuum.Y+560 + ScrollY, 1.0f, -1.0f);
                else
                    AddLitSpriteScaled(LIGHTLIST_VACUUM, Sprite, Vacuum.X-140 + ScrollX, Vacuum.Y-560 + ScrollY, 1.0f, 1.0f);
            }
        }
	}
    
	if (DevMode)
	{
		gxDrawString(5, 64, 16, 0xffffffff, "Vacuum State=%d Timer=%d y=%d\n      [On%02d Jam%02d TOn%02d TOff%02d]", 
			Vacuum.State, Vacuum.Timer, (int)Vacuum.Y, (int)(VacuumOnSound.volume*99), (int)(VacuumJamSound.volume*99), (int)(VacuumTurnOnSound.volume*99), (int)(VacuumTurnOffSound.volume*99));

		//gxDrawRectangleFilled(0, (int)Vacuum.Y + ScrollY, gxScreenWidth, 2, gxRGBA32(255,0,0,255));
	}
}

void UpdateVacuumSound()
{
	float TargetVolume;
	if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		TargetVolume = 0.8f;
	}
	else
	{
		TargetVolume = 1.0f;
	}

	if (Vacuum.Volume != TargetVolume)
	{
		if (Vacuum.Volume < TargetVolume)
			Vacuum.Volume += 0.01f;
		else
			Vacuum.Volume -= 0.01f;
	}

	if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		if (VacuumOnSound.volume < 1.0f)
			sxSetSoundVolume(&VacuumOnSound, VacuumOnSound.volume + 0.1f);
		if (VacuumJamSound.volume > 0.0f)
			sxSetSoundVolume(&VacuumJamSound, VacuumJamSound.volume - 0.1f);
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		if (VacuumJamSound.volume < 1.0f)
			sxSetSoundVolume(&VacuumJamSound, VacuumJamSound.volume + 0.1f);
		if (VacuumOnSound.volume > 0.0f)
			sxSetSoundVolume(&VacuumOnSound, VacuumOnSound.volume - 0.1f);
	}
	else if (Vacuum.State == VACUUMSTATE_OFF)
	{
		if (VacuumTurnOffSound.volume < 1.0f)
			sxSetSoundVolume(&VacuumTurnOffSound, VacuumTurnOffSound.volume + 0.1f);
		if (VacuumOnSound.volume > 0.0f)
			sxSetSoundVolume(&VacuumOnSound, VacuumOnSound.volume - 0.1f);
	}
}

static void CreateVacuumSmoke(int Count)
{
    
    // Always spawn smoke on screen or just off screen, for quick feedback.
    float SmokeY = Vacuum.Y + 150.0f;
    if (SmokeY + ScrollY > LitScreenHeight + 100.0f)
        SmokeY = ( LitScreenHeight + 100.0f ) - ScrollY;
    
    CreateSmoke(384.0f, SmokeY, Count);
}

static float GetVacuumTargetX()
{
    if (Vacuum.Type == VACUUM_NORMAL)
    {
        return 384 - ScrollX;
    }
    else if (Vacuum.Type == VACUUM_DUSTBUSTER)
    {
        if (Vacuum.Side == VACUUMSIDE_LEFT)                    
            return 192 - ScrollX;
        else
            return 576 - ScrollX;
    }
    return 0.0f;
}

void UpdateVacuum()
{
#ifdef PLATFORM_WINDOWS
	// V key to summon the vacuum.
	if (kbIsKeyDown(KB_V) && !kbWasKeyDown(KB_V))
		Vacuum.Timer = 0;

	// Shift-V key to banish the vacuum.
	if (kbIsKeyDown(KB_LSHIFT) && kbIsKeyDown(KB_V) && !kbWasKeyDown(KB_V))
	{
		JamVacuum();
		Vacuum.Timer = 1000000;
	}
#endif

	// If the vacuum is disabled for this page, don't update at all.
	if (Chapter.PageProps.VacuumOff || Settings.DisableVacuum)
		return;

	if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
		if (Dusty.State != DUSTYSTATE_DIE)
		{
            if (!TutorialOverrides.FocusOnVacuum && !Vacuum.Charging)
            {
                Vacuum.ChargeTimer--;
                if (Vacuum.ChargeTimer <= 0)
                {
                    Vacuum.Charging = true;
                    Vacuum.ChargeTimer = 5 * 60;   
                    Vacuum.Timer = 0;
                    int LightsOffset = Chapter.PageProps.LightsOff ? 768 : 0;
                    if (Vacuum.Dir == VACUUMDIR_UP)
                        Vacuum.Y = (float)-ScrollY + LitScreenHeight + LightsOffset;
                    else
                        Vacuum.Y = (float)-ScrollY - LightsOffset;
                }
            }
            
            if (Vacuum.Charging)
            {
                float TargetY;
                int LightsOffset = Chapter.PageProps.LightsOff ? 768 : 0;
                if (Vacuum.Dir == VACUUMDIR_UP)
                    TargetY = (float)-ScrollY + LitScreenHeight + LightsOffset;
                else
                    TargetY = (float)-ScrollY - LightsOffset;
                TargetY -= Vacuum.Timer * 1.5f * Chapter.PageProps.VacuumSpeed;
                if (TargetY < Vacuum.Y)
                    Vacuum.Y = TargetY;
                
//                Vacuum.ChargeTimer--;
//                if (Vacuum.ChargeTimer <= 0)
//                {
//                    Vacuum.ChargeTimer = 5*60 * Chapter.PageProps.VacuumSpeed;
//                    Vacuum.Charging = false;
//                }
            }
            else
            {
                float VacuumSpeed;
                if (Vacuum.Timer < 0)
                    VacuumSpeed = 0;
                else if (Vacuum.Timer < 4*60)
                    VacuumSpeed = 3.0f * Chapter.PageProps.VacuumSpeed;
                else if (Vacuum.Timer < 8*60)
                    VacuumSpeed = 3.5f * Chapter.PageProps.VacuumSpeed;
                else
                    VacuumSpeed = 6.0f * Chapter.PageProps.VacuumSpeed;
                
                if (Vacuum.Dir == VACUUMDIR_UP)
                    Vacuum.Y -= VacuumSpeed;
                else
                    Vacuum.Y += VacuumSpeed;
            }

			Vacuum.Timer++;

            Vacuum.X = Vacuum.X*0.8f + GetVacuumTargetX()*0.2f;
            
			if (IsInVacuum(Dusty.FloatX, Dusty.FloatY))
			{
				SetDustyState_Die(DEATH_VACUUM);
			}
		}
	}
	else if (Vacuum.State == VACUUMSTATE_RETREAT)
	{
		Vacuum.Timer--;

        if ((Vacuum.Timer % 20 == 0))
            CreateVacuumSmoke(1);

		// Move the vacuum back down the screen.
		if (Vacuum.Timer <= 0)
		{
			Vacuum.State = VACUUMSTATE_ONSCREEN;
            Vacuum.Timer = 0;
		}
	}
    
    if (Vacuum.State == VACUUMSTATE_ONSCREEN || Vacuum.State == VACUUMSTATE_RETREAT)
    {
        Vacuum.BlinkTimer--;
        if (Vacuum.BlinkTimer < 0)
            Vacuum.BlinkTimer = 120 + rand() % 120;
    }
    
    // Zoom the screen when the vacuum is on screen.
    float TargetZoom;
    if (Vacuum.State == VACUUMSTATE_ONSCREEN && Dusty.State != DUSTYSTATE_DIE && Vacuum.Timer >= 0 && fabsf(Vacuum.Y - Dusty.FloatY) < 700)
    {
        LitSceneOffsetX = Clamp(LitSceneOffsetX + (rand() % 13) - 6, -10, 10);
        LitSceneOffsetY = Clamp(LitSceneOffsetY + (rand() % 13) - 6, -10, 10);
        TargetZoom = 1.1f;
    }
    else if (Vacuum.State == VACUUMSTATE_RETREAT && Vacuum.Timer >= VACUUM_RETREAT_TIME - 60 && Dusty.State != DUSTYSTATE_DIE)
    {
        LitSceneOffsetX = Clamp(LitSceneOffsetX + (rand() % 13) - 6, -10, 10);
        LitSceneOffsetY = Clamp(LitSceneOffsetY + (rand() % 13) - 6, -10, 10);
        TargetZoom = 1.1f;
    }
    else
    {
        LitSceneOffsetX = 0.0f;
        LitSceneOffsetY = 0.0f;
        TargetZoom = 1.0f;
    }
    LitSceneZoom = LitSceneZoom * 0.9f + TargetZoom * 0.1f;

    // Process vacuum forces.
    UpdateVacuumForceMap();
}

void JamVacuum()
{
	if (Vacuum.State == VACUUMSTATE_ONSCREEN)
	{
        if (Vacuum.Damage < 5)
            Vacuum.Damage++;
        
        CreateVacuumSmoke(2);
        
		Vacuum.State = VACUUMSTATE_RETREAT;
		Vacuum.Timer = VACUUM_RETREAT_TIME;
        
        Vacuum.Charging = false;
        Vacuum.ChargeTimer = VACUUM_CHARGE_DELAY;
	}
	//else
	//	Vacuum.Timer += VACUUM_UNJAM_TIME;
}

void TurnOffVacuum()
{
	if (Vacuum.State != VACUUMSTATE_OFF)
	{
		Vacuum.State = VACUUMSTATE_OFF;
		sxSetSoundVolume(&VacuumTurnOffSound, 0);
		sxPlaySound(&VacuumTurnOffSound);
	}
    
    LitSceneOffsetX = 0.0f;
    LitSceneOffsetY = 0.0f;
    LitSceneZoom = 1.0f;
}

void TurnOnVacuum(float InitialDistance, float DelayBeforeMoving, bool Charging)
{
    Vacuum.State = VACUUMSTATE_ONSCREEN;

    Vacuum.Timer = -(int)(DelayBeforeMoving*60);
    int LightsOffset = Chapter.PageProps.LightsOff ? 768 : 0;
    
    Vacuum.X = GetVacuumTargetX();
    
    // Vacuum.Y is the leading edge of the vacuum.
    if (Vacuum.Dir == VACUUMDIR_UP)
        Vacuum.Y = (float)-ScrollY + InitialDistance + LitScreenHeight + LightsOffset;
    else
        Vacuum.Y = (float)-ScrollY - InitialDistance - LightsOffset;

    Vacuum.Charging = Charging;
    
//    sxSetSoundVolume(&VacuumTurnOnSound, 0);
//    sxPlaySound(&VacuumTurnOnSound);
}

bool IsInVacuum(float X, float Y)
{
    if (Vacuum.State != VACUUMSTATE_ONSCREEN)
        return false;
    
    return IsNearVacuum(X, Y);
 }

bool IsNearVacuum(float X, float Y)
{
    if (Vacuum.Type == VACUUM_NORMAL)
    {
        if (Vacuum.Dir == VACUUMDIR_UP)
            return Y >= Vacuum.Y;
        else
            return Y <= Vacuum.Y;
    }
    else if (Vacuum.Type == VACUUM_DUSTBUSTER)
    {
        return Distance(X, Y, Vacuum.X, Vacuum.Y) <= 150;
    }
    
    return false;
}

float GetDistanceToVacuum(float X, float Y)
{
    if (Vacuum.State != VACUUMSTATE_ONSCREEN)
        return 1000000.0f;
    
    if (Vacuum.Type == VACUUM_NORMAL)
    {
        return fabsf( Y - Vacuum.Y );
    }
    else if (Vacuum.Type == VACUUM_DUSTBUSTER)
    {
        return Distance(X, Y, Vacuum.X, Vacuum.Y);
    }
    
    return 1000000.0f;    
}

#ifdef PLATFORM_WINDOWS
#pragma optimize("gt", on)
#endif

static void RestartVacuumForceMap()
{
    for (int Offset = 0; Offset < Vacuum.ForceMapWidth * Vacuum.ForceMapHeight; Offset++)
        Vacuum.NextForceMap[Offset] = (Chapter.PageBlocks[Offset] == SPECIALBLOCKID_BLANK) ? 0 : INT_MAX;
    
    int CurX = (int)(Vacuum.X / 64 + 0.5f);
    int CurY = (int)(Vacuum.Y / 64 + 0.5f);
    
    if (CurX < 0) CurX = 0;
    if (CurY < 0) CurY = 0;
    if (CurX >= Vacuum.ForceMapWidth) CurX = Vacuum.ForceMapWidth-1;
    if (CurY >= Vacuum.ForceMapHeight) CurY = Vacuum.ForceMapHeight-1;
    
    int InitialForce = 30;
    
    Vacuum.NextForceMap[CurY * Vacuum.ForceMapWidth + CurX] = InitialForce;
    
    int NextVacuumFreeHead = VacuumQueue[VacuumFreeHead].Next;
    
    VacuumQueue[VacuumFreeHead].X = CurX;
    VacuumQueue[VacuumFreeHead].Y = CurY;
    VacuumQueue[VacuumFreeHead].Force = InitialForce;
    VacuumQueue[VacuumFreeHead].Next = -1;
    
    VacuumQueueHead = VacuumFreeHead;
    VacuumQueueTail = VacuumFreeHead;
    
    VacuumFreeHead = NextVacuumFreeHead;
    
    //LogMessage("Recalculating vacuum forces\n");
}

static void UpdateVacuumForceMap()
{    
    int MaxIterations = 100;
    int TotalIterations = 0;
    
    while (VacuumQueueHead != -1 && TotalIterations < MaxIterations)
    {
        TotalIterations++;
        
        SVacuumQueueNode* Node = &VacuumQueue[VacuumQueueHead];

        int CurX = Node->X;
        int CurY = Node->Y;

        unsigned int NewForce = Node->Force-1;
        if (NewForce > 0)
        {
            for (int X = -1; X <= 1; X++)
            {
                int NewX = CurX + X;
                if (NewX < 0 || NewX >= Vacuum.ForceMapWidth)
                    continue;

                for (int Y = -1; Y <= 1; Y++)
                {
                    if (X == 0 && Y == 0)
                        continue;

                    int NewY = CurY + Y;
                    if (NewY < 0 || NewY >= Vacuum.ForceMapHeight)
                        continue;

                    int OldForce = Vacuum.NextForceMap[NewY * Vacuum.ForceMapWidth + NewX];
                    if ( OldForce >= NewForce )
                        continue;

                    Vacuum.NextForceMap[NewY * Vacuum.ForceMapWidth + NewX] = NewForce;
                    
                    if (VacuumFreeHead == -1)
                    {
                        LogMessage("Ran out of Vacuum force nodes.\n");
                        continue;
                    }

                    SVacuumQueueNode* NewNode = &VacuumQueue[VacuumFreeHead];
                    NewNode->X = NewX;
                    NewNode->Y = NewY;
                    NewNode->Force = NewForce;

                    VacuumQueue[VacuumQueueTail].Next = VacuumFreeHead;
                    VacuumQueueTail = VacuumFreeHead;
                    VacuumFreeHead = NewNode->Next;
                    NewNode->Next = -1;
                }
            }
        }

        int NextQueueHead = Node->Next;

        Node->Next = VacuumFreeHead;
        VacuumFreeHead = VacuumQueueHead;

        VacuumQueueHead = NextQueueHead;
    }
    
    if (VacuumQueueHead == -1)
    {
        memcpy(Vacuum.ForceMap, Vacuum.NextForceMap, Vacuum.ForceMapWidth*Vacuum.ForceMapHeight*sizeof(int));
        RestartVacuumForceMap();
    }
    
#if 0
    for (int CurY = 0; CurY < Vacuum.ForceMapHeight; CurY++)
    {
        for (int CurX = 0; CurX < Vacuum.ForceMapWidth; CurX++)
        {
            float X = CurX*64+32 + ScrollX;
            float Y = CurY*64+32 + ScrollY;
            if (Y >= 0 && Y < LitScreenHeight)
            {
                int Force = Vacuum.ForceMap[CurY*Vacuum.ForceMapWidth+CurX];
                if (Force != INT_MAX)
                    AddDebugLine(X-Force, Y, X+Force, Y, gxRGBA32(255, 0, 0, 255), 1.0f/60.0f);
            }
        }
    }
#endif
}

void GetVacuumForce(float X, float Y, float* VX, float* VY, float Strength, bool FollowLevel)
{
	// If the vacuum is disabled for this page, no vacuum forces.
	if (Chapter.PageProps.VacuumOff || Settings.DisableVacuum || Vacuum.State != VACUUMSTATE_ONSCREEN)
	{
		*VX = 0;
		*VY = 0;
		return;
	}

    if (FollowLevel)
    {
        // Cheap solution, not accurate.
        float TotalVX, TotalVY;

        int CurX = (int)((X+32)/64);
        int CurY = (int)((Y+32)/64);

        if (CurX < 0 || CurX >= Vacuum.ForceMapWidth || CurY < 0 || CurY >= Vacuum.ForceMapHeight)
        {
            *VX = 0;
            *VY = 0;
            return;        
        }
        
        TotalVX = 0;
        TotalVY = 0;

        int BaseForce = Vacuum.ForceMap[CurY * Vacuum.ForceMapWidth + CurX];

        for (int TX = CurX-1; TX <= CurX+1; TX++)
        {
            if (TX < 0 || TX >= Vacuum.ForceMapWidth)
                continue;

            for (int TY = CurY-1; TY <= CurY+1; TY++)
            {
                if (TY < 0 || TY >= Vacuum.ForceMapHeight)
                    continue;

                if (TX == 0 && TY == 0)
                    continue;
            
                int Force = Vacuum.ForceMap[TY * Vacuum.ForceMapWidth + TX];
                if (Force == INT_MAX)
                    continue;
                if (Force <= BaseForce)
                    continue;
                
                float DirX = (float)( TX - CurX );
                float DirY = (float)( TY - CurY );

                TotalVX += DirX * Force*(Force-BaseForce) / 80.0f;
                TotalVY += DirY * Force*(Force-BaseForce) / 80.0f;
            }
        }

        float TotalLength = sqrtf(TotalVX*TotalVX + TotalVY*TotalVY);
        if (TotalLength > 0.001f)
        {
//            TotalVX /= TotalLength;
//            TotalVY /= TotalLength;
            *VX = TotalVX * Strength;
            *VY = TotalVY * Strength;
            return;
        }

        *VX = 0;
        *VY = 0;
    }
    else
    {
	    float DirX = Vacuum.X - X;
	    float DirY = Vacuum.Y - Y;
	
	    float Length = sqrtf(DirX*DirX + DirY*DirY);

	    if (Length >= 1000)
	    {
		    *VX = 0;
		    *VY = 0;
		    return;
	    }

	    DirX /= Length;
	    DirY /= Length;

	    float AttenuatedStrength = Strength * Lerp(Length, 1000, 200, 1.5f, 20.0f); 

	    *VX = DirX * AttenuatedStrength;
	    *VY = DirY * AttenuatedStrength;
    }
}

void ParseVacuumTriggerProperties(SBlock* Block, rapidxml::xml_node<char>* PropertiesNode)
{
	SVacuumTriggerProperties* Props = (SVacuumTriggerProperties*)malloc(sizeof(SVacuumTriggerProperties));
    
    Props->Direction = VACUUMDIR_UP;
    Props->Side = VACUUMSIDE_LEFT;
    
	rapidxml::xml_node<char>* PropertyNode = PropertiesNode->first_node("property");
	while (PropertyNode)
	{
		const char* Name = PropertyNode->first_attribute("name")->value();
		const char* ConstValue = PropertyNode->first_attribute("value")->value();
        
		if (strcmp(Name, "direction") == 0)
		{
            if (strcmp(ConstValue, "up") == 0)
                Props->Direction = VACUUMDIR_UP;
            else if (strcmp(ConstValue, "down") == 0)
                Props->Direction = VACUUMDIR_DOWN;
            else
                ReportError("Unknown value '%s' for vacuum trigger 'direction' property; allowed values are 'up' and 'down'.  Fix this and re-save the TMX file.", ConstValue);
        }
		else if (strcmp(Name, "side") == 0)
		{
            if (strcmp(ConstValue, "left") == 0)
                Props->Side = VACUUMSIDE_LEFT;
            else if (strcmp(ConstValue, "right") == 0)
                Props->Side = VACUUMSIDE_RIGHT;
            else if (strcmp(ConstValue, "center") == 0)
                Props->Side = VACUUMSIDE_CENTER;
            else
                ReportError("Unknown value '%s' for vacuum trigger 'side' property; allowed values are 'left' and 'right'.  Fix this and re-save the TMX file.", ConstValue);
        }
        
		PropertyNode = PropertyNode->next_sibling("property");
	}
    
	Block->Properties = Props;
}

void CreateVacuumTrigger(int X, int Y, SVacuumTriggerProperties* Props)
{
	if (NVacuumTriggers >= MAX_VACUUM_TRIGGERS)
		ReportError("Exceeded the maximum of %d total vacuum triggers.", MAX_VACUUM_TRIGGERS);
    
	SVacuumTrigger* Trigger = &VacuumTriggers[NVacuumTriggers++];
    
    Trigger->Props = Props;
    
	Trigger->X = (float)X + 32;
	Trigger->Y = (float)Y + 32;
    
    Trigger->Active = true;
}

void ClearVacuumTriggers()
{
	NVacuumTriggers = 0;
}

void UpdateVacuumTriggers()
{
    for (int i = 0; i < NVacuumTriggers; i++)
    {
        SVacuumTrigger* Trigger = &VacuumTriggers[i];
        if (!Trigger->Active)
            continue;
        
        if (Dusty.FloatY <= Trigger->Y)
        {
//            float ScreenDist = (Vacuum.Y + ScrollY) - LitScreenHeight/2;
//            if (ScreenDist > LitScreenHeight/2 + 256)
            {            
                Vacuum.Dir = Trigger->Props->Direction;
                Vacuum.Side = Trigger->Props->Side;
                TurnOnVacuum(400, 2.0f, true);
            }
            
            Trigger->Active = false;
        }
    }
}

