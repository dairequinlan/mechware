#ifndef TAPHOLD_PLUGIN
#define TAPHOLD_PLUGIN
#include "KeyPlugin.h"

enum TapHoldState {WAITING, PRESSED, HOLDING};

class TapHoldPlugin : public KeyPlugin {
    public :
        TapHoldPlugin(uint8_t* scanCodes, uint8_t* holdCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);

        uint8_t holdScanCode = KEY_T;
        uint32_t pressedTS;
        uint8_t pressedScanCode = NOK;
        
        TapHoldState state = WAITING;
};

#endif