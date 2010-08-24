//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Assets.h"

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Sprite Assets                                                                        //
//-----------------------------------------------------------------------------------------------------------------------------------------//

gxSprite DustyHop1Sprite;
gxSprite DustyHop2Sprite;
gxSprite DustyHop3Sprite;
gxSprite DustyHop4Sprite;
gxSprite DustyHop5Sprite;

gxSprite DustyWallJumpSprite;

gxSprite DustyDeathSprite;

gxSprite BackgroundCardboardSprite;

gxSprite TileUnknownSprite;

gxSprite BarrelBackSprite;
gxSprite BarrelFrontSprite;

gxSprite CrumbStandSprite;

gxSprite CoinSpin1Sprite;
gxSprite CoinSpin2Sprite;
gxSprite CoinSpin3Sprite;
gxSprite CoinSpin4Sprite;
gxSprite CoinSpin5Sprite;
gxSprite CoinSpin6Sprite;

gxSprite TennisBallSpin1Sprite;
gxSprite TennisBallSpin2Sprite;
gxSprite TennisBallSpin3Sprite;
gxSprite TennisBallSpin4Sprite;

gxSprite GearSprite;

gxSprite DustMoteSprite;
gxSprite DustArrowSprite;

gxSprite FireWorkRocketSprite;
gxSprite FireWorkBangSprite;

gxSprite VacuumBackSprite;
gxSprite VacuumFrontSprite;

gxSprite ScreenStart1Sprite;
gxSprite ScreenStart2Sprite;

gxSprite ScreenLose1Sprite;
gxSprite ScreenLose2Sprite;

gxSprite ScreenWin1Sprite;
gxSprite ScreenWin2Sprite;

gxSprite TutorialInitialSprite;
gxSprite TutorialBarrelSprite;
gxSprite TutorialCoinSprite;
gxSprite TutorialFireWorkSprite;
gxSprite TutorialGearSprite;
gxSprite TutorialBallSprite;
gxSprite TutorialJumpSprite;
gxSprite TutorialWallJumpSprite;

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Sound Assets                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

sxSound DustyToJumpSound;
sxSound DustyJumpSound;
sxSound DustyWallJumpSound;
sxSound DustyLaunchSound;
sxSound DustyWinSound;

sxSound Song1Sound;
sxSound Song2Sound;
sxSound Song3Sound;

sxSound VacuumClogSound;
sxSound VacuumClangSound;
sxSound VacuumRunSound;

sxSound BlockBreakSound;
sxSound JelloSound;
sxSound CoinVacuumedUpSound;
sxSound GearGrindSound;
sxSound TennisBallVacuumedUpSound;


//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Asset Function Definition                                                            //
//-----------------------------------------------------------------------------------------------------------------------------------------//

void LoadAssets()
{
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Sprite Assets                                                                        //
	//-----------------------------------------------------------------------------------------------------------------------------------------//

	gxLoadSprite("Data/dusty-hop-1.png", &DustyHop1Sprite);
	gxLoadSprite("Data/dusty-hop-2.png", &DustyHop2Sprite);
	gxLoadSprite("Data/dusty-hop-3.png", &DustyHop3Sprite);
	gxLoadSprite("Data/dusty-hop-4.png", &DustyHop4Sprite);
	gxLoadSprite("Data/dusty-hop-5.png", &DustyHop5Sprite);

	gxLoadSprite("Data/dusty-walljump.png", &DustyWallJumpSprite);

	gxLoadSprite("Data/dusty-die.png", &DustyDeathSprite);

	gxLoadSprite("Data/background-cardboard.png", &BackgroundCardboardSprite);

	gxLoadSprite("Data/tile-unknown.png", &TileUnknownSprite);

	gxLoadSprite("Data/barrel-back.png", &BarrelBackSprite);
	gxLoadSprite("Data/barrel-front.png", &BarrelFrontSprite);

	gxLoadSprite("Data/crumb-stand.png", &CrumbStandSprite);

	gxLoadSprite("Data/coin-spin-1.png", &CoinSpin1Sprite);
	gxLoadSprite("Data/coin-spin-2.png", &CoinSpin2Sprite);
	gxLoadSprite("Data/coin-spin-3.png", &CoinSpin3Sprite);
	gxLoadSprite("Data/coin-spin-4.png", &CoinSpin4Sprite);
	gxLoadSprite("Data/coin-spin-5.png", &CoinSpin5Sprite);
	gxLoadSprite("Data/coin-spin-6.png", &CoinSpin6Sprite);

	gxLoadSprite("Data/tennisball-spin-1.png", &TennisBallSpin1Sprite);
	gxLoadSprite("Data/tennisball-spin-2.png", &TennisBallSpin2Sprite);
	gxLoadSprite("Data/tennisball-spin-3.png", &TennisBallSpin3Sprite);
	gxLoadSprite("Data/tennisball-spin-4.png", &TennisBallSpin4Sprite);

	gxLoadSprite("Data/gear.png", &GearSprite);

	gxLoadSprite("Data/dust-mote.png", &DustMoteSprite);
	gxLoadSprite("Data/dust-arrow.png", &DustArrowSprite);

	gxLoadSprite("Data/firework-rocket.png", &FireWorkRocketSprite);
	gxLoadSprite("Data/firework-bang.png", &FireWorkBangSprite);

	gxLoadSprite("Data/vacuum-back.png", &VacuumBackSprite);
	gxLoadSprite("Data/vacuum-front.png", &VacuumFrontSprite);

	gxLoadSprite("Data/screen-start-1.png", &ScreenStart1Sprite);
	gxLoadSprite("Data/screen-start-2.png", &ScreenStart2Sprite);

	gxLoadSprite("Data/screen-lose-1.png", &ScreenLose1Sprite);
	gxLoadSprite("Data/screen-lose-2.png", &ScreenLose2Sprite);

	gxLoadSprite("Data/screen-win-1.png", &ScreenWin1Sprite);
	gxLoadSprite("Data/screen-win-2.png", &ScreenWin2Sprite);

	gxLoadSprite("Data/tutorial-initial.png", &TutorialInitialSprite);
	gxLoadSprite("Data/tutorial-barrel.png", &TutorialBarrelSprite);
	gxLoadSprite("Data/tutorial-coin.png", &TutorialCoinSprite);
	gxLoadSprite("Data/tutorial-firework.png", &TutorialFireWorkSprite);
	gxLoadSprite("Data/tutorial-gear.png", &TutorialGearSprite);
	gxLoadSprite("Data/tutorial-tennisball.png", &TutorialBallSprite);
	gxLoadSprite("Data/tutorial-jump.png", &TutorialJumpSprite);
	gxLoadSprite("Data/tutorial-walljump.png", &TutorialWallJumpSprite);

	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Sound Assets                                                                         //
	//-----------------------------------------------------------------------------------------------------------------------------------------//

	sxLoadSound("Data/dusty-to-jump.wav", &DustyToJumpSound);
	sxLoadSound("Data/dusty-jump.wav", &DustyJumpSound);
	sxLoadSound("Data/dusty-walljump.wav", &DustyWallJumpSound);
	sxLoadSound("Data/dusty-launch.wav", &DustyLaunchSound);
	sxLoadSound("Data/dusty-win.wav", &DustyWinSound);

	sxLoadSound("Data/song-1.wav", &Song1Sound);
	sxLoadSound("Data/song-2.wav", &Song2Sound);
	sxLoadSound("Data/song-3.wav", &Song3Sound);

	sxLoadSound("Data/vacuum-clog.wav", &VacuumClogSound);
	sxLoadSound("Data/vacuum-clang.wav", &VacuumClangSound);
	sxLoadSound("Data/vacuum-run.wav", &VacuumRunSound);
	
	sxLoadSound("Data/BlockBreak.wav", &BlockBreakSound);
	sxLoadSound("Data/Jello.wav", &JelloSound);
	sxLoadSound("Data/CoinVacuumedUp.wav", &CoinVacuumedUpSound);
	sxLoadSound("Data/GearGrind.wav", &GearGrindSound);
	sxLoadSound("Data/TennisBallVacuumedUp.wav", &TennisBallVacuumedUpSound);
}

