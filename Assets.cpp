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
gxSprite FireWorkBangSprite;

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

gxSprite Score1Sprite;

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
    
	FILE* AssetFile = gxOpenFile(FileName, "r");    
	if (!AssetFile)
		ReportError("Unable to open asset list file.  Check that all required files and tools are present, and re-build the XCode project to fix.");
    
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

            FILE* RawFile = gxOpenFile(SpriteAsset->RawFileName, "rb");
   
            fseek(RawFile, 0, SEEK_END);
            int FileSize = ftell(RawFile);
            rewind(RawFile);
            
            char* Pixels = (char*)malloc(FileSize);
            fread(Pixels, FileSize, 1, RawFile);
            fclose(RawFile);
            
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
    gxLoadSprite(FileName, Sprite);
#endif
}

void LoadAssets()
{
    float StartTime = GetCurrentTime();
    
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Assets List                                                                          //
	//-----------------------------------------------------------------------------------------------------------------------------------------//
#ifdef PLATFORM_IPHONE    
    LoadAssetList("Converted/Assets.xml");
#endif    
    
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Sprite Assets                                                                        //
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	LoadSpriteAsset("Data/white.png", &WhiteSprite);

	LoadSpriteAsset("Data/wipe-diagonal.png", &WipeDiagonalSprite);
	
	LoadSpriteAsset("Data/light-flashlight.png", &LightFlashlightSprite);
	LoadSpriteAsset("Data/light-vacuum.png", &LightVacuumSprite);

	LoadSpriteAsset("Data/spark.png", &SparkSprite);
	LoadSpriteAsset("Data/flare.png", &FlareSprite);

	LoadSpriteAsset("Data/firework-white.png", &FireWorkWhiteSprite);

	LoadSpriteAsset("Data/dusty-hop-1.png", &DustyHop1Sprite);
	LoadSpriteAsset("Data/dusty-hop-2.png", &DustyHop2Sprite);
	LoadSpriteAsset("Data/dusty-hop-3.png", &DustyHop3Sprite);
	LoadSpriteAsset("Data/dusty-hop-4.png", &DustyHop4Sprite);
	LoadSpriteAsset("Data/dusty-hop-5.png", &DustyHop5Sprite);

	LoadSpriteAsset("Data/dusty-hop-2b.png", &DustyHop2bSprite);
	LoadSpriteAsset("Data/dusty-hop-2c.png", &DustyHop2cSprite);
	LoadSpriteAsset("Data/dusty-hop-4b.png", &DustyHop4bSprite);
	LoadSpriteAsset("Data/dusty-hop-4c.png", &DustyHop4cSprite);

	LoadSpriteAsset("Data/dusty-idle-1.png", &DustyIdle1Sprite);
	LoadSpriteAsset("Data/dusty-idle-2.png", &DustyIdle2Sprite);
	LoadSpriteAsset("Data/dusty-idle-3.png", &DustyIdle3Sprite);

	LoadSpriteAsset("Data/dusty-slide-1.png", &DustySlide1Sprite);
	LoadSpriteAsset("Data/dusty-slide-2.png", &DustySlide2Sprite);
	LoadSpriteAsset("Data/dusty-slide-3.png", &DustySlide3Sprite);

	LoadSpriteAsset("Data/dusty-walljump.png", &DustyWallJumpSprite);
	LoadSpriteAsset("Data/dusty-cornerjump.png", &DustyCornerJumpSprite);

	LoadSpriteAsset("Data/dusty-die.png", &DustyDeathSprite);

    LoadSpriteAsset("Data/logo.png", &LogoSprite);
    
	LoadSpriteAsset("Data/background-cardboard.png", &BackgroundCardboardSprite);
	LoadSpriteAsset("Data/background-paper.png", &BackgroundPaperSprite);
	LoadSpriteAsset("Data/background-fridge.png", &BackgroundFridgeSprite);

	LoadSpriteAsset("Data/tile-wtf.png", &TileUnknownSprite);
	
	LoadSpriteAsset("Data/barrel-back.png", &BarrelBackSprite);
	LoadSpriteAsset("Data/barrel-front.png", &BarrelFrontSprite);
	LoadSpriteAsset("Data/barrel-nail.png", &BarrelNailSprite);

	LoadSpriteAsset("Data/fan.png", &FanSprite);

	LoadSpriteAsset("Data/crumb-stand.png", &CrumbStandSprite);

	LoadSpriteAsset("Data/coin-spin-1.png", &CoinSpin1Sprite);
	LoadSpriteAsset("Data/coin-spin-2.png", &CoinSpin2Sprite);
	LoadSpriteAsset("Data/coin-spin-3.png", &CoinSpin3Sprite);
	LoadSpriteAsset("Data/coin-spin-4.png", &CoinSpin4Sprite);
	LoadSpriteAsset("Data/coin-spin-5.png", &CoinSpin5Sprite);
	LoadSpriteAsset("Data/coin-spin-6.png", &CoinSpin6Sprite);

	LoadSpriteAsset("Data/coin-lives-1.png", &CoinLife1Sprite);
	LoadSpriteAsset("Data/coin-lives-2.png", &CoinLife2Sprite);
	LoadSpriteAsset("Data/coin-lives-3.png", &CoinLife3Sprite);
	LoadSpriteAsset("Data/coin-lives-4.png", &CoinLife4Sprite);
	LoadSpriteAsset("Data/coin-lives-5.png", &CoinLife5Sprite);
	LoadSpriteAsset("Data/coin-lives-6.png", &CoinLife6Sprite);
	
    LoadSpriteAsset("Data/number-0.png", &ScoreNumber0Sprite);
    LoadSpriteAsset("Data/number-1.png", &ScoreNumber1Sprite);
    LoadSpriteAsset("Data/number-2.png", &ScoreNumber2Sprite);
    LoadSpriteAsset("Data/number-3.png", &ScoreNumber3Sprite);
    LoadSpriteAsset("Data/number-4.png", &ScoreNumber4Sprite);
    LoadSpriteAsset("Data/number-5.png", &ScoreNumber5Sprite);
    LoadSpriteAsset("Data/number-6.png", &ScoreNumber6Sprite);
    LoadSpriteAsset("Data/number-7.png", &ScoreNumber7Sprite);
    LoadSpriteAsset("Data/number-8.png", &ScoreNumber8Sprite);
    LoadSpriteAsset("Data/number-9.png", &ScoreNumber9Sprite);
	
    LoadSpriteAsset("Data/button-pause.png", &ButtonPauseSprite);
    LoadSpriteAsset("Data/button-play.png", &ButtonPlaySprite);
    LoadSpriteAsset("Data/button-home.png", &ButtonHomeSprite);
    LoadSpriteAsset("Data/button-mute.png", &ButtonMuteSprite);
    LoadSpriteAsset("Data/button-unmute.png", &ButtonUnmuteSprite);

    LoadSpriteAsset("Data/portal.png", &PortalSprite);

	LoadSpriteAsset("Data/tennisball-spin-1.png", &TennisBallSpin1Sprite);
	LoadSpriteAsset("Data/tennisball-spin-2.png", &TennisBallSpin2Sprite);
	LoadSpriteAsset("Data/tennisball-spin-3.png", &TennisBallSpin3Sprite);
	LoadSpriteAsset("Data/tennisball-spin-4.png", &TennisBallSpin4Sprite);

	LoadSpriteAsset("Data/gear.png", &GearSprite);
	
    LoadSpriteAsset("Data/staplerextendup.png", &StaplerExtendUpSprite);
    LoadSpriteAsset("Data/staplerup.png", &StaplerUpSprite);
    LoadSpriteAsset("Data/staplerdown.png", &StaplerDownSprite);
    
    LoadSpriteAsset("Data/powerjump1sprite.png", &PowerJump1Sprite);
    LoadSpriteAsset("Data/powerjump2sprite.png", &PowerJump2Sprite);
    LoadSpriteAsset("Data/powerjump3sprite.png", &PowerJump3Sprite);
    LoadSpriteAsset("Data/powerjump4sprite.png", &PowerJump4Sprite);
    LoadSpriteAsset("Data/powerjump5sprite.png", &PowerJump5Sprite);
    LoadSpriteAsset("Data/powerjump6sprite.png", &PowerJump6Sprite);
    LoadSpriteAsset("Data/powerjump7sprite.png", &PowerJump7Sprite);
    LoadSpriteAsset("Data/powerjump8sprite.png", &PowerJump8Sprite);
    LoadSpriteAsset("Data/powerjump9sprite.png", &PowerJump9Sprite);
    LoadSpriteAsset("Data/powerjump10sprite.png", &PowerJump10Sprite);

	LoadSpriteAsset("Data/dust-mote.png", &DustMoteSprite);
	LoadSpriteAsset("Data/dust-arrow.png", &DustArrowSprite);

	LoadSpriteAsset("Data/firework-rocket.png", &FireWorkRocketSprite);
	LoadSpriteAsset("Data/firework-bang.png", &FireWorkBangSprite);

	LoadSpriteAsset("Data/vacuum-front.png", &VacuumFrontSprite);

	LoadSpriteAsset("Data/screen-start-1.png", &ScreenStart1Sprite);
	LoadSpriteAsset("Data/screen-start-2.png", &ScreenStart2Sprite);
    LoadSpriteAsset("Data/screen-help-1.png", &ScreenHelp1Sprite);
    LoadSpriteAsset("Data/screen-help-2.png", &ScreenHelp2Sprite);
    LoadSpriteAsset("Data/screen-credits-1.png", &ScreenCredits1Sprite);
    LoadSpriteAsset("Data/screen-credits-2.png", &ScreenCredits2Sprite);
    
	LoadSpriteAsset("Data/icon-start-1.png", &IconStart1Sprite);
	LoadSpriteAsset("Data/icon-start-2.png", &IconStart2Sprite);
	LoadSpriteAsset("Data/icon-help-1.png", &IconHelp1Sprite);
	LoadSpriteAsset("Data/icon-help-2.png", &IconHelp2Sprite);
	LoadSpriteAsset("Data/icon-credits-1.png", &IconCredits1Sprite);
	LoadSpriteAsset("Data/icon-credits-2.png", &IconCredits2Sprite);

	LoadSpriteAsset("Data/screen-lose-1.png", &ScreenLose1Sprite);
	LoadSpriteAsset("Data/screen-lose-2.png", &ScreenLose2Sprite);
	
	LoadSpriteAsset("Data/screen-lose-ghost.png", &ScreenLoseGhostSprite);
	LoadSpriteAsset("Data/screen-lose-grave-1.png", &ScreenLoseGrave1Sprite);
	LoadSpriteAsset("Data/screen-lose-grave-2.png", &ScreenLoseGrave2Sprite);
	
	LoadSpriteAsset("Data/screen-win-1.png", &ScreenWin1Sprite);
	LoadSpriteAsset("Data/screen-win-2.png", &ScreenWin2Sprite);

	LoadSpriteAsset("Data/tutorial-initial.png", &TutorialInitialSprite);
	LoadSpriteAsset("Data/tutorial-barrel.png", &TutorialBarrelSprite);
	LoadSpriteAsset("Data/tutorial-coin.png", &TutorialCoinSprite);
	LoadSpriteAsset("Data/tutorial-firework.png", &TutorialFireWorkSprite);
	LoadSpriteAsset("Data/tutorial-gear.png", &TutorialGearSprite);
	LoadSpriteAsset("Data/tutorial-tennisball.png", &TutorialBallSprite);
	LoadSpriteAsset("Data/tutorial-jump.png", &TutorialJumpSprite);
	LoadSpriteAsset("Data/tutorial-walljump.png", &TutorialWallJumpSprite);
	
    LoadSpriteAsset("Data/chapter-title.png", &ChapterTitle);
    
    LoadSpriteAsset("Data/Score.png", &Score1Sprite);
	

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
    
    double EndTime = GetCurrentTime();
    LogMessage("Asset loading took %.1f seconds.\n", EndTime-StartTime);
}

