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

#ifdef PLATFORM_IPHONE
#include <zlib.h>
#endif

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

gxSprite DustySlide1Sprite;
gxSprite DustySlide2Sprite;
gxSprite DustySlide3Sprite;

gxSprite DustyWallJumpSprite;
gxSprite DustyCornerJumpSprite;

gxSprite DustyDeathSprite;

gxSprite LogoSprite;

gxSprite BackgroundCardboardSprite;
gxSprite BackgroundPaperSprite;
gxSprite BackgroundFridgeSprite;

gxSprite TileUnknownSprite;

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

gxSprite CoinLife1Sprite;
gxSprite CoinLife2Sprite;
gxSprite CoinLife3Sprite;
gxSprite CoinLife4Sprite;
gxSprite CoinLife5Sprite;
gxSprite CoinLife6Sprite;

gxSprite ScoreNumber0Sprite;
gxSprite ScoreNumber1Sprite;
gxSprite ScoreNumber2Sprite;
gxSprite ScoreNumber3Sprite;
gxSprite ScoreNumber4Sprite;
gxSprite ScoreNumber5Sprite;
gxSprite ScoreNumber6Sprite;
gxSprite ScoreNumber7Sprite;
gxSprite ScoreNumber8Sprite;
gxSprite ScoreNumber9Sprite;

gxSprite FullScoreSprite;

gxSprite ButtonPauseSprite;
gxSprite ButtonPlaySprite;
gxSprite ButtonMuteSprite;
gxSprite ButtonUnmuteSprite;
gxSprite ButtonHomeSprite;

gxSprite PortalSprite;

gxSprite TennisBallSpin1Sprite;
gxSprite TennisBallSpin2Sprite;
gxSprite TennisBallSpin3Sprite;
gxSprite TennisBallSpin4Sprite;

gxSprite GearSprite;

gxSprite StaplerExtendUpSprite;
gxSprite StaplerUpSprite;
gxSprite StaplerDownSprite;

gxSprite PowerJump1Sprite; 
gxSprite PowerJump2Sprite; 
gxSprite PowerJump3Sprite; 
gxSprite PowerJump4Sprite; 
gxSprite PowerJump5Sprite; 
gxSprite PowerJump6Sprite; 
gxSprite PowerJump7Sprite; 
gxSprite PowerJump8Sprite; 
gxSprite PowerJump9Sprite; 
gxSprite PowerJump10Sprite; 

gxSprite DustMoteSprite;
gxSprite DustArrowSprite;

gxSprite FireWorkRocketSprite;

gxSprite VacuumFrontSprite;
gxSprite Vacuum1Sprite;
gxSprite Vacuum2Sprite;
gxSprite Vacuum3Sprite;

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
//                                                    iOS Compressed Sprite Loading                                                        //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifdef PLATFORM_IPHONE

#define MAX_SPRITE_ASSETS 512


struct SSpriteAsset
{
    char* SourceFileName;
    char* RawFileName;
    int Width;
    int Height;
    int TexWidth;
    int TexHeight;
};


int NSpriteAssets = 0;
SSpriteAsset SpriteAssets[MAX_SPRITE_ASSETS];


void LoadAssetList(const char* FileName)
{
	PushErrorContext("While loading asset list '%s':\n", FileName);
    
	FILE* AssetFile = OpenAssetFile(FileName, "r");    
	if (!AssetFile)
        return;
    //ReportError("Unable to open asset list file.  Check that all required files and tools are present, and re-build the XCode project to fix.");
    
	// Read the entire XML file into a text buffer.
	fseek(AssetFile, 0, SEEK_END);
	int FileSize = ftell(AssetFile);
	rewind(AssetFile);
    
	char* XML = (char*)malloc(FileSize + 1);
	fread(XML, FileSize, 1, AssetFile);
	fclose(AssetFile);
	XML[FileSize] = '\0';
    
	// Parse the XML text buffer into a Document hierarchy.
	rapidxml::xml_document<> Document;
	Document.parse<0>(XML);

    // Get the root <assets> node.
	rapidxml::xml_node<char>* AssetsNode = Document.first_node("Assets");
	if (AssetsNode == NULL)
		ReportError("Missing <assets> node.  Re-building the XCode project may help.");
    
    // Iterate over all the <asset> nodes.
	rapidxml::xml_node<char>* AssetNode = AssetsNode->first_node("SpriteAsset");
	while (AssetNode)
	{
        if (NSpriteAssets >= MAX_SPRITE_ASSETS)
            ReportError("Exceeded the limit of %d sprite assets.", MAX_SPRITE_ASSETS);
        
        SSpriteAsset* SpriteAsset = &SpriteAssets[NSpriteAssets++];
        
        rapidxml::xml_attribute<char>* Name = AssetNode->first_attribute("name");
        if (!Name)
            ReportError("SpriteAsset is missing the Name property.  Re-building the XCode project may help.");
        SpriteAsset->SourceFileName = strdup(Name->value());
        
        rapidxml::xml_attribute<char>* TexName = AssetNode->first_attribute("texName");
        if (!TexName)
            ReportError("SpriteAsset is missing the texName property.  Re-building the XCode project may help.");
        SpriteAsset->RawFileName = strdup(TexName->value());
        
        rapidxml::xml_attribute<char>* Width = AssetNode->first_attribute("width");
        if (!Width)
            ReportError("SpriteAsset is missing the width property.  Re-building the XCode project may help.");
        SpriteAsset->Width = atoi(Width->value());
        
        rapidxml::xml_attribute<char>* Height = AssetNode->first_attribute("height");
        if (!Height)
            ReportError("SpriteAsset is missing the height property.  Re-building the XCode project may help.");
        SpriteAsset->Height = atoi(Height->value());
        
        rapidxml::xml_attribute<char>* TexWidth = AssetNode->first_attribute("texWidth");
        if (!TexWidth)
            ReportError("SpriteAsset is missing the texWidth property.  Re-building the XCode project may help.");
        SpriteAsset->TexWidth = atoi(TexWidth->value());
        
        rapidxml::xml_attribute<char>* TexHeight = AssetNode->first_attribute("texHeight");
        if (!TexHeight)
            ReportError("SpriteAsset is missing the texHeight property.  Re-building the XCode project may help.");
        SpriteAsset->TexHeight = atoi(TexHeight->value());
		
		AssetNode = AssetNode->next_sibling("SpriteAsset");
	}
    
	PopErrorContext();
}

#endif

void LoadSpriteAsset(const char* FileName, gxSprite* Sprite)
{
#ifdef PLATFORM_IPHONE
    for (int i = 0; i < NSpriteAssets; i++)
    {
        SSpriteAsset* SpriteAsset = &SpriteAssets[i];
        
        if (strcmp(FileName, SpriteAsset->SourceFileName) == 0)
        {
            Sprite->width = SpriteAsset->Width;
            Sprite->height = SpriteAsset->Height;
            Sprite->texWidth = SpriteAsset->TexWidth;
            Sprite->texHeight = SpriteAsset->TexHeight;

            char work[1024];
            GetAssetFileName(SpriteAsset->RawFileName, work, sizeof(work));

            gzFile RawFile = gzopen(work, "rb");

            if (!RawFile)
                break;
            
            int MaxFileSize = 1024 * 1024 * 4;
            
            char* Pixels = (char*)malloc(MaxFileSize);
            int FileSize = gzread(RawFile, Pixels, MaxFileSize);
            gzclose(RawFile);
            
            glGenTextures(1, &Sprite->tex);
            glBindTexture(GL_TEXTURE_2D, Sprite->tex);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            int BPP = 4;
            bool HasAlpha = true;
            
            GLenum Format;
            if (HasAlpha) 
                Format = (BPP == 4) ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            else
                Format = (BPP == 4) ? GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;

            int MipLevel = 0;
            int MipWidth = Sprite->texWidth;
            int MipHeight = Sprite->texHeight;
            int DataOffset = 0;
            
            do
            {
                GLsizei DataSize = MipWidth * MipHeight * BPP / 8;
                if (DataSize < 32) 
                    DataSize = 32;
                
                glCompressedTexImage2D(GL_TEXTURE_2D, MipLevel, Format, MipWidth, MipHeight, 0, DataSize, Pixels + DataOffset);
                
                MipLevel++;
                MipWidth /= 2;
                MipHeight /= 2;
                
                DataOffset += DataSize;
                
            } while (DataOffset < FileSize);
            
            free(Pixels);

            return;
        }
    }
    
    gxLoadSprite(FileName, Sprite);
#endif
    
#ifdef PLATFORM_WINDOWS
	char Work[1024];
	GetAssetFileName(FileName, Work, sizeof(Work));

    gxLoadSprite(Work, Sprite);
#endif
}

void LoadAssets()
{
    double StartTime = GetCurrentTime();
    
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Assets List                                                                          //
	//-----------------------------------------------------------------------------------------------------------------------------------------//
#ifdef PLATFORM_IPHONE    
    LoadAssetList("Converted/Assets.xml");
#endif    
    
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Sprite Assets                                                                        //
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	LoadSpriteAsset("Assets/white.png", &WhiteSprite);

	LoadSpriteAsset("Assets/wipe-diagonal.png", &WipeDiagonalSprite);
	
	LoadSpriteAsset("Assets/light-flashlight.png", &LightFlashlightSprite);
	LoadSpriteAsset("Assets/light-vacuum.png", &LightVacuumSprite);

	LoadSpriteAsset("Assets/spark.png", &SparkSprite);
	LoadSpriteAsset("Assets/flare.png", &FlareSprite);

	LoadSpriteAsset("Assets/firework-white.png", &FireWorkWhiteSprite);

	LoadSpriteAsset("Assets/dusty-hop-1.png", &DustyHop1Sprite);
	LoadSpriteAsset("Assets/dusty-hop-2.png", &DustyHop2Sprite);
	LoadSpriteAsset("Assets/dusty-hop-3.png", &DustyHop3Sprite);
	LoadSpriteAsset("Assets/dusty-hop-4.png", &DustyHop4Sprite);
	LoadSpriteAsset("Assets/dusty-hop-5.png", &DustyHop5Sprite);

	LoadSpriteAsset("Assets/dusty-hop-2b.png", &DustyHop2bSprite);
	LoadSpriteAsset("Assets/dusty-hop-2c.png", &DustyHop2cSprite);
	LoadSpriteAsset("Assets/dusty-hop-4b.png", &DustyHop4bSprite);
	LoadSpriteAsset("Assets/dusty-hop-4c.png", &DustyHop4cSprite);

	LoadSpriteAsset("Assets/dusty-idle-1.png", &DustyIdle1Sprite);
	LoadSpriteAsset("Assets/dusty-idle-2.png", &DustyIdle2Sprite);
	LoadSpriteAsset("Assets/dusty-idle-3.png", &DustyIdle3Sprite);

	LoadSpriteAsset("Assets/dusty-slide-1.png", &DustySlide1Sprite);
	LoadSpriteAsset("Assets/dusty-slide-2.png", &DustySlide2Sprite);
	LoadSpriteAsset("Assets/dusty-slide-3.png", &DustySlide3Sprite);

	LoadSpriteAsset("Assets/dusty-walljump.png", &DustyWallJumpSprite);
	LoadSpriteAsset("Assets/dusty-cornerjump.png", &DustyCornerJumpSprite);

	LoadSpriteAsset("Assets/dusty-die.png", &DustyDeathSprite);

    LoadSpriteAsset("Assets/logo.png", &LogoSprite);
    
	LoadSpriteAsset("Assets/background-cardboard.png", &BackgroundCardboardSprite);
	LoadSpriteAsset("Assets/background-paper.png", &BackgroundPaperSprite);
	LoadSpriteAsset("Assets/background-fridge.png", &BackgroundFridgeSprite);

	LoadSpriteAsset("Assets/tile-wtf.png", &TileUnknownSprite);
	
	LoadSpriteAsset("Assets/barrel-back.png", &BarrelBackSprite);
	LoadSpriteAsset("Assets/barrel-front.png", &BarrelFrontSprite);
	LoadSpriteAsset("Assets/barrel-nail.png", &BarrelNailSprite);

	LoadSpriteAsset("Assets/fan.png", &FanSprite);

	LoadSpriteAsset("Assets/crumb-stand.png", &CrumbStandSprite);

	LoadSpriteAsset("Assets/coin-spin-1.png", &CoinSpin1Sprite);
	LoadSpriteAsset("Assets/coin-spin-2.png", &CoinSpin2Sprite);
	LoadSpriteAsset("Assets/coin-spin-3.png", &CoinSpin3Sprite);
	LoadSpriteAsset("Assets/coin-spin-4.png", &CoinSpin4Sprite);
	LoadSpriteAsset("Assets/coin-spin-5.png", &CoinSpin5Sprite);
	LoadSpriteAsset("Assets/coin-spin-6.png", &CoinSpin6Sprite);

	LoadSpriteAsset("Assets/coin-lives-1.png", &CoinLife1Sprite);
	LoadSpriteAsset("Assets/coin-lives-2.png", &CoinLife2Sprite);
	LoadSpriteAsset("Assets/coin-lives-3.png", &CoinLife3Sprite);
	LoadSpriteAsset("Assets/coin-lives-4.png", &CoinLife4Sprite);
	LoadSpriteAsset("Assets/coin-lives-5.png", &CoinLife5Sprite);
	LoadSpriteAsset("Assets/coin-lives-6.png", &CoinLife6Sprite);
	
    LoadSpriteAsset("Assets/number-0.png", &ScoreNumber0Sprite);
    LoadSpriteAsset("Assets/number-1.png", &ScoreNumber1Sprite);
    LoadSpriteAsset("Assets/number-2.png", &ScoreNumber2Sprite);
    LoadSpriteAsset("Assets/number-3.png", &ScoreNumber3Sprite);
    LoadSpriteAsset("Assets/number-4.png", &ScoreNumber4Sprite);
    LoadSpriteAsset("Assets/number-5.png", &ScoreNumber5Sprite);
    LoadSpriteAsset("Assets/number-6.png", &ScoreNumber6Sprite);
    LoadSpriteAsset("Assets/number-7.png", &ScoreNumber7Sprite);
    LoadSpriteAsset("Assets/number-8.png", &ScoreNumber8Sprite);
    LoadSpriteAsset("Assets/number-9.png", &ScoreNumber9Sprite);
    
    LoadSpriteAsset("Assets/full-score.png", &FullScoreSprite);
	
    LoadSpriteAsset("Assets/button-pause.png", &ButtonPauseSprite);
    LoadSpriteAsset("Assets/button-play.png", &ButtonPlaySprite);
    LoadSpriteAsset("Assets/button-home.png", &ButtonHomeSprite);
    LoadSpriteAsset("Assets/button-mute.png", &ButtonMuteSprite);
    LoadSpriteAsset("Assets/button-unmute.png", &ButtonUnmuteSprite);

    LoadSpriteAsset("Assets/portal.png", &PortalSprite);

	LoadSpriteAsset("Assets/tennisball-spin-1.png", &TennisBallSpin1Sprite);
	LoadSpriteAsset("Assets/tennisball-spin-2.png", &TennisBallSpin2Sprite);
	LoadSpriteAsset("Assets/tennisball-spin-3.png", &TennisBallSpin3Sprite);
	LoadSpriteAsset("Assets/tennisball-spin-4.png", &TennisBallSpin4Sprite);

	LoadSpriteAsset("Assets/gear.png", &GearSprite);
	
    LoadSpriteAsset("Assets/staplerextendup.png", &StaplerExtendUpSprite);
    LoadSpriteAsset("Assets/staplerup.png", &StaplerUpSprite);
    LoadSpriteAsset("Assets/staplerdown.png", &StaplerDownSprite);
    
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump1Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump2Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump3Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump4Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump5Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump6Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump7Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump8Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump9Sprite);
    LoadSpriteAsset("Assets/power-jump.png", &PowerJump10Sprite);

	LoadSpriteAsset("Assets/dust-mote.png", &DustMoteSprite);
	LoadSpriteAsset("Assets/dust-arrow.png", &DustArrowSprite);

	LoadSpriteAsset("Assets/firework-rocket.png", &FireWorkRocketSprite);

	LoadSpriteAsset("Assets/vacuum-front.png", &VacuumFrontSprite);
	LoadSpriteAsset("Assets/vacuum-1.png", &Vacuum1Sprite);
	LoadSpriteAsset("Assets/vacuum-2.png", &Vacuum2Sprite);
	LoadSpriteAsset("Assets/vacuum-3.png", &Vacuum3Sprite);

	LoadSpriteAsset("Assets/screen-start-1.png", &ScreenStart1Sprite);
	LoadSpriteAsset("Assets/screen-start-2.png", &ScreenStart2Sprite);
    LoadSpriteAsset("Assets/screen-help-1.png", &ScreenHelp1Sprite);
    LoadSpriteAsset("Assets/screen-help-2.png", &ScreenHelp2Sprite);
    LoadSpriteAsset("Assets/screen-credits-1.png", &ScreenCredits1Sprite);
    LoadSpriteAsset("Assets/screen-credits-2.png", &ScreenCredits2Sprite);
    
	LoadSpriteAsset("Assets/icon-start-1.png", &IconStart1Sprite);
	LoadSpriteAsset("Assets/icon-start-2.png", &IconStart2Sprite);
	LoadSpriteAsset("Assets/icon-help-1.png", &IconHelp1Sprite);
	LoadSpriteAsset("Assets/icon-help-2.png", &IconHelp2Sprite);
	LoadSpriteAsset("Assets/icon-credits-1.png", &IconCredits1Sprite);
	LoadSpriteAsset("Assets/icon-credits-2.png", &IconCredits2Sprite);

	LoadSpriteAsset("Assets/screen-lose-1.png", &ScreenLose1Sprite);
	LoadSpriteAsset("Assets/screen-lose-2.png", &ScreenLose2Sprite);
	
	LoadSpriteAsset("Assets/screen-lose-ghost.png", &ScreenLoseGhostSprite);
	LoadSpriteAsset("Assets/screen-lose-grave-1.png", &ScreenLoseGrave1Sprite);
	LoadSpriteAsset("Assets/screen-lose-grave-2.png", &ScreenLoseGrave2Sprite);
	
	LoadSpriteAsset("Assets/screen-win-1.png", &ScreenWin1Sprite);
	LoadSpriteAsset("Assets/screen-win-2.png", &ScreenWin2Sprite);

	LoadSpriteAsset("Assets/tutorial-initial.png", &TutorialInitialSprite);
	LoadSpriteAsset("Assets/tutorial-barrel.png", &TutorialBarrelSprite);
	LoadSpriteAsset("Assets/tutorial-coin.png", &TutorialCoinSprite);
	LoadSpriteAsset("Assets/tutorial-firework.png", &TutorialFireWorkSprite);
	LoadSpriteAsset("Assets/tutorial-gear.png", &TutorialGearSprite);
	LoadSpriteAsset("Assets/tutorial-tennisball.png", &TutorialBallSprite);
	LoadSpriteAsset("Assets/tutorial-jump.png", &TutorialJumpSprite);
	LoadSpriteAsset("Assets/tutorial-walljump.png", &TutorialWallJumpSprite);
	
    LoadSpriteAsset("Assets/chapter-title.png", &ChapterTitle);
    

	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Sound Assets                                                                         //
	//-----------------------------------------------------------------------------------------------------------------------------------------//

	sxLoadSound("Assets/dusty-to-jump.wav", &DustyToJumpSound);
	sxLoadSound("Assets/dusty-jump.wav", &DustyJumpSound);
	sxLoadSound("Assets/dusty-walljump.wav", &DustyWallJumpSound);
	sxLoadSound("Assets/dusty-launch.wav", &DustyLaunchSound);
	sxLoadSound("Assets/dusty-win.wav", &DustyWinSound);

	sxLoadSound("Assets/song-1.wav", &Song1Sound);
	sxLoadSound("Assets/song-2.wav", &Song2Sound);
	sxLoadSound("Assets/song-3.wav", &Song3Sound);

	sxLoadSound("Assets/vacuum-clog.wav", &VacuumClogSound);
	sxLoadSound("Assets/vacuum-clang.wav", &VacuumClangSound);
	sxLoadSound("Assets/vacuum-turnon.wav", &VacuumTurnOnSound);
	sxLoadSound("Assets/vacuum-turnoff.wav", &VacuumTurnOffSound);
	sxLoadSound("Assets/vacuum-jam.wav", &VacuumJamSound);
	sxLoadSound("Assets/vacuum-on.wav", &VacuumOnSound);
	
	sxLoadSound("Assets/BlockBreak.wav", &BlockBreakSound);
	sxLoadSound("Assets/Jello.wav", &JelloSound);
	sxLoadSound("Assets/CoinVacuumedUp.wav", &CoinVacuumedUpSound);
	sxLoadSound("Assets/GearGrind.wav", &GearGrindSound);
	sxLoadSound("Assets/TennisBallVacuumedUp.wav", &TennisBallVacuumedUpSound);
    
    double EndTime = GetCurrentTime();
    LogMessage("Asset loading took %.1f seconds.\n", EndTime-StartTime);
}

