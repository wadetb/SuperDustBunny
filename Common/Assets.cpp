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
#import "SuperDustBunnyViewController.h"
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                    Sprite Assets                                                                        //
//-----------------------------------------------------------------------------------------------------------------------------------------//

gxSprite WhiteSprite;

gxSprite WipeDiagonalSprite;

gxSprite AlphabetSprite;

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
gxSprite DustyHop3bSprite;
gxSprite DustyHop4bSprite;
gxSprite DustyHop4cSprite;
gxSprite DustyHop5bSprite;

gxSprite DustyIdle1Sprite;
gxSprite DustyIdle2Sprite;
gxSprite DustyIdle3Sprite;

gxSprite DustySlide1Sprite;
gxSprite DustySlide2Sprite;
gxSprite DustySlide3Sprite;

gxSprite DustyWallJumpSprite;
gxSprite DustyWallJumpbSprite;
gxSprite DustyWallJumpcSprite;
gxSprite DustyCornerJumpSprite;
gxSprite DustyCornerJumpbSprite;
gxSprite DustyCornerJumpcSprite;
gxSprite DustyCornerJumpdSprite;

gxSprite DustyDeathSprite;

gxSprite LogoSprite;

gxSprite NewUnlockSprite;

gxSprite BronzeMedalSprite;
gxSprite SilverMedalSprite;
gxSprite GoldMedalSprite;

gxSprite BackgroundCardboardSprite;
gxSprite BackgroundPaperSprite;
gxSprite BackgroundFridgeSprite;

gxSprite TileUnknownSprite;

gxSprite BarrelBackSprite;
gxSprite BarrelFrontSprite;
gxSprite BarrelNailSprite;

gxSprite FanSprite;

gxSprite CoinSpin1Sprite;
gxSprite CoinSpin2Sprite;
gxSprite CoinSpin3Sprite;
gxSprite CoinSpin4Sprite;
gxSprite CoinSpin5Sprite;
gxSprite CoinSpin6Sprite;

gxSprite FullScoreSprite;

gxSprite ButtonPauseSprite;
gxSprite ButtonPlaySprite;
gxSprite ButtonMuteSprite;
gxSprite ButtonUnmuteSprite;
gxSprite ButtonHomeSprite;
gxSprite ButtonFastForwardSprite;

gxSprite PortalSprite;

gxSprite TennisBallSpin1Sprite;
gxSprite TennisBallSpin2Sprite;
gxSprite TennisBallSpin3Sprite;
gxSprite TennisBallSpin4Sprite;

gxSprite GearSprite;

gxSprite Stapler1Sprite;
gxSprite Stapler2Sprite;
gxSprite Stapler3Sprite;

gxSprite Jello1Sprite;
gxSprite Jello2Sprite;
gxSprite Jello3Sprite;
gxSprite Jello4Sprite;
gxSprite Jello5Sprite;

gxSprite Clothespin1Sprite;
gxSprite Clothespin2Sprite;

gxSprite DustMoteSprite;

gxSprite FireWorkRocketSprite;

gxSprite Flame1Sprite;
gxSprite Flame2Sprite;

gxSprite Vacuum1Sprite;
gxSprite Vacuum2Sprite;
gxSprite Vacuum3Sprite;
gxSprite VacuumSmokeSprite;

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
gxSprite IconLockedSprite;

gxSprite ScreenLose1Sprite;
gxSprite ScreenLose2Sprite;

gxSprite ScreenLoseGhostSprite;
gxSprite ScreenLoseGrave1Sprite;
gxSprite ScreenLoseGrave2Sprite;

gxSprite ScreenWin1Sprite;
gxSprite ScreenWin2Sprite;

gxSprite TextFlickToJumpSprite;
gxSprite TextExtraLifeSprite;
gxSprite TextBonusSprite;
gxSprite TextBubbleSprite;

gxSprite BubbleMinus1SecSprite;
gxSprite Bubble1UpSprite;

gxSprite ChapterTitleSprite;
gxSprite ChapterTextSprite;
gxSprite ChapterNumbersSprite;
gxSprite ChapterCompleteSprite;
gxSprite ChapterTimeSprite;
gxSprite ChapterBonusSprite;
gxSprite ChapterNewSprite;
gxSprite ChapterNewRecordSprite;
gxSprite ChapterRecordSprite;
gxSprite ChapterTotalTimeSprite;

gxSprite TapToContinueSprite;

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

#ifdef PLATFORM_MAC

char RootDirectory[1024];

#endif

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
    char* MD5; 
    
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

static void* LoadFileData(const char* FileName, void** Data=NULL, fpos_t* DataSize=NULL)
{
    long FileDataSize;
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
    
    try 
    {
        Document.parse<0>(XML);
    } 
    catch (rapidxml::parse_error e)
    {
        unlink(FileName);
        ReportError("Failed to parse asset list %s:\n%s", FileName, e.what());
    }
    
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
		
        rapidxml::xml_attribute<char>* MD5 = AssetNode->first_attribute("md5");
        if (!MD5)
            continue;
        SpriteAsset->MD5 = strdup(MD5->value());
		
        rapidxml::xml_attribute<char>* Name = AssetNode->first_attribute("name");
        if (!Name)
        {
            free(SpriteAsset->MD5);
            SpriteAsset->MD5 = NULL;
            continue;
        }
        SpriteAsset->SourceFileName = strdup(Name->value());
        
        rapidxml::xml_attribute<char>* TexName = AssetNode->first_attribute("texName");
        if (!TexName)
        {
            free(SpriteAsset->MD5);
            SpriteAsset->MD5 = NULL;
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
        
        rapidxml::xml_attribute<char>* MD5 = AssetNode->first_attribute("md5");
        if (!MD5)
            continue;
        RawAsset->MD5 = strdup(MD5->value());

        rapidxml::xml_attribute<char>* Name = AssetNode->first_attribute("name");
        if (!Name)
        {
            free(RawAsset->MD5);
            RawAsset->MD5 = NULL;
            continue;
        }
        RawAsset->SourceFileName = strdup(Name->value());
        
        rapidxml::xml_attribute<char>* RawName = AssetNode->first_attribute("rawName");
        if (!RawName)
        {
            free(RawAsset->MD5);
            RawAsset->MD5 = NULL;
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

void ClearLiveAssetCache()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cacheDirectory = [paths objectAtIndex:0];

    NSError *error = [[NSError alloc] init];
    [[NSFileManager defaultManager] removeItemAtPath:cacheDirectory error:&error];
}

bool DownloadLiveAssetFile(const char* FileName)
{
    NSString* URLString = [NSString stringWithFormat:@"%@%s", [NSString stringWithUTF8String:Settings.AssetServer], FileName];
    NSURL *URL = [NSURL URLWithString:URLString];
    
    NSLog(@"DOWNLOAD '%@'...", URLString);

    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData timeoutInterval:30];
    
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

    [theViewController startSpinner];

#ifdef PLATFORM_IPHONE
    [TestFlight passCheckpoint:[NSString stringWithFormat:@"Updated Live Assets"]];
#endif

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
            SAsset* BundleAsset = GetAssetFromAssetList(NewAsset->SourceFileName, &BundleAssets);
            
            bool Download = false;
            
            if (strlen(NewAsset->MD5) > 0)
            {
                if (!BundleAsset && !OldAsset)
                {
                    printf("Downloading '%s', new asset.\n", NewAsset->SourceFileName);
                    Download = true;
                }
                
                if (!Download)
                {
                    if (OldAsset)
                    {
                        if (strcmp(OldAsset->MD5, NewAsset->MD5))
                        {
                            printf("Downloading '%s', MD5 differs from cache.\n", NewAsset->SourceFileName);
                            Download = true;
                        }
                    }
                    else
                    {
                        if (BundleAsset)
                        {
                            if (strcmp(BundleAsset->MD5, NewAsset->MD5))
                            {
                                printf("Downloading '%s', MD5 differs from bundle.\n", NewAsset->SourceFileName);
                                Download = true;
                            }
                        }
                    }
                }
            }
            
            /*
            char Work[1024];
            snprintf(Work, sizeof(Work), "%s/%s", NewCacheAssets.RootDirectory, NewAsset->RawFileName);
            if (!Download && ![[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:Work]])
            {
                printf("Downloading '%s', cache file is missing.\n", NewAsset->SourceFileName);
                Download = true;
            }
            */
            
            if (Download)
            {
                DownloadLiveAssetFile(NewAsset->RawFileName);
            }
        }

        ClearAssetList(&CacheAssets);
        ClearAssetList(&NewCacheAssets);

        LoadAssetList(Work, &CacheAssets);
    }
    
    [theViewController stopSpinner];
    
    LoadAssets();

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
            if (BundleAsset == NULL || strcmp(LiveAsset->MD5, BundleAsset->MD5))
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
static void GetBundleFileName(const char* FileName, char* Buf, int BufSize)
{
#ifdef PLATFORM_MAC
    snprintf(Buf, BufSize, "%s/%s", RootDirectory, FileName);
#endif
    
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

void* LoadAssetFile(const char* FileName, void** Data, fpos_t* DataSize)
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
    
#ifdef PLATFORM_WINDOWS_OR_MAC
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

    if (!SpriteAsset)
        ReportError("Could not find sprite asset '%s'.", FileName);

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
            
    int MaxFileSize = 1024 * 1024 * 16;
    
    char* Pixels = (char*)malloc(MaxFileSize);
    int UncompressedSize = gzread(F, Pixels, MaxFileSize);
    gzclose(F);
    
    glGenTextures(1, &Sprite->tex);
    glBindTexture(GL_TEXTURE_2D, Sprite->tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
    
    int MaxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTextureSize);
    
    if (MipWidth > MaxTextureSize || MipHeight > MaxTextureSize)
    {
        while (MipWidth > MaxTextureSize || MipHeight > MaxTextureSize)
        {
            GLsizei DataSize = MipWidth * MipHeight * BPP / 8;
            if (DataSize < 32) 
                DataSize = 32;
            
            MipWidth /= 2;
            MipHeight /= 2;

            DataOffset += DataSize;
        }

        NSLog(@"Texture '%s' exceeds the maximum size for this device.  Resizing to %d x %d.\n", FileName, MipWidth, MipHeight);
    }

    if (DataOffset >= UncompressedSize)
        ReportError("Texture '%s' exceeds the maximum size for this device, and does not have enough mip levels to be resized.\n", FileName);

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
#endif
    
#ifdef PLATFORM_WINDOWS_OR_MAC
	char Work[1024];
	GetBundleFileName(FileName, Work, sizeof(Work));

    gxLoadSprite(Work, Sprite);
#endif
}

void LoadBundleAssetList()
{
#ifdef PLATFORM_IPHONE  
    char Work[1024];
    GetBundleFileName("Converted/Assets.xml", Work, sizeof(Work));
    LoadAssetList(Work, &BundleAssets);
#endif    
}

void LoadAssets()
{
    double StartTime = GetCurrentTime();
    
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//                                                    Sprite Assets                                                                        //
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	LoadSpriteAsset("Assets/white.png", &WhiteSprite);

	LoadSpriteAsset("Assets/wipe-diagonal.png", &WipeDiagonalSprite);
	
    LoadSpriteAsset("Assets/alphabet.png", &AlphabetSprite);

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
	LoadSpriteAsset("Assets/dusty-hop-3b.png", &DustyHop3bSprite);
	LoadSpriteAsset("Assets/dusty-hop-4b.png", &DustyHop4bSprite);
	LoadSpriteAsset("Assets/dusty-hop-4c.png", &DustyHop4cSprite);
	LoadSpriteAsset("Assets/dusty-hop-5b.png", &DustyHop5bSprite);

	LoadSpriteAsset("Assets/dusty-idle-1.png", &DustyIdle1Sprite);
	LoadSpriteAsset("Assets/dusty-idle-2.png", &DustyIdle2Sprite);
	LoadSpriteAsset("Assets/dusty-idle-3.png", &DustyIdle3Sprite);

	LoadSpriteAsset("Assets/dusty-slide-1.png", &DustySlide1Sprite);
	LoadSpriteAsset("Assets/dusty-slide-2.png", &DustySlide2Sprite);
	LoadSpriteAsset("Assets/dusty-slide-3.png", &DustySlide3Sprite);

	LoadSpriteAsset("Assets/dusty-walljump.png", &DustyWallJumpSprite);
	LoadSpriteAsset("Assets/dusty-walljumpb.png", &DustyWallJumpbSprite);
	LoadSpriteAsset("Assets/dusty-walljumpc.png", &DustyWallJumpcSprite);
	LoadSpriteAsset("Assets/dusty-cornerjump.png", &DustyCornerJumpSprite);
	LoadSpriteAsset("Assets/dusty-cornerjumpb.png", &DustyCornerJumpbSprite);
	LoadSpriteAsset("Assets/dusty-cornerjumpc.png", &DustyCornerJumpcSprite);
	LoadSpriteAsset("Assets/dusty-cornerjumpd.png", &DustyCornerJumpdSprite);

	LoadSpriteAsset("Assets/dusty-die.png", &DustyDeathSprite);

    LoadSpriteAsset("Assets/title.png", &LogoSprite);
    
    LoadSpriteAsset("Assets/new-unlock.png", &NewUnlockSprite);
    
    LoadSpriteAsset("Assets/METAL_Small_BRONZE.png", &BronzeMedalSprite);
    LoadSpriteAsset("Assets/METAL_Small_SILVER.png", &SilverMedalSprite);
    LoadSpriteAsset("Assets/METAL_Small_GOLD.png", &GoldMedalSprite);
    
	LoadSpriteAsset("Assets/background-cardboard.png", &BackgroundCardboardSprite);
	LoadSpriteAsset("Assets/background-paper.png", &BackgroundPaperSprite);
	LoadSpriteAsset("Assets/background-fridge.png", &BackgroundFridgeSprite);

	LoadSpriteAsset("Assets/tile-wtf.png", &TileUnknownSprite);
	
	LoadSpriteAsset("Assets/barrel-beans-back.png", &BarrelBackSprite);
	LoadSpriteAsset("Assets/barrel-beans-front.png", &BarrelFrontSprite);
	LoadSpriteAsset("Assets/barrel-nail.png", &BarrelNailSprite);

	LoadSpriteAsset("Assets/fan.png", &FanSprite);

	LoadSpriteAsset("Assets/coin-spin-1.png", &CoinSpin1Sprite);
	LoadSpriteAsset("Assets/coin-spin-2.png", &CoinSpin2Sprite);
	LoadSpriteAsset("Assets/coin-spin-3.png", &CoinSpin3Sprite);
	LoadSpriteAsset("Assets/coin-spin-4.png", &CoinSpin4Sprite);
	LoadSpriteAsset("Assets/coin-spin-5.png", &CoinSpin5Sprite);
	LoadSpriteAsset("Assets/coin-spin-6.png", &CoinSpin6Sprite);

    LoadSpriteAsset("Assets/full-score.png", &FullScoreSprite);
	
    LoadSpriteAsset("Assets/button-pause.png", &ButtonPauseSprite);
    LoadSpriteAsset("Assets/button-play.png", &ButtonPlaySprite);
    LoadSpriteAsset("Assets/button-home.png", &ButtonHomeSprite);
    LoadSpriteAsset("Assets/button-mute.png", &ButtonMuteSprite);
    LoadSpriteAsset("Assets/button-unmute.png", &ButtonUnmuteSprite);
    LoadSpriteAsset("Assets/button-fast-forward.png", &ButtonFastForwardSprite);

    LoadSpriteAsset("Assets/portal.png", &PortalSprite);

	LoadSpriteAsset("Assets/tennisball-spin-1.png", &TennisBallSpin1Sprite);
	LoadSpriteAsset("Assets/tennisball-spin-2.png", &TennisBallSpin2Sprite);
	LoadSpriteAsset("Assets/tennisball-spin-3.png", &TennisBallSpin3Sprite);
	LoadSpriteAsset("Assets/tennisball-spin-4.png", &TennisBallSpin4Sprite);

	LoadSpriteAsset("Assets/gear.png", &GearSprite);
	
    LoadSpriteAsset("Assets/stapler-1.png", &Stapler1Sprite);
    LoadSpriteAsset("Assets/stapler-2.png", &Stapler2Sprite);
    LoadSpriteAsset("Assets/stapler-3.png", &Stapler3Sprite);
    
    LoadSpriteAsset("Assets/jello-1.png", &Jello1Sprite);
    LoadSpriteAsset("Assets/jello-2.png", &Jello2Sprite);
    LoadSpriteAsset("Assets/jello-3.png", &Jello3Sprite);
    LoadSpriteAsset("Assets/jello-4.png", &Jello4Sprite);
    LoadSpriteAsset("Assets/jello-5.png", &Jello5Sprite);
    
    LoadSpriteAsset("Assets/clothespin-1.png", &Clothespin1Sprite);
    LoadSpriteAsset("Assets/clothespin-2.png", &Clothespin2Sprite);
    
	LoadSpriteAsset("Assets/dust-mote.png", &DustMoteSprite);

	LoadSpriteAsset("Assets/firework-rocket.png", &FireWorkRocketSprite);

    LoadSpriteAsset("Assets/flame-1.png", &Flame1Sprite);
	LoadSpriteAsset("Assets/flame-2.png", &Flame2Sprite);

	LoadSpriteAsset("Assets/vacuum-1.png", &Vacuum1Sprite);
	LoadSpriteAsset("Assets/vacuum-2.png", &Vacuum2Sprite);
	LoadSpriteAsset("Assets/vacuum-3.png", &Vacuum3Sprite);
	LoadSpriteAsset("Assets/vacuum-smoke.png", &VacuumSmokeSprite);

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
	LoadSpriteAsset("Assets/icon-locked.png", &IconLockedSprite);

	LoadSpriteAsset("Assets/screen-lose-1.png", &ScreenLose1Sprite);
	LoadSpriteAsset("Assets/screen-lose-2.png", &ScreenLose2Sprite);
	
	LoadSpriteAsset("Assets/screen-lose-ghost.png", &ScreenLoseGhostSprite);
	LoadSpriteAsset("Assets/screen-lose-grave-1.png", &ScreenLoseGrave1Sprite);
	LoadSpriteAsset("Assets/screen-lose-grave-2.png", &ScreenLoseGrave2Sprite);
	
	LoadSpriteAsset("Assets/screen-win-1.png", &ScreenWin1Sprite);
	LoadSpriteAsset("Assets/screen-win-2.png", &ScreenWin2Sprite);

	LoadSpriteAsset("Assets/text-flick-to-jump.png", &TextFlickToJumpSprite);
	LoadSpriteAsset("Assets/text-extra-life.png", &TextExtraLifeSprite);
    LoadSpriteAsset("Assets/text-bonus.png", &TextBonusSprite);
    LoadSpriteAsset("Assets/text-bubble.png", &TextBubbleSprite);

    LoadSpriteAsset("Assets/bubble-minus-1-sec.png", &BubbleMinus1SecSprite);
    LoadSpriteAsset("Assets/bubble-1up.png", &Bubble1UpSprite);

	LoadSpriteAsset("Assets/chapter-title.png", &ChapterTitleSprite);
	LoadSpriteAsset("Assets/chapter.png", &ChapterTextSprite);
	LoadSpriteAsset("Assets/chapter-numbers.png", &ChapterNumbersSprite);
	LoadSpriteAsset("Assets/chapter-complete.png", &ChapterCompleteSprite);
	LoadSpriteAsset("Assets/chapter-time.png", &ChapterTimeSprite);
	LoadSpriteAsset("Assets/chapter-bonus.png", &ChapterBonusSprite);
	LoadSpriteAsset("Assets/chapter-new.png", &ChapterNewSprite);
	LoadSpriteAsset("Assets/chapter-new-record.png", &ChapterNewRecordSprite);
	LoadSpriteAsset("Assets/chapter-record.png", &ChapterRecordSprite);
	LoadSpriteAsset("Assets/chapter-total-time.png", &ChapterTotalTimeSprite);

    LoadSpriteAsset("Assets/text-tap-to-continue.png", &TapToContinueSprite);

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

