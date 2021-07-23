
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

#include "board.h"

KeyboardState *keyboardState;

extern uint8_t keyMaps[NUM_KEYMAPS][NUM_ROWS][NUM_COLS];
extern InputDevice* inputDevices[];
extern KeyPlugin* keyPlugins[];
extern WireHandler* wireHandlers[];

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

    #if defined(TEENSY)
     Serial.begin(9600); //needs platformio.ini or teensy setup to be updated to add serial USB functionality.
    #endif
    //TODO HAL serial comms layer (plugin)
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
