#ifndef PICO_REBOOT_PLUGIN
#define PICO_REBOOT_PLUGIN

#if defined(PICO)

#include "KeyPlugin.h"

class PicoRebootPlugin:public KeyPlugin {
    public :
        PicoRebootPlugin();
        PicoRebootPlugin(uint8_t *scanCodes, int nCodes);
        virtual bool inputEvent(InputEvent* event, KeyboardState* kbState);
};
#endif
#endif