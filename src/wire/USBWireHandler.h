#ifndef USB_WIRE_HANDLER
#define USB_WIRE_HANDLER
#include "WireHandler.h"

class USBWireHandler : public WireHandler {
    public:
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
};

#endif