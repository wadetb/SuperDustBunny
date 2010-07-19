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
	DUSTYSTATE_WALLJUMP_LEFT		
};

struct SDusty
{

	EDustyState State;

	int X;
	int Y;
	int DirectionX;
	int DirectionY;
	int Left;
	int Right;
	int Top;
	int Bottom;

	int VerticalCounter;
	int JumpQueue;
	int JumpRightSprite;
	int JumpLeftSprite;
	int WallStickTimer;
	int FallRightSprite;
	int FallLeftSprite;
	int HopRightSprite;
	int HopLeftSprite;
	int LastDirectionSprite;
	int SpriteTransition;
	int LeftSide;
	int RightSide;
	int TopSide;
	int BottomSide;

	bool RightSideIsInPlatform;
	bool LeftSideIsInPlatform;
	bool TopSideIsInPlatform;
	bool BottomSideIsInPlatform;
	bool AreRecsIntersecting;
	bool IsJumping;
	bool CanWallJump;
	bool OnPlatform;
	bool StraightFallTrigger;
	bool CollideWithLeftSide;
	bool CollideWithRightSide;
	bool CollideWithTopSide;
	bool CollideWithBottomSide;

};

extern SDusty Dusty;
void DisplayDusty();
void UpdateDusty();
void InitDusty();

#endif