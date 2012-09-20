//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         //
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#ifndef STORESCREEN_H
#define STORESCREEN_H

void LoadInventoryAssets();
gxSprite* GetInventoryHatSprite(int Hat);
int GetActiveInventoryHat();

void LoadInventory();
void SaveInventory();

void InitStoreScreen();
void UpdateStoreScreen();
void DisplayStoreScreen();

#endif
