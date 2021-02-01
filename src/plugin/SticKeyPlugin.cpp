#include "../KeyDefines.h"
#include "SticKeyPlugin.h"


//click & double click keys inside this time period to lock the key until pressed again.
#define STICKEY_TIMEOUT 200

SticKeyPlugin::SticKeyPlugin(int* scanCodes, int nCodes):KeyPlugin(scanCodes, nCodes) {
    lastKeyClicked = NOK;
    lastKeyPressed = NOK;
}

bool SticKeyPlugin::keyEvent(int state, int scanCode, KeyboardState* kbState) {
    if(state == KEY_PRESSED) {
        lastKeyPressed = scanCode;
        lastKeyPressedTs = millis();
    } else { // key released, this is where all the magic happens
        if(lastKeyPressed == scanCode && (millis() - lastKeyPressedTs < STICKEY_TIMEOUT)) { // A CLICK ! A VERITABLE CLICK ! TODO check timing here
           //this is a CLICK so check see if the LAST clicked key WAS THIS ONE AS WELL
           if(lastKeyClicked == scanCode && (millis()-lastKeyClickedTs < STICKEY_TIMEOUT)) { // ok so last click WAS ALSO this scan code i.e. double click.
               if(isKey(scanCode)){
                   //so we have a double click on one of our keys.
                   //we return false to swallow this KEY_RELEASED
                   return false;
               }
               lastKeyClicked = NOK;
           } else {
               lastKeyClicked = scanCode; 
               lastKeyClickedTs = millis();
           }
        }
    }
    return true;
}