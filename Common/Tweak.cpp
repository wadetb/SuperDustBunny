//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Tweak.h"


STweak Tweak;


static void SetTweakDefaults()
{
    #define TWEAK_FLOAT(Name, Default, Description) \
        Tweak.Name = Default;
        
    #define TWEAK_INT(Name, Default, Description) \
        Tweak.Name = Default;
    
    #include "TweakList.h"
    
    #undef TWEAK_FLOAT
    #undef TWEAK_INT
}

static void PrintTweakXML()
{
    LogMessage("\nTWEAK XML:\n\n");
    LogMessage("<Tweaks>\n");
    
    #define TWEAK_FLOAT(Name, Default, Description) \
        LogMessage("  <Tweak Name='%s' Value='%f' Description='%s' />\n", #Name, Tweak.Name, Description);
        
    #define TWEAK_INT(Name, Default, Description) \
        LogMessage("  <Tweak Name='%s' Value='%d' Description='%s' />\n", #Name, Tweak.Name, Description);
        
    #include "TweakList.h"
        
    #undef TWEAK_FLOAT
    #undef TWEAK_INT
    
    LogMessage("</Tweaks>\n\n");
}

static void PrintTweakModifications(STweak* NewTweak)
{
    
    LogMessage("\nTWEAK MODIFICATIONS:\n\n");
    
    #define TWEAK_FLOAT(Name, Default, Description) \
        if (Tweak.Name != NewTweak->Name) \
            LogMessage("%s modified from %f -> %f\n", #Name, Tweak.Name, NewTweak->Name);
        
    #define TWEAK_INT(Name, Default, Description) \
        if (Tweak.Name != NewTweak->Name) \
            LogMessage("%s modified from %d -> %d\n", #Name, Tweak.Name, NewTweak->Name);
        
    #include "TweakList.h"
        
    #undef TWEAK_FLOAT
    #undef TWEAK_INT
    
    LogMessage("\n\n");    
}

void LoadTweaks()
{
	PushErrorContext("While loading tweaks:\n");

    SetTweakDefaults();
 
    STweak NewTweak = Tweak;
    
    char* XML = (char*)LoadAssetFile("Tweaks.xml", NULL, NULL);
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
        ReportError("Failed to parse tweaks file:\n%s", e.what());
    }
    
  	// Get the <Tweaks> node and valiate.
	rapidxml::xml_node<char>* TweaksNode = Document.first_node("Tweaks");
	if (TweaksNode == NULL)
		ReportError("Missing <Tweaks> node.  Check for errors in the XML.");
    
    rapidxml::xml_node<char>* TweakNode = TweaksNode->first_node("Tweak");
    while (TweakNode != NULL)
    {
        rapidxml::xml_attribute<char>* __NameAttr = TweakNode->first_attribute("Name");
        if (__NameAttr == NULL)
            ReportError("Missing 'Name' attribute.  Check for errors in the XML.");
    
        rapidxml::xml_attribute<char>* __ValueAttr = TweakNode->first_attribute("Value");
        if (__ValueAttr == NULL)
            ReportError("Missing 'Value' attribute.  Check for errors in the XML.");

        const char* ConstValue = __ValueAttr->value();
        char* WritableValue = strdup(ConstValue);
        char* Value = WritableValue;

        if (false)
        {
        }
        
        #define TWEAK_FLOAT(Name, Default, Description) \
            else if (strcmp(__NameAttr->value(), #Name) == 0) \
            { \
                NewTweak.Name = (float)atof(Value); \
                goto __Next; \
            }        
                
        #define TWEAK_INT(Name, Default, Description) \
            else if (strcmp(__NameAttr->value(), #Name) == 0) \
            { \
                NewTweak.Name = atoi(Value); \
                goto __Next; \
            }        
                
        #include "TweakList.h"
        
        #undef TWEAK_FLOAT
        #undef TWEAK_INT

        else
        {
            ReportError("Unknown Tweak '%s' with value '%s'.  Check for errors in the XML.", __NameAttr->value(), Value); \
        }
__Next:
        
        free(WritableValue);
        TweakNode = TweakNode->next_sibling("Tweak");
    }

    PrintTweakModifications(&NewTweak);
    
    Tweak = NewTweak;
    
    PrintTweakXML();
    
    PopErrorContext();
}
