
#include "MacroPlugin.h"
#include "../KeyDefines.h"


MacroPlugin::MacroPlugin(int *scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes){
    lastKeyPressed = NOK;
    stored = new int[MACRO_SIZE];
    storedIndex = 0;
}

void MacroPlugin::replayMacro(KeyboardState* kbState) {
    for (int key = 0; key < storedIndex; key += 2) {
        kbState->runWireHandlers(stored[key],stored[key+1]);
    }
}

void MacroPlugin::storeCode(int state, int scanCode) {
    stored[storedIndex++]  = state;
    stored[storedIndex++]  = scanCode;
}

bool MacroPlugin::keyEvent(int state, int scanCode, KeyboardState* kbState){

    if(pluginState == RECORDING) {
        if(isKey(scanCode)) {
            pluginState = IDLE;
        } else {
            storeCode(state, scanCode);
            return false;
        }
    } else if (pluginState == ACTIVATED) {
        //click
        if(isKey(scanCode)){
            if (lastKeyPressed == scanCode)
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
            storeCode(state,scanCode);
            return false;
        }
    } else { // IDLE
        if(isKey(scanCode)) {
            pluginState = ACTIVATED;
        }
        lastKeyPressed = scanCode;
        lastKeyPressedTs = millis();
    }

    return true; 
}
