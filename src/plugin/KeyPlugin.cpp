#include "KeyPlugin.h"

KeyPlugin::KeyPlugin() {
    nCodes = 0;
}

KeyPlugin::KeyPlugin(int *scanCodes, int nCodes) {
    this->scanCodes = scanCodes;
    this->nCodes = nCodes;
}

bool KeyPlugin::isKey(int scanCode) {
    for (int i = 0; i < nCodes; i++) {
        if(scanCodes[i] == scanCode) {
            return true;
        }
    }
    return false;
}
bool KeyPlugin::keyEvent(int state, int scanCode, KeyboardState* kbState){
    return true;
}