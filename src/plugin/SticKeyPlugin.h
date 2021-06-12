#ifndef STICKEY_PLUGIN
#define STICKEY_PLUGIN
#include "KeyPlugin.h"

class SticKeyPlugin : public KeyPlugin {
    public :
        SticKeyPlugin(int* scanCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);

        uint8_t lastKeyPressed = NOK;
        uint8_t lastKeyClicked = NOK;
        uint32_t lastKeyPressedTs = 0;
        uint32_t lastKeyClickedTs = 0;

};

#endif