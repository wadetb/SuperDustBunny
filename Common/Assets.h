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

extern gxSprite AlphabetSprite;

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

extern gxSprite DustySlide1Sprite;
extern gxSprite DustySlide2Sprite;
extern gxSprite DustySlide3Sprite;

extern gxSprite DustyWallJumpSprite;
extern gxSprite DustyCornerJumpSprite;

extern gxSprite DustyDeathSprite;

extern gxSprite LogoSprite;

extern gxSprite NewUnlockSprite;

extern gxSprite BronzeMedalSprite;
extern gxSprite SilverMedalSprite;
extern gxSprite GoldMedalSprite;

extern gxSprite BackgroundCardboardSprite;
extern gxSprite BackgroundPaperSprite;
extern gxSprite BackgroundFridgeSprite;

extern gxSprite TileUnknownSprite;

extern gxSprite BarrelBackSprite;
extern gxSprite BarrelFrontSprite;
extern gxSprite BarrelNailSprite;

extern gxSprite FanSprite;

extern gxSprite CoinSpin1Sprite;
extern gxSprite CoinSpin2Sprite;
extern gxSprite CoinSpin3Sprite;
extern gxSprite CoinSpin4Sprite;
extern gxSprite CoinSpin5Sprite;
extern gxSprite CoinSpin6Sprite;

extern gxSprite FullScoreSprite;

extern gxSprite ButtonPauseSprite;
extern gxSprite ButtonPlaySprite;
extern gxSprite ButtonMuteSprite;
extern gxSprite ButtonUnmuteSprite;
extern gxSprite ButtonHomeSprite;
extern gxSprite ButtonFastForwardSprite;

extern gxSprite PortalSprite;

extern gxSprite TennisBallSpin1Sprite;
extern gxSprite TennisBallSpin2Sprite;
extern gxSprite TennisBallSpin3Sprite;
extern gxSprite TennisBallSpin4Sprite;

extern gxSprite GearSprite;

extern gxSprite Stapler1Sprite;
extern gxSprite Stapler2Sprite;
extern gxSprite Stapler3Sprite;

extern gxSprite Jello1Sprite;
extern gxSprite Jello2Sprite;
extern gxSprite Jello3Sprite;
extern gxSprite Jello4Sprite;
extern gxSprite Jello5Sprite;

extern gxSprite Clothespin1Sprite;
extern gxSprite Clothespin2Sprite;

extern gxSprite DustMoteSprite;

extern gxSprite FireWorkRocketSprite;

extern gxSprite Flame1Sprite;
extern gxSprite Flame2Sprite;

extern gxSprite Vacuum1Sprite;
extern gxSprite Vacuum2Sprite;
extern gxSprite Vacuum3Sprite;
extern gxSprite VacuumSmokeSprite;

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
extern gxSprite IconLockedSprite;

extern gxSprite ScreenLose1Sprite;
extern gxSprite ScreenLose2Sprite;

extern gxSprite ScreenLoseGhostSprite;
extern gxSprite ScreenLoseGrave1Sprite;
extern gxSprite ScreenLoseGrave2Sprite;

extern gxSprite ScreenWin1Sprite;
extern gxSprite ScreenWin2Sprite;

extern gxSprite TextFlickToJumpSprite;
extern gxSprite TextExtraLifeSprite;
extern gxSprite TextBonusSprite;

extern gxSprite BubbleMinus1SecSprite;
extern gxSprite Bubble1UpSprite;

extern gxSprite ChapterTitleSprite;
extern gxSprite ChapterTextSprite;
extern gxSprite ChapterNumbersSprite;
extern gxSprite ChapterCompleteSprite;
extern gxSprite ChapterTimeSprite;
extern gxSprite ChapterBonusSprite;
extern gxSprite ChapterNewSprite;
extern gxSprite ChapterNewRecordSprite;
extern gxSprite ChapterRecordSprite;
extern gxSprite ChapterTotalTimeSprite;

extern gxSprite TapToContinueSprite;

extern gxSprite PowerUpSprite;

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

void UpdateLiveAssetCache();
void LoadBundleAssetList();
void LoadAssets();

void LoadSpriteAsset(const char* FileName, gxSprite* Sprite);
void* LoadAssetFile(const char* FileName, void** Data, int* DataSize);

#endif