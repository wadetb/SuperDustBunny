//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "common.h"
#include "Dusty.h"
#include "Assets.h"
#include "Settings.h"

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

gxSprite ChapterTitleSprite;
gxSprite ChapterTextSprite;
gxSprite ChapterNumbersSprite;

gxSprite PowerUpSprite;

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



#ifdef PLATFORM_IPHONE

#define MAX_ASSETS   512


struct SSpriteAsset
{
    int Width;
    int Height;
    int TexWidth;
    int TexHeight;
    int Left;
    int Right;
    int Top;
    int Bottom;
};

struct SRawAsset
{
};

struct SAsset
{
    char* SourceFileName;
    char* RawFileName;
    float ModTime; 
    
    union
    {
        SSpriteAsset Sprite;
        SRawAsset Raw;
    };
};

struct SAssetList
{
    char* RootDirectory;
    
    int NAssets;
    SAsset Assets[MAX_ASSETS];
};


SAssetList BundleAssets;
SAssetList CacheAssets;
SAssetList OldCacheAssets;

#endif


//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                          Loading Entire Files                                                           //
//-----------------------------------------------------------------------------------------------------------------------------------------//

void* LoadFileData(const char* FileName, void** Data=NULL, int* DataSize=NULL)
{
    int FileDataSize;
    char* FileData;
    
    FILE* F = fopen(FileName, "rb");
    if (!F)
        return NULL;
    
    fseek(F, 0, SEEK_END);
    FileDataSize = ftell(F);
    fseek(F, 0, SEEK_SET);
    
    FileData = (char*)malloc(FileDataSize+1);
    fread(FileData, 1, FileDataSize, F);
    fclose(F);
    
    FileData[FileDataSize] = 0;
    
    if (Data) *Data = FileData;
    if (DataSize) *DataSize = FileDataSize;
    
    return FileData;    
}

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                          Asset Lists                                                                    //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifdef PLATFORM_IPHONE

void ClearAssetList(SAssetList* AssetList)
{
    free(AssetList->RootDirectory);
    
    for (int i = 0; i < AssetList->NAssets; i++)
    {
        SAsset* Asset = &AssetList->Assets[i];
        if (Asset->SourceFileName) 
        {
            free(Asset->SourceFileName);
            Asset->SourceFileName = NULL;
        }        
        if (Asset->RawFileName) 
        {
            free(Asset->RawFileName);
            Asset->RawFileName = NULL;
        }
    }
    
    AssetList->NAssets = 0;
}

bool LoadAssetList(char* FileName, SAssetList* AssetList)
{
    AssetList->NAssets = 0;
    
    char* XML = (char*)LoadFileData(FileName);
    if (!XML)
        return false;
    
    NSString* rootDirectory = [[[NSString stringWithUTF8String:FileName] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
    AssetList->RootDirectory = strdup([rootDirectory UTF8String]);
    
	// Parse the XML text buffer into a Document hierarchy.
	rapidxml::xml_document<> Document;
	Document.parse<0>(XML);
    
    // Get the root <assets> node.
	rapidxml::xml_node<char>* AssetsNode = Document.first_node("Assets");
	if (AssetsNode == NULL)
		return false;
    
    // Iterate over all the <SpriteAsset> nodes.
	for (rapidxml::xml_node<char>* AssetNode = AssetsNode->first_node("SpriteAsset"); AssetNode; AssetNode = AssetNode->next_sibling("SpriteAsset"))
	{
        if (AssetList->NAssets >= MAX_ASSETS)
            break;
        
        SAsset* SpriteAsset = &AssetList->Assets[AssetList->NAssets];
        
        rapidxml::xml_attribute<char>* Width = AssetNode->first_attribute("width");
        if (!Width)
            continue;
        SpriteAsset->Sprite.Width = atoi(Width->value());
        
        rapidxml::xml_attribute<char>* Height = AssetNode->first_attribute("height");
        if (!Height)
            continue;
        SpriteAsset->Sprite.Height = atoi(Height->value());
        
        rapidxml::xml_attribute<char>* TexWidth = AssetNode->first_attribute("texWidth");
        if (!TexWidth)
            continue;
        SpriteAsset->Sprite.TexWidth = atoi(TexWidth->value());
        
        rapidxml::xml_attribute<char>* TexHeight = AssetNode->first_attribute("texHeight");
        if (!TexHeight)
            continue;
        SpriteAsset->Sprite.TexHeight = atoi(TexHeight->value());
		
        rapidxml::xml_attribute<char>* Left = AssetNode->first_attribute("left");
        if (!Left)
            continue;
        SpriteAsset->Sprite.Left = atoi(Left->value());
		
        rapidxml::xml_attribute<char>* Right = AssetNode->first_attribute("right");
        if (!Right)
            continue;
        SpriteAsset->Sprite.Right = atoi(Right->value());
		
        rapidxml::xml_attribute<char>* Top = AssetNode->first_attribute("top");
        if (!Top)
            continue;
        SpriteAsset->Sprite.Top = atoi(Top->value());
		
        rapidxml::xml_attribute<char>* Bottom = AssetNode->first_attribute("bottom");
        if (!Bottom)
            continue;
        SpriteAsset->Sprite.Bottom = atoi(Bottom->value());
		
        rapidxml::xml_attribute<char>* ModTime = AssetNode->first_attribute("modTime");
        if (!ModTime)
            continue;
        SpriteAsset->ModTime = atof(ModTime->value());
		
        rapidxml::xml_attribute<char>* Name = AssetNode->first_attribute("name");
        if (!Name)
            continue;
        SpriteAsset->SourceFileName = strdup(Name->value());
        
        rapidxml::xml_attribute<char>* TexName = AssetNode->first_attribute("texName");
        if (!TexName)
        {
            free(SpriteAsset->SourceFileName);
            SpriteAsset->SourceFileName = NULL;
            continue;
        }
        
        SpriteAsset->RawFileName = strdup(TexName->value());

        AssetList->NAssets++;
	}
    
    // Iterate over all the <RawAsset> nodes.
	for (rapidxml::xml_node<char>* AssetNode = AssetsNode->first_node("RawAsset"); AssetNode; AssetNode = AssetNode->next_sibling("RawAsset"))
	{
        if (AssetList->NAssets >= MAX_ASSETS)
            break;
        
        SAsset* RawAsset = &AssetList->Assets[AssetList->NAssets];
        
        rapidxml::xml_attribute<char>* ModTime = AssetNode->first_attribute("modTime");
        if (!ModTime)
            continue;
        RawAsset->ModTime = atof(ModTime->value());

        rapidxml::xml_attribute<char>* Name = AssetNode->first_attribute("name");
        if (!Name)
            continue;
        RawAsset->SourceFileName = strdup(Name->value());
        
        rapidxml::xml_attribute<char>* RawName = AssetNode->first_attribute("rawName");
        if (!RawName)
        {
            free(RawAsset->SourceFileName);
            continue;
        }
        RawAsset->RawFileName = strdup(RawName->value());
		
        AssetList->NAssets++;
	}
    
    return AssetList->NAssets > 0;
}


SAsset* GetAssetFromAssetList(const char* FileName, SAssetList* AssetList)
{
    for (int i = 0; i < AssetList->NAssets; i++)
    {
        SAsset* Asset = &AssetList->Assets[i];
        if (strcmp(Asset->SourceFileName, FileName) == 0)
            return Asset;
    }
    return NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                          Live Assets                                                                    //
//-----------------------------------------------------------------------------------------------------------------------------------------//

void GetLiveAssetFileName(const char* FileName, char* Buf, int BufSize)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cacheDirectory = [paths objectAtIndex:0];
    
    NSString *cachedFileName = [NSString stringWithFormat:@"%@/%s", cacheDirectory, FileName];
    
	snprintf(Buf, BufSize, "%s", [cachedFileName UTF8String]);
}

bool DownloadLiveAssetFile(const char* FileName)
{
    NSString* URLString = [NSString stringWithFormat:@"http://pluszerogames.com/sdb/live/%d/%s", Settings.LiveAssetSlot, FileName];
    NSURL *URL = [NSURL URLWithString:URLString];
    
    NSLog(@"DOWNLOAD '%@'...", URLString);

    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReturnCacheDataElseLoad timeoutInterval:10];
    
    NSURLResponse *response;
    NSError *error = [[NSError alloc] init];
    NSData* result = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
        
    if (result && [(NSHTTPURLResponse*)response statusCode] == 200)
    {
        NSLog(@"DONE...");

        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        NSString *cacheDirectory = [paths objectAtIndex:0];
        
        NSString *cachedFileName = [NSString stringWithFormat:@"%@/%s", cacheDirectory, FileName];
        
        NSString *cachedPath = [cachedFileName stringByDeletingLastPathComponent];
        if (![[NSFileManager defaultManager] fileExistsAtPath:cachedPath])
        {
            NSLog(@"MKDIR '%@'...", cachedPath);
            [[NSFileManager defaultManager] createDirectoryAtPath:cachedPath withIntermediateDirectories:YES attributes:nil error:&error];
        }
        
        [result writeToFile:cachedFileName atomically:TRUE];
        NSLog(@"SAVED '%@'.\n", cachedFileName);

        return true;
    }

    NSLog(@"FAIL\n");

    return false;
}

void UpdateLiveAssetCache()
{
    double StartTime = GetCurrentTime();

    ClearAssetList(&CacheAssets);
    
    char Work[1024];
    GetLiveAssetFileName("Converted/Assets.xml", Work, sizeof(Work));
    LoadAssetList(Work, &CacheAssets);

    bool LiveAssetsAvailable = DownloadLiveAssetFile("Converted/Assets.xml");
    
    if (LiveAssetsAvailable)
    {
        SAssetList NewCacheAssets;        
        LoadAssetList(Work, &NewCacheAssets);
        
        for (int i = 0; i < NewCacheAssets.NAssets; i++)
        {
            SAsset* NewAsset = &NewCacheAssets.Assets[i];
            
            SAsset* OldAsset = GetAssetFromAssetList(NewAsset->SourceFileName, &CacheAssets);
            
            bool Download = false;
            
            if (!OldAsset)
            {
                printf("Downloading '%s', new asset.\n", NewAsset->SourceFileName);
                Download = true;
            }
            
            if (!Download && OldAsset && OldAsset->ModTime != NewAsset->ModTime)
            {
                printf("Downloading '%s', different file date.\n", NewAsset->SourceFileName);
                Download = true;
            }
            
            char Work[1024];
            snprintf(Work, sizeof(Work), "%s/%s", NewCacheAssets.RootDirectory, NewAsset->RawFileName);
            if (!Download && ![[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:Work]])
            {
                printf("Downloading '%s', cache file is missing.\n", NewAsset->SourceFileName);
                Download = true;
            }
            
            if (Download)
            {
                DownloadLiveAssetFile(NewAsset->RawFileName);
            }
        }

        ClearAssetList(&CacheAssets);
        ClearAssetList(&NewCacheAssets);

        LoadAssetList(Work, &CacheAssets);
    }
    
    double EndTime = GetCurrentTime();
    LogMessage("Live asset cache update took %.1f seconds.\n", EndTime-StartTime);
}

void GetAsset(const char* FileName, SAssetList** AssetList, SAsset** Asset)
{
    SAsset* BundleAsset = GetAssetFromAssetList(FileName, &BundleAssets);
    
    if (Settings.LiveAssets)
    {
        SAsset* LiveAsset = GetAssetFromAssetList(FileName, &CacheAssets);
        
        if (LiveAsset)
        {
            if (BundleAsset == NULL || LiveAsset->ModTime >= BundleAsset->ModTime)
            {
                //printf("Using live asset '%s'.\n", LiveAsset->RawFileName);
                *AssetList = &CacheAssets;
                *Asset = LiveAsset;   
                return;
            }
        }
    }

    //if (BundleAsset)
    //    printf("Using bundled asset '%s'.\n", BundleAsset->RawFileName);
    
    *AssetList = &BundleAssets;
    *Asset = BundleAsset;
}

#endif

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Asset file loading (platform specific)                                             //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
void GetBundleFileName(const char* FileName, char* Buf, int BufSize)
{
#ifdef PLATFORM_IPHONE
	CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    
	UInt8 BundlePath[1024];
	CFURLGetFileSystemRepresentation(url, YES, BundlePath, sizeof(BundlePath));
    
	CFRelease(url);
    
	snprintf(Buf, BufSize, "%s/%s", (char*)BundlePath, FileName);
#endif
    
#ifdef PLATFORM_WINDOWS
	char ModulePath[1024];
	GetModuleFileName(NULL, ModulePath, sizeof(ModulePath));
	PathRemoveFileSpec(ModulePath);
    
	// Go from e.g. /SuperDustBunny/trunk/Build/win/ to /SuperDustBunny/trunk/
	snprintf(Buf, BufSize, "%s/../../%s", ModulePath, FileName);
#endif
}

void* LoadAssetFile(const char* FileName, void** Data, int* DataSize)
{
#ifdef PLATFORM_IPHONE
    SAssetList* AssetList;
    SAsset* Asset;
    GetAsset(FileName, &AssetList, &Asset);
    
    if (Asset)
    {
        char Work[1024];
        snprintf(Work, sizeof(Work), "%s/%s", AssetList->RootDirectory, Asset->RawFileName);
        return LoadFileData(Work, Data, DataSize);
    }
    else
    {
        if (Data) *Data = NULL;
        if (Data) *DataSize = 0;
        return NULL;
    }
#endif
    
#ifdef PLATFORM_WINDOWS
    char Work[1024];
    GetBundleFileName(FileName, Work, sizeof(Work));
    
    return LoadFileData(Work, Data, DataSize);
#endif
}

void LoadSpriteAsset(const char* FileName, gxSprite* Sprite)
{
#ifdef PLATFORM_IPHONE
    if (Sprite->tex)
        gxDestroySprite(Sprite);
    
    SAssetList* AssetList;
    SAsset* SpriteAsset;
    GetAsset(FileName, &AssetList, &SpriteAsset);

    if (SpriteAsset)
    {
        Sprite->width = SpriteAsset->Sprite.Width;
        Sprite->height = SpriteAsset->Sprite.Height;
        Sprite->texWidth = SpriteAsset->Sprite.TexWidth;
        Sprite->texHeight = SpriteAsset->Sprite.TexHeight;
        Sprite->top = SpriteAsset->Sprite.Top;
        Sprite->bottom = SpriteAsset->Sprite.Bottom;
        Sprite->left = SpriteAsset->Sprite.Left;
        Sprite->right = SpriteAsset->Sprite.Right;

        char Work[1024];
        snprintf(Work, sizeof(Work), "%s/%s", AssetList->RootDirectory, SpriteAsset->RawFileName);

        gzFile F = gzopen(Work, "rb");
        if (!F)
            return;
                
        int MaxFileSize = 1024 * 1024 * 4;
        
        char* Pixels = (char*)malloc(MaxFileSize);
        int UncompressedSize = gzread(F, Pixels, MaxFileSize);
        gzclose(F);
        
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
            
        } while (DataOffset < UncompressedSize);
        
        free(Pixels);
    }
    else
    {
        gxLoadSprite(FileName, Sprite);
    }
#endif
    
#ifdef PLATFORM_WINDOWS
	char Work[1024];
	GetBundleFileName(FileName, Work, sizeof(Work));

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
    char Work[1024];
    GetBundleFileName("Converted/Assets.xml", Work, sizeof(Work));
    LoadAssetList(Work, &BundleAssets);
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
	
	LoadSpriteAsset("Assets/chapter-title.png", &ChapterTitleSprite);
	LoadSpriteAsset("Assets/chapter.png", &ChapterTextSprite);
	LoadSpriteAsset("Assets/chapter_numbers.png", &ChapterNumbersSprite);
    
    LoadSpriteAsset("Assets/powerup.png", &PowerUpSprite);
    

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

