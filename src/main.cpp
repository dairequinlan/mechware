
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

#if defined(LEFT_HAND_SIDE)
uint8_t keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{ 
{HID_KEY_F1, HID_KEY_F2, HID_KEY_ESCAPE, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6},
{HID_KEY_F3, HID_KEY_F4, HID_KEY_TAB,HID_KEY_NONE,HID_KEY_Q,HID_KEY_W,HID_KEY_E,HID_KEY_R,HID_KEY_T},
{HID_KEY_F5, HID_KEY_F6, KEY_FUNCTION,HID_KEY_NONE,HID_KEY_A,HID_KEY_S,HID_KEY_D,HID_KEY_F,HID_KEY_G},
{HID_KEY_F7, HID_KEY_F8, HID_KEY_SHIFT_LEFT,HID_KEY_EUROPE_2,HID_KEY_Z,HID_KEY_X,HID_KEY_C,HID_KEY_V,HID_KEY_B},
{HID_KEY_F9, HID_KEY_F10, HID_KEY_CONTROL_LEFT,KEY_FUNCTION,HID_KEY_GUI_LEFT,HID_KEY_ALT_LEFT,HID_KEY_NONE,HID_KEY_SPACE,HID_KEY_NONE}
},{
{KEY_REBOOT, HID_KEY_NONE, HID_KEY_GRAVE, HID_KEY_F1, HID_KEY_F2, HID_KEY_F3, HID_KEY_F4, HID_KEY_F5, HID_KEY_F6},
{HID_KEY_NONE, HID_KEY_NONE, TRNS,HID_KEY_NONE,TRNS,HID_KEY_ARROW_UP,TRNS,TRNS,TRNS},
{HID_KEY_NONE, HID_KEY_NONE, KEY_FUNCTION,HID_KEY_NONE,HID_KEY_ARROW_LEFT,HID_KEY_ARROW_DOWN,HID_KEY_ARROW_RIGHT,TRNS,TRNS},
{HID_KEY_NONE, HID_KEY_NONE, HID_KEY_SHIFT_LEFT,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS},
{HID_KEY_NONE, HID_KEY_NONE, HID_KEY_CONTROL_LEFT,KEY_FUNCTION,HID_KEY_GUI_LEFT,HID_KEY_ALT_LEFT,HID_KEY_NONE,HID_KEY_SPACE,HID_KEY_NONE}
}};
#elif defined(RIGHT_HAND_SIDE)
uint8_t keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS] = {{ 
{HID_KEY_7, HID_KEY_NONE, HID_KEY_8, HID_KEY_9, HID_KEY_0, HID_KEY_MINUS, HID_KEY_EQUAL, HID_KEY_BACKSPACE, HID_KEY_INSERT},
{HID_KEY_Y, HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P, HID_KEY_BRACKET_LEFT, HID_KEY_BRACKET_RIGHT, HID_KEY_RETURN, HID_KEY_DELETE},
{HID_KEY_H, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE, HID_KEY_EUROPE_1, HID_KEY_NONE, HID_KEY_HOME},
{HID_KEY_N, HID_KEY_M, HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH, HID_KEY_NONE, HID_KEY_SHIFT_RIGHT, HID_KEY_ARROW_UP, HID_KEY_END},
{HID_KEY_SPACE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_ALT_RIGHT, KEY_FUNCTION, HID_KEY_CONTROL_RIGHT, HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_RIGHT}
},{
{HID_KEY_F7, HID_KEY_F8, HID_KEY_F9, HID_KEY_F10, HID_KEY_F11, HID_KEY_F12, TRNS, TRNS, KEY_REBOOT},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, HID_KEY_PAGE_UP},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, HID_KEY_PAGE_DOWN},
{TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS},
}};
#endif

//keylists for the plugins
//int macroList[] = {KEY_SPECIAL};
//int sticKeyList[] = {KEY_FUNCTION,MODIFIERKEY_SHIFT};
uint8_t funcKeyList[] = {KEY_FUNCTION};
//int tapHoldTap[] = {KEY_Y};
//int tapHoldHold[] = {KEY_T};
uint8_t rebootList[] = {KEY_REBOOT};

#define NUM_KEYPLUGINS 1
KeyPlugin* keyPlugins[] = {   
        //new MacroPlugin(macroList,1),
        //new SticKeyPlugin(sticKeyList,2),
        //new TapHoldPlugin(tapHoldTap, tapHoldHold, 1), 
        new FnPlugin(funcKeyList,1),
        new PicoRebootPlugin(rebootList,1)
        };

#if defined(PRIMARY)
 bool primary = true; 
 I2CWireHandler* I2CIOController = new I2CWireHandler(primary);
 #define NUM_WIREHANDLERS 2
 WireHandler* wireHandlers[] = { 
            I2CIOController,
            new TinyUSBWireHandler() 
        };
#else
 bool primary = false;
 I2CWireHandler* I2CIOController = new I2CWireHandler(primary);
 #define NUM_WIREHANDLERS 1
 WireHandler* wireHandlers[] = { 
            I2CIOController,
        };
#endif


#define NUM_INPUT_DEVICES 2
InputDevice* inputDevices[] = { 
            new MatrixInput(),
            I2CIOController 
        };


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

    //Serial.begin(9600); //needs platformio.ini or teensy setup to be updated to add serial USB functionality.
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
