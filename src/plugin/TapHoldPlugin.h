#ifndef TAPHOLD_PLUGIN
#define TAPHOLD_PLUGIN
#include "KeyPlugin.h"

class TapHoldPlugin : public KeyPlugin {
    public :
        TapHoldPlugin(int* scanCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);

};

#endif