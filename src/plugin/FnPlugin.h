#ifndef FN_PLUGIN
#define FN_PLUGIN
#include "KeyPlugin.h"

class FnPlugin:public KeyPlugin {
    public :
        FnPlugin();
        FnPlugin(int *scanCodes, int nCodes);
        virtual bool keyEvent(int state, int scanCode, KeyboardState* kbState);
};

#endif