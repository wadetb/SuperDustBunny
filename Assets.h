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

extern gxSprite DustyHop1Sprite;
extern gxSprite DustyHop2Sprite;
extern gxSprite DustyHop3Sprite;
extern gxSprite DustyHop4Sprite;
extern gxSprite DustyHop5Sprite;

extern gxSprite DustyWallJumpSprite;

extern gxSprite DustyDeathSprite;

extern gxSprite BackgroundCardboardSprite;

extern gxSprite TileUnknownSprite;

extern gxSprite BarrelSprite;

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

extern gxSprite ScreenLose1Sprite;
extern gxSprite ScreenLose2Sprite;

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
extern sxSound VacuumRunSound;

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Asset Function Declarations                                                          //
//-----------------------------------------------------------------------------------------------------------------------------------------//

void LoadAssets();

#endif