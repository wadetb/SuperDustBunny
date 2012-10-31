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

enum EDustySprite
{
    DUSTYSPRITE_HOP_1,
    DUSTYSPRITE_HOP_2,
    DUSTYSPRITE_HOP_3,
    DUSTYSPRITE_HOP_4,
    DUSTYSPRITE_HOP_5,
    
    DUSTYSPRITE_HOP_2B,
    DUSTYSPRITE_HOP_2C,

    DUSTYSPRITE_HOP_3B,
    
    DUSTYSPRITE_HOP_4B,
    DUSTYSPRITE_HOP_4C,
    
    DUSTYSPRITE_HOP_5B,
    
    DUSTYSPRITE_IDLE_1,
    DUSTYSPRITE_IDLE_2,
    DUSTYSPRITE_IDLE_3,
    
    DUSTYSPRITE_SLIDE_1,
    DUSTYSPRITE_SLIDE_2,
    DUSTYSPRITE_SLIDE_3,
    
    DUSTYSPRITE_WALLJUMP,
    DUSTYSPRITE_WALLJUMP_B,
    DUSTYSPRITE_WALLJUMP_C,
    
    DUSTYSPRITE_CORNERJUMP,
    DUSTYSPRITE_CORNERJUMP_B,
    DUSTYSPRITE_CORNERJUMP_C,
    DUSTYSPRITE_CORNERJUMP_D,
    
    DUSTYSPRITE_DEATH,

    DUSTYSPRITE_KICK,

    DUSTYSPRITE_COUNT
};

struct SDustyHatOffset
{
    float Angle;
    float X;
    float Y;
    float Scale;
};

enum EDustyHat
{
    DUSTYHAT_NONE,
    
    DUSTYHAT_APPLE,
    DUSTYHAT_BASEBALL_CAP,
    DUSTYHAT_BEE,
    DUSTYHAT_BOWTIE,
    DUSTYHAT_CLOWN_NOSE,
    DUSTYHAT_CROWN,
    DUSTYHAT_DISGUISE,
    DUSTYHAT_EARMUFFS,
    DUSTYHAT_EARPHONES,
    DUSTYHAT_EYEGLASSES,
    DUSTYHAT_EYEPATCH,
    DUSTYHAT_FLOWER,
    DUSTYHAT_FROG_CROWN,
    DUSTYHAT_GRADUATION,
    DUSTYHAT_GREEN_FEATHER,
    DUSTYHAT_JESTER,
    DUSTYHAT_KARATE,
    DUSTYHAT_MONOCLE,
    DUSTYHAT_NURSE,
    DUSTYHAT_PARTY,
    DUSTYHAT_PINK_BOW,
    DUSTYHAT_PINK_SHADES,
    DUSTYHAT_PINK_TIARA,
    DUSTYHAT_PIRATE,
    DUSTYHAT_PURPLE_FEATHER,
    DUSTYHAT_SNORKEL,
    DUSTYHAT_SUNGLASSES,
    DUSTYHAT_TOPHAT,
    DUSTYHAT_TUTU,
    DUSTYHAT_WITCH,
    DUSTYHAT_YELLOW_TOPHAT,
    
    DUSTYHAT_COUNT
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

extern SDustyHatOffset DustyHatOffsets[DUSTYSPRITE_COUNT];

extern SDusty Dusty;

extern gxSprite* DustySprite[DUSTYSPRITE_COUNT];
extern SDustyHatOffset DustyHatOffsets[DUSTYSPRITE_COUNT];
extern gxSprite* DustyHatSprites[DUSTYHAT_COUNT];

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
