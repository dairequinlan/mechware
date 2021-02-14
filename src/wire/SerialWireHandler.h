#ifndef SERIAL_WIRE_HANDLER
#define SERIAL_WIRE_HANDLER
#include "WireHandler.h"

class SerialWireHandler : public WireHandler {
    public:
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
};

#endif