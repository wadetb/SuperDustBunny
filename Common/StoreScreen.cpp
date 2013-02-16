//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         //
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "StoreScreen.h"
#include "Dusty.h"
#include "Chapter.h"
#include "Text.h"
#include "Settings.h"
#include "LeaderboardScreen.h"
#include "Input.h"
#import "Music.h"
#include "Keyboard.h"


struct SInventory
{
    const char* Name;
    const char* Description;
    int Cost;
    EDustyHat Hat;
    bool Owned;
    bool Active;
    gxSprite Sprite;
};

struct SStoreScreen
{
    int FirstItem;
    
    bool ItemDisplayed;
    int ActiveItem;
    
    bool ReleasedAtLeastOnce;
    
    bool Pressed;
};


SStoreScreen StoreScreen;

SInventory Inventory[] =
{
    { "Assets/hat-apple.png", "apple\ngives vacuum\npause", 5, DUSTYHAT_APPLE },
    { "Assets/hat-baseball-cap.png", "baseball cap\ndusty is faster!", 5, DUSTYHAT_BASEBALL_CAP },
    { "Assets/hat-bee.png", "bee hat\nlaunch and\nfly faster", 5, DUSTYHAT_BEE },
    { "Assets/hat-bowtie.png", "bowtie\nimmune to\nfire and spikes", 5, DUSTYHAT_BOWTIE },
    { "Assets/hat-clown-nose.png", "clown nose\nprovides light\nin dark places", 30, DUSTYHAT_CLOWN_NOSE },
    { "Assets/hat-crown.png", "crown\nadds extra\ncoin", 5, DUSTYHAT_CROWN },
    { "Assets/hat-disguise.png", "disguise", 5, DUSTYHAT_DISGUISE },
    { "Assets/hat-earmuffs.png", "ear muffs", 5, DUSTYHAT_EARMUFFS },
    { "Assets/hat-earphones.png", "ear phones", 5, DUSTYHAT_EARPHONES },
    { "Assets/hat-eyeglasses.png", "eye glasses", 5, DUSTYHAT_EYEGLASSES },
    { "Assets/hat-eyepatch.png", "eye patch", 5, DUSTYHAT_EYEPATCH },
    { "Assets/hat-flower.png", "flower", 5, DUSTYHAT_FLOWER },
    { "Assets/hat-frog-crown.png", "frog crown", 5, DUSTYHAT_FROG_CROWN },
    { "Assets/hat-graduation.png", "graduation cap", 5, DUSTYHAT_GRADUATION },
    { "Assets/hat-green-feather.png", "green feather\nfall slower", 5, DUSTYHAT_GREEN_FEATHER },
    { "Assets/hat-jester.png", "jester hat", 5, DUSTYHAT_JESTER },
    { "Assets/hat-karate.png", "karate hat\ngives vacuum\npause", 5, DUSTYHAT_KARATE },
    { "Assets/hat-monocle.png", "monocle", 5, DUSTYHAT_MONOCLE },
    { "Assets/hat-nurse.png", "nurse cap\ndusty is faster!", 5, DUSTYHAT_NURSE },
    { "Assets/hat-party.png", "party hat\nlaunch and\nfly faster", 5, DUSTYHAT_PARTY },
    { "Assets/hat-pink-bow.png", "pink bow\nimmune to\nfire and spikes", 5, DUSTYHAT_PINK_BOW },
    { "Assets/hat-pink-shades.png", "pink shades", 5, DUSTYHAT_PINK_SHADES },
    { "Assets/hat-pink-tiara.png", "pink tiara\nadds extra\ncoin", 5, DUSTYHAT_PINK_TIARA },
    { "Assets/hat-pirate.png", "pirate hat", 5, DUSTYHAT_PIRATE },
    { "Assets/hat-purple-feather.png", "yellow feather\nfall slower", 5, DUSTYHAT_PURPLE_FEATHER },
    { "Assets/hat-snorkel.png", "snorkel", 5, DUSTYHAT_SNORKEL },
    { "Assets/hat-sunglasses.png", "sunglasses", 5, DUSTYHAT_SUNGLASSES },
    { "Assets/hat-tophat.png", "top hat", 5, DUSTYHAT_TOPHAT },
    { "Assets/hat-tutu.png", "tutu", 5, DUSTYHAT_TUTU },
    { "Assets/hat-witch.png", "witch hat", 5, DUSTYHAT_WITCH },
    { "Assets/hat-yellow-tophat.png", "yellow top hat", 5, DUSTYHAT_YELLOW_TOPHAT },
};


void LoadInventoryAssets()
{
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
        LoadSpriteAsset(Inventory[i].Name, &Inventory[i].Sprite);
}

gxSprite* GetInventoryHatSprite(int Hat)
{
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
        if (Inventory[i].Hat == Hat)
            return &Inventory[i].Sprite;
    return NULL;
}

int GetActiveInventoryHat()
{
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
        if (Inventory[i].Owned && Inventory[i].Active && Inventory[i].Hat != DUSTYHAT_NONE)
            return Inventory[i].Hat;
    return DUSTYHAT_NONE;
}

void LoadInventory()
{
    PushErrorContext("While loading inventory:");
    
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
    {
        Inventory[i].Owned = false;
        Inventory[i].Active = false;
    }
    
#ifdef PLATFORM_IPHONE_OR_MAC
    @try
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:@"store.inventory"];
        
        NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:filePath];
        if ( !dict )
        {
            PopErrorContext();
            return;
        }
        
        NSNumber *version = [dict objectForKey:@"version"];
        if ( [version intValue] != 1 )
        {
            PopErrorContext();
            return;
        }
        
        NSArray *inventory = [dict objectForKey:@"inventory"];
        
        for (int i = 0; i < [inventory count]; i++)
        {
            NSDictionary *savedItem = [inventory objectAtIndex:i];
            
            NSString *savedName = [savedItem objectForKey:@"name"];
            
            SInventory* Item;
            for (int j = 0; j < ARRAY_COUNT(Inventory); j++)
            {
                if ([savedName isEqualToString:[NSString stringWithUTF8String:Inventory[j].Name]])
                {
                    Item = &Inventory[j];
                    break;
                }
            }
            
            if (Item)
            {
                Item->Owned = [[savedItem objectForKey:@"owned"] boolValue];
                Item->Active = [[savedItem objectForKey:@"active"] boolValue];
            }
        }
    }
    @catch (NSException *e)
    {
        NSLog(@"Caught exception while loading inventory: %@\n", e);
    }
#endif
    
    Dusty.Hat = (EDustyHat)GetActiveInventoryHat();
    
    PopErrorContext();
}

void SaveInventory()
{
    PushErrorContext("While saving inventory:\n");
    
#ifdef PLATFORM_IPHONE_OR_MAC
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent:@"store.inventory"];
    
    NSMutableArray *inventory = [[NSMutableArray alloc] init];
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
    {
        SInventory* Item = &Inventory[i];
        
        NSDictionary *item = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSString stringWithUTF8String:Item->Name], @"name",
                                 [NSNumber numberWithBool:Item->Owned], @"owned",
                                 [NSNumber numberWithBool:Item->Active], @"active", nil];
        [inventory addObject:item];
    }
    
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInt:1], @"version",
                          inventory, @"inventory", nil];
    
    [dict writeToFile:filePath atomically:YES];
    
    [inventory release];
#endif
    
    PopErrorContext();
}

#define STORE_X 60
#define STORE_Y 200
#define STORE_ROWS 3
#define STORE_COLS 3
#define STORE_ROW_SZ 200
#define STORE_COL_SZ ((768-STORE_X*2)/STORE_COLS)
#define STORE_ITEM_SCALE 2.0

void InitStoreScreen()
{
    LoadInventory();

    ResetLightState();
    
    InitDusty();
    
    SetDustyState_IntroStand();
    Dusty.FloatX = 384;
    Dusty.FloatY = LitScreenHeight - 160;
    Dusty.NoCollision = true;
    RemoteControl.Enabled = true;
    
    ScrollX = 0;
    ScrollY = 0;
    
    StoreScreen.ReleasedAtLeastOnce = false;
    StoreScreen.ActiveItem = 0;
    StoreScreen.FirstItem = 0;
    StoreScreen.ItemDisplayed = false;
    
    StopMusicTrack(MUSIC_CHAPTER);
    QueueMusicTrack(MUSIC_CHAPTER, &TitleScreenMusic);
}

void DisplayStoreScreen()
{
    AddLitSpriteSizedAlpha(LIGHTLIST_BACKGROUND, &ScreenStoreSprite, 0, 0, 768, LitScreenHeight, 1.0f);

    AddLitSprite(LIGHTLIST_VACUUM, &ScreenCoinBackgroundSprite, 0, LitScreenHeight - 120);
    AddLitSpriteScaled(LIGHTLIST_VACUUM, &CoinIconSprite, 240, LitScreenHeight - 110, 0.65f*1.0f, 0.65f*1.0f);
    char Work[20];
    snprintf(Work, sizeof(Work), "x%d", Settings.Lives);
    DisplayString(LIGHTLIST_VACUUM, Work, 0, 350, LitScreenHeight - 80, 1.1f);
    
    for (int i = 0; i < STORE_ROWS * STORE_COLS; i++)
    {
        if (StoreScreen.FirstItem + i >= ARRAY_COUNT(Inventory))
            break;
        
        int Row = i / STORE_ROWS;
        int Col = i % STORE_ROWS;
        
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &Inventory[StoreScreen.FirstItem + i].Sprite,
                                        STORE_X+Col*STORE_COL_SZ + STORE_COL_SZ/2,
                                        STORE_Y+Row*STORE_ROW_SZ + STORE_ROW_SZ/2, STORE_ITEM_SCALE, 1.0f);
        if (Inventory[StoreScreen.FirstItem + i].Owned)
            AddLitSpriteScaled(LIGHTLIST_FOREGROUND, &CheckMarkSprite, STORE_X+Col*STORE_COL_SZ, STORE_Y+Row*STORE_ROW_SZ, 2.0f, 2.0f);
    }

    if (StoreScreen.ItemDisplayed)
    {
        AddLitSprite(LIGHTLIST_VACUUM, &LeaderboardBackgroundSprite, 0, 0);
        
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &Inventory[StoreScreen.ActiveItem].Sprite, 384, 250, 4.0f, 1.0f);

        DisplayMultilineStringAlpha(LIGHTLIST_WIPE, Inventory[StoreScreen.ActiveItem].Description, FORMAT_CENTER_X, 384, 400, 0.8f, 1.0f);

        AddLitSpriteScaled(LIGHTLIST_WIPE, &CoinIconSprite, 240, 560, 0.65f*1.0f, 0.65f*1.0f);
        snprintf(Work, sizeof(Work), "x%d", Inventory[StoreScreen.ActiveItem].Cost);
        DisplayString(LIGHTLIST_WIPE, Work, 0, 350, 590, 1.1f);

        if (Inventory[StoreScreen.ActiveItem].Owned)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenWearItSprite, 384, 800, 1.0f, StoreScreen.Pressed ? 0.5f : 1.0f);
        else
        {
            if (Inventory[StoreScreen.ActiveItem].Cost <= Settings.Lives)
                AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenBuyItSprite, 384, 800, 1.0f, StoreScreen.Pressed ? 0.5f : 1.0f);
            else
                AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenBuyItSprite, 384, 800, 1.0f, 0.5f);
        }
    }
    else
    {
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenGoBackSprite, 60, 60, 1.0f, 1.0f);
        //AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenBuyCoinsSprite, 620, 60, 1.0f, 1.0f);
        if (StoreScreen.FirstItem > 0)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonFastForwardSprite, 192-64, LitScreenHeight-110, -1.1f, 1.0f);
        if (StoreScreen.FirstItem + STORE_ROWS * STORE_COLS < ARRAY_COUNT(Inventory))
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonFastForwardSprite, 768-192+64, LitScreenHeight-110, 1.1f, 1.0f);
    }
    
    DisplayDusty();
}

void UpdateStoreScreen()
{    
    UpdateDusty();

    if (!StoreScreen.ReleasedAtLeastOnce)
    {
        if (!msButton1)
            StoreScreen.ReleasedAtLeastOnce = true;
        return;
    }
    
    if (StoreScreen.ItemDisplayed)
    {
        if (!msButton1 && msOldButton1)
        {
            if (msX < 200 && msY < 200)
            {
                StoreScreen.ItemDisplayed = false;
                StoreScreen.ReleasedAtLeastOnce = false;
            }
        }

        StoreScreen.Pressed = false;
        if (msButton1)
        {
            if (msY >= 600 && msY <= 900)
            {
                StoreScreen.Pressed = true;
            }
        }
        
        if (!msButton1 && msOldButton1)
        {
            if (msY >= 600 && msY <= 900)
            {
                if (Inventory[StoreScreen.ActiveItem].Owned)
                {
                    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
                        if (Inventory[i].Hat != DUSTYHAT_NONE && Inventory[i].Active)
                            Inventory[i].Active = false;
                    Inventory[StoreScreen.ActiveItem].Active = true;
                    Dusty.Hat = Inventory[StoreScreen.ActiveItem].Hat;
                    SaveInventory();
                    StoreScreen.ItemDisplayed = false;
                    StoreScreen.ReleasedAtLeastOnce = false;
                }
                else
                {
                    if (Settings.Lives >= Inventory[StoreScreen.ActiveItem].Cost)
                    {
                        Settings.Lives -= Inventory[StoreScreen.ActiveItem].Cost;
                        Inventory[StoreScreen.ActiveItem].Owned = true;
                        for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
                            if (Inventory[i].Hat != DUSTYHAT_NONE && Inventory[i].Active)
                                Inventory[i].Active = false;
                        Inventory[StoreScreen.ActiveItem].Active = true;
                        Dusty.Hat = Inventory[StoreScreen.ActiveItem].Hat;
                        SaveSettings();
                        SaveInventory();
                        StoreScreen.ItemDisplayed = false;
                        StoreScreen.ReleasedAtLeastOnce = false;
                    }
                }
            }
        }
    }
    else
    {
        if (!msButton1 && msOldButton1)
        {
            if (msX < 200 && msY <= 200)
            {
                SetGameState_StartScreen();
                return;
            }
            
            if (msX >= STORE_X && msX <= STORE_X + STORE_COLS * STORE_COL_SZ &&
                msY >= STORE_Y && msY <= STORE_Y + STORE_ROWS * STORE_ROW_SZ)
            {
                int Col = (msX - STORE_X) / STORE_COL_SZ;
                int Row = (msY - STORE_Y) / STORE_ROW_SZ;
                
                if (StoreScreen.FirstItem + Row*STORE_COLS + Col < ARRAY_COUNT(Inventory))
                {
                    StoreScreen.ActiveItem = StoreScreen.FirstItem + Row*STORE_COLS + Col;
                    StoreScreen.ItemDisplayed = true;
                    StoreScreen.ReleasedAtLeastOnce = false;
                }
            }
            
            if (msY >= 800)
            {
                if (msX <= 384)
                {
                    if (StoreScreen.FirstItem > 0)
                        StoreScreen.FirstItem -= STORE_ROWS * STORE_COLS;
                }
                else
                {
                    if (StoreScreen.FirstItem + STORE_ROWS * STORE_COLS < ARRAY_COUNT(Inventory))
                        StoreScreen.FirstItem += STORE_ROWS * STORE_COLS;
                }
            }
        }
    }
}
