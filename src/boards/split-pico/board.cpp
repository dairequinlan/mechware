#include "board.h"
#include "../../wire/TinyUSBWireHandler.h"
#include "../../input/MatrixInput.h"
#include "../../plugin/FnPlugin.h"
#include "../../plugin/PicoRebootPlugin.h"
#include "../../wire/I2CWireHandler.h"

#if defined(LEFT_HAND_SIDE)
 unsigned char rowPins[NUM_ROWS] = {13,12,11,10,9};
 unsigned char colPins[NUM_COLS] = {15,14,8,7,6,5,4,3,2};
#elif defined(RIGHT_HAND_SIDE)
 unsigned char rowPins[NUM_ROWS] = {1,2,3,4,5};
 unsigned char colPins[NUM_COLS] = {13,12,11,10,9,8,7,6,0};
#endif

#if defined(LEFT_HAND_SIDE)
uint8_t keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{ 
{ KC_F1,  KC_F2,  KC_ESCAPE,  KC_1,  KC_2,  KC_3,  KC_4,  KC_5,  KC_6},
{ KC_F3,  KC_F4,  KC_TAB, KC_NONE, KC_Q, KC_W, KC_E, KC_R, KC_T},
{ KC_F5,  KC_F6,  KC_FUNCTION, KC_NONE, KC_A, KC_S, KC_D, KC_F, KC_G},
{ KC_F7,  KC_F8,  KC_SHIFT_LEFT, KC_EUROPE_2, KC_Z, KC_X, KC_C, KC_V, KC_B},
{ KC_F9,  KC_F10,  KC_CONTROL_LEFT, KC_FUNCTION, KC_GUI_LEFT, KC_ALT_LEFT, KC_NONE, KC_SPACE, KC_NONE}
},{
{ KC_REBOOT,  KC_NONE,  KC_GRAVE,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6},
{ KC_NONE,  KC_NONE, TRNS, KC_NONE,TRNS, KC_ARROW_UP,TRNS,TRNS,TRNS},
{ KC_NONE,  KC_NONE,  KC_FUNCTION, KC_NONE, KC_ARROW_LEFT, KC_ARROW_DOWN, KC_ARROW_RIGHT,TRNS,TRNS},
{ KC_NONE,  KC_NONE,  KC_SHIFT_LEFT,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS},
{ KC_F11,  KC_12,  KC_CONTROL_LEFT, KC_FUNCTION, KC_GUI_LEFT, KC_ALT_LEFT, KC_NONE, KC_SPACE, KC_NONE}
}};
#elif defined(RIGHT_HAND_SIDE)
uint8_t keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{ 
{ KC_7,  KC_NONE,  KC_8,  KC_9,  KC_0,  KC_MINUS,  KC_EQUAL,  KC_BACKSPACE,  KC_INSERT},
{ KC_Y,  KC_U,  KC_I,  KC_O,  KC_P,  KC_BRACKET_LEFT,  KC_BRACKET_RIGHT,  KC_RETURN,  KC_DELETE},
{ KC_H,  KC_J,  KC_K,  KC_L,  KC_SEMICOLON,  KC_APOSTROPHE,  KC_EUROPE_1,  KC_NONE,  KC_HOME},
{ KC_N,  KC_M,  KC_COMMA,  KC_PERIOD,  KC_SLASH,  KC_NONE,  KC_SHIFT_RIGHT,  KC_ARROW_UP,  KC_END},
{ KC_SPACE,  KC_NONE,  KC_NONE,  KC_ALT_RIGHT,  KC_FUNCTION,  KC_CONTROL_RIGHT,  KC_ARROW_LEFT,  KC_ARROW_DOWN,  KC_ARROW_RIGHT}
},{
{ KC_F7,  KC_NONE, KC_F8,  KC_F9,  KC_F10,  KC_F11,  KC_F12, TRNS, KC_REBOOT},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  KC_PAGE_UP},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  KC_PAGE_DOWN},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS},
}};
#endif

uint8_t rebootList[] = { KC_REBOOT};
uint8_t fnKeyList[] = {KC_FUNCTION};


 KeyPlugin* keyPlugins[] = {   
        new FnPlugin(fnKeyList,1),
        new PicoRebootPlugin(rebootList,1)
        };

#if defined(PRIMARY)
 bool primary = true; 
 I2CWireHandler* I2CIOController = new I2CWireHandler(primary);
 WireHandler* wireHandlers[] = { 
            I2CIOController,
            new TinyUSBWireHandler() 
        };
#elif defined(SECONDARY)
 bool primary = false;
 I2CWireHandler* I2CIOController = new I2CWireHandler(primary);
 WireHandler* wireHandlers[] = { 
            I2CIOController,
        };
#endif

InputDevice* inputDevices[] = { 
             new MatrixInput(),
             I2CIOController 
         };