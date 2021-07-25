#include "KeyPlugin.h"

KeyPlugin::KeyPlugin() {
    nCodes = 0;
}

KeyPlugin::KeyPlugin(uint8_t *scanCodes, int nCodes) {
    this->scanCodes = scanCodes;
    this->nCodes = nCodes;
}

bool KeyPlugin::isKey(uint8_t scanCode) {
    for (int i = 0; i < nCodes; i++) {
        if(scanCodes[i] == scanCode) {
            return true;
        }
    }
    return false;
}
bool KeyPlugin::inputEvent(InputEvent* event, KeyboardState* kbState){
    return true;
}