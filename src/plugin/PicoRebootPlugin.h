#ifndef PICO_REBOOT_PLUGIN
#define PICO_REBOOT_PLUGIN
#include "KeyPlugin.h"

class PicoRebootPlugin:public KeyPlugin {
    public :
        PicoRebootPlugin();
        PicoRebootPlugin(int *scanCodes, int nCodes);
        virtual bool inputEvent(InputEvent* event, KeyboardState* kbState);
};

#endif