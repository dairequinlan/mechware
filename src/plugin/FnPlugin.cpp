
#include "FnPlugin.h"
#include "KeyDefines.h"


FnPlugin::FnPlugin(uint8_t *scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes){
}

bool FnPlugin::inputEvent(InputEvent* event, KeyboardState* kbState){
    if(isKey(event->scancode)) {
        if(event->state == KEY_PRESSED) {
            kbState->raise();
        } else {
            kbState->lower();
        }
    }
    return true; 
}
