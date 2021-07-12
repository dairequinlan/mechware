#include <Arduino.h>
#include "MatrixInput.h"

byte rowPins[NUM_ROWS] = {13,21,20,18,19};
byte colPins[NUM_COLS] = {12,11,10,9,8,7,6,5,4,3,2,1,17,16,15,14};

MatrixInput::MatrixInput() {

  for(int row=0; row < NUM_ROWS; row++) {
        pinMode(rowPins[row], INPUT);
  }
  
  for (int col=0; col < NUM_COLS; col++) {
        pinMode(colPins[col], INPUT_PULLUP);
  }
}

bool MatrixInput::scan(KeyboardState* keyboardState) {
  //First loop runs through each of the rows,
  for (int row=0; row < NUM_ROWS; row++) {
        //for each row pin, set to output LOW 
        pinMode(rowPins[row], OUTPUT);
        digitalWrite(rowPins[row], LOW);
        
        //now iterate through each of the columns, set to input_pullup, 
        //the Row is output and low, and we have input pullup on the column pins,
        //so a '1' is an un pressed switch, and a '0' is a pressed switch.
        for (int col=0; col < NUM_COLS; col++) {
            byte value = digitalRead(colPins[col]);
            //if the value is different from the stored value, then reset the count and set the stored value.
            if(value == KEY_PRESSED && keyboardState->keyState[row][col] == KEY_RELEASED) {
              keyboardState->keyState[row][col] = KEY_PRESSED;
              keyboardState->keyIterCount[row][col] = 0;
            } else if (value == KEY_RELEASED && keyboardState->keyState[row][col] == KEY_PRESSED) {
              keyboardState->keyState[row][col] = KEY_RELEASED;
              keyboardState->keyIterCount[row][col] = 0;
            } else {
              //Stored value is the same as the current value, this is where our debounce magic happens.
              //if the keyIterCount < debounce iter then increment the keyIterCount and move on
              //if it's == debounce iter then trigger the key & increment it so the trigger doesn't happen again.
              //if it's > debounce iter then we do nothing, except check for the FN key being pressed.
              if(keyboardState->keyIterCount[row][col] < DEBOUNCE_ITER) {
                keyboardState->keyIterCount[row][col] ++;
              } else if (keyboardState->keyIterCount[row][col] == DEBOUNCE_ITER) {
                keyboardState->keyIterCount[row][col] ++;
                int scanCode = keyboardState->getScanCode(row,col);
                InputEvent* inputEvent = keyboardState->inputEventPool.getInputEvent(SCANCODE);
                inputEvent->scancode = scanCode;
                inputEvent->state = keyboardState->keyState[row][col];
                keyboardState->inputEvent(inputEvent);
              }
            }
        }
        //now just reset the pin mode (effectively disabling it)
        pinMode(rowPins[row], INPUT);
  }
  return true;
}
