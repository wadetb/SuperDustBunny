//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef DUSTY_H
#define DUSTY_H

enum EDustyState
{
	DUSTYSTATE_STAND				,
	DUSTYSTATE_JUMP					,
	DUSTYSTATE_FALL					,
	DUSTYSTATE_HOP_RIGHT			,
	DUSTYSTATE_HOP_LEFT				,
	DUSTYSTATE_WALLJUMP_RIGHT		,
	DUSTYSTATE_WALLJUMP_LEFT		,
	DUSTYSTATE_PREPARELAUNCH        ,
	DUSTYSTATE_LAUNCH               ,
	DUSTYSTATE_DIE                  ,
};

struct SDusty
{
	EDustyState State;

	int Left;
	int Right;
	int Top;
	int Bottom;

	float FloatX;
	float FloatY;
	float FloatGravity;
	float FloatVelocityX;
	float FloatVelocityY;

	int JumpQueue;

	int HopRightSprite;
	int HopLeftSprite;

	int LastDirectionSprite;

	int SpriteTransition;

	bool CanWallJump;
	int WallStickTimer;

	bool CollideWithLeftSide;
	bool CollideWithRightSide;
	bool CollideWithTopSide;
	bool CollideWithBottomSide;
};

extern SDusty Dusty;
void DisplayDusty();
void UpdateDusty();
void InitDusty();

void SetDustyState_PrepareLaunch();
void SetDustyState_Launch(float VelocityX, float VelocityY);
void SetDustyState_Die();

#endif