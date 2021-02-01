#ifndef KEY_PLUGIN
#define KEY_PLUGIN
#include "../KeyboardState.h"

class KeyboardState;

class KeyPlugin {
    public :
        KeyPlugin();
        KeyPlugin(int *scanCodes, int nCodes);
        bool isKey(int scanCode);
        virtual bool keyEvent(int state, int scanCode, KeyboardState* kbState);
        int *scanCodes;
        int nCodes;
};


#endif