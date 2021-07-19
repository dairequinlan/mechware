#ifndef STICKEY_PLUGIN
#define STICKEY_PLUGIN
#include "KeyPlugin.h"

class SticKeyPlugin : public KeyPlugin {
    public :
        SticKeyPlugin(uint8_t *scanCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);

        uint8_t lastKeyPressed = KC_NONE;
        uint8_t lastKeyClicked = KC_NONE;
        uint32_t lastKeyPressedTs = 0;
        uint32_t lastKeyClickedTs = 0;

};

#endif