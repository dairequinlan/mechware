#ifndef WIRE_HANDLER
#define WIRE_HANDLER
#include "../InputEvent.h"
#include "../KeyboardState.h"
class KeyboardState;

class WireHandler {
    public :
        WireHandler();
        virtual bool inputEvent(InputEvent * event, KeyboardState* kbState);
};


#endif