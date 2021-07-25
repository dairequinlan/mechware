#include <usb_keyboard.h>
#include "../KeyDefines.h"
#include "WireHandler.h"
#include "USBWireHandler.h"

bool USBWireHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->type == SCANCODE && event->scancode < KC_SPECIAL){
        uint16_t scancode = event->scancode;
        if (scancode < KC_CONTROL_LEFT) {
            scancode = scancode | 0xF000;
        } else {
            switch(scancode) {
                case KC_CONTROL_LEFT:   scancode =  0x01;//0xE0
                                        break;
                case KC_SHIFT_LEFT:     scancode =  0x02;//0xE1
                                        break;
                case KC_ALT_LEFT:       scancode =  0x04; //0xE2
                                        break;
                case KC_GUI_LEFT:       scancode =  0x08; //0xE3
                                        break;
                case KC_CONTROL_RIGHT:  scancode =  0x10; //0xE4
                                        break; 
                case KC_SHIFT_RIGHT:    scancode =  0x20; //0xE5
                                        break;
                case KC_ALT_RIGHT:      scancode =  0x40; //0xE6
                                        break;
                case KC_GUI_RIGHT:      scancode =  0x80; //0xE7
                                        break;
                default:                scancode = KC_NONE;
            }
            scancode = scancode | 0xE000;
        }
        
        if(event->state == KEY_PRESSED) {
            Keyboard.press(scancode);
        } else {
            Keyboard.release(scancode);
        }
    }
    return false;
} 
