#include "../KeyDefines.h"
#include "WireHandler.h"
#include "TinyUSBWireHandler.h"


bool TinyUSBWireHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {

    if(event->type == SCANCODE && event->scancode < KC_SPECIAL){

        if(event->state == KEY_PRESSED) {
            addKey(event->scancode);
            if(event->scancode >= HID_KEY_CONTROL_LEFT && event->scancode <= HID_KEY_GUI_RIGHT) {
                setModifier(event->scancode - HID_KEY_CONTROL_LEFT,1);
            }
        } else {        
            removeKey(event->scancode);
            if(event->scancode >= HID_KEY_CONTROL_LEFT && event->scancode <= HID_KEY_GUI_RIGHT) {
                setModifier(event->scancode - HID_KEY_CONTROL_LEFT,0); 
            }
        }
        //send the new keyboard report here IFF we're able. Otherwise set the 
        //dirty flag and it will be sent on the next timer event tick
        if (tud_hid_ready()) {
            tud_hid_keyboard_report(HID_PROTOCOL_KEYBOARD,modifiers,keys);
            //reset the dirty flag if we've managed to sent the report, could happen 
            //on next input event without having to wait for timer event.
            kbStateDirty = false;
        } else {
            kbStateDirty = true;
        }
    } else if (event->type == TIMER) { //timer event
        if (tud_hid_ready() && kbStateDirty) {
            tud_hid_keyboard_report(HID_PROTOCOL_KEYBOARD,modifiers,keys);    
            kbStateDirty = false;
        }
    }
    return false;
}

void TinyUSBWireHandler::addKey(uint8_t scancode) {
    //let's check if it's already IN the keys
    for(int i = 0; i < 6; i++) {
        if(keys[i] == scancode){
            return;
        }
    }
    //now lets find an empty slot
    //no empty slot => no keypress
    for(int i = 0; i < 6; i++) {
        if(keys[i] == 0) {
            keys[i] = scancode;
            break;
        }
    }
}

void TinyUSBWireHandler::removeKey(uint8_t scancode) {
    for(int i = 0; i < 6; i++) {
        if(keys[i] == scancode) {
            keys[i] = 0;
            break;
        }
    }
}

void TinyUSBWireHandler::setModifier(uint8_t modifier, uint8_t state) {
    state ? modifiers |= 1UL << modifier : modifiers &= ~(1UL << modifier);
}
