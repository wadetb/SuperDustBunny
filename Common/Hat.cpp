//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         //
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Hat.h"
#include "Chapter.h"


gxSprite* DustyHatSprites[DUSTYHAT_COUNT];

SDustyHatOffset DustyHatAdjustments[DUSTYHAT_COUNT] =
{
    { 0, 0, 0, 1 },                     //    DUSTYHAT_NONE, No Special Effect
    { 0, 0, -20, 1 },                   //    DUSTYHAT_APPLE, Increases Vacuum Retreat time by x amount.
    { 0, 5, -5, 1 },                    //    DUSTYHAT_BASEBALL_CAP, Speed up Dusty
    { 0, 15, -15, 1 },                  //    DUSTYHAT_BEE, Launch from barrels faster
    { 0, 15, 60, 1 },                     //    DUSTYHAT_BOWTIE, Dusty is immune to getting hurt by spikes,flames
    { 0, 30, 20, 1 },                   //    DUSTYHAT_CLOWN_NOSE, Provides extra light on dark levels.
    { 0, -5, -17, 1 },                  //    DUSTYHAT_CROWN, Adds one extra coin
    { 0, 12, 10, 1 },                   //    DUSTYHAT_DISGUISE
    { 0, 0, 10, 1 },                    //    DUSTYHAT_EARMUFFS
    { 0, -5, 8, 1 },                    //    DUSTYHAT_EARPHONES
    { 0, 0, 12, 1 },                    //    DUSTYHAT_EYEGLASSES,
    { 0, 0, 15, 1 },                    //    DUSTYHAT_EYEPATCH,
    { 0, -10, 0, 1 },                   //    DUSTYHAT_FLOWER,
    { 0, 0, -15, 1 },                   //    DUSTYHAT_FROG_CROWN,
    { 0, -5, 5, 1 },                    //    DUSTYHAT_GRADUATION,
    { 0, -17, 0, 1 },                   //    DUSTYHAT_GREEN_FEATHER, Slows Dusty's Fall
    { 0, -15, -10, 1 },                 //    DUSTYHAT_JESTER,
    { 0, -10, 28, 1 },                  //    DUSTYHAT_KARATE, Increases Vacuum Retreat time by x amount.
    { 0, 8, 23, 1 },                    //    DUSTYHAT_MONOCLE,
    { 0, 0, 0, 1 },                     //    DUSTYHAT_NURSE, Speed up Dusty
    { 0, 0, 0, 1 },                     //    DUSTYHAT_PARTY, Launch from barrels faster
    { 0, 0, 0, 1 },                     //    DUSTYHAT_PINK_BOW, Dusty is immune to getting hurt by spikes,flames
    { 0, 0, 15, 1 },                    //    DUSTYHAT_PINK_SHADES,
    { 0, 0, -5, 1 },                    //    DUSTYHAT_PINK_TIARA, Adds one extra coin
    { 0, -5, -10, 1 },                  //    DUSTYHAT_PIRATE,
    { 0, -25, 2, 1 },                   //    DUSTYHAT_PURPLE_FEATHER, Slows Dusty's Fall
    { 0, 0, 0, 1 },                     //    DUSTYHAT_SNORKEL,
    { 0, 0, 14, 1 },                    //    DUSTYHAT_SUNGLASSES,
    { 0, -4, -18, 1 },                  //    DUSTYHAT_TOPHAT,
    { 0, -10, 70, 1 },                  //    DUSTYHAT_TUTU,
    { 0, -25, 0, 1 },                   //    DUSTYHAT_WITCH,
    { 0, -5, -15, 1 },                  //    DUSTYHAT_YELLOW_TOPHAT,
};


SDustyHatOffset DustyHatOffsets[DUSTYSPRITE_COUNT] =
{
    { -17, 137, 124 },
    { -30, 152, 58 },
    { -60, 100, 32 },
    { -5, 121, 53 },
    { 36, 204, 133 },
    { -32, 152, 58 },
    { -28, 152, 58 },
    { -62, 100, 32 },
    { -3, 121, 53 },
    { -7, 121, 53 },
    { 36, 204, 133 },
    { -10, 140, 126 },
    { -20, 132, 114 },
    { -9, 140, 121 },
    { -43, 121, 133 },
    { -43, 121, 133 },
    { -43, 121, 133 },
    { -35, 140, 90 },
    { -35, 140, 90 },
    { -35, 140, 90 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 115, 114 },
    { 0, 115, 114 }, // FIXME
};


int DustyHatAdjX = 0;
int DustyHatAdjY = 0;


static void SaveHats()
{
    printf("<Hats>\n");
    
   // printf("  <Hat Name='%s' Cost='%d' Description='%d' Sprite='%s' />\n", #Name, Tweak.Name, Description);
    
    printf("</Hats>\n\n");
}

static void LoadHats()
{
	PushErrorContext("While loading hats:\n");
    
    char* XML = (char*)LoadAssetFile("Hats.xml", NULL, NULL);
    if (!XML)
    {
        PopErrorContext();
        return;
    }
    
    // Parse the XML text buffer into a Document hierarchy.
	rapidxml::xml_document<> Document;
    try
    {
        Document.parse<0>(XML);
    }
    catch (rapidxml::parse_error e)
    {
        ReportError("Failed to parse hats file:\n%s", e.what());
    }
    
  	// Get the <Tweaks> node and valiate.
	rapidxml::xml_node<char>* HatsNode = Document.first_node("Hats");
	if (HatsNode == NULL)
		ReportError("Missing <Tweaks> node.  Check for errors in the XML.");
    
    rapidxml::xml_node<char>* HatNode = HatsNode->first_node("Hat");
    while (HatNode != NULL)
    {
        rapidxml::xml_attribute<char>* __NameAttr = HatNode->first_attribute("Name");
        if (__NameAttr == NULL)
            ReportError("Missing 'Name' attribute.  Check for errors in the XML.");
        
        rapidxml::xml_attribute<char>* __ValueAttr = HatNode->first_attribute("Description");
        if (__ValueAttr == NULL)
            ReportError("Missing 'Value' attribute.  Check for errors in the XML.");
        
        HatNode = HatNode->next_sibling("Hat");
    }
    
    PopErrorContext();
}

