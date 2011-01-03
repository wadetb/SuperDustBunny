//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef ASSETS_H
#define ASSETS_H

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Sprite Assets                                                                        //
//-----------------------------------------------------------------------------------------------------------------------------------------//

extern gxSprite WhiteSprite;

extern gxSprite WipeDiagonalSprite;

extern gxSprite LightFlashlightSprite;
extern gxSprite LightVacuumSprite;

extern gxSprite SparkSprite;
extern gxSprite FlareSprite;

extern gxSprite FireWorkWhiteSprite;

extern gxSprite DustyHop1Sprite;
extern gxSprite DustyHop2Sprite;
extern gxSprite DustyHop3Sprite;
extern gxSprite DustyHop4Sprite;
extern gxSprite DustyHop5Sprite;

extern gxSprite DustyHop2bSprite;
extern gxSprite DustyHop2cSprite;
extern gxSprite DustyHop4bSprite;
extern gxSprite DustyHop4cSprite;

extern gxSprite DustyIdle1Sprite;
extern gxSprite DustyIdle2Sprite;
extern gxSprite DustyIdle3Sprite;

extern gxSprite DustyWallJumpSprite;
extern gxSprite DustyCornerJumpSprite;

extern gxSprite DustyDeathSprite;

extern gxSprite BackgroundCardboardSprite;
extern gxSprite BackgroundPaperSprite;

extern gxSprite TileUnknownSprite;

extern gxSprite TileDelayDest;
extern gxSprite TileGreenDelayDest;
extern gxSprite TileYellowDelayDest;
extern gxSprite TileRedDelayDest;

extern gxSprite BarrelBackSprite;
extern gxSprite BarrelFrontSprite;
extern gxSprite BarrelNailSprite;

extern gxSprite FanSprite;

extern gxSprite CrumbStandSprite;

extern gxSprite CoinSpin1Sprite;
extern gxSprite CoinSpin2Sprite;
extern gxSprite CoinSpin3Sprite;
extern gxSprite CoinSpin4Sprite;
extern gxSprite CoinSpin5Sprite;
extern gxSprite CoinSpin6Sprite;
extern gxSprite CoinSpin7Sprite;
extern gxSprite CoinSpin8Sprite;
extern gxSprite CoinSpin9Sprite;
extern gxSprite CoinSpin10Sprite;
extern gxSprite CoinSpin11Sprite;
extern gxSprite CoinSpin12Sprite;

extern gxSprite CoinLife1Sprite;
extern gxSprite CoinLife2Sprite;
extern gxSprite CoinLife3Sprite;
extern gxSprite CoinLife4Sprite;
extern gxSprite CoinLife5Sprite;
extern gxSprite CoinLife6Sprite;

extern gxSprite Pause1Sprite;
extern gxSprite Pause2Sprite;

extern gxSprite TennisBallSpin1Sprite;
extern gxSprite TennisBallSpin2Sprite;
extern gxSprite TennisBallSpin3Sprite;
extern gxSprite TennisBallSpin4Sprite;

extern gxSprite GearSprite;

extern gxSprite DustMoteSprite;
extern gxSprite DustArrowSprite;

extern gxSprite FireWorkRocketSprite;
extern gxSprite FireWorkBangSprite;

extern gxSprite VacuumBackSprite;
extern gxSprite VacuumFrontSprite;

extern gxSprite ScreenStart1Sprite;
extern gxSprite ScreenStart2Sprite;
extern gxSprite ScreenHelp1Sprite;
extern gxSprite ScreenHelp2Sprite;
extern gxSprite ScreenCredits1Sprite;
extern gxSprite ScreenCredits2Sprite;

extern gxSprite IconStart1Sprite;
extern gxSprite IconStart2Sprite;
extern gxSprite IconHelp1Sprite;
extern gxSprite IconHelp2Sprite;
extern gxSprite IconCredits1Sprite;
extern gxSprite IconCredits2Sprite;

extern gxSprite ScreenLose1Sprite;
extern gxSprite ScreenLose2Sprite;

extern gxSprite ScreenLoseGhostSprite;
extern gxSprite ScreenLoseGrave1Sprite;
extern gxSprite ScreenLoseGrave2Sprite;

extern gxSprite ScreenWin1Sprite;
extern gxSprite ScreenWin2Sprite;

extern gxSprite TutorialInitialSprite;
extern gxSprite TutorialBarrelSprite;
extern gxSprite TutorialCoinSprite;
extern gxSprite TutorialFireWorkSprite;
extern gxSprite TutorialGearSprite;
extern gxSprite TutorialBallSprite;
extern gxSprite TutorialJumpSprite;
extern gxSprite TutorialWallJumpSprite;

extern gxSprite ChapterTitle;

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Sound Assets                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

extern sxSound DustyToJumpSound;
extern sxSound DustyJumpSound;
extern sxSound DustyWallJumpSound;
extern sxSound DustyLaunchSound;
extern sxSound DustyWinSound;

extern sxSound Song1Sound;
extern sxSound Song2Sound;
extern sxSound Song3Sound;

extern sxSound VacuumClogSound;
extern sxSound VacuumClangSound;
extern sxSound VacuumTurnOnSound;
extern sxSound VacuumTurnOffSound;
extern sxSound VacuumJamSound;
extern sxSound VacuumOnSound;

extern sxSound BlockBreakSound;
extern sxSound JelloSound;
extern sxSound CoinVacuumedUpSound;
extern sxSound GearGrindSound;
extern sxSound TennisBallVacuumedUpSound;

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Asset Function Declarations                                                          //
//-----------------------------------------------------------------------------------------------------------------------------------------//

void LoadAssets();

#endif
