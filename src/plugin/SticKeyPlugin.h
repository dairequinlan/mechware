#ifndef STICKEY_PLUGIN
#define STICKEY_PLUGIN
#include "KeyPlugin.h"

class SticKeyPlugin : public KeyPlugin {
    public :
        SticKeyPlugin(int* scanCodes, int nCodes);
        bool keyEvent(int state, int scanCode, KeyboardState* kbState);

        int lastKeyPressed = NOK;
        int lastKeyClicked = NOK;
        unsigned long lastKeyPressedTs = 0;
        unsigned long lastKeyClickedTs = 0;

};

#endif