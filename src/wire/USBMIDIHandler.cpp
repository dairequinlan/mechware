#include <usb_keyboard.h>
#include "../KeyDefines.h"
#include "WireHandler.h"
#include "USBMIDIHandler.h"

struct KeyNote {
    int scancode;
    uint8_t note;
};

#define NUM_SONOME_NOTES 47
struct KeyNote sonome[NUM_SONOME_NOTES] = {
    { KC_1, 24},
    { KC_2, 31},
    { KC_3, 38},
    { KC_4, 45},
    { KC_5, 52},
    { KC_6, 59},
    { KC_7, 66},
    { KC_8, 73},
    { KC_9, 80},
    { KC_0, 87},
    { KC_MINUS, 94},
    { KC_EQUAL, 101},

    { KC_Q, 28},
    { KC_W, 35},
    { KC_E, 42},
    { KC_R, 49},
    { KC_T, 56},
    { KC_Y, 63},
    { KC_U, 70},
    { KC_I, 77},
    { KC_O, 84},
    { KC_P, 91},
    { KC_BRACKET_LEFT, 98},
    { KC_BRACKET_RIGHT, 105},

    { KC_A, 32},
    { KC_S, 39},
    { KC_D, 46},
    { KC_F, 53},
    { KC_G, 60},
    { KC_H, 67},
    { KC_J, 74},
    { KC_K, 81},
    { KC_L, 88}, 
    { KC_SEMICOLON, 95},
    { KC_APOSTROPHE, 102},
    {KC_EUROPE_1, 109},
    
    {KC_EUROPE_2, 29},
    { KC_Z, 36},
    { KC_X, 43},
    { KC_C, 50},
    { KC_V, 57},
    { KC_B, 64},
    { KC_N, 71},
    { KC_M, 78},
    { KC_COMMA, 85},
    { KC_PERIOD, 92},
    { KC_SLASH, 99}
};

#define NUM_JANKO_NOTES 47
struct KeyNote janko[NUM_JANKO_NOTES] = {
    { KC_1, 62},
    { KC_2, 64},
    { KC_3, 66},
    { KC_4, 68},
    { KC_5, 70},
    { KC_6, 72},
    { KC_7, 74},
    { KC_8, 76},
    { KC_9, 78},
    { KC_0, 80},
    { KC_MINUS, 82},
    { KC_EQUAL, 84},

    { KC_Q, 63},
    { KC_W, 65},
    { KC_E, 67},
    { KC_R, 69},
    { KC_T, 71},
    { KC_Y, 73},
    { KC_U, 75},
    { KC_I, 77},
    { KC_O, 79},
    { KC_P, 81},
    { KC_BRACKET_LEFT, 83},
    { KC_BRACKET_RIGHT, 85},

    { KC_A, 52},
    { KC_S, 54},
    { KC_D, 56},
    { KC_F, 58},
    { KC_G, 60},
    { KC_H, 62},
    { KC_J, 64},
    { KC_K, 66},
    { KC_L, 68}, 
    { KC_SEMICOLON, 70},
    { KC_APOSTROPHE, 72},
    {KC_EUROPE_1, 74},
    
    {KC_EUROPE_2, 51},
    { KC_Z, 53},
    { KC_X, 55},
    { KC_C, 57},
    { KC_V, 59},
    { KC_B, 61},
    { KC_N, 63},
    { KC_M, 65},
    { KC_COMMA, 67},
    { KC_PERIOD, 69},
    { KC_SLASH, 71}
};

#define NUM_CHROMATIC_NOTES 19
struct KeyNote chromatic[NUM_CHROMATIC_NOTES] = {
    { KC_APOSTROPHE, 71},
    { KC_BRACKET_LEFT, 70},
    { KC_SEMICOLON, 69},
    { KC_P,68},
    { KC_L,67},
    { KC_O,66},
    { KC_K,65},
    { KC_J,64},
    { KC_U,63},
    { KC_H,62},
    { KC_Y,61},
    { KC_G,60},
    { KC_F,59},
    { KC_R,58},
    { KC_D,57},
    { KC_E,56},
    { KC_S,55},
    { KC_W,54},
    { KC_A,53}    
};

USBMIDIHandler::USBMIDIHandler(uint8_t *scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes){
    usbMIDI.begin();
}

uint8_t USBMIDIHandler::getNote(MIDIMode mode, int scancode) {

    int size = 0;
    KeyNote* which = NULL;

    switch(mode) {
        case JANKO:
                size = NUM_JANKO_NOTES;
                which = janko;
                break;
        case SONOME:
                size = NUM_SONOME_NOTES;
                which = sonome;
                break;
        case CHROMATIC:
        default:
                size = NUM_CHROMATIC_NOTES;
                which = chromatic;
    } 

    for(int i = 0; i < size; i++) {
        if(scancode == which[i].scancode) {
            return which[i].note;
        }
    }

    return 0;
}

bool USBMIDIHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {

    if(event->type == SCANCODE) {
        //do the switcheroo with modes if the appropriate key is hit.
        if(nCodes > 0 && event->scancode == scanCodes[0]
            && event->state == KEY_RELEASED) {
            switch(mode) {
                case DISABLED:  mode = JANKO;
                                break;
                case JANKO:     mode = SONOME;                
                                break;
                case SONOME:    mode = CHROMATIC;
                                break;
                case CHROMATIC: mode = DISABLED;
                                break;
            }
            Serial.print("MIDI MODE: ");
            Serial.println(mode);
            return false;
        }

        if(mode != DISABLED) {
            uint8_t note = getNote(mode, event->scancode);

            if(event->state ==  KEY_PRESSED) {
                if (note != 0) usbMIDI.sendNoteOn(note, 99, 1);
            } else {
                if (note != 0) usbMIDI.sendNoteOff(note, 99, 1);
            }
            return false;
        } 
    }

    return true;
}
