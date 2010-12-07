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

gxSprite WhiteSprite;

gxSprite WipeDiagonalSprite;

gxSprite LightFlashlightSprite;
gxSprite LightVacuumSprite;

gxSprite SparkSprite;
gxSprite FlareSprite;

gxSprite FireWorkWhiteSprite;

gxSprite DustyHop1Sprite;
gxSprite DustyHop2Sprite;
gxSprite DustyHop3Sprite;
gxSprite DustyHop4Sprite;
gxSprite DustyHop5Sprite;

gxSprite DustyHop2bSprite;
gxSprite DustyHop2cSprite;
gxSprite DustyHop4bSprite;
gxSprite DustyHop4cSprite;

gxSprite DustyIdle1Sprite;
gxSprite DustyIdle2Sprite;
gxSprite DustyIdle3Sprite;

gxSprite DustyWallJumpSprite;
gxSprite DustyCornerJumpSprite;

gxSprite DustyDeathSprite;

gxSprite BackgroundCardboardSprite;
gxSprite BackgroundPaperSprite;

gxSprite TileUnknownSprite;

gxSprite TileDelayDest;
gxSprite TileGreenDelayDest;
gxSprite TileYellowDelayDest;
gxSprite TileRedDelayDest;

gxSprite BarrelBackSprite;
gxSprite BarrelFrontSprite;
gxSprite BarrelNailSprite;

gxSprite FanSprite;

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
gxSprite ScreenHelp1Sprite;
gxSprite ScreenHelp2Sprite;
gxSprite ScreenCredits1Sprite;
gxSprite ScreenCredits2Sprite;

gxSprite IconStart1Sprite;
gxSprite IconStart2Sprite;
gxSprite IconHelp1Sprite;
gxSprite IconHelp2Sprite;
gxSprite IconCredits1Sprite;
gxSprite IconCredits2Sprite;

gxSprite ScreenLose1Sprite;
gxSprite ScreenLose2Sprite;

gxSprite ScreenLoseGhostSprite;
gxSprite ScreenLoseGrave1Sprite;
gxSprite ScreenLoseGrave2Sprite;

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

gxSprite ChapterTitle;

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
sxSound VacuumTurnOnSound;
sxSound VacuumTurnOffSound;
sxSound VacuumJamSound;
sxSound VacuumOnSound;

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

	gxLoadSprite("Data/white.png", &WhiteSprite);

	gxLoadSprite("Data/wipe-diagonal.png", &WipeDiagonalSprite);
	
	gxLoadSprite("Data/light-flashlight.png", &LightFlashlightSprite);
	gxLoadSprite("Data/light-vacuum.png", &LightVacuumSprite);

	gxLoadSprite("Data/spark.png", &SparkSprite);
	gxLoadSprite("Data/flare.png", &FlareSprite);

	gxLoadSprite("Data/firework-white.png", &FireWorkWhiteSprite);

	gxLoadSprite("Data/dusty-hop-1.png", &DustyHop1Sprite);
	gxLoadSprite("Data/dusty-hop-2.png", &DustyHop2Sprite);
	gxLoadSprite("Data/dusty-hop-3.png", &DustyHop3Sprite);
	gxLoadSprite("Data/dusty-hop-4.png", &DustyHop4Sprite);
	gxLoadSprite("Data/dusty-hop-5.png", &DustyHop5Sprite);

	gxLoadSprite("Data/dusty-hop-2b.png", &DustyHop2bSprite);
	gxLoadSprite("Data/dusty-hop-2c.png", &DustyHop2cSprite);
	gxLoadSprite("Data/dusty-hop-4b.png", &DustyHop4bSprite);
	gxLoadSprite("Data/dusty-hop-4c.png", &DustyHop4cSprite);

	gxLoadSprite("Data/dusty-idle-1.png", &DustyIdle1Sprite);
	gxLoadSprite("Data/dusty-idle-2.png", &DustyIdle2Sprite);
	gxLoadSprite("Data/dusty-idle-3.png", &DustyIdle3Sprite);

	gxLoadSprite("Data/dusty-walljump.png", &DustyWallJumpSprite);
	gxLoadSprite("Data/dusty-cornerjump.png", &DustyCornerJumpSprite);

	gxLoadSprite("Data/dusty-die.png", &DustyDeathSprite);

	gxLoadSprite("Data/background-cardboard.png", &BackgroundCardboardSprite);
	gxLoadSprite("Data/background-paper.png", &BackgroundPaperSprite);

	gxLoadSprite("Data/tile-wtf.png", &TileUnknownSprite);
	
	gxLoadSprite("Data/tile-platform.png", &TileDelayDest);
	gxLoadSprite("Data/tile-greenplatform.png", &TileGreenDelayDest);
	gxLoadSprite("Data/tile-yellowplatform.png", &TileYellowDelayDest);
	gxLoadSprite("Data/tile-redplatform.png", &TileRedDelayDest);

	gxLoadSprite("Data/barrel-back.png", &BarrelBackSprite);
	gxLoadSprite("Data/barrel-front.png", &BarrelFrontSprite);
	gxLoadSprite("Data/barrel-nail.png", &BarrelNailSprite);

	gxLoadSprite("Data/fan.png", &FanSprite);

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
    gxLoadSprite("Data/screen-help-1.png", &ScreenHelp1Sprite);
    gxLoadSprite("Data/screen-help-2.png", &ScreenHelp2Sprite);
    gxLoadSprite("Data/screen-credits-1.png", &ScreenCredits1Sprite);
    gxLoadSprite("Data/screen-credits-2.png", &ScreenCredits2Sprite);
    
	gxLoadSprite("Data/icon-start-1.png", &IconStart1Sprite);
	gxLoadSprite("Data/icon-start-2.png", &IconStart2Sprite);
	gxLoadSprite("Data/icon-help-1.png", &IconHelp1Sprite);
	gxLoadSprite("Data/icon-help-2.png", &IconHelp2Sprite);
	gxLoadSprite("Data/icon-credits-1.png", &IconCredits1Sprite);
	gxLoadSprite("Data/icon-credits-2.png", &IconCredits2Sprite);

	gxLoadSprite("Data/screen-lose-1.png", &ScreenLose1Sprite);
	gxLoadSprite("Data/screen-lose-2.png", &ScreenLose2Sprite);
	
	gxLoadSprite("Data/screen-lose-ghost.png", &ScreenLoseGhostSprite);
	gxLoadSprite("Data/screen-lose-grave-1.png", &ScreenLoseGrave1Sprite);
	gxLoadSprite("Data/screen-lose-grave-2.png", &ScreenLoseGrave2Sprite);
	
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
	
	gxLoadSprite("Data/chapter-title.png", &ChapterTitle);

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
	sxLoadSound("Data/vacuum-turnon.wav", &VacuumTurnOnSound);
	sxLoadSound("Data/vacuum-turnoff.wav", &VacuumTurnOffSound);
	sxLoadSound("Data/vacuum-jam.wav", &VacuumJamSound);
	sxLoadSound("Data/vacuum-on.wav", &VacuumOnSound);
	
	sxLoadSound("Data/BlockBreak.wav", &BlockBreakSound);
	sxLoadSound("Data/Jello.wav", &JelloSound);
	sxLoadSound("Data/CoinVacuumedUp.wav", &CoinVacuumedUpSound);
	sxLoadSound("Data/GearGrind.wav", &GearGrindSound);
	sxLoadSound("Data/TennisBallVacuumedUp.wav", &TennisBallVacuumedUpSound);
}

