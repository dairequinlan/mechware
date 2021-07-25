#include "../KeyDefines.h"
#include "SticKeyPlugin.h"


//click & double click keys inside this time period to lock the key until pressed again.
#define STICKEY_TIMEOUT 200

SticKeyPlugin::SticKeyPlugin(uint8_t *scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes) {
    lastKeyClicked = KC_NONE;
    lastKeyPressed = KC_NONE;
}

bool SticKeyPlugin::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->state == KEY_PRESSED) {
        lastKeyPressed = event->scancode;
        lastKeyPressedTs = millis();
    } else { // key released, this is where all the magic happens
        if(lastKeyPressed == event->scancode && (millis() - lastKeyPressedTs < STICKEY_TIMEOUT)) { // A CLICK ! A VERITABLE CLICK ! TODO check timing here
           //this is a CLICK so check see if the LAST clicked key WAS THIS ONE AS WELL
           if(lastKeyClicked == event->scancode && (millis()-lastKeyClickedTs < STICKEY_TIMEOUT)) { // ok so last click WAS ALSO this scan code i.e. double click.
               if(isKey(event->scancode)){
                   //so we have a double click on one of our keys.
                   //we return false to swallow this KEY_RELEASED
                   return false;
               }
               lastKeyClicked = KC_NONE;
           } else {
               lastKeyClicked = event->scancode; 
               lastKeyClickedTs = millis();
           }
        }
    }
    return true;
}