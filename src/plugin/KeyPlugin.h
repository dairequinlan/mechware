#ifndef KEY_PLUGIN
#define KEY_PLUGIN
#include "../KeyboardState.h"

class KeyboardState;

class KeyPlugin {
    public :
        KeyPlugin();
        KeyPlugin(uint8_t *scanCodes, int nCodes);
        bool isKey(uint8_t scanCode);
        virtual bool inputEvent(InputEvent* event, KeyboardState* kbState);
        uint8_t *scanCodes;
        int nCodes;
};


#endif