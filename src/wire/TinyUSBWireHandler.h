#ifndef TINY_USB_WIRE_HANDLER
#define TINY_USB_WIRE_HANDLER
#include "WireHandler.h"
#include "tusb.h"

class TinyUSBWireHandler : public WireHandler {
    public:
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
    private:
        void addKey(unsigned char scanCode);
        void removeKey(unsigned char scanCode);
        void setModifier(unsigned char modifier, unsigned char state);
        unsigned char keys[6] = {0};
        unsigned char modifiers = 0;
        bool kbStateDirty = false;
};

#endif