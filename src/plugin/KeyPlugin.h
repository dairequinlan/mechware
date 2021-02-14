#ifndef KEY_PLUGIN
#define KEY_PLUGIN
#include "../KeyboardState.h"

class KeyboardState;

class KeyPlugin {
    public :
        KeyPlugin();
        KeyPlugin(int *scanCodes, int nCodes);
        bool isKey(int scanCode);
        virtual bool inputEvent(InputEvent* event, KeyboardState* kbState);
        int *scanCodes;
        int nCodes;
};


#endif