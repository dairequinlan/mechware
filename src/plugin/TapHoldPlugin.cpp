#include "../KeyDefines.h"
#include "TapHoldPlugin.h"


//click & double click keys inside this time period to lock the key until pressed again.
#define TAPHOLD_TIMEOUT 200000

TapHoldPlugin::TapHoldPlugin(uint8_t* scanCodes, uint8_t* holdCodes, int nCodes):KeyPlugin(scanCodes, nCodes) {
}

bool TapHoldPlugin::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->type == SCANCODE) {
        if ( event->state == KEY_PRESSED) {
            if(isKey(event->scancode)) {
                pressedTS = micros();
                pressedScanCode = event->scancode;
                state = PRESSED;
                //now swallow the key pressed event.
                return false;
            }
        } else if (event->state == KEY_RELEASED) {
            if(event->scancode == pressedScanCode) {
                //if in key and we're NOT in the HOLDING state then do the 
                //tap KEY_PRESSED, the KEY_RELEASED will be handled normally
                //if we're in key and we ARE in the HOLDING state then we do the 
                //hold action KEY_RELEASED, presuming it's already been pressed ... 
                if(state == PRESSED) {
                    InputEvent* pressed = kbState->inputEventPool.getInputEvent(SCANCODE);
                    pressed->scancode = pressedScanCode;
                    pressed->state = KEY_PRESSED;
                    kbState->runWireHandlers(pressed);
                } else if (state == HOLDING ){ 
                    //we're past the timeout so we've already DONE the press (hopefully)
                    //so we just have to handle the release. We'll cheat a bit ...
                    event->scancode = holdScanCode;
                }
                //annnnd we're all done, reset the state here to wait for the next key.
                state = WAITING;
            }
        }
    } else if (event->type == TIMER) { 
        //check time against time out for active keys, 
        //if the timeout > TAPHOLD_TIMEOUT then we do the hold action KEY_PRESSED
        //and update the state from PRESSED to HOLDING
        if(state == PRESSED) {
            Serial.println("STATE PRESSED");
        }
        if(state == PRESSED && (event->timestamp - pressedTS > TAPHOLD_TIMEOUT)) {
            //more than TAPHOLD_TIMEOUT has passed so lets hit pressed for the holdkey
            state = HOLDING;
            InputEvent* pressed = kbState->inputEventPool.getInputEvent(SCANCODE);
            pressed->state = KEY_PRESSED;
            pressed->scancode = holdScanCode;
            kbState->runWireHandlers(pressed);
        }
    }
    return true;
}
