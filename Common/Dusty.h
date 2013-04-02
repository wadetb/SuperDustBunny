//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef __DUSTY_H__
#define __DUSTY_H__

#include "Hat.h"

enum EDustyDirection
{
	DIRECTION_NONE,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

enum EDustyState
{
	DUSTYSTATE_STAND,
	DUSTYSTATE_JUMP,
	DUSTYSTATE_HOP,
	DUSTYSTATE_WALLJUMP,
	DUSTYSTATE_BALLOONRIDE,
	DUSTYSTATE_CORNERJUMP,
	DUSTYSTATE_PREPARELAUNCH,
	DUSTYSTATE_LAUNCH,
	DUSTYSTATE_DIE,
	DUSTYSTATE_STUCK,
	DUSTYSTATE_HURT,
	DUSTYSTATE_INTROHOP,
	DUSTYSTATE_INTROSTAND,
};

enum ENearbyBlock
{
    NEARBY_UP_LEFT        = 1<<0,
    NEARBY_UP_CENTER      = 1<<1,
    NEARBY_UP_RIGHT       = 1<<2,
    NEARBY_CENTER_LEFT    = 1<<3,
    NEARBY_CENTER_CENTER  = 1<<4,
    NEARBY_CENTER_RIGHT   = 1<<5,
    NEARBY_DOWN_LEFT      = 1<<6,
    NEARBY_DOWN_CENTER    = 1<<7,
    NEARBY_DOWN_RIGHT     = 1<<8
};

enum EDeathType
{
    DEATH_VACUUM,
    DEATH_GHOST
};

struct SDusty
{
	EDustyState State;

	int Left;
	int Right;
	int Top;
	int Bottom;

	EDustyDirection Direction;

	float FloatX;
	float FloatY;
	float FloatVelocityX;
	float FloatVelocityY;

    float PrevVelocityX;
    float PrevVelocityY;

    float LastX;
    float LastY;
    float LastScaleX;
    EDustySprite LastSprite;
    float LastXAdj;
    float LastXMirrorAdj;
    float LastYAdj;
    
	int SpriteTransition;

    EDustyHat Hat;
    
    EDeathType Death;
    
	int WallStickTimer;
	EDustyDirection LastWall;
	int WallJumpTimer;

    bool Stuck;
	int StuckJumpCount;
	int StuckTimer;

    int JumpGraceTimer;
    int AirJumpCount;
    
    int LandTimer;
    
    int HurtTimer;
    
    int ComboCount;
    
    bool Hidden;

    int OnFireTimer;
    int PowerUpTimer;

    bool HasJumped;
	bool GainLife;

	bool NoCollision;

	bool CollideWithLeftSide;
	bool CollideWithRightSide;
	bool CollideWithTopSide;
	bool CollideWithBottomSide;

	bool CollideWithBottomLeftCorner;
	bool CollideWithBottomRightCorner;

	int CollideMaterial;
    
    float SwipeAngle;
    float SwipePower;
    
    int BlockX, BlockY;
    float XInBlock, YInBlock;
    unsigned int NearbyBlocks;
};

extern SDusty Dusty;

extern gxSprite* DustySprite[DUSTYSPRITE_COUNT];

void DisplayDusty();
void UpdateDusty();
void InitDusty();

void SetDustyPosition(float x, float y, EDustyDirection direction);

void ResetDustyCollision();

void SetDustyState_PrepareLaunch();
void SetDustyState_JumpWithVelocity( float VX, float VY );
void SetDustyState_Launch(float VelocityX, float VelocityY);
void SetDustyState_BalloonRide();
void SetDustyState_Hurt();
void SetDustyState_Die(EDeathType Death);
void SetDustyState_Stuck();
void SetDustyState_Stand();
void SetDustyState_Hop(EDustyDirection Direction);
void SetDustyState_IntroHop(EDustyDirection Direction);
void SetDustyState_IntroStand();

#endif
