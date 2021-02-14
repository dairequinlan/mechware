#include <Arduino.h>
#include "KeyDefines.h"
#include "KeyboardState.h"
#include "wire/USBWireHandler.h"
#include "wire/SerialWireHandler.h"
#include "plugin/KeyPlugin.h"
#include "plugin/FnPlugin.h"
#include "plugin/SticKeyPlugin.h"
#include "plugin/MacroPlugin.h"
#include "input/InputDevice.h"
#include "input/MatrixInput.h"

KeyboardState *keyboardState;

#define NUM_KEYMAPS 2
int keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{
{KEY_ESC,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,KEY_INSERT,KEY_HOME},
{KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,NOK,KEY_DELETE,KEY_END},
{KEY_FUNCTION,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,KC_NONUS_HASH,KEY_ENTER,KEY_SPECIAL,KEY_PAGE_UP},
{MODIFIERKEY_SHIFT,KC_NONUS_BACKSLASH,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,MODIFIERKEY_RIGHT_SHIFT,NOK,KEY_UP,KEY_PAGE_DOWN},
{MODIFIERKEY_CTRL,MODIFIERKEY_GUI,MODIFIERKEY_ALT,NOK,NOK,NOK,KEY_SPACE,NOK,NOK,NOK,MODIFIERKEY_RIGHT_ALT,KEY_FUNCTION,MODIFIERKEY_RIGHT_CTRL,KEY_LEFT,KEY_DOWN,KEY_RIGHT}                                 
},{
{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,TRNS,TRNS,TRNS},
{TRNS,NOK,KEY_UP,NOK,NOK,NOK,NOK,KEY_INSERT,KEY_HOME,KEY_PAGE_UP,NOK,NOK,NOK,NOK,TRNS,TRNS},
{TRNS,KEY_LEFT,KEY_DOWN,KEY_RIGHT,NOK,NOK,NOK,KEY_DELETE,KEY_END,KEY_PAGE_DOWN,NOK,NOK,NOK,TRNS,KEY_FUNCTION,TRNS},
{TRNS,NOK,NOK,NOK,NOK,NOK,NOK,NOK,NOK,NOK,NOK,NOK,TRNS,NOK,TRNS,TRNS},
{TRNS,TRNS,TRNS,NOK,NOK,NOK,TRNS,NOK,NOK,NOK,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS}                               
}};

//keylists for the plugins.
int macroList[] = {KEY_SPECIAL};
int sticKeyList[] = {KEY_FUNCTION,MODIFIERKEY_SHIFT};
int funcKeyList[] = {KEY_FUNCTION};

#define NUM_KEYPLUGINS 3
KeyPlugin* keyPlugins[] = {   
      new MacroPlugin(macroList,1),
      new SticKeyPlugin(sticKeyList,2),
      new FnPlugin(funcKeyList,1) };

#define NUM_WIREHANDLERS 1
WireHandler* wireHandlers[] = { //new SerialWireHandler(),
      new USBWireHandler() };

#define NUM_INPUT_DEVICES 1
InputDevice* inputDevices[] = { new MatrixInput() };

void setup() {
  //Serial.begin(9600); //needs platformio.ini or teensy setup to be updated to add serial USB functionality.
  keyboardState = new KeyboardState(keyMaps, NUM_KEYMAPS, 
                                    keyPlugins, NUM_KEYPLUGINS, 
                                    wireHandlers, NUM_WIREHANDLERS);
}

void loop() {
    static unsigned long tick;
    unsigned long elapsed = micros() - tick;
    if (elapsed >= SCAN_PERIOD) {
        tick = elapsed - SCAN_PERIOD;
        //now run our input handlers.
        for(int inputDevice = 0; inputDevice < NUM_INPUT_DEVICES; inputDevice ++) {
            if(!inputDevices[inputDevice]->scan(keyboardState)) {
                  return;
            }
        }
    }
}