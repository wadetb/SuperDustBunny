//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Dusty.h"
#include "Input.h"
#include "Chapter.h"
#include "Dust.h"
#include "Vacuum.h"
#include "Recorder.h"
#include "Settings.h"
#include "Stapler.h"
#include "PowerUp.h"
#include "Smoke.h"
#include "Ghost.h"


SDusty Dusty;


#define MAX_DUSTY_TRAIL 30

struct SDustyTrail
{
    float X;
    float Y;
    float ScaleX;
    EDustySprite Sprite;
    float Alpha;
};

SDustyTrail DustyTrail[MAX_DUSTY_TRAIL];


gxSprite* DustySprite[DUSTYSPRITE_COUNT] =
{
    &DustyHop1Sprite,
    &DustyHop2Sprite,
    &DustyHop3Sprite,
    &DustyHop4Sprite,
    &DustyHop5Sprite,
    &DustyHop2bSprite,
    &DustyHop2cSprite,
    &DustyHop3bSprite,
    &DustyHop4bSprite,
    &DustyHop4cSprite,
    &DustyHop5bSprite,
    &DustyIdle1Sprite,
    &DustyIdle2Sprite,
    &DustyIdle3Sprite,
    &DustySlide1Sprite,
    &DustySlide2Sprite,
    &DustySlide3Sprite,
    &DustyWallJumpSprite,
    &DustyWallJumpbSprite,
    &DustyWallJumpcSprite,
    &DustyCornerJumpSprite,
    &DustyCornerJumpbSprite,
    &DustyCornerJumpcSprite,
    &DustyCornerJumpdSprite,
    &DustyDeathSprite
};

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  Dusty initialization function                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitDusty()
{
	Dusty.State = DUSTYSTATE_STAND; 

	Dusty.Direction = DIRECTION_RIGHT;

	Dusty.Left = -32;
	Dusty.Right = 32;
	Dusty.Top = -110;
	Dusty.Bottom = 0;

	Dusty.FloatVelocityX = 0.0f;
	Dusty.FloatVelocityY = 0.0f;
	
	Dusty.FloatGravity = 0.3f;

    Dusty.LastX = 0;
    Dusty.LastY = 0;
    Dusty.LastScaleX = 1.0f;
    Dusty.LastSprite = DUSTYSPRITE_DEATH;
    
	Dusty.SpriteTransition = 5;

	Dusty.NoCollision = false;

	Dusty.WallStickTimer = 0;
	Dusty.LastWall = DIRECTION_NONE;
	Dusty.WallJumpTimer = 0;

    Dusty.JumpGraceTimer = 0;
    Dusty.AirJumpCount = 0;
    
    Dusty.LandTimer = 0;
    
    Dusty.ComboCount = 0;
    
    Dusty.Hidden = false;
    
    Dusty.OnFireTimer = 0;
    Dusty.PowerUpTimer = 0;
    
    Dusty.HasJumped = false;
    
	Dusty.GainLife = false;
	
	Dusty.CollideWithLeftSide = false;
	Dusty.CollideWithRightSide = false;
	Dusty.CollideWithTopSide = false;
	Dusty.CollideWithBottomSide = false;
    
    Dusty.JumpPower = 21.0f;

    memset(DustyTrail, 0, sizeof(DustyTrail));
};

void SetDustyPosition(float x, float y)
{
	Dusty.FloatX = x;
	Dusty.FloatY = y;
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  State switching function declarations                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand();
void SetDustyState_Jump( bool OffWall );
void SetDustyState_JumpWithVelocity( float VX, float VY );
void SetDustyState_Fall();
void SetDustyState_Hop( EDustyDirection Direction );
void SetDustyState_WallJump();
void SetDustyState_CornerJump();
void SetDustyState_PrepareLaunch();
void SetDustyState_Launch();
void SetDustyState_IntroHop();
void SetDustyState_IntroStand();


void UpdateDusty_JumpCommon();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  General purpose control                                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

static void UpdateDusty_MakeDust(float XOffset, float YOffset, int Count)
{
    // Spawn some dust motes.
    for (int i = 0; i < 5; i++)
        MakeDustMote(Dusty.FloatX, Dusty.FloatY - 50);    
}

static void UpdateDusty_GetNearbyBlocks()
{
    Dusty.NearbyBlocks = 0;
    
    Dusty.BlockX = (int)( ( Dusty.FloatX + (Dusty.Left+Dusty.Right)/2 ) / 64 );
    Dusty.BlockY = (int)( ( Dusty.FloatY + (Dusty.Top+Dusty.Bottom)/2 ) / 64 );
    
    for (int i = 0; i <= 2; i++)
        for (int j = 0; j <= 2; j++)
            Dusty.NearbyBlocks |= IsBlockSolid(Dusty.BlockX+i-1, Dusty.BlockY+j-1) * (1<<(j*3+i));
    
    Dusty.XInBlock = Dusty.FloatX - Dusty.BlockX*64;
    Dusty.YInBlock = Dusty.FloatY - Dusty.BlockY*64;
}

static bool UpdateDusty_NearbyBlocksAreClear(unsigned int Mask)
{
    return (Dusty.NearbyBlocks & Mask) == 0;
}

static bool UpdateDusty_CheckSwipeJump(float Angle, float Range)
{
    if (GetInput_IsSwipedUsed())
        return false;
    
    if (GetInput_GetSwipeTimeLeft() < 1.0f/20.0f)
        return false;
    
    float Current = GetInput_GetSwipeCurrent();
        
//    if (!GetInput_CheckSwipeStraightness(Current - 1.0f/20.0f, Current, 15.0f))
//        return false;
    
    float StartX, StartY;
    GetInput_GetSwipePosAtTime(&StartX, &StartY, Current - 1.0f/20.0f);

    float EndX, EndY;
    GetInput_GetSwipePosAtTime(&EndX, &EndY, Current);
    
    float dX = (EndX - StartX);
    float dY = (EndY - StartY);
    
    float L = Length(dX, dY);
    if (L < 0.0001f)
        return false;
    
    dX /= L;
    dY /= L;
    
    float aX = cosf(DegreesToRadians(Angle));
    float aY = -sinf(DegreesToRadians(Angle));
    
    float Dot = aX*dX + aY*dY;
    float cR = cosf(DegreesToRadians(Range));
    
    if ( Dot >= cR )
    {
        float Power = Dusty.JumpPower;
        //float Power = Remap(L, 50.0f, 100.0f, Dusty.JumpPower/3, Dusty.JumpPower, true);
        //float Power = Remap(MaxSpeed, 2000.0f, 5000.0f, Dusty.JumpPower/3, Dusty.JumpPower, true);
        dX = dX * Power * 0.8f;
        dY = dY * ( dY > 0 ? Power * 0.8f : Power );

        Dusty.SwipeAngle = NormalizeAngle(RadiansToDegrees(atan2f(-dY, dX)));
        Dusty.SwipePower = Power;
        
#ifdef SWIPE_DEBUG
        AddDebugLine(Dusty.FloatX + ScrollX, Dusty.FloatY + ScrollY, Dusty.FloatX + cosf(DegreesToRadians(Angle+Range))*100 + ScrollX, Dusty.FloatY + -sinf(DegreesToRadians(Angle+Range))*100 + ScrollY, gxRGB32(192, 192, 128), 0.5f);
        AddDebugLine(Dusty.FloatX + ScrollX, Dusty.FloatY + ScrollY, Dusty.FloatX + cosf(DegreesToRadians(Angle-Range))*100 + ScrollX, Dusty.FloatY + -sinf(DegreesToRadians(Angle-Range))*100 + ScrollY, gxRGB32(192, 192, 128), 0.5f);
#endif

        return true;
    }

    return false;
}

static bool UpdateDusty_CheckSwipeJumpRange(float MinAngle, float MaxAngle)
{
    float Spread = AngleDifference(MinAngle, MaxAngle)/2;
    return UpdateDusty_CheckSwipeJump(MinAngle + Spread, Spread);
}

static void UpdateDusty_DoSwipeJump(float Angle, float Power)
{
    float dX = cosf(DegreesToRadians(Angle)) * Power;
    float dY = -sinf(DegreesToRadians(Angle)) * Power;
    
    //printf("Jump: Count=%d MaxSpeed=%f L=%f Power=%f\n", Swipe.ValidCount, MaxSpeed, L, Power);

    SetDustyState_JumpWithVelocity(dX, dY);
    
#ifdef SWIPE_DEBUG
    AddDebugLine(Dusty.FloatX + ScrollX, Dusty.FloatY + ScrollY, Dusty.FloatX + dX*10 + ScrollX, Dusty.FloatY + dY*10 + ScrollY, gxRGB32(192, 128, 128), 0.5f);
#endif
    
    GetInput_SetSwipeUsed();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  General purpose display                                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

static void DisplayDustySprite(EDustySprite Sprite, float XAdj = 0.0f, float XMirrorAdj = 0.0f, float YAdj = 0.0f)
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}

    float X = Dusty.FloatX + OffsetX + XAdj + XMirrorAdj*ScaleX;
    float Y = Dusty.FloatY + YAdj;

    Dusty.LastX = X;
    Dusty.LastY = Y;
    Dusty.LastScaleX = ScaleX;
    Dusty.LastSprite = Sprite;
    
    // Display trail.
    for (int i = 0; i < MAX_DUSTY_TRAIL; i++)
    {
        SDustyTrail* Trail = &DustyTrail[i];

        if (Trail->Sprite != DUSTYSPRITE_COUNT && Trail->Alpha > 0.1f)
        {
            unsigned int Color = gxRGBA32(0x40, 0x40, 0x40, (int)(255*Trail->Alpha));
	        AddLitSpriteScaledColor(LIGHTLIST_DUST, DustySprite[Trail->Sprite], Trail->X + ScrollX, Trail->Y + ScrollY, Trail->ScaleX, 1.0f, Color);
        }
    }

    // Add to trail.
    if (Dusty.PowerUpTimer > 0)
    {
        SDustyTrail* Trail = &DustyTrail[0];
        Trail->X = X;
        Trail->Y = Y;
        Trail->ScaleX = ScaleX;
        Trail->Sprite = Sprite;
    }

	AddLitSpriteScaled(LIGHTLIST_FOREGROUND, DustySprite[Sprite], X + ScrollX, Y + ScrollY, ScaleX, 1.0f);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_STAND Implementation                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand()
{
	Dusty.LastWall = DIRECTION_NONE;

	if (Dusty.CollideMaterial != MATERIAL_ICE)
		Dusty.FloatVelocityX = 0;

	Dusty.FloatVelocityY = 0;

	Dusty.SpriteTransition = 0;	

    Dusty.AirJumpCount = 0;
    Dusty.LandTimer = 0;
    Dusty.ComboCount = 0;

	Dusty.State = DUSTYSTATE_STAND;
    
    GetInput_ClearSwipe();
}

static void DisplayDusty_Stand()
{
	if (Dusty.CollideMaterial == MATERIAL_ICE && fabsf(Dusty.FloatVelocityX) > 1)
	{
		EDustySprite SlideSprites[] =
		{
			DUSTYSPRITE_SLIDE_1,
			DUSTYSPRITE_SLIDE_2,
			DUSTYSPRITE_SLIDE_3
		};

		int Index = (Dusty.SpriteTransition/5) % 3;
		DisplayDustySprite(SlideSprites[Index], -128, 5, -221);
	}
	else
	{
		if (Dusty.SpriteTransition <= 5)
			DisplayDustySprite(DUSTYSPRITE_HOP_5B, -119, 18, -218);
		else
        if (Dusty.SpriteTransition <= 15 && Dusty.State != DUSTYSTATE_INTROSTAND)
            DisplayDustySprite(DUSTYSPRITE_HOP_5B, -119, 18, -218);
        else
		{
			if (Dusty.SpriteTransition % 45 < 10)
				DisplayDustySprite(DUSTYSPRITE_IDLE_1, -124, 5, -221);
			else if (Dusty.SpriteTransition % 45 < 40)
				DisplayDustySprite(DUSTYSPRITE_IDLE_2, -124, 5, -221);
			else
				DisplayDustySprite(DUSTYSPRITE_IDLE_3, -124, 5, -221);
		}
	}
}

static void UpdateDusty_Stand()
{
	// Check for hitting something sticky.
	if (Dusty.CollideMaterial == MATERIAL_STICKY)
	{
		SetDustyState_Stuck();
		return;
	}
    
//    Dusty.LandTimer++;
//    if (Dusty.LandTimer <= 5)
//        return;

	if (Dusty.CollideMaterial == MATERIAL_ICE)
	{
		Dusty.FloatX += Dusty.FloatVelocityX;
		Dusty.FloatVelocityX *= 0.9f;
	}
    
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if ( GetInput_Jump() )      
        {
            SetDustyState_Jump( false );
            return;
        }

        if ( GetInput_MoveRight() )
        {
            SetDustyState_Hop(DIRECTION_RIGHT);
            return;
        }

        if ( GetInput_MoveLeft() )
        {
            SetDustyState_Hop(DIRECTION_LEFT);
            return;
        }
    }
    else
    {
        // Angle up and away.  
        if (UpdateDusty_CheckSwipeJump(130.0f, 40.0f))
        {
            // If totally clear, use as is.
            if (UpdateDusty_NearbyBlocksAreClear(NEARBY_CENTER_LEFT | NEARBY_UP_LEFT | NEARBY_UP_CENTER))
            {
                UpdateDusty_DoSwipeJump(Dusty.SwipeAngle, Dusty.SwipePower);
                return;
            }
            // If blocked to the left, go straight up.  Consider a delayed air boost?
            else if (UpdateDusty_NearbyBlocksAreClear(NEARBY_UP_CENTER))
            {
                UpdateDusty_DoSwipeJump(90.0f, Dusty.SwipePower);
                return;
            }
        }
        if (UpdateDusty_CheckSwipeJump(50.0f, 40.0f))
        {
            // If totally clear, use as is.
            if (UpdateDusty_NearbyBlocksAreClear(NEARBY_CENTER_RIGHT | NEARBY_UP_RIGHT | NEARBY_UP_CENTER))
            {
                UpdateDusty_DoSwipeJump(Dusty.SwipeAngle, Dusty.SwipePower);
                return;
            }
            // If blocked to the right, go straight up.  Consider a delayed air boost?
            else if (UpdateDusty_NearbyBlocksAreClear(NEARBY_UP_CENTER))
            {
                UpdateDusty_DoSwipeJump(90.0f, Dusty.SwipePower);
                return;
            }
        }
        
        // Lower range - 10 degrees up, 30 degrees down from horizontal
        // Use a short hop and clamp angle.
        if (UpdateDusty_CheckSwipeJump(190.0f, 20.0f) && UpdateDusty_NearbyBlocksAreClear(NEARBY_CENTER_LEFT))
        {
            UpdateDusty_DoSwipeJump(160.0f, Dusty.SwipePower*0.75f);
            return;
        }
        if (UpdateDusty_CheckSwipeJump(350.0f, 20.0f) && UpdateDusty_NearbyBlocksAreClear(NEARBY_CENTER_RIGHT))
        {
            UpdateDusty_DoSwipeJump(20.0f, Dusty.SwipePower*0.75f);
            return;
        }
        // Bottom range - from 30 degrees down to straight down
        // Use a shorter hop and clamp angle.
        if (UpdateDusty_CheckSwipeJump(240, 30.0f) && UpdateDusty_NearbyBlocksAreClear(NEARBY_CENTER_LEFT))
        {
            UpdateDusty_DoSwipeJump(150.0f, Dusty.SwipePower*0.5f);
            return;
        }
        if (UpdateDusty_CheckSwipeJump(300.0f, 30.0f) && UpdateDusty_NearbyBlocksAreClear(NEARBY_CENTER_RIGHT))
        {
            UpdateDusty_DoSwipeJump(30.0f, Dusty.SwipePower*0.5f);
            return;
        }
    }
    
	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}

	// If Dusty comes to a stand atop a corner, use the corner jump state to prevent him from floating
	// in the air.
#if 0
	if (Dusty.CollideWithBottomLeftCorner)
	{
		Dusty.Direction = DIRECTION_LEFT;
		SetDustyState_CornerJump();
		return;
	}

	if (Dusty.CollideWithBottomRightCorner)
	{
		Dusty.Direction = DIRECTION_RIGHT;
		SetDustyState_CornerJump();
		return;
	}
#endif
    
	Dusty.SpriteTransition += 1;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_JUMP Implementation                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Jump( bool OffWall )
{
	if ( OffWall )
	{
		sxPlaySound( &DustyWallJumpSound );
	}
	else
	{
		//sxPlaySound( &DustyJumps );
	}

    if (Dusty.PowerUpTimer > 0)
	{
//		Dusty.FloatY -= 60.0f;
		Dusty.FloatVelocityX = 12.0f;
		Dusty.FloatVelocityY = -24.0f;
	}
    else
    {
//    	Dusty.FloatY -= 40.0f;
	    Dusty.FloatVelocityX = 6.0f;
	    Dusty.FloatVelocityY = -16.0f;
    }

    if (Dusty.Direction == DIRECTION_LEFT)
        Dusty.FloatVelocityX = -Dusty.FloatVelocityX;

    Dusty.LandTimer = 0;

    Dusty.HasJumped = true;
    
	Dusty.State = DUSTYSTATE_JUMP;
}

void SetDustyState_JumpWithVelocity( float VX, float VY )
{
	Dusty.FloatY -= 5.0f;
    
    if (Dusty.ComboCount >= 3)
    {
        VX *= 1.25f;
        VY *= 1.25f;
    }
    else if (Dusty.ComboCount >= 1)
    {
        VX *= 1.10f;
        VY *= 1.10f;
    }
    Dusty.ComboCount++;
    
    if (Dusty.PowerUpTimer > 0)
    {
	    VX *= 1.25f;
	    VY *= 1.25f;
    }
    
    Dusty.FloatVelocityX = VX;
    Dusty.FloatVelocityY = VY;

    if (Dusty.FloatVelocityX < 0)
        Dusty.Direction = DIRECTION_LEFT;
    else
        Dusty.Direction = DIRECTION_RIGHT;
        
    Dusty.LandTimer = 0;

    UpdateDusty_MakeDust(0, -50, 10 * (Dusty.ComboCount+1));    

    Dusty.HasJumped = true;
    
	Dusty.State = DUSTYSTATE_JUMP;
}

// This is a specialized way to get into the Jump state, without the initial velocity boost.
void SetDustyState_Fall()
{
    Dusty.HasJumped = true;
    Dusty.ComboCount = 0;

	Dusty.State = DUSTYSTATE_JUMP;
    
    Dusty.JumpGraceTimer = 20;
}

static void DisplayDusty_Jump()
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

	if (Dusty.FloatVelocityY >= 5)
		DisplayDustySprite(Hop4Sprites[(Dusty.SpriteTransition/5) % 3], -124, -18, -150);
	else if (Dusty.FloatVelocityY <= -5)
		DisplayDustySprite(Hop2Sprites[(Dusty.SpriteTransition/5) % 3], -124, -18, -150);
	else
		DisplayDustySprite(Hop3Sprites[(Dusty.SpriteTransition/10) % 2], -124, -18, -150);
}

static void UpdateDusty_Jump()
{       
    // Grace period jump.  Allows a midair jump within a few frames of falling.
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (Dusty.JumpGraceTimer > 0)
        {
            Dusty.JumpGraceTimer--;
            
            if (GetInput_Jump())
            {
                Dusty.JumpGraceTimer = 0;
                
                SetDustyState_Jump( false );
                return;
            }
        }
    }
    
	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOP Implementation                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Hop(EDustyDirection Direction)
{
	Dusty.Direction = Direction;

	Dusty.SpriteTransition = 0;

	Dusty.FloatVelocityY = 0;

	Dusty.State = DUSTYSTATE_HOP;
}

static void DisplayDusty_Hop()
{
	switch (Dusty.SpriteTransition)
	{
	case 0:    DisplayDustySprite(DUSTYSPRITE_HOP_2, -122,  0, -201); break;
	case 1:    DisplayDustySprite(DUSTYSPRITE_HOP_2, -122,  0, -212); break;
	case 2:    DisplayDustySprite(DUSTYSPRITE_HOP_2, -122,  0, -218); break;
	case 3:    DisplayDustySprite(DUSTYSPRITE_HOP_2, -122,  0, -224); break;
	case 4:    DisplayDustySprite(DUSTYSPRITE_HOP_2, -122,  0, -228); break;
	case 5:    DisplayDustySprite(DUSTYSPRITE_HOP_2, -122,  0, -236); break;
	case 6:    DisplayDustySprite(DUSTYSPRITE_HOP_3, -122,  0, -226); break;
	case 7:    DisplayDustySprite(DUSTYSPRITE_HOP_3, -122,  0, -226); break;
	case 8:    DisplayDustySprite(DUSTYSPRITE_HOP_3, -122,  0, -226); break;
	case 9:    DisplayDustySprite(DUSTYSPRITE_HOP_3, -122,  0, -226); break;
	case 10:   DisplayDustySprite(DUSTYSPRITE_HOP_3, -122,  0, -226); break;
	case 11:   DisplayDustySprite(DUSTYSPRITE_HOP_3, -122,  0, -226); break;
	case 12:   DisplayDustySprite(DUSTYSPRITE_HOP_4, -119,  0, -220); break;
	case 13:   DisplayDustySprite(DUSTYSPRITE_HOP_4, -119,  0, -215); break;
	case 14:   DisplayDustySprite(DUSTYSPRITE_HOP_4, -119,  0, -210); break;
	case 15:   DisplayDustySprite(DUSTYSPRITE_HOP_4, -119,  0, -205); break;
	case 16:   DisplayDustySprite(DUSTYSPRITE_HOP_4, -119,  0, -200); break;
	case 17:   DisplayDustySprite(DUSTYSPRITE_HOP_4, -119,  0, -195); break;
	case 18:   DisplayDustySprite(DUSTYSPRITE_HOP_5, -119, 18, -217); break;
	case 19:   DisplayDustySprite(DUSTYSPRITE_HOP_5, -119, 18, -217); break;
	case 20:   DisplayDustySprite(DUSTYSPRITE_HOP_5, -119, 18, -217); break;
	case 21:   DisplayDustySprite(DUSTYSPRITE_HOP_5, -119, 18, -217); break;
	case 22:   DisplayDustySprite(DUSTYSPRITE_HOP_5, -119, 18, -217); break;
	case 23:   DisplayDustySprite(DUSTYSPRITE_HOP_5, -119, 18, -217); break;
	default:   break;
	}
}

static void UpdateDusty_Hop()
{
	// Check for hitting something sticky.
	if (Dusty.CollideMaterial == MATERIAL_STICKY)
	{
		SetDustyState_Stuck();
		return;
	}

    // Set Velocity first.
    float VelocitySign = Dusty.Direction == DIRECTION_LEFT ? -1.0f : 1.0f;
    
	if (Dusty.CollideMaterial == MATERIAL_ICE)
		Dusty.FloatVelocityX = 6.0f * VelocitySign;
	else
		Dusty.FloatVelocityX = 8.25f * VelocitySign;
    
	// Jump immediately if requested.
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            SetDustyState_Jump( false );
        }
    }

	// Update animation
	Dusty.SpriteTransition += 1;

	// Check for end of hop animation.
	if (Dusty.SpriteTransition == 24)
	{
		// Spawn some dust motes.
		for (int i = 0; i < 6; i++)
			MakeDustMote(Dusty.FloatX, Dusty.FloatY);

        if (Settings.ControlStyle == CONTROL_TILT)
        {
            // If still holding right, reset animation and continue hopping.
            if ( ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveRight() ) || 
                 ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveLeft() ) )
            {
                Dusty.FloatX += Dusty.FloatVelocityX*3;
                Dusty.SpriteTransition = 0;
            }
            else  // If not, revert to stand.
            {	
                                                 
                SetDustyState_Stand();
                return;
            }
        }
        else
        {
            SetDustyState_Stand();
            return;
        }        
	}

	// Fall if he walks off the current platform.
	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}
    
	// Change direction mid hop.
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveLeft())
        {
            Dusty.Direction = DIRECTION_LEFT;
            Dusty.FloatVelocityX = -Dusty.FloatVelocityX;
        }
        if (Dusty.Direction == DIRECTION_LEFT && GetInput_MoveRight())
        {
            Dusty.Direction = DIRECTION_RIGHT;
            Dusty.FloatVelocityX = -Dusty.FloatVelocityX;
        }
    }

	// If we have not switched states, move Dusty.
	if (Dusty.SpriteTransition == 18)
	{
		Dusty.FloatX += Dusty.FloatVelocityX*6;
	}
	else if (Dusty.SpriteTransition >= 0 && Dusty.SpriteTransition <= 17)
	{
		Dusty.FloatX += Dusty.FloatVelocityX;
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UpdateDusty_JumpCommon Implementation										           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void UpdateDusty_JumpCommon()
{  
	Dusty.FloatX += Dusty.FloatVelocityX;                                                   
    Dusty.FloatY += Dusty.FloatVelocityY;
    
    if ( Dusty.State == DUSTYSTATE_LAUNCH )
    {
        // Reduced gravity when launching, and no friction.
        Dusty.FloatVelocityY += Dusty.FloatGravity * 0.75f; 
    }
    else
    {
        Dusty.FloatVelocityY += Dusty.FloatGravity;        

        if ( Settings.ControlStyle == CONTROL_SWIPE )
        {
            Dusty.FloatVelocityX *= 0.98f;
            Dusty.FloatVelocityY *= 0.98f;
        }
    }
    
	if (Dusty.FloatVelocityY > 15.0f)
		Dusty.FloatVelocityY = 15.0f;
    
	Dusty.SpriteTransition++;

    Dusty.LandTimer++;

	// Air control.
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_MoveLeft())
        {
            if (Dusty.FloatVelocityX >= -6)
                Dusty.FloatVelocityX -= 1.0f;
        }
        if (GetInput_MoveRight())
        {
            if (Dusty.FloatVelocityX <= 6)
                Dusty.FloatVelocityX += 1.0f;
        }
    }
    else
    {
#if 0
        if (msAccelX < -0.10f)
        {
            if (Dusty.FloatVelocityX >= -6)
                Dusty.FloatVelocityX -= 1.0f;
        }
        if (msAccelX > 0.10f)
        {
            if (Dusty.FloatVelocityX <= 6)
                Dusty.FloatVelocityX += 1.0f;
        }
#elif 1
        // Swipe in mid air gives a small boost.
        if (UpdateDusty_CheckSwipeJump(0, 180.0f))
        {
            float dX = cosf(DegreesToRadians(Dusty.SwipeAngle)) * 5.0f;
            float dY = -sinf(DegreesToRadians(Dusty.SwipeAngle)) * 5.0f;

            Dusty.FloatVelocityX += dX;
            Dusty.FloatVelocityY += dY;
            
            GetInput_SetSwipeUsed();
        }
        // Allow for dragging him around mid-air.
        else if (GetInput_GetSwipeTimeLeft() >= 1.0f/20.0f)
        {
            float Current = GetInput_GetSwipeCurrent();
            
            float StartX, StartY;
            GetInput_GetSwipePosAtTime(&StartX, &StartY, Current - 1.0f/20.0f);
            
            float EndX, EndY;
            GetInput_GetSwipePosAtTime(&EndX, &EndY, Current);
            
            float dX = EndX - StartX;
            float dY = EndY - StartY;
            
            float L = sqrtf(dX*dX + dY*dY);
            if (L > 1.0f)
            {
                if (fabsf(Dusty.FloatVelocityX) < 10)
                    Dusty.FloatVelocityX += dX >= 0 ? 1.0f : -1.0f;
                if (fabsf(Dusty.FloatVelocityY) < 10)
                    Dusty.FloatVelocityY += dY >= 0 ? 0.5f : -0.5f;
            }
        }
#endif
    }

    if (Dusty.FloatVelocityX > 0)
        Dusty.Direction = DIRECTION_RIGHT;
    else if (Dusty.FloatVelocityX < 0)
        Dusty.Direction = DIRECTION_LEFT;

#if 0
	// Collision with corners is indicated by separate collision variables being set.
	if (Dusty.CollideWithBottomLeftCorner && Dusty.FloatVelocityY >= 0)
	{
		Dusty.Direction = DIRECTION_LEFT;
		SetDustyState_CornerJump();
		return;
	}
	if (Dusty.CollideWithBottomRightCorner && Dusty.FloatVelocityY >= 0)
	{
		Dusty.Direction = DIRECTION_RIGHT;
		SetDustyState_CornerJump();
		return;
	}
#endif
    
	// Collision with either side translates to a possible wall jump.
	// Dusty is not allowed to collide with the same side twice in a row, unless he stands once in between.
	// Wade: Currently this stuff is tweaked around as an experiment- he can only walljump again after a delay.
	Dusty.WallJumpTimer++;

    if (Dusty.LandTimer >= 1)
    {
        if (Dusty.CollideWithLeftSide && Dusty.FloatVelocityX <= 0 /*&& (Dusty.WallJumpTimer >= 30 || Dusty.LastWall != DIRECTION_LEFT)*/)
        {
            SetDustyState_WallJump();
            return;
        }

        if (Dusty.CollideWithRightSide && Dusty.FloatVelocityX >= 0 /*&& (Dusty.WallJumpTimer >= 30 || Dusty.LastWall != DIRECTION_RIGHT)*/)
        {
            SetDustyState_WallJump();
            return;
        }

        // When landing on something, revert to standing.
        if (Dusty.CollideWithBottomSide == true )
        {	
            // Spawn some dust motes.
            for (int i = 0; i < 3; i++)
                MakeDustMote(Dusty.FloatX, Dusty.FloatY);

            SetDustyState_Stand();
            return;
        } 
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_WallJump()
{   	
	if (Dusty.CollideMaterial == MATERIAL_ICE)
		Dusty.WallStickTimer = 0;
	else
		Dusty.WallStickTimer = 15;

	Dusty.FloatVelocityX = 0;

    if (Dusty.FloatVelocityY > 0)
        Dusty.FloatVelocityY = 0;

    if (Dusty.CollideMaterial == MATERIAL_ICE)
        Dusty.FloatVelocityY *= 0.75f;
    else
        Dusty.FloatVelocityY *= 0.3f;

	Dusty.LastWall = Dusty.Direction;

	Dusty.WallJumpTimer = 0;
    Dusty.AirJumpCount = 0;
    Dusty.LandTimer = 0;
    Dusty.SpriteTransition = 0;

	// Switch directions when entering a wall jump.
	if (Dusty.Direction == DIRECTION_RIGHT)
		Dusty.Direction = DIRECTION_LEFT;
	else
		Dusty.Direction = DIRECTION_RIGHT;

    UpdateDusty_MakeDust(0, -50, 10);    

	Dusty.State = DUSTYSTATE_WALLJUMP;
    
    GetInput_ClearSwipe();
}

static void DisplayDusty_WallJump()
{    
	EDustySprite WallJumpSprites[2] =
	{
		DUSTYSPRITE_WALLJUMP_B,
		DUSTYSPRITE_WALLJUMP_C,
	};
    
	if (Dusty.FloatVelocityY <= 3.5f)
        DisplayDustySprite(DUSTYSPRITE_WALLJUMP, -128, -8, -200);
	else
		DisplayDustySprite(WallJumpSprites[(Dusty.SpriteTransition/5) % 2], -128, -8, -200);
}

static void UpdateDusty_WallJump()
{
	// Check for hitting something sticky.
	if (Dusty.CollideMaterial == MATERIAL_STICKY)
	{
		SetDustyState_Stuck();
		return;
	}

    Dusty.SpriteTransition += 1;

//    Dusty.LandTimer++;
//    if (Dusty.LandTimer <= 5)
//        return;
    
//	if (Dusty.WallStickTimer > 0)
//		Dusty.WallStickTimer--;

//	if (Dusty.WallStickTimer <= 0)
	{
		// Sliding down the wall... If you hit the ground, switch to stand state.
		if (Dusty.CollideWithBottomSide == true)
		{			  
			SetDustyState_Stand();
			return;
		}
		// If contact with the wall is lost, switch to fall state.
		else if (Dusty.CollideWithRightSide == false && Dusty.CollideWithLeftSide == false)
		{
			SetDustyState_Fall();
			return;
		}
		else // Normal sliding.
		{
			Dusty.FloatY += Dusty.FloatVelocityY;

			if (Dusty.CollideMaterial == MATERIAL_ICE)
			{
				Dusty.FloatVelocityY += Dusty.FloatGravity;
                Dusty.FloatVelocityY *= 0.99f;

				if (Dusty.FloatVelocityY > 15.0f)
					Dusty.FloatVelocityY = 15.0f;
			}
			else
			{
				Dusty.FloatVelocityY += Dusty.FloatGravity/2;
                Dusty.FloatVelocityY *= 0.95f;

				if (Dusty.FloatVelocityY > 7.0f)
					Dusty.FloatVelocityY = 7.0f;
			}
		}
	}

	// Jump off wall by pressing jump
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            SetDustyState_Jump( true );
            return;
        }

        // Let go of wall by pressing direction.
        // TODO: GetInput_HardMoveLeft/Right.
        if ( ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveLeft() ) ||
             ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveRight() ) )
        {
            SetDustyState_Fall();
            return;
        }
    }
    else
    {
        if (Dusty.Direction == DIRECTION_LEFT)
        {
            if ( UpdateDusty_CheckSwipeJump(180.0f, 80.0f) )
            {
                UpdateDusty_DoSwipeJump(Dusty.SwipeAngle, Dusty.SwipePower);
                return;
            }
            if ( UpdateDusty_CheckSwipeJump(60.0f, 40.0f) )
            {
                UpdateDusty_DoSwipeJump(100.0f, Dusty.SwipePower);
                return;
            }
            if ( UpdateDusty_CheckSwipeJump(320.0f, 60.0f) )
            {
                UpdateDusty_DoSwipeJump(200.0f, Dusty.SwipePower*0.25f);
                return;
            }
        }
        else // DIRECTION_RIGHT
        {
            if ( UpdateDusty_CheckSwipeJump(0.0f, 80.0f) )
            {
                UpdateDusty_DoSwipeJump(Dusty.SwipeAngle, Dusty.SwipePower);
                return;
            }
            if ( UpdateDusty_CheckSwipeJump(120.0f, 40.0f) )
            {
                UpdateDusty_DoSwipeJump(80.0f, Dusty.SwipePower);
                return;
            }
            if ( UpdateDusty_CheckSwipeJump(240.0f, 60.0f) )
            {
                UpdateDusty_DoSwipeJump(340.0f, Dusty.SwipePower*0.25f);
                return;
            }
        }
    }
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_CORNERJUMP Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_CornerJump()
{   	
	Dusty.FloatVelocityX = 0;
	Dusty.FloatVelocityY = 0;

	Dusty.LastWall = Dusty.Direction;

	Dusty.WallJumpTimer = 0;
    Dusty.AirJumpCount = 0;
    Dusty.LandTimer = 0;
    Dusty.ComboCount = 0;

	Dusty.SpriteTransition = 0;

	// Switch directions when entering a corner jump.
	if (Dusty.Direction == DIRECTION_RIGHT)
		Dusty.Direction = DIRECTION_LEFT;
	else
		Dusty.Direction = DIRECTION_RIGHT;

	Dusty.State = DUSTYSTATE_CORNERJUMP;
    
    GetInput_ClearSwipe();
}

static void DisplayDusty_CornerJump()
{
    if (Dusty.SpriteTransition < 10)
    {
        DisplayDustySprite(DUSTYSPRITE_CORNERJUMP, -128, -32, -150);
    }
    else
    {
        if (Dusty.SpriteTransition % 45 < 10)
            DisplayDustySprite(DUSTYSPRITE_CORNERJUMP_B, -128, -32, -150);
        else if (Dusty.SpriteTransition % 45 < 40)
            DisplayDustySprite(DUSTYSPRITE_CORNERJUMP_C, -128, -32, -150);
        else
            DisplayDustySprite(DUSTYSPRITE_CORNERJUMP_D, -128, -32, -150);
    }

}

static void UpdateDusty_CornerJump()
{
    Dusty.SpriteTransition += 1;
    
    Dusty.LandTimer += 1;
	if (Dusty.LandTimer <= 5)
		return;
    
	// Jump off wall by pressing jump
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            SetDustyState_Jump( true );
            return;
        }

        // Let go of wall by pressing direction away from corner.
        if ( ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveLeft() ) ||
             ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveRight() ) )
        {
            SetDustyState_Fall();
            return;
        }

        // Get up and walk by pressing direction towards corner.
        if ( ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveRight() ) ||
             ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveLeft() ) )
        {
            EDustyDirection NewDirection;

            // Help him along to prevent going back into corner jump.
            if (Dusty.Direction == DIRECTION_LEFT)
            {
                NewDirection = DIRECTION_RIGHT;
                Dusty.FloatX += 48;
            }
            else
            {
                NewDirection = DIRECTION_LEFT;
                Dusty.FloatX -= 48;
            }

            SetDustyState_Hop(NewDirection);
            return;
        }
    }
    else
    {
        if ( Dusty.Direction == DIRECTION_LEFT )
        {            
            if ( UpdateDusty_CheckSwipeJump(135.0f, 135.0f) )
            {
                UpdateDusty_DoSwipeJump(Dusty.SwipeAngle, Dusty.SwipePower);
                return;
            }
        }
        else
        {
            if ( UpdateDusty_CheckSwipeJump(45.0f, 135.0f) )
            {
                UpdateDusty_DoSwipeJump(Dusty.SwipeAngle, Dusty.SwipePower);
                return;
            }
        }
    }
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_PREPARELAUNCH Implementation                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_PrepareLaunch()
{
    Dusty.LastWall = DIRECTION_NONE;
    Dusty.ComboCount = 0;

    Dusty.State = DUSTYSTATE_PREPARELAUNCH;
}

static void DisplayDusty_PrepareLaunch()
{
	DisplayDustySprite(DUSTYSPRITE_HOP_5, -119, 18, -170);
}

static void UpdateDusty_PrepareLaunch()
{
    GetInput_ConsumeAllSwipe();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_LAUNCH Implementation                                                       //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Launch(float VelocityX, float VelocityY)
{
	Dusty.FloatVelocityX = VelocityX;
	Dusty.FloatVelocityY = VelocityY;

	if (Dusty.FloatVelocityX >= 0)
		Dusty.Direction = DIRECTION_RIGHT;
	else
		Dusty.Direction = DIRECTION_LEFT;

	sxPlaySound( &DustyLaunchSound );    

    Dusty.ComboCount = 0;

    Dusty.State = DUSTYSTATE_LAUNCH;
}

static void DisplayDusty_Launch()
{
	DisplayDusty_Jump();
}

static void UpdateDusty_Launch()
{
	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_DIE Implementation                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Die()
{
	Dusty.FloatVelocityX = Dusty.FloatX > Vacuum.X ? -15.0f : 15.0f;
	Dusty.FloatVelocityY = -30;

	Dusty.SpriteTransition = 0;

	//sxPlaySound( &DieSound );    

	Dusty.NoCollision = true;

    Dusty.ComboCount = 0;

	Dusty.State = DUSTYSTATE_DIE;
}

static void DisplayDusty_Die()
{
	DisplayDustySprite(DUSTYSPRITE_DEATH, -125, 0, -181);
}

static void UpdateDusty_Die()
{
	Dusty.SpriteTransition += 1;

	Dusty.FloatX += Dusty.FloatVelocityX;
	Dusty.FloatY += Dusty.FloatVelocityY;

	Dusty.FloatVelocityY += Dusty.FloatGravity;

	float DirX, DirY;
	GetVacuumForce(Dusty.FloatX, Dusty.FloatY, &DirX, &DirY, (float)Dusty.SpriteTransition/400.0f, false);
	Dusty.FloatVelocityX += DirX;
	Dusty.FloatVelocityY += DirY;

	Dusty.FloatVelocityX *= 0.99f;
	Dusty.FloatVelocityY *= 0.99f;

	if (Dusty.SpriteTransition > 120 && Distance(Dusty.FloatX, Dusty.FloatY, Vacuum.X, Vacuum.Y) < 40.0f)
	{
		if (Dusty.Lives > 0) //Check before the Die Screen Transition
		{
            if (!Settings.InfiniteLives)
                Dusty.Lives -= 1;
            
            if (IsRecordingActive())
    			StopRecording(RESULT_RESTART_PAGE);
            
			SetGameState_Transition(GAMETRANSITION_RESTART_PAGE);

#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Died"]];
#endif
		}
		else
		{
            if (IsRecordingActive())
			    StopRecording(RESULT_DIE);
			
            SetGameState_Transition(GAMETRANSITION_DIE_SCREEN);

#ifdef PLATFORM_IPHONE
            [TestFlight passCheckpoint:[NSString stringWithFormat:@"Game over"]];
#endif
		}
		return;
	} 
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_STUCK Implementation                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stuck()
{
	Dusty.StuckTimer = 30;
	Dusty.StuckJumpCount = 0;

    Dusty.ComboCount = 0;

	Dusty.State = DUSTYSTATE_STUCK;
}

static void DisplayDusty_Stuck()
{
	// TODO: Get sprites for attempting to escape stickiness, and use them when StuckTimer >= 20 or so.

	if (Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide)
	{
		DisplayDustySprite(DUSTYSPRITE_WALLJUMP, -128, -8, -200);
	}
	else
	{
		DisplayDustySprite(DUSTYSPRITE_IDLE_3, -124, 5, -221);
	}
}

static void UpdateDusty_Stuck()
{
	if (Dusty.StuckTimer > 0)
	{
		Dusty.StuckTimer--;
		return;
	}

    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            Dusty.StuckJumpCount++;

            if (Dusty.StuckJumpCount == 2)
            {
                SetDustyState_Jump(false);
                return;
            }

            Dusty.StuckTimer = 30;
        }
    }
    else
    {
        // SWIPE TODO
    }
}

void SetDustyState_Hurt()
{   
    Dusty.SpriteTransition = 0;

	if (Dusty.CollideWithBottomSide)
	{
		Dusty.FloatVelocityY = -15;
		//Dusty.FloatY += -40;
		Dusty.CollideWithBottomSide = false;
	}

    Dusty.ComboCount = 0;

	Dusty.State = DUSTYSTATE_HURT;
}

static void DisplayDusty_Hurt()
{       
	// TODO: Need new sprite
	DisplayDusty_Die();
}

static void UpdateDusty_Hurt()
{
	Dusty.FloatX = Dusty.FloatX + Dusty.FloatVelocityX;                                                   
	Dusty.FloatY += Dusty.FloatVelocityY;

	Dusty.FloatVelocityY += Dusty.FloatGravity;

	if (Dusty.FloatVelocityY > 15.0f)
		Dusty.FloatVelocityY = 15.0f;

	// If Dusty lands or the timer expires, revert to stand state.
	Dusty.SpriteTransition += 1;
    if (Dusty.SpriteTransition >= 60 || Dusty.CollideWithBottomSide)
    {
		SetDustyState_Stand();
		return;
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOPIN Implementation                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_IntroHop(EDustyDirection Direction)
{
	Dusty.Direction = Direction;

	Dusty.SpriteTransition = 0;

	Dusty.FloatVelocityY = 0;

	Dusty.State = DUSTYSTATE_INTROHOP;
}

static void UpdateDusty_IntroHop()
{
	// Set Velocity first.
	float VelocitySign = Dusty.Direction == DIRECTION_LEFT ? -1.0f : 1.0f;

	Dusty.FloatVelocityX = 8.25f * VelocitySign;

	// Update animation
	Dusty.SpriteTransition += 1;

	// Check for end of hop animation.
	if (Dusty.SpriteTransition == 24)
	{
		// Spawn some dust motes.
		for (int i = 0; i < 6; i++)
			MakeDustMote(Dusty.FloatX, Dusty.FloatY);

		// If still holding right, reset animation and continue hopping.
		if ( ( Dusty.Direction == DIRECTION_RIGHT && RemoteControl.MoveRight ) || 
			 ( Dusty.Direction == DIRECTION_LEFT  && RemoteControl.MoveLeft ) )
		{
			Dusty.FloatX += Dusty.FloatVelocityX*3;
			Dusty.SpriteTransition = 0;
		}
		else  // If not, revert to stand.
		{	
			SetDustyState_IntroStand();
			return;
		}
	}

	// If we have not switched states, move Dusty.
	if (Dusty.SpriteTransition == 18)
	{
		Dusty.FloatX += Dusty.FloatVelocityX*6;
	}
	else if (Dusty.SpriteTransition >= 0 && Dusty.SpriteTransition <= 17)
	{
		Dusty.FloatX += Dusty.FloatVelocityX;
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_INTROSTAND Implementation                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_IntroStand()
{
	Dusty.FloatVelocityY = 0;

	Dusty.SpriteTransition = 0;	

	Dusty.State = DUSTYSTATE_INTROSTAND;
}

static void UpdateDusty_IntroStand()
{
	if (!RemoteControl.Enabled)
	{
		Dusty.State = DUSTYSTATE_STAND;
		return;
	}

	if ( RemoteControl.MoveRight )
	{
		SetDustyState_IntroHop(DIRECTION_RIGHT);
		return;
	}

	if ( RemoteControl.MoveLeft )
	{
		SetDustyState_IntroHop(DIRECTION_LEFT);
		return;
	}

	Dusty.SpriteTransition += 1;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UpdateDusty_Collision Implementation                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

static void UpdateDusty_Collision()
{
	// Initialize all collision variables to false.  One or more of these will be set to true in this function.
	// This function also corrects Dusty's position to not intersect anything.
	Dusty.CollideWithRightSide = false;
	Dusty.CollideWithLeftSide = false;
	Dusty.CollideWithBottomSide = false;
	Dusty.CollideWithTopSide = false;
	Dusty.CollideWithBottomLeftCorner = false;
	Dusty.CollideWithBottomRightCorner = false;
    
	UpdateStapler_Collision();

	Dusty.CollideMaterial = MATERIAL_NORMAL;

	// Collision with right side of the right screen and Dusty's right Side
	if (Dusty.FloatX + Dusty.Right >= (float)Chapter.PageWidth * 64)
	{
        Dusty.FloatVelocityX *= -0.25f;
		Dusty.FloatX = (float)Chapter.PageWidth * 64 - (float)Dusty.Right;	
	} 

	//Collision with the left side of the screen    
	if (Dusty.FloatX + Dusty.Left <= 0)
	{
        Dusty.FloatVelocityX *= -0.25f;
		Dusty.FloatX = -(float)Dusty.Left;
	}

	//Collision with the bottom side of the screen
	if (Dusty.FloatY + Dusty.Bottom >= Chapter.PageHeight * 64)
	{	
		Dusty.CollideWithBottomSide = true;
		Dusty.FloatY = (float)Chapter.PageHeight * 64 - (float)Dusty.Bottom;
	} 
    
	//Collision with just above the top side of the screen
	if (Dusty.FloatY + Dusty.Top <= -64 )
	{	
		Dusty.CollideWithTopSide = true;
		Dusty.FloatY = -64-(float)Dusty.Top;
	} 
    
    // Collision with the vacuum when it's off.
    if (Vacuum.State == VACUUMSTATE_RETREAT)
    {
        if (Dusty.FloatY + Dusty.Bottom >= Vacuum.Y)
        {
            Dusty.CollideWithBottomSide = true;
            Dusty.FloatY = Vacuum.Y - Dusty.Bottom;
        }
    }
    
	for (int y = 0; y < Chapter.PageHeight; y++)
	{
		for (int x = 0; x < Chapter.PageWidth; x++)
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

			// Check to see if Dusty's rectangle overlaps with this block.
			// 
			// The general idea here is to compare the smaller of the bottom sides (Min of Maxes) against the larger of the top sides (Max of Mins).  
			// If the min is less than the max, there is overlap.  The same calculation is also done in X.
			//
			// +-----------------+    +--------+   MnMx = Minimum of Maximum values (e.g. smaller of the two right edges)
			// |                 |    |        |   MxMn = Maximum of Minimum values (e.g. larger of the two left edges)
			// +-----------------+    +--------+        
			//              MnMx-^    ^-MxMn            In this diagram MxMn is greater than MnMx, so there is no overlap.
			//
			// The Y axis is considered first because the level is larger on the Y axis, so this will reject most blocks earlier (code optimization).
			if (Max(Dusty.FloatY + Dusty.Top, BlockTop) <= Min(Dusty.FloatY + Dusty.Bottom, BlockBottom))
			{
				// Check to see if Dusty's rectangle also overlaps with the block in X.
				if(Max(Dusty.FloatX + Dusty.Left, BlockLeft) <= Min(Dusty.FloatX + Dusty.Right, BlockRight))
				{    
					// {Left,Right,Top,Bottom}BlockIsEmpty are used to prevent collisions with internal edges (edges between blocks).  If Dusty jumps up into the bottom of a wide platform
					// and hits a lucky spot between two blocks, the collision code can determine that it would be easier to push him left or right instead of down.
					// This is bad and causes all kinds of potential collision problems.
					//
					// Helpful diagram:
					//
					// +---++---++---++---++---++---+ 
					// | A || B ||   ||   ||   ||   | 
					// +---++---++---++---++---++---+ 
					//      ^--Dusty hits right here.  
					//
					// In this example Dusty might collide with the right edge of block A OR the left edge of block B, which would be terrible since we really 
					// only want him to collide with the bottom sides of the blocks.
					//
					// The solution is to detect when blocks have other blocks next to them.  When they do, the internal edges between blocks are ignored for collision.
					//
					bool LeftBlockIsEmpty   = IsBlockEmpty(x-1, y);
					bool RightBlockIsEmpty  = IsBlockEmpty(x+1, y);
					bool TopBlockIsEmpty    = IsBlockEmpty(x, y-1);
					bool BottomBlockIsEmpty = IsBlockEmpty(x, y+1);

					// Calculate the distance Dusty would have to be pushed in each possible direction to get him out of intersecting the block.
					float LeftDistance	= (Dusty.FloatX + Dusty.Right)  - (BlockLeft);
					float RightDistance	= (BlockRight) - (Dusty.FloatX +  Dusty.Left);
					float DownDistance	= (BlockBottom)- (Dusty.FloatY +  Dusty.Top );
					float UpDistance	= (Dusty.FloatY + Dusty.Bottom) - (BlockTop );

					bool BlockCollideWithLeftSide = false;
					bool BlockCollideWithRightSide = false;
					bool BlockCollideWithTopSide = false;
					bool BlockCollideWithBottomSide = false;

#if 0
                    int CornerThreshold = 32;
#endif
                    
					// Prefer to collide with the side of the block that would push Dusty out the least distance.
					// (Only consider sides that are not adjacent to another solid block).
					if (LeftBlockIsEmpty && LeftDistance < RightDistance && LeftDistance < DownDistance && LeftDistance < UpDistance)
					{
						BlockCollideWithRightSide = true;
						Dusty.CollideWithRightSide = true;//Collision with Dusty's Right Side but the left side of the platform
						Dusty.FloatX -= LeftDistance;

#if 0
						if (TopBlockIsEmpty && abs(UpDistance - LeftDistance) < CornerThreshold)
						{
 							Dusty.CollideWithBottomRightCorner = true;
							Dusty.FloatY -= UpDistance;
							Dusty.FloatVelocityY = 0;
						}
#endif
                        
						if (Dusty.FloatVelocityX > 0)
							Dusty.FloatVelocityX = 0;
					}
					if (RightBlockIsEmpty && RightDistance < LeftDistance && RightDistance < DownDistance && RightDistance < UpDistance)
					{
						BlockCollideWithLeftSide = true;
						Dusty.CollideWithLeftSide = true;//Collision with Dusty's Left Side but the right side of the platform
						Dusty.FloatX += RightDistance;

#if 0
						if (TopBlockIsEmpty && abs(UpDistance - RightDistance) < CornerThreshold)
						{
							Dusty.CollideWithBottomLeftCorner = true;
							Dusty.FloatY -= UpDistance;
							Dusty.FloatVelocityY = 0;
						}
#endif
                        
						if (Dusty.FloatVelocityX < 0)
							Dusty.FloatVelocityX = 0;
					}
					if (BottomBlockIsEmpty && DownDistance < RightDistance && DownDistance < LeftDistance && DownDistance < UpDistance)
					{
						BlockCollideWithTopSide = true;
						Dusty.CollideWithTopSide = true;//Collision with Dusty's Top Side but the Bottom side of the platform
						Dusty.FloatY += DownDistance;
						if (Dusty.FloatVelocityY < 0)
							Dusty.FloatVelocityY = 0;
					}
					if (TopBlockIsEmpty && UpDistance < RightDistance && UpDistance < DownDistance && UpDistance < LeftDistance)
					{
						BlockCollideWithBottomSide = true;
						Dusty.CollideWithBottomSide = true;//Collision with Dusty's Bottom Side but the Top side of the platform
						Dusty.FloatY -= UpDistance;
						if (Dusty.FloatVelocityY > 0)
							Dusty.FloatVelocityY = 0;

#if 0
						// Top side corner jump is back in the game.
						if (LeftBlockIsEmpty && abs(LeftDistance - UpDistance) < CornerThreshold)
						{
							Dusty.CollideWithBottomRightCorner = true;
							Dusty.FloatX -= LeftDistance;
						}
						if (RightBlockIsEmpty && abs(RightDistance - UpDistance) < CornerThreshold)
						{
							Dusty.CollideWithBottomLeftCorner = true;
							Dusty.FloatX += RightDistance;
						}
#endif
					}

					int BlockID = GetBlockID(x, y);
					if (BlockID < SPECIALBLOCKID_FIRST)
					{			
						SBlock* Block = &Chapter.Blocks[BlockID];

#ifndef PLATFORM_IPHONE
						if (Dusty.CollideMaterial == MATERIAL_NORMAL)
						{
							Dusty.CollideMaterial = Block->Material;
						}
#endif

						if (Block->Type == BLOCKTYPE_NAIL && Dusty.State != DUSTYSTATE_HURT)
						{
							int SharpDir = ((SNailProperties*)Block->Properties)->Dir;

							bool Sharp = false;
							if (BlockCollideWithBottomSide && SharpDir ==   0) Sharp = true;
							if (BlockCollideWithLeftSide   && SharpDir ==  90) Sharp = true;
							if (BlockCollideWithTopSide    && SharpDir == 180) Sharp = true;
							if (BlockCollideWithRightSide  && SharpDir == 270) Sharp = true;

							if (Sharp)
							{
								SetDustyState_Hurt();
							}
						}      
					}
				}
			}
		}
	}
}

static void UpdateDusty_OnFire()
{
    if (Dusty.OnFireTimer > 0)
    {
        Dusty.OnFireTimer--;
        CreateSmallSmoke(Dusty.FloatX, Dusty.FloatY);
    }
}

static void UpdateDusty_PowerUp()
{
    if (Dusty.PowerUpTimer > 0)
    {
        Dusty.PowerUpTimer--;

        // Rotate trail.
        if ((Dusty.PowerUpTimer % 4) == 0)
        {
            for (int i = MAX_DUSTY_TRAIL-1; i > 0; i--)
                DustyTrail[i] = DustyTrail[i-1];
            DustyTrail[0].Sprite = DUSTYSPRITE_COUNT;
            DustyTrail[0].Alpha = 1.0f;
        }
    }

    // Fade out trail.
    for (int i = 0; i < MAX_DUSTY_TRAIL; i++)
        DustyTrail[i].Alpha -= 1.0f/MAX_DUSTY_TRAIL;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  Dusty Display & Update Implementation                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void DisplayDusty()
{
    if (!Dusty.Hidden)
    {    
        switch (Dusty.State)
        {
        case DUSTYSTATE_STAND:			    DisplayDusty_Stand(); break;
        case DUSTYSTATE_JUMP:			    DisplayDusty_Jump(); break;
        case DUSTYSTATE_HOP:			    DisplayDusty_Hop(); break;
        case DUSTYSTATE_WALLJUMP:			DisplayDusty_WallJump(); break;
        case DUSTYSTATE_CORNERJUMP:			DisplayDusty_CornerJump(); break;
        case DUSTYSTATE_PREPARELAUNCH:      DisplayDusty_PrepareLaunch(); break;
        case DUSTYSTATE_LAUNCH:             DisplayDusty_Launch(); break;	
        case DUSTYSTATE_DIE:				DisplayDusty_Die(); break;
        case DUSTYSTATE_STUCK:				DisplayDusty_Stuck(); break;
        case DUSTYSTATE_HURT:				DisplayDusty_Hurt(); break;
		case DUSTYSTATE_INTROHOP:			DisplayDusty_Hop(); break;
		case DUSTYSTATE_INTROSTAND:			DisplayDusty_Stand(); break;
        }
    }

#ifdef SWIPE_DEBUG
    DisplayDebugLine(Dusty.FloatX + Dusty.Left  + ScrollX, Dusty.FloatY + Dusty.Top    + ScrollY, Dusty.FloatX + Dusty.Right + ScrollX, Dusty.FloatY + Dusty.Top    + ScrollY, 2.0f, gxRGB32(255, 0, 0));
    DisplayDebugLine(Dusty.FloatX + Dusty.Left  + ScrollX, Dusty.FloatY + Dusty.Bottom + ScrollY, Dusty.FloatX + Dusty.Right + ScrollX, Dusty.FloatY + Dusty.Bottom + ScrollY, 2.0f, gxRGB32(255, 0, 0));
    DisplayDebugLine(Dusty.FloatX + Dusty.Left  + ScrollX, Dusty.FloatY + Dusty.Top    + ScrollY, Dusty.FloatX + Dusty.Left  + ScrollX, Dusty.FloatY + Dusty.Bottom + ScrollY, 2.0f, gxRGB32(255, 0, 0));
    DisplayDebugLine(Dusty.FloatX + Dusty.Right + ScrollX, Dusty.FloatY + Dusty.Top    + ScrollY, Dusty.FloatX + Dusty.Right + ScrollX, Dusty.FloatY + Dusty.Bottom + ScrollY, 2.0f, gxRGB32(255, 0, 0));

    DisplayDebugLine(Dusty.FloatX - 4 + ScrollX, Dusty.FloatY     + ScrollY, Dusty.FloatX + 4 + ScrollX, Dusty.FloatY     + ScrollY, 2.0f, gxRGB32(255, 255, 0));
    DisplayDebugLine(Dusty.FloatX     + ScrollX, Dusty.FloatY - 4 + ScrollY, Dusty.FloatX     + ScrollX, Dusty.FloatY + 4 + ScrollY, 2.0f, gxRGB32(255, 255, 0));
    
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            if (Dusty.NearbyBlocks & (1<<(j*3+i)))
                DisplayDebugLine((Dusty.BlockX-1+i)*64 + ScrollX,    (Dusty.BlockY-1+j)*64+32+ScrollY, 
                                 (Dusty.BlockX-1+i)*64+64 + ScrollX, (Dusty.BlockY-1+j)*64+32+ScrollY, 32.0f, gxRGBA32(255, 255, 128, 128));
        }
    }
    DisplayDebugLine(Dusty.BlockX*64 + ScrollX,    Dusty.BlockY*64+32+ScrollY, 
                     Dusty.BlockX*64+64 + ScrollX, Dusty.BlockY*64+32+ScrollY, 32.0f, gxRGBA32(255, 128, 128, 128));
#endif
}

void UpdateDusty()
{
	if (!Dusty.NoCollision)
	{
		UpdateDusty_Collision();
	}
    
    UpdateDusty_GetNearbyBlocks();
		
	if (Dusty.State != DUSTYSTATE_DIE && Distance(Dusty.FloatX, Dusty.FloatY, Chapter.EndX, Chapter.EndY) < 100)
	{
		AdvanceToNextPage();
		return;
	}

	switch (Dusty.State)
	{
	case DUSTYSTATE_STAND:			    UpdateDusty_Stand(); break;
	case DUSTYSTATE_JUMP:			    UpdateDusty_Jump(); break;
	case DUSTYSTATE_HOP:				UpdateDusty_Hop(); break;
	case DUSTYSTATE_WALLJUMP:			UpdateDusty_WallJump(); break;
	case DUSTYSTATE_CORNERJUMP:			UpdateDusty_CornerJump(); break;
	case DUSTYSTATE_PREPARELAUNCH:      UpdateDusty_PrepareLaunch(); break;
	case DUSTYSTATE_LAUNCH:             UpdateDusty_Launch(); break;
	case DUSTYSTATE_DIE:				UpdateDusty_Die(); break;
	case DUSTYSTATE_STUCK:				UpdateDusty_Stuck(); break;
	case DUSTYSTATE_HURT:				UpdateDusty_Hurt(); break;
	case DUSTYSTATE_INTROHOP:			UpdateDusty_IntroHop(); break;
	case DUSTYSTATE_INTROSTAND:			UpdateDusty_IntroStand(); break;
	}

    UpdateDusty_OnFire();
    UpdateDusty_PowerUp();
    
    if (IsGhostRecordingActive())
        AddGhostEvent(Dusty.LastX, Dusty.LastY, Dusty.LastScaleX, Dusty.LastSprite);
    
    if (GetInput_GetSwipeTimeLeft() >= 1.0f/20.0f)
        GetInput_ConsumeSwipe(1.0f/60.0f);
}
