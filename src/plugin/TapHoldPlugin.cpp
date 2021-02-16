#include "../KeyDefines.h"
#include "TapHoldPlugin.h"


//click & double click keys inside this time period to lock the key until pressed again.
#define TAPHOLD_TIMEOUT 200

TapHoldPlugin::TapHoldPlugin(int* scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes) {
}

bool TapHoldPlugin::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->type == SCANCODE) {
        if ( event->state == KEY_PRESSED) {
        //if inkey then tag the key with the millis and wait
        } else if (event->state == KEY_RELEASED) {
            //if in key and we're < TIMEOUT then do the 
            //tap KEY_PRESSED then KEY_RELEASED
            //if we're in key and > TIMEOUT then we do the 
            //hold action KEY_RELEASED, presuming it's already been pressed ... 
        }
    } else if (event->type == TIMER) { 
        //check time against time out for active keys, 
        //if the timeout > TAPHOLD_TIMEOUT then we do the hold action KEY_PRESSED
    }
    return true;
}