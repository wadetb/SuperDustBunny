//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef TUTORIAL_H
#define TUTORIAL_H

enum ETutorialState
{
	TUTORIALSTATE_NONE,
	TUTORIALSTATE_INITIAL,
	TUTORIALSTATE_COIN,
	TUTORIALSTATE_BALL,
	TUTORIALSTATE_FIREWORK,
	TUTORIALSTATE_BARREL,
	TUTORIALSTATE_WALLJUMP,
	TUTORIALSTATE_JUMP,
	TUTORIALSTATE_GEAR,
};

struct STutorial
{
	ETutorialState State;

	bool CoinDisplayed;
	bool FireworkDisplayed;
	bool BallDisplayed;
	bool BarrelDisplayed;
	bool WallJumpDisplayed;
	bool InitialDisplayed;
	bool JumpDisplayed;
	bool GearDisplayed;

	// Causes the game to ignore the jump key until it's released.
	bool JumpInhibit;
};

extern STutorial Tutorial;

void InitTutorial();
void SkipTutorials();
void UpdateTutorial();
void DisplayTutorial();

#endif
