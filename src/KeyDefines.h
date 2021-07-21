//stick some global key defines here, useful for a bunch of the other modules.
#ifndef GLOBAL_KEY_DEFINES
#define GLOBAL_KEY_DEFINES

#define KEY_RELEASED 1
#define KEY_PRESSED 0

//number of iterations of identical keyscan values before we trigger a keypress
#define DEBOUNCE_ITER 5

//microseconds between each scan. SCAN_PERIOD * DEBOUNCE_ITER = minimum response time
#define SCAN_PERIOD 3000
//do a timer event every 20 milliseconds.
#define TIMER_TICK_PERIOD 20000 

//some global key defines for function / transparent keys / custom defined etc.
//we're going to define the lowest bound of our 'special' keys as KEY_SPECIAL and
//nothing >= KEY_SPECIAL will ever be sent over the USB link as it's a keyboard
//firmware specific thing
#define KC_SPECIAL 0xFC
#define KC_REBOOT (KC_SPECIAL+1)
#define TRNS (KC_SPECIAL+2)
#define KC_FUNCTION (KC_SPECIAL+3)


//General HID keydefines, these will work out of the box for anything that uses TinyUSB,
//the mods have to be special cased for the Arduino Keyboard lib as that uses that 16 bit
//keycode so the individual USB Wire handlers have to handle the special cases 

#ifndef  KC_NONE
 #define  KC_NONE               0x00
 #define  KC_A                  0x04
 #define  KC_B                  0x05
 #define  KC_C                  0x06
 #define  KC_D                  0x07
 #define  KC_E                  0x08
 #define  KC_F                  0x09
 #define  KC_G                  0x0A
 #define  KC_H                  0x0B
 #define  KC_I                  0x0C
 #define  KC_J                  0x0D
 #define  KC_K                  0x0E
 #define  KC_L                  0x0F
 #define  KC_M                  0x10
 #define  KC_N                  0x11
 #define  KC_O                  0x12
 #define  KC_P                  0x13
 #define  KC_Q                  0x14
 #define  KC_R                  0x15
 #define  KC_S                  0x16
 #define  KC_T                  0x17
 #define  KC_U                  0x18
 #define  KC_V                  0x19
 #define  KC_W                  0x1A
 #define  KC_X                  0x1B
 #define  KC_Y                  0x1C
 #define  KC_Z                  0x1D
 #define  KC_1                  0x1E
 #define  KC_2                  0x1F
 #define  KC_3                  0x20
 #define  KC_4                  0x21
 #define  KC_5                  0x22
 #define  KC_6                  0x23
 #define  KC_7                  0x24
 #define  KC_8                  0x25
 #define  KC_9                  0x26
 #define  KC_0                  0x27
 #define  KC_RETURN             0x28
 #define  KC_ESCAPE             0x29
 #define  KC_BACKSPACE          0x2A
 #define  KC_TAB                0x2B
 #define  KC_SPACE              0x2C
 #define  KC_MINUS              0x2D
 #define  KC_EQUAL              0x2E
 #define  KC_BRACKET_LEFT       0x2F
 #define  KC_BRACKET_RIGHT      0x30
 #define  KC_BACKSLASH          0x31
 #define  KC_EUROPE_1           0x32
 #define  KC_SEMICOLON          0x33
 #define  KC_APOSTROPHE         0x34
 #define  KC_GRAVE              0x35
 #define  KC_COMMA              0x36
 #define  KC_PERIOD             0x37
 #define  KC_SLASH              0x38
 #define  KC_CAPS_LOCK          0x39
 #define  KC_F1                 0x3A
 #define  KC_F2                 0x3B
 #define  KC_F3                 0x3C
 #define  KC_F4                 0x3D
 #define  KC_F5                 0x3E
 #define  KC_F6                 0x3F
 #define  KC_F7                 0x40
 #define  KC_F8                 0x41
 #define  KC_F9                 0x42
 #define  KC_F10                0x43
 #define  KC_F11                0x44
 #define  KC_F12                0x45
 #define  KC_PRINT_SCREEN       0x46
 #define  KC_SCROLL_LOCK        0x47
 #define  KC_PAUSE              0x48
 #define  KC_INSERT             0x49
 #define  KC_HOME               0x4A
 #define  KC_PAGE_UP            0x4B
 #define  KC_DELETE             0x4C
 #define  KC_END                0x4D
 #define  KC_PAGE_DOWN          0x4E
 #define  KC_ARROW_RIGHT        0x4F
 #define  KC_ARROW_LEFT         0x50
 #define  KC_ARROW_DOWN         0x51
 #define  KC_ARROW_UP           0x52
 #define  KC_NUM_LOCK           0x53
 #define  KC_KEYPAD_DIVIDE      0x54
 #define  KC_KEYPAD_MULTIPLY    0x55
 #define  KC_KEYPAD_SUBTRACT    0x56
 #define  KC_KEYPAD_ADD         0x57
 #define  KC_KEYPAD_ENTER       0x58
 #define  KC_KEYPAD_1           0x59
 #define  KC_KEYPAD_2           0x5A
 #define  KC_KEYPAD_3           0x5B
 #define  KC_KEYPAD_4           0x5C
 #define  KC_KEYPAD_5           0x5D
 #define  KC_KEYPAD_6           0x5E
 #define  KC_KEYPAD_7           0x5F
 #define  KC_KEYPAD_8           0x60
 #define  KC_KEYPAD_9           0x61
 #define  KC_KEYPAD_0           0x62
 #define  KC_KEYPAD_DECIMAL     0x63
 #define  KC_EUROPE_2           0x64
 #define  KC_APPLICATION        0x65
 #define  KC_POWER              0x66
 #define  KC_KEYPAD_EQUAL       0x67
 #define  KC_F13                0x68
 #define  KC_F14                0x69
 #define  KC_F15                0x6A
 #define  KC_CONTROL_LEFT       0xE0
 #define  KC_SHIFT_LEFT         0xE1
 #define  KC_ALT_LEFT           0xE2
 #define  KC_GUI_LEFT           0xE3
 #define  KC_CONTROL_RIGHT      0xE4
 #define  KC_SHIFT_RIGHT        0xE5
 #define  KC_ALT_RIGHT          0xE6
 #define  KC_GUI_RIGHT          0xE7
#endif //keydefines

#endif