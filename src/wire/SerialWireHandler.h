#ifndef SERIAL_WIRE_HANDLER
#define SERIAL_WIRE_HANDLER
#include "WireHandler.h"

class SerialWireHandler : public WireHandler {
    public:
        bool keyEvent(int state, int scanCode, KeyboardState* kbState);
};

#endif