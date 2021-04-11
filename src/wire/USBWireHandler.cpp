#include <usb_keyboard.h>
#include "../KeyDefines.h"
#include "WireHandler.h"
#include "TinyUSBWireHandler.h"

bool TinyUSBWireHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->type == SCANCODE){
        if(event->state == KEY_PRESSED) {
            Keyboard.press(event->scancode);
        } else {
            Keyboard.release(event->scancode);
        }
    }
    return false;
}
