#include <usb_keyboard.h>
#include "../KeyDefines.h"
#include "WireHandler.h"
#include "USBWireHandler.h"

bool USBWireHandler::keyEvent(int state, int scanCode, KeyboardState* kbState) {
    if(state == KEY_PRESSED) {
        Keyboard.press(scanCode);
    } else {
        Keyboard.release(scanCode);
    }
    return false;
}

bool USBWireHandler::printStr(char* str, KeyboardState* kbState) {
    Keyboard.print(str);
    return false;
}