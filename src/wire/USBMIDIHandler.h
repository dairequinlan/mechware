#ifndef USB_MIDI_HANDLER
#define USB_MIDI_HANDLER
#include "WireHandler.h"

enum MIDIMode {DISABLED,JANKO,CHROMATIC};

class USBMIDIHandler : public WireHandler, public KeyPlugin {
    public:
        USBMIDIHandler (uint8_t *scanCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
    private:
        uint8_t getNote(MIDIMode mode, int scancode);
        MIDIMode mode = DISABLED;
};

#endif