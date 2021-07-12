#ifndef USB_MIDI_HANDLER
#define USB_MIDI_HANDLER
#include "WireHandler.h"

class USBMIDIHandler : public WireHandler {
    public:
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
        uint8_t getNote(int scancode);
};

#endif