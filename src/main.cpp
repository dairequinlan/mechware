
#if defined(PICO)
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 
 #include "pico/stdlib.h"
 #include "pico/bootrom.h"

 #include "bsp/board.h"
 #include "tusb.h"
 #include "usb_descriptors.h"
#elif defined(TEENSY) 
 #include <Arduino.h>
#endif

#include "KeyDefines.h"
#include "KeyboardState.h"
#include "wire/WireHandler.h"
#if defined(PICO)
 #include "wire/TinyUSBWireHandler.h"
 #include "plugin/PicoRebootPlugin.h"
 #include "wire/I2CWireHandler.h"
#elif defined(TEENSY)
 #include "wire/USBWireHandler.h"
 #include "wire/SerialWireHandler.h" 
#endif 

#include "plugin/KeyPlugin.h"
#include "plugin/FnPlugin.h"
//#include "plugin/SticKeyPlugin.h"
//#include "plugin/MacroPlugin.h"
//#include "plugin/TapHoldPlugin.h"
#include "input/InputDevice.h"
#include "input/MatrixInput.h"

KeyboardState *keyboardState;

/* TODO move these out somewhere to some board specific file. At the moment the 
   right/left defines are VERY specific to my PICO split build, and the default
   singular case is specific to that teensy board I built. Needs a neat way of 
   encapsulating these into a board specific header file of some description
   */
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
{ KC_NONE,  KC_NONE,  KC_CONTROL_LEFT, KC_FUNCTION, KC_GUI_LEFT, KC_ALT_LEFT, KC_NONE, KC_SPACE, KC_NONE}
}};
#elif defined(RIGHT_HAND_SIDE)
uint8_t keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{ 
{ KC_7,  KC_NONE,  KC_8,  KC_9,  KC_0,  KC_MINUS,  KC_EQUAL,  KC_BACKSPACE,  KC_INSERT},
{ KC_Y,  KC_U,  KC_I,  KC_O,  KC_P,  KC_BRACKET_LEFT,  KC_BRACKET_RIGHT,  KC_RETURN,  KC_DELETE},
{ KC_H,  KC_J,  KC_K,  KC_L,  KC_SEMICOLON,  KC_APOSTROPHE,  KC_EUROPE_1,  KC_NONE,  KC_HOME},
{ KC_N,  KC_M,  KC_COMMA,  KC_PERIOD,  KC_SLASH,  KC_NONE,  KC_SHIFT_RIGHT,  KC_ARROW_UP,  KC_END},
{ KC_SPACE,  KC_NONE,  KC_NONE,  KC_ALT_RIGHT,  KC_FUNCTION,  KC_CONTROL_RIGHT,  KC_ARROW_LEFT,  KC_ARROW_DOWN,  KC_ARROW_RIGHT}
},{
{ KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_F11,  KC_F12, TRNS, TRNS,  KC_REBOOT},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  KC_PAGE_UP},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  KC_PAGE_DOWN},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS},
}};
#else
uint8_t keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{
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
#endif

//keylists for the plugins
//int macroList[] = { KC_SPECIAL};
//int sticKeyList[] = { KC_FUNCTION,MODIFIER KC_SHIFT};
uint8_t funcKeyList[] = { KC_FUNCTION};
//int tapHoldTap[] = { KC_Y};
//int tapHoldHold[] = { KC_T};
uint8_t rebootList[] = { KC_REBOOT};


#if defined(PICO) 
 #define NUM_KEYPLUGINS 2
 KeyPlugin* keyPlugins[] = {   
        new FnPlugin(funcKeyList,1),
        new PicoRebootPlugin(rebootList,1)
        };
#elif defined(TEENSY)
 #define NUM_KEYPLUGINS 1
 KeyPlugin* keyPlugins[] = {   
        //new MacroPlugin(macroList,1),
        //new SticKeyPlugin(sticKeyList,2),
        //new TapHoldPlugin(tapHoldTap, tapHoldHold, 1), 
        new FnPlugin(funcKeyList,1),
        };
#endif

#if defined(PRIMARY)
 bool primary = true; 
 I2CWireHandler* I2CIOController = new I2CWireHandler(primary);
 #define NUM_WIREHANDLERS 2
 WireHandler* wireHandlers[] = { 
            I2CIOController,
            new TinyUSBWireHandler() 
        };
#elif defined(SECONDARY)
 bool primary = false;
 I2CWireHandler* I2CIOController = new I2CWireHandler(primary);
 #define NUM_WIREHANDLERS 1
 WireHandler* wireHandlers[] = { 
            I2CIOController,
        };
#else  //TODO generalise this; default single board case, which coincides at the moment with the teensy board
 #define NUM_WIREHANDLERS 1
 WireHandler* wireHandlers[] = {
            new USBWireHandler() 
        };
 
#endif
 
#if defined(PRIMARY) || defined(SECONDARY)
 #define NUM_INPUT_DEVICES 2
 InputDevice* inputDevices[] = { 
             new MatrixInput(),
             I2CIOController 
         };
#else
 #define NUM_INPUT_DEVICES 1
 InputDevice* inputDevices[] = {
            new MatrixInput()
        };
#endif


void doInputs() {
    //now run our input handlers.
    for(int inputDevice = 0; inputDevice < NUM_INPUT_DEVICES; inputDevice ++) {
        if(!inputDevices[inputDevice]->scan(keyboardState)) {
            return;
        }
    }
}

unsigned long get_micros() {
    #if defined(PICO) 
        return time_us_32();
    #elif defined(TEENSY)
        return micros();
    #endif
}

void doTimerTick() {
    InputEvent* timerEvent = keyboardState->inputEventPool.getInputEvent(TIMER);
    timerEvent->timestamp = get_micros();
    keyboardState->inputEvent(timerEvent);
}

void setup() {

    Serial.begin(9600); //needs platformio.ini or teensy setup to be updated to add serial USB functionality.
    //TODO HAL serial comms layer (plugin)
    //usbMIDI.begin();
    keyboardState = new KeyboardState(keyMaps, NUM_KEYMAPS, 
                                    keyPlugins, NUM_KEYPLUGINS, 
                                    wireHandlers, NUM_WIREHANDLERS);                          
}

void loop() {
    static unsigned long scanTick = get_micros();
    static unsigned long timerTick = get_micros();

    //lets do a timer tick if necessary
    unsigned long elapsed = get_micros() - timerTick;
    if (elapsed >= TIMER_TICK_PERIOD ) {
        timerTick = get_micros() - (elapsed - TIMER_TICK_PERIOD);
        doTimerTick();
    }
    //now check if we have to run our inputs
    elapsed = get_micros() - scanTick;
    if (elapsed >= SCAN_PERIOD) {
        scanTick = get_micros() - (elapsed - SCAN_PERIOD);
        doInputs();
    }
}

#if defined(PICO)
// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    // TODO not Implemented
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    // TODO set LED based on CAPLOCK, NUMLOCK etc...
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void)
{
    // connected and there are data available
    if ( tud_cdc_available() )
    {
        // read datas
        char buf[64];
        uint32_t count = tud_cdc_read(buf, sizeof(buf));
        (void) count;
    }
}

int main() {
    stdio_init_all();
    tusb_init();
    setup();
    while(true) {
        tud_task();
        cdc_task();
        loop();
    }
}
#endif
