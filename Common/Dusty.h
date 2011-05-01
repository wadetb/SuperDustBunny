﻿#ifndef DUSTY_H
#define DUSTY_H

enum EDirection
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
	DUSTYSTATE_CORNERJUMP,
	DUSTYSTATE_PREPARELAUNCH,
	DUSTYSTATE_LAUNCH,
	DUSTYSTATE_DIE,
	DUSTYSTATE_STUCK,
	DUSTYSTATE_HURT,
	DUSTYSTATE_STAPLERLAUNCH,
};

struct SDusty
{
	EDustyState State;

	int Left;
	int Right;
	int Top;
	int Bottom;

	EDirection Direction;

	float FloatX;
	float FloatY;
	float FloatGravity;
	float FloatVelocityX;
	float FloatVelocityY;
    
	int SpriteTransition;

	int WallStickTimer;
	EDirection LastWall;
	int WallJumpTimer;

	int StuckJumpCount;
	int StuckTimer;

    int JumpGraceTimer;
    int AirJumpCount;
    
    int LandTimer;
    
	int Lives;
    
    bool Hidden;

	bool GainLife;

	bool NoCollision;
	bool RemoteControl;

	bool CollideWithLeftSide;
	bool CollideWithRightSide;
	bool CollideWithTopSide;
	bool CollideWithBottomSide;

	bool CollideWithBottomLeftCorner;
	bool CollideWithBottomRightCorner;

	int CollideMaterial;
    
    float JumpPower;
};

extern SDusty Dusty;

void DisplayDusty();
void UpdateDusty();
void InitDusty();

void SetDustyPosition(float x, float y);

void SetDustyState_Hop_On();
void SetDustyState_PrepareLaunch();
void SetDustyState_Launch(float VelocityX, float VelocityY);
void SetDustyState_Die();
void SetDustyState_Stuck();
void SetDustyState_Stand();
void SetDustyState_Hop(EDirection Direction);
void SetDustyState_StaplerLaunch();

void DustyGainLife();

#endif