#include "board.h"
#include "../../wire/USBWireHandler.h"
#include "../../input/MatrixInput.h"
#include "../../plugin/FnPlugin.h"

unsigned char rowPins[NUM_ROWS] = {13,21,20,18,19};
unsigned char colPins[NUM_COLS] = {12,11,10,9,8,7,6,5,4,3,2,1,17,16,15,14};

unsigned char keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{
{ KC_ESCAPE, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINUS, KC_EQUAL, KC_BACKSPACE, KC_INSERT, KC_HOME},
{ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BRACKET_LEFT, KC_BRACKET_RIGHT, KC_NONE, KC_DELETE, KC_END},
{ KC_FUNCTION, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SEMICOLON, KC_APOSTROPHE,KC_EUROPE_1, KC_RETURN, KC_SPECIAL, KC_PAGE_UP},
{ KC_SHIFT_LEFT,KC_EUROPE_2, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMMA, KC_PERIOD, KC_SLASH,KC_SHIFT_RIGHT, KC_NONE, KC_ARROW_UP, KC_PAGE_DOWN},
{ KC_CONTROL_LEFT,KC_GUI_LEFT, KC_ALT_LEFT, KC_NONE, KC_NONE, KC_NONE, KC_SPACE, KC_NONE, KC_NONE, KC_NONE,KC_ALT_RIGHT, KC_FUNCTION,KC_CONTROL_RIGHT, KC_ARROW_LEFT, KC_ARROW_DOWN, KC_ARROW_RIGHT}                                 
},{
{ KC_GRAVE, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,TRNS,TRNS,TRNS},
{TRNS, KC_NONE, KC_ARROW_UP, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_INSERT, KC_HOME, KC_PAGE_UP, KC_NONE, KC_NONE, KC_NONE, KC_NONE,TRNS,TRNS},
{TRNS, KC_ARROW_LEFT, KC_ARROW_DOWN, KC_ARROW_RIGHT, KC_NONE, KC_NONE, KC_NONE, KC_DELETE, KC_END, KC_PAGE_DOWN, KC_NONE, KC_NONE, KC_NONE,TRNS, KC_FUNCTION,TRNS},
{TRNS, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_NONE, KC_NONE,TRNS, KC_NONE,TRNS,TRNS},
{TRNS,TRNS,TRNS, KC_NONE, KC_NONE, KC_NONE,TRNS, KC_NONE, KC_NONE, KC_NONE,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS}                               
}};

//plugins config for the board
//first Input devices, then KeyPlugins, then WireHandlers

 InputDevice* inputDevices[] = {
            new MatrixInput()
        };

//keylists for the plugins
//int sticKeyList[] = { KC_FUNCTION,MODIFIER KC_SHIFT};

uint8_t fnKeyList[] = {KC_FUNCTION};
KeyPlugin* keyPlugins[] = {   
        //new SticKeyPlugin(sticKeyList,2),
        new FnPlugin(fnKeyList,1),
        };


 WireHandler* wireHandlers[] = {
            new USBWireHandler() 
        };



