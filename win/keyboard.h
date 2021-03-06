/*--------------------------------------------------------------------------------------------------------

  Keyboard Library v1.0 by Wade Brainerd

  Library Overview:

  Constants:
    KB_XXXXXX               Scan code value for each key on the keyboard, for use with the kbKeyStatus array.

  Global Variables:
    kbKeyStatus             Array containing the status of each key on the keyboard, updated by kbUpdateKeys..
    kbOldKeyStatus          Array containing the status of each key on the keyboard -before- the last call to kbUpdateKeys.

  Functions:
    kbInit                  Captures the keyboard from Windows.
    kbDeinit                Restores the keyboard to Windows.

    kbUpdateKeys            Reads the status of all the keys from the hardware.

--------------------------------------------------------------------------------------------------------*/
#ifndef KEYBD_H
#define KEYBD_H

/*--------------------------------------------------------------------------------------------------------

  Keyboard code definitions

--------------------------------------------------------------------------------------------------------*/
#define KB_ESCAPE          0x01
#define KB_1               0x02
#define KB_2               0x03
#define KB_3               0x04
#define KB_4               0x05
#define KB_5               0x06
#define KB_6               0x07
#define KB_7               0x08
#define KB_8               0x09
#define KB_9               0x0A
#define KB_0               0x0B
#define KB_MINUS           0x0C    /* - on main keyboard */
#define KB_EQUALS          0x0D
#define KB_BACK            0x0E    /* backspace */
#define KB_TAB             0x0F
#define KB_Q               0x10
#define KB_W               0x11
#define KB_E               0x12
#define KB_R               0x13
#define KB_T               0x14
#define KB_Y               0x15
#define KB_U               0x16
#define KB_I               0x17
#define KB_O               0x18
#define KB_P               0x19
#define KB_LBRACKET        0x1A
#define KB_RBRACKET        0x1B
#define KB_RETURN          0x1C    /* Enter on main keyboard */
#define KB_LCONTROL        0x1D
#define KB_A               0x1E
#define KB_S               0x1F
#define KB_D               0x20
#define KB_F               0x21
#define KB_G               0x22
#define KB_H               0x23
#define KB_J               0x24
#define KB_K               0x25
#define KB_L               0x26
#define KB_SEMICOLON       0x27
#define KB_APOSTROPHE      0x28
#define KB_GRAVE           0x29    /* accent grave */
#define KB_LSHIFT          0x2A
#define KB_BACKSLASH       0x2B
#define KB_Z               0x2C
#define KB_X               0x2D
#define KB_C               0x2E
#define KB_V               0x2F
#define KB_B               0x30
#define KB_N               0x31
#define KB_M               0x32
#define KB_COMMA           0x33
#define KB_PERIOD          0x34    /* . on main keyboard */
#define KB_SLASH           0x35    /* / on main keyboard */
#define KB_RSHIFT          0x36
#define KB_MULTIPLY        0x37    /* * on numeric keypad */
#define KB_LMENU           0x38    /* left Alt */
#define KB_SPACE           0x39
#define KB_CAPITAL         0x3A
#define KB_F1              0x3B
#define KB_F2              0x3C
#define KB_F3              0x3D
#define KB_F4              0x3E
#define KB_F5              0x3F
#define KB_F6              0x40
#define KB_F7              0x41
#define KB_F8              0x42
#define KB_F9              0x43
#define KB_F10             0x44
#define KB_NUMLOCK         0x45
#define KB_SCROLL          0x46    /* Scroll Lock */
#define KB_NUMPAD7         0x47
#define KB_NUMPAD8         0x48
#define KB_NUMPAD9         0x49
#define KB_SUBTRACT        0x4A    /* - on numeric keypad */
#define KB_NUMPAD4         0x4B
#define KB_NUMPAD5         0x4C
#define KB_NUMPAD6         0x4D
#define KB_ADD             0x4E    /* + on numeric keypad */
#define KB_NUMPAD1         0x4F
#define KB_NUMPAD2         0x50
#define KB_NUMPAD3         0x51
#define KB_NUMPAD0         0x52
#define KB_DECIMAL         0x53    /* . on numeric keypad */
#define KB_OEM_102         0x56    /* < > | on UK/Germany keyboards */
#define KB_F11             0x57
#define KB_F12             0x58
#define KB_F13             0x64    /*                     (NEC PC98) */
#define KB_F14             0x65    /*                     (NEC PC98) */
#define KB_F15             0x66    /*                     (NEC PC98) */
#define KB_KANA            0x70    /* (Japanese keyboard)            */
#define KB_ABNT_C1         0x73    /* / ? on Portugese (Brazilian) keyboards */
#define KB_CONVERT         0x79    /* (Japanese keyboard)            */
#define KB_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define KB_YEN             0x7D    /* (Japanese keyboard)            */
#define KB_ABNT_C2         0x7E    /* Numpad . on Portugese (Brazilian) keyboards */
#define KB_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define KB_PREVTRACK       0x90    /* Previous Track (KB_CIRCUMFLEX on Japanese keyboard) */
#define KB_AT              0x91    /*                     (NEC PC98) */
#define KB_COLON           0x92    /*                     (NEC PC98) */
#define KB_UNDERLINE       0x93    /*                     (NEC PC98) */
#define KB_KANJI           0x94    /* (Japanese keyboard)            */
#define KB_STOP            0x95    /*                     (NEC PC98) */
#define KB_AX              0x96    /*                     (Japan AX) */
#define KB_UNLABELED       0x97    /*                        (J3100) */
#define KB_NEXTTRACK       0x99    /* Next Track */
#define KB_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define KB_RCONTROL        0x9D
#define KB_MUTE            0xA0    /* Mute */
#define KB_CALCULATOR      0xA1    /* Calculator */
#define KB_PLAYPAUSE       0xA2    /* Play / Pause */
#define KB_MEDIASTOP       0xA4    /* Media Stop */
#define KB_VOLUMEDOWN      0xAE    /* Volume - */
#define KB_VOLUMEUP        0xB0    /* Volume + */
#define KB_WEBHOME         0xB2    /* Web home */
#define KB_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define KB_DIVIDE          0xB5    /* / on numeric keypad */
#define KB_SYSRQ           0xB7
#define KB_RMENU           0xB8    /* right Alt */
#define KB_PAUSE           0xC5    /* Pause */
#define KB_HOME            0xC7    /* Home on arrow keypad */
#define KB_UP              0xC8    /* UpArrow on arrow keypad */
#define KB_PRIOR           0xC9    /* PgUp on arrow keypad */
#define KB_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define KB_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define KB_END             0xCF    /* End on arrow keypad */
#define KB_DOWN            0xD0    /* DownArrow on arrow keypad */
#define KB_NEXT            0xD1    /* PgDn on arrow keypad */
#define KB_INSERT          0xD2    /* Insert on arrow keypad */
#define KB_DELETE          0xD3    /* Delete on arrow keypad */
#define KB_LWIN            0xDB    /* Left Windows key */
#define KB_RWIN            0xDC    /* Right Windows key */
#define KB_APPS            0xDD    /* AppMenu key */
#define KB_POWER           0xDE    /* System Power */
#define KB_SLEEP           0xDF    /* System Sleep */
#define KB_WAKE            0xE3    /* System Wake */
#define KB_WEBSEARCH       0xE5    /* Web Search */
#define KB_WEBFAVORITES    0xE6    /* Web Favorites */
#define KB_WEBREFRESH      0xE7    /* Web Refresh */
#define KB_WEBSTOP         0xE8    /* Web Stop */
#define KB_WEBFORWARD      0xE9    /* Web Forward */
#define KB_WEBBACK         0xEA    /* Web Back */
#define KB_MYCOMPUTER      0xEB    /* My Computer */
#define KB_MAIL            0xEC    /* Mail */
#define KB_MEDIASELECT     0xED    /* Media Select */

/*--------------------------------------------------------------------------------------------------------

  kbInit

  Captures the keyboard from Windows.

--------------------------------------------------------------------------------------------------------*/
void kbInit( void );

/*--------------------------------------------------------------------------------------------------------

  kbDeinit

  Restores the keyboard to Windows.

--------------------------------------------------------------------------------------------------------*/
void kbDeinit( void );

/*--------------------------------------------------------------------------------------------------------

  kbUpdateKeyStatus

  Updates the status of each key on the keyboard.  Subsequent calls to kbGetKey can return a different 
  key status only after a call to this function has been made.  Call it regularly to keep the keyboard
  status current.

--------------------------------------------------------------------------------------------------------*/
void kbUpdateKeys( void );

/*--------------------------------------------------------------------------------------------------------

  kbIsKeyDown

  This function returns true if a key on the keyboard is pressed.  
  The values are updated by calling kbUpdateKeyStatus regularly.

  Example:

  // See if the space bar is down.
  if ( kbIsKeyDown(KB_SPACE) )
  {
    // Space bar is currently pressed.
  }

--------------------------------------------------------------------------------------------------------*/
bool kbIsKeyDown(int key);

/*--------------------------------------------------------------------------------------------------------

  kbWasKeyDown

  This function returns true if the key was pressed BEFORE the last keyboard update. 
  This makes detecting transitions and keypresses possible.

  Example:

  // Check for a press of the X button.
  if ( kbIsKeyDown(KB_X) && !kbWasKeyDown(KB_X) )
  {
    // X button was pressed.
  }

--------------------------------------------------------------------------------------------------------*/
bool kbWasKeyDown(int key);

void kbClearKey(int key);

#endif
