//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Tutorial.h"

STutorial Tutorial;

void InitTutorial()
{
	Tutorial.State = TUTORIALSTATE_NONE;

	Tutorial.CoinDisplayed = false;
	Tutorial.FireworkDisplayed = false;
	Tutorial.BallDisplayed = false;
	Tutorial.BarrelDisplayed = false;
	Tutorial.WallJumpDisplayed = false;
	Tutorial.InitialDisplayed = false;
	Tutorial.JumpDisplayed = false;
	Tutorial.GearDisplayed = false;

	Tutorial.JumpInhibit = false;
}

void SkipTutorials()
{
	Tutorial.CoinDisplayed = true;
	Tutorial.FireworkDisplayed = true;
	Tutorial.BallDisplayed = true;
	Tutorial.BarrelDisplayed = true;
	Tutorial.WallJumpDisplayed = true;
	Tutorial.InitialDisplayed = true;
	Tutorial.JumpDisplayed = true;
	Tutorial.GearDisplayed = true;
}

void DisplayTutorial()
{
	if (Tutorial.State == TUTORIALSTATE_INITIAL)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialInitialSprite, 380, 400, 1.0f, 0.0f);
	}

	if (Tutorial.State == TUTORIALSTATE_BALL)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialBallSprite, 380, 400, 1.0f, 0.0f);
	}

	if (Tutorial.State == TUTORIALSTATE_BARREL)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialBarrelSprite, 380, 400, 1.0f, 0.0f);
	}

	if (Tutorial.State == TUTORIALSTATE_COIN)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialCoinSprite, 380, 400, 1.0f, 0.0f);
	}

	if (Tutorial.State == TUTORIALSTATE_FIREWORK)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialFireWorkSprite, 380, 400, 1.0f, 0.0f);
	}

	if (Tutorial.State == TUTORIALSTATE_JUMP)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialJumpSprite, 380, 400, 1.0f, 0.0f);
	}

	if (Tutorial.State == TUTORIALSTATE_WALLJUMP)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialWallJumpSprite, 380, 400, 1.0f, 0.0f);
	}

	if (Tutorial.State == TUTORIALSTATE_GEAR)
	{
		AddLitSpriteCenteredScaledRotated(LIGHTLIST_FOREGROUND, &TutorialGearSprite, 380, 400, 1.0f, 0.0f);
	}

	AddLitSprite(LIGHTLIST_FOREGROUND, &CrumbStandSprite, 768-384, 1024-364);

#ifdef PLATFORM_WINDOWS
	gxDrawString(0, 1024-32, 32, gxRGB32(255, 255, 255), "Press SPACE to continue ");
#endif
#ifdef PLATFORM_IPHONE
	gxDrawString(0, 1024-32, 32, gxRGB32(255, 255, 255), "    Tap to continue     ");
#endif
}

void UpdateTutorial()
{   
	if (Tutorial.State == TUTORIALSTATE_INITIAL)
	{
		Tutorial.InitialDisplayed = true;
	}

	if (Tutorial.State == TUTORIALSTATE_BALL)
	{
		Tutorial.BallDisplayed = true;
	}

	if (Tutorial.State == TUTORIALSTATE_BARREL)
	{
		Tutorial.BarrelDisplayed = true;
	}

	if (Tutorial.State == TUTORIALSTATE_COIN)
	{
		Tutorial.CoinDisplayed = true;
	}

	if (Tutorial.State == TUTORIALSTATE_FIREWORK)
	{
		Tutorial.FireworkDisplayed = true;
	}

	if (Tutorial.State == TUTORIALSTATE_JUMP)
	{
		Tutorial.JumpDisplayed = true;
	}

	if (Tutorial.State == TUTORIALSTATE_WALLJUMP)
	{
		Tutorial.WallJumpDisplayed = true;
	}

	if (Tutorial.State == TUTORIALSTATE_GEAR)
	{
		Tutorial.GearDisplayed = true;
	}

	// Use the jump key to skip the tutorial.  Note that GetInput_Jump is not used here because
	// I need to wait for the key to be released and pressed again.  GetInput_Jump continues to
	// return true as long as the button is held down, which would be bad.
	bool JumpPressed = false;
#ifdef PLATFORM_WINDOWS
	JumpPressed = kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE);
#endif
#ifdef PLATFORM_IPHONE
	JumpPressed = msButton1 && !msOldButton1;
#endif
	if (JumpPressed)
	{
		// Prevent this from being read by the game as a jump command until the key is released.
		Tutorial.JumpInhibit = true;

		SetGameState_Playing();
		return;
	}
}

