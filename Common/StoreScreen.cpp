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
#include "Music.h"


struct SInventory
{
    const char* Name;
    const char* Description;
    const char* Power;
    int Cost;
    EDustyHat Hat;
    bool Owned;
    bool Active;
    gxSprite Sprite;
};

struct SStoreScreen
{
    float HopTimer;
    float HopDelay;
    
    int FirstItem;
    
    bool ItemDisplayed;
    int ActiveItem;
    
    bool ReleasedAtLeastOnce;
    
    bool Pressed;
};


#define STORE_X 60
#define STORE_Y 80
#define STORE_COLS 3
#define STORE_ROW_SZ 260
#define STORE_COL_SZ ((768-STORE_X*2)/STORE_COLS)
#define STORE_ITEM_SCALE 2.0
#define STORE_ITEMS_PER_PAGE (STORE_ROWS*STORE_COLS)

int STORE_ROWS;


SStoreScreen StoreScreen;

SInventory Inventory[] =
{
//Tier 1
    { "Assets/hat-no-hat.png", "no hat", "", 0, DUSTYHAT_NONE },
    { "Assets/hat-graduation.png", "graduation cap", "higher learning", 0, DUSTYHAT_GRADUATION },
    { "Assets/hat-apple.png", "apple", "gives vacuum a pause", 5, DUSTYHAT_APPLE },
    { "Assets/hat-bowtie.png", "bowtie", "immune to fire\nand spikes", 5, DUSTYHAT_BOWTIE },
    { "Assets/hat-pink-bow.png", "pink bow", "immune to\nfire and spikes", 5, DUSTYHAT_PINK_BOW },
    { "Assets/hat-clown-nose.png", "reindeer nose", "provides light\nin dark places", 5, DUSTYHAT_CLOWN_NOSE },
    { "Assets/hat-purple-feather.png", "yellow feather", "fall slower", 5, DUSTYHAT_PURPLE_FEATHER },
    { "Assets/hat-baseball-cap.png", "baseball cap", "dusty is faster!", 10, DUSTYHAT_BASEBALL_CAP },
    { "Assets/hat-bee.png", "bee hat", "launch and\nfly faster", 10, DUSTYHAT_BEE },
    { "Assets/hat-party.png", "party hat", "launch and\nfly faster", 10, DUSTYHAT_PARTY },
    //    { "Assets/hat-green-feather.png", "green feather", "fall slower", 10, DUSTYHAT_GREEN_FEATHER },
    //    { "Assets/hat-nurse.png", "nurse cap", "dusty is faster!", 10, DUSTYHAT_NURSE },
    //    { "Assets/hat-karate.png", "karate hat", "gives vacuum\npause", 5, DUSTYHAT_KARATE },
//Tier 2
    //{ "Assets/hat-crown.png", "crown", "adds extra\ncoin", 25, DUSTYHAT_CROWN },
    //    { "Assets/hat-pink-tiara.png", "pink tiara", "adds extra\ncoin", 25, DUSTYHAT_PINK_TIARA },
    //{ "Assets/hat-disguise.png", "disguise\nvacuum retreats longer\ndusty is faster!", 25, DUSTYHAT_DISGUISE },
    //    { "Assets/hat-earmuffs.png", "ear muffs\nvacuum pauses longer\nimmuen to fire\nand spikes", 25, DUSTYHAT_EARMUFFS },
    //    { "Assets/hat-flower.png", "flower", 25, DUSTYHAT_FLOWER },
    //    { "Assets/hat-witch.png", "witch hat", 25, DUSTYHAT_WITCH },
    //    { "Assets/hat-jester.png", "jester hat", 25, DUSTYHAT_JESTER },
    //    { "Assets/hat-pirate.png", "pirate hat", 25, DUSTYHAT_PIRATE },
    //    { "Assets/hat-snorkel.png", "snorkel", 25, DUSTYHAT_SNORKEL },
    //    { "Assets/hat-tophat.png", "top hat", 25, DUSTYHAT_TOPHAT },
    //    { "Assets/hat-tutu.png", "tutu", 25, DUSTYHAT_TUTU },
    //    { "Assets/hat-yellow-tophat.png", "yellow top hat", 35, DUSTYHAT_YELLOW_TOPHAT },
//Tier 3
//    { "Assets/hat-sunglasses.png", "sunglasses", "jump faster!\nfall slower\nimmune to injury", 50, DUSTYHAT_SUNGLASSES },
//    { "Assets/hat-pink-shades.png", "pink shades", "jump faster!\nfall slower\nimmune to injury", 50, DUSTYHAT_PINK_SHADES },
//    { "Assets/hat-monocle.png", "monocle", "proides light\nvacuum retreats longer\nlaunch faster", 50, DUSTYHAT_MONOCLE },
//    { "Assets/hat-earphones.png", "ear phones", "launch faster\njump faster\nvacuum retreats longer", 50, DUSTYHAT_EARPHONES },
//    { "Assets/hat-eyepatch.png", "eye patch", "proides light\nvacuum retreats longer\nlaunch faster", 50, DUSTYHAT_EYEPATCH },
//    { "Assets/hat-frog-crown.png", "frog crown", "adds extra coin\nprovides light\nimmune to injury", 60, DUSTYHAT_FROG_CROWN },
//    { "Assets/hat-eyeglasses.png", "eye glasses", "immune to injury\nfall slower\nadds extra coin", 60, DUSTYHAT_EYEGLASSES },    
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

void InitStoreScreen()
{
    LoadInventory();

    ResetLightState();

#ifdef PLATFORM_MAC
    STORE_ROWS = 2;
#else
    if (ScreenAspect == ASPECT_IPAD)
        STORE_ROWS = 2;
    else
        STORE_ROWS = 3;
#endif

    InitDusty();
    
    SetDustyState_IntroStand();
    Chapter.EndX = -100000;
    Chapter.EndY = -100000;
    Dusty.CollideMaterial = MATERIAL_NORMAL;
    Dusty.FloatX = STORE_X+(STORE_COLS-1)*STORE_COL_SZ + STORE_COL_SZ/2-30;
    Dusty.FloatY = STORE_Y+(STORE_ROWS)*STORE_ROW_SZ + STORE_ROW_SZ-50-20;
    Dusty.NoCollision = true;
    RemoteControl.Enabled = true;
    
    ScrollX = 0;
    ScrollY = 0;
    
    StoreScreen.ReleasedAtLeastOnce = false;
    StoreScreen.ActiveItem = 0;
    StoreScreen.FirstItem = 0;
    StoreScreen.ItemDisplayed = false;
    
    GiveHat(DUSTYHAT_NONE);
    
    StopMusicTrack(MUSIC_CHAPTER);
    QueueMusicTrack(MUSIC_CHAPTER, &TitleScreenMusic);
}

void DisplayStoreScreen()
{
    AddLitSpriteAlpha(LIGHTLIST_BACKGROUND, &ScreenStoreSprite, 768/2 - ScreenStoreSprite.width/2, -20, 1);
    AddLitSpriteAlpha(LIGHTLIST_BACKGROUND, &ScreenStoreSprite, 768/2 - ScreenStoreSprite.width/2, ScreenStoreSprite.height-20, 1);
    //AddLitSpriteAlpha(LIGHTLIST_BACKGROUND, &ScreenStoreSprite, 768/2 - ScreenStoreSprite.width/2, LitScreenHeight/2 - ScreenStoreSprite.height/2, 1);
    //AddLitSpriteSizedAlpha(LIGHTLIST_BACKGROUND, &ScreenStoreSprite, 0, 0, 768, LitScreenHeight, 1.0f);

    AddLitSprite(LIGHTLIST_VACUUM, &ScreenCoinBackgroundSprite, 0, LitScreenHeight - 120);
    AddLitSpriteScaled(LIGHTLIST_VACUUM, &CoinIconSprite, 240, LitScreenHeight - 110, 0.65f*1.0f, 0.65f*1.0f);
    char Work[20];
    snprintf(Work, sizeof(Work), "x%d", Settings.Lives);
    DisplayString(LIGHTLIST_VACUUM, Work, 0, 350, LitScreenHeight - 80, 1.1f);
    
    for (int i = 0; i < STORE_ITEMS_PER_PAGE; i++)
    {
        if (StoreScreen.FirstItem + i >= ARRAY_COUNT(Inventory))
            break;
        
        int Row = i / STORE_COLS;
        int Col = i % STORE_COLS;
        
        if (Inventory[StoreScreen.FirstItem + i].Owned)
        {
            if (Inventory[StoreScreen.FirstItem].Hat == DUSTYHAT_NONE)                
                AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &Inventory[StoreScreen.FirstItem + i].Sprite,
                                                STORE_X+Col*STORE_COL_SZ + STORE_COL_SZ/2,
                                                STORE_Y+Row*STORE_ROW_SZ + STORE_ROW_SZ/2, STORE_ITEM_SCALE*0.5f, 1.0f);
            else
                AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND, &Inventory[StoreScreen.FirstItem + i].Sprite,
                                                STORE_X+Col*STORE_COL_SZ + STORE_COL_SZ/2,
                                                STORE_Y+Row*STORE_ROW_SZ + STORE_ROW_SZ/2, STORE_ITEM_SCALE, 1.0f);
//            AddLitSpriteScaled(LIGHTLIST_FOREGROUND, &CheckMarkSprite, STORE_X+Col*STORE_COL_SZ, STORE_Y+Row*STORE_ROW_SZ, 2.0f, 2.0f);
        }
        else
        {
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_FOREGROUND_GRAYSCALE, &Inventory[StoreScreen.FirstItem + i].Sprite,
                                            STORE_X+Col*STORE_COL_SZ + STORE_COL_SZ/2,
                                            STORE_Y+Row*STORE_ROW_SZ + STORE_ROW_SZ/2, STORE_ITEM_SCALE*0.75f, 0.75f);
        }
    }

    if (StoreScreen.ItemDisplayed)
    {
        AddLitSprite(LIGHTLIST_VACUUM, &LeaderboardBackgroundSprite, 0, 0);
        
        if (Inventory[StoreScreen.ActiveItem].Hat == DUSTYHAT_NONE)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &Inventory[StoreScreen.ActiveItem].Sprite, 384, 250, 1.5f, 1.0f);
        else
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &Inventory[StoreScreen.ActiveItem].Sprite, 384, 250, 3.0f, 1.0f);

        DisplayMultilineStringAlpha(LIGHTLIST_WIPE, Inventory[StoreScreen.ActiveItem].Description, FORMAT_CENTER_X, 384, 390, 1.0f, 1.0f);

        DisplayMultilineStringAlpha(LIGHTLIST_WIPE, Inventory[StoreScreen.ActiveItem].Power, FORMAT_CENTER_X, 384, 460, 0.6f, 1.0f);

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
        AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenGoBackSprite, 70, 70, 0.5f, 1.0f);
        //AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ScreenBuyCoinsSprite, 620, 60, 1.0f, 1.0f);
        if (StoreScreen.FirstItem > 0)
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonFastForwardSprite, 75, LitScreenHeight-80, -1.0f, 1.0f);
        if (StoreScreen.FirstItem + STORE_ITEMS_PER_PAGE < ARRAY_COUNT(Inventory))
            AddLitSpriteCenteredScaledAlpha(LIGHTLIST_WIPE, &ButtonFastForwardSprite, 768-75, LitScreenHeight-80, 1.0, 1.0f);
    }
    
    DisplayDusty();
}

static void GiveStoreItem(int Item)
{
    Inventory[Item].Owned = true;
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
        if (Inventory[i].Hat != DUSTYHAT_NONE && Inventory[i].Active)
            Inventory[i].Active = false;
    Inventory[Item].Active = true;
    Dusty.Hat = Inventory[Item].Hat;
    SaveSettings();
    SaveInventory();    
}

bool HasHat(int Hat)
{
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
    {
        if (Inventory[i].Hat == Hat)
        {
            return Inventory[i].Owned;
        }
    }
    return false;
}

void GiveHat(int Hat)
{
    for (int i = 0; i < ARRAY_COUNT(Inventory); i++)
        if (Inventory[i].Hat == Hat)
        {
            GiveStoreItem(i);
            return;
        }
}

void UpdateStoreScreen()
{
    StoreScreen.HopTimer += 1.0f/60.0f;
    if (StoreScreen.HopTimer > 0.25f)
    {
        RemoteControl.MoveRight = false;
        RemoteControl.MoveLeft = false;
    }
    if (StoreScreen.HopTimer > StoreScreen.HopDelay)
    {
        float Choice = Random(0.0f, 1.0f);
        RemoteControl.MoveLeft = false;
        RemoteControl.MoveRight = false;
        if (Choice < 0.3 && Dusty.FloatX < STORE_X+STORE_COL_SZ*(STORE_COLS-1))
            RemoteControl.MoveRight = true;
        else if (Choice < 0.6 && Dusty.FloatX > STORE_X+STORE_COL_SZ*1)
            RemoteControl.MoveLeft = true;
        StoreScreen.HopDelay = Random(0.5f, 5.0f);
        StoreScreen.HopTimer = 0;
    }
    if (Dusty.FloatX < STORE_X + STORE_COL_SZ/2)
        Dusty.FloatX = STORE_X + STORE_COL_SZ/2;
    if (Dusty.FloatX > STORE_X + STORE_COLS*STORE_COL_SZ - STORE_COL_SZ/2)
        Dusty.FloatX = STORE_X + STORE_COLS*STORE_COL_SZ - STORE_COL_SZ/2;
    
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
                        GiveStoreItem(StoreScreen.ActiveItem);
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
            if (msX < 100 && msY <= 100)
            {
                SetGameState_StartScreen();
                return;
            }
            
            if (msY > STORE_Y + STORE_ROWS * STORE_ROW_SZ)
            {
                StoreScreen.HopDelay = 0;
            }
            
            if (msX >= STORE_X && msX <= STORE_X + STORE_COLS * STORE_COL_SZ &&
                msY >= STORE_Y && msY <= STORE_Y + STORE_ROWS * STORE_ROW_SZ)
            {
                int Col = (msX - STORE_X) / STORE_COL_SZ;
                int Row = (msY - STORE_Y) / STORE_ROW_SZ;
                
                int Item = Row*STORE_COLS + Col;

                if (Item < STORE_ITEMS_PER_PAGE && StoreScreen.FirstItem + Item < ARRAY_COUNT(Inventory))
                {
                    StoreScreen.ActiveItem = StoreScreen.FirstItem + Row*STORE_COLS + Col;
                    StoreScreen.ItemDisplayed = true;
                    StoreScreen.ReleasedAtLeastOnce = false;
                }
            }
            
            if (msY > LitScreenHeight - 200)
            {
                if (msX <= 300)
                {
                    if (StoreScreen.FirstItem > 0)
                        StoreScreen.FirstItem -= STORE_ROWS * STORE_COLS;
                }
                else if (msX >= 768-300)
                {
                    if (StoreScreen.FirstItem + STORE_ROWS * STORE_COLS < ARRAY_COUNT(Inventory))
                        StoreScreen.FirstItem += STORE_ROWS * STORE_COLS;
                }
            }
        }
    }
}
