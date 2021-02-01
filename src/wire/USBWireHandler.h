#ifndef USB_WIRE_HANDLER
#define USB_WIRE_HANDLER
#include "WireHandler.h"

class USBWireHandler : public WireHandler {
    public:
        bool keyEvent(int state, int scanCode, KeyboardState* kbState);
        bool printStr(char* str, KeyboardState* kbState);
};

#endif