
#include "MacroPlugin.h"
#include "../KeyDefines.h"


MacroPlugin::MacroPlugin(int *scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes){
    lastKeyPressed = NOK;
    stored = new int[MACRO_SIZE];
    storedIndex = 0;
}

void MacroPlugin::replayMacro(KeyboardState* kbState) {
    for (int key = 0; key < storedIndex; key += 2) {
        InputEvent* ie = kbState->inputEventPool.getInputEvent(SCANCODE);
        ie->state = stored[key];
        ie->scancode = stored[key+1];
        kbState->runWireHandlers(ie);
    }
}

void MacroPlugin::storeCode(int state, int scanCode) {
    stored[storedIndex++]  = state;
    stored[storedIndex++]  = scanCode;
}

bool MacroPlugin::inputEvent(InputEvent* event, KeyboardState* kbState){

    if(pluginState == RECORDING) {
        if(isKey(event->scancode)) {
            pluginState = IDLE;
        } else {
            storeCode(event->state, event->scancode);
            return false;
        }
    } else if (pluginState == ACTIVATED) {
        //click
        if(isKey(event->scancode)){
            if (lastKeyPressed == event->scancode)
            {   //so last key pressed was the MACRO key, and then released. 
                //if the timeout is < MACRO_TIMEOUT it's a click we'll replay the macro.
                //if it's bigger than the NUKE_TIMEOUT we'll nuke the macro.
                if (millis() - lastKeyPressedTs < MACRO_TIMEOUT) {
                    replayMacro(kbState);
                } else if (millis() - lastKeyPressedTs > NUKE_TIMEOUT) {
                    storedIndex = 0;
                }
            }
            pluginState = IDLE;
        } else {
            pluginState = RECORDING;
            //only reason for handing all three states is to know when to zero out the storedIndex
            storedIndex = 0;
            storeCode(event->state,event->scancode);
            return false;
        }
    } else { // IDLE
        if(isKey(event->scancode)) {
            pluginState = ACTIVATED;
        }
        lastKeyPressed = event->scancode;
        lastKeyPressedTs = millis();
    }

    return true; 
}
