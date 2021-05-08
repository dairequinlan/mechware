
#include "PicoRebootPlugin.h"
#include "KeyDefines.h"
#include "pico/bootrom.h"

PicoRebootPlugin::PicoRebootPlugin(int *scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes){
}

bool PicoRebootPlugin::inputEvent(InputEvent* event, KeyboardState* kbState){
    if(isKey(event->scancode)) {
        reset_usb_boot(0, 0);
    }
    return true; 
}