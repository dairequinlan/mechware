#include "KeyboardState.h"

KeyboardState::KeyboardState(
                            int keyMaps[2][NUM_ROWS][NUM_COLS] , int nKeyMaps,
                            KeyPlugin* keyPlugins[], int nKeyPlugins,
                            WireHandler* wireHandlers[], int nWireHandlers) {
  this->keyMaps = keyMaps;
  this->nKeyMaps = nKeyMaps;
  this->keyPlugins = keyPlugins;
  this->nKeyPlugins = nKeyPlugins;
  this->wireHandlers = wireHandlers;
  this->nWireHandlers = nWireHandlers;
  //set the initial values on the iter count and state arrays.
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      //initial iter value is debounce + 1 so that a key transition isn't immediately detected on startup.
      keyIterCount[row][col] = DEBOUNCE_ITER + 1;
      keyState[row][col] = KEY_RELEASED;
    }
  }
}

//functions to raise and lower the active keymap
//for the Fn key this will likely just be 0 and 1 but for multi layer keebs 
//this could be arbitrary so may as well bake this in from the start
void KeyboardState::raise() {
  if(activeKeyMapIndex < (nKeyMaps-1)) { //we can do a raise op
    activeKeyMapIndex ++;
    resetKeyStates(keyMaps[activeKeyMapIndex-1], keyMaps[activeKeyMapIndex]);
  }
}

void KeyboardState::lower() {
  if(activeKeyMapIndex > 0) {  //we can do a lower op
  	activeKeyMapIndex --;
    resetKeyStates(keyMaps[activeKeyMapIndex+1], keyMaps[activeKeyMapIndex]);
  }
}

//gets the scancode from the currently active keymap
int KeyboardState::getScanCode(int row, int col) {
  return getScanCode(keyMaps[activeKeyMapIndex],row,col);
}

//gets the scancode given a keymap and row/col
//given the layer and row/col, get the appropriate scancode
int KeyboardState::getScanCode(int keyMap[5][16], int row, int col) {
  int scanCode = keyMap[row][col];
  int trnsIndex = activeKeyMapIndex;
  while(scanCode == TRNS && trnsIndex >= 0) {
    scanCode = keyMaps[trnsIndex--][row][col];
  }
  return scanCode;
}

//input event raised by one of the input plugins
void KeyboardState::inputEvent(InputEvent* event) {
//first plugins, and bail out of the loop and method
  //if any of them return false.
  for(int plugin = 0; plugin < nKeyPlugins; plugin ++) {
    if(!keyPlugins[plugin]->inputEvent(event, this)) {
      event->clear();
      return;
    }
  }
  //now our wire handler(s). We'll do this in a separate method
  //because the plugins can actually call this same method and
  //in that case we want all the wire handlers run as though there
  //was a key event.
  runWireHandlers(event);
  event->clear();
}

//run the wire handlers in order, bail if any of them return false;
//this can be run from
//    1. the normal key event method above, 
//    2. plugins that transmit something directly on the wire
//    3. a couple of other methods here like the reset 
void KeyboardState::runWireHandlers(InputEvent* event) {
  for(int wireHandler = 0; wireHandler < nWireHandlers; wireHandler++ ){
    if(!wireHandlers[wireHandler]->inputEvent(event, this)) {
        //TODO have to call clear here as well because this 'runWireHandlers'
        //     is called from everywhere to add keys to the events.
        //     should be removed when we're adding the keys to a queue.
        //     (which means they'll ALL go through the 'inputEvent' above)
        event->clear();
        return;
    }
  }
  //ditto on comment above.
  event->clear();
}

//clear all the keystate, if it's 'pressed' then we want 
//to do a 'KEY_RELEASED' event on the wire 
void KeyboardState::clearKeyStates() {
    for (int row = 0; row < NUM_ROWS; row ++) {
      for (int col = 0; col < NUM_COLS; col++) {
        if(keyState[row][col] == KEY_PRESSED) {
            runWireHandlers(new InputEvent(KEY_RELEASED, getScanCode(row,col)));
        }
        keyIterCount[row][col] = DEBOUNCE_ITER + 1;
        keyState[row][col] = KEY_RELEASED;
      }
    }
}

//Pressing the FN key could potentially shift the scan code between the key being pressed 
//and being released. If the FN key is hit then any pressed keys have to be reset to be in the 
//'released' state and their iter counts set to DEBOUNCE_ITER+1. 
//Quick improvement: Only do this if the scan codes are different in the two maps. This means that 
//any keys that are the same between the layers like the modifiers will remain pressed.
void KeyboardState::resetKeyStates(int fromKeyMap[5][16], int toKeyMap[5][16]) {
  //set the initial values on the iter count and state arrays.
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      //only reset if it's not a 'dupe' key i.e. same as previous layer either explicitly or with the TRNS keycode.
      if(fromKeyMap[row][col] != TRNS && toKeyMap[row][col] != TRNS && toKeyMap[row][col] != fromKeyMap[row][col]) {
        keyIterCount[row][col] = DEBOUNCE_ITER + 1;
        //if it's currently PRESSED then we have to 'release' the 'from' map keycode, and 'press' the 'to' keycode
         if(keyState[row][col] == KEY_PRESSED) {
            int fromKeyCode = getScanCode(fromKeyMap,row,col);
            int toKeyCode =   getScanCode(toKeyMap,row,col);
            runWireHandlers(new InputEvent(KEY_RELEASED, fromKeyCode));
            runWireHandlers(new InputEvent(KEY_PRESSED, toKeyCode));
        }       
      }
    }
  }
}