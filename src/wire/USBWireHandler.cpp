#include <usb_keyboard.h>
#include "../KeyDefines.h"
#include "WireHandler.h"
#include "USBWireHandler.h"

bool USBWireHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->state == KEY_PRESSED) {
        Keyboard.press(event->scancode);
    } else {
        Keyboard.release(event->scancode);
    }
    return false;
}
