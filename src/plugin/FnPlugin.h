#ifndef FN_PLUGIN
#define FN_PLUGIN
#include "KeyPlugin.h"

class FnPlugin:public KeyPlugin {
    public :
        FnPlugin();
        FnPlugin(uint8_t *scanCodes, int nCodes);
        virtual bool inputEvent(InputEvent* event, KeyboardState* kbState);
};

#endif