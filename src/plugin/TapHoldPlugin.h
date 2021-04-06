#ifndef TAPHOLD_PLUGIN
#define TAPHOLD_PLUGIN
#include "KeyPlugin.h"

enum TapHoldState {WAITING, PRESSED, HOLDING};

class TapHoldPlugin : public KeyPlugin {
    public :
        TapHoldPlugin(int* scanCodes, int* holdCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);

        int holdScanCode = KEY_T;
        unsigned long pressedTS;
        int pressedScanCode = NOK;
        
        TapHoldState state = WAITING;
};

#endif