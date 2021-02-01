
#include "FnPlugin.h"
#include "KeyDefines.h"


FnPlugin::FnPlugin(int *scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes){
}

bool FnPlugin::keyEvent(int state, int scanCode, KeyboardState* kbState){
    if(isKey(scanCode)) {
        if(state == KEY_PRESSED) {
            kbState->raise();
        } else {
            kbState->lower();
        }
    }
    return true; 
}
