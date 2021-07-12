#include <usb_keyboard.h>
#include "../KeyDefines.h"
#include "WireHandler.h"
#include "USBMIDIHandler.h"

struct KeyNote {
    int scancode;
    uint8_t note;
};

#define NUM_JANKO_NOTES 47
struct KeyNote janko[NUM_JANKO_NOTES] = {
    {KEY_1, 62},
    {KEY_2, 64},
    {KEY_3, 66},
    {KEY_4, 68},
    {KEY_5, 70},
    {KEY_6, 72},
    {KEY_7, 74},
    {KEY_8, 76},
    {KEY_9, 78},
    {KEY_0, 80},
    {KEY_MINUS, 82},
    {KEY_EQUAL, 84},

    {KEY_Q, 63},
    {KEY_W, 65},
    {KEY_E, 67},
    {KEY_R, 69},
    {KEY_T, 71},
    {KEY_Y, 73},
    {KEY_U, 75},
    {KEY_I, 77},
    {KEY_O, 79},
    {KEY_P, 81},
    {KEY_LEFT_BRACE, 83},
    {KEY_RIGHT_BRACE, 85},

    {KEY_A, 52},
    {KEY_S, 54},
    {KEY_D, 56},
    {KEY_F, 58},
    {KEY_G, 60},
    {KEY_H, 62},
    {KEY_J, 64},
    {KEY_K, 66},
    {KEY_L, 68}, 
    {KEY_SEMICOLON, 70},
    {KEY_QUOTE, 72},
    {KC_NONUS_HASH, 74},
    
    {KC_NONUS_BACKSLASH, 51},
    {KEY_Z, 53},
    {KEY_X, 55},
    {KEY_C, 57},
    {KEY_V, 59},
    {KEY_B, 61},
    {KEY_N, 63},
    {KEY_M, 65},
    {KEY_COMMA, 67},
    {KEY_PERIOD, 69},
    {KEY_SLASH, 71}
};

#define NUM_CHROMATIC_NOTES 19
struct KeyNote chromatic[NUM_CHROMATIC_NOTES] = {
    {KEY_QUOTE, 71},
    {KEY_LEFT_BRACE, 70},
    {KEY_SEMICOLON, 69},
    {KEY_P,68},
    {KEY_L,67},
    {KEY_O,66},
    {KEY_K,65},
    {KEY_J,64},
    {KEY_U,63},
    {KEY_H,62},
    {KEY_Y,61},
    {KEY_G,60},
    {KEY_F,59},
    {KEY_R,58},
    {KEY_D,57},
    {KEY_E,56},
    {KEY_S,55},
    {KEY_W,54},
    {KEY_A,53}    
};

uint8_t USBMIDIHandler::getNote(int scancode) {

    for(int i = 0; i < NUM_JANKO_NOTES; i++) {
        if(scancode == janko[i].scancode) {
            return janko[i].note;
        }
    }

    return 0;
}

bool USBMIDIHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->type == SCANCODE){
        uint8_t note = getNote(event->scancode);
        if(event->state == KEY_PRESSED) {
             if (note != 0) usbMIDI.sendNoteOn(note, 99, 1);
        } else {
            if (note != 0) usbMIDI.sendNoteOff(note, 99, 1);
        }
    }
    return false;
}
