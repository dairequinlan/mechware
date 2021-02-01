#ifndef WIRE_HANDLER
#define WIRE_HANDLER
#include "../KeyboardState.h"
class KeyboardState;

class WireHandler {
    public :
        WireHandler();
        virtual bool keyEvent(int state, int scanCode, KeyboardState* kbState);
        virtual bool printStr(char* str, KeyboardState* kbState);
};


#endif