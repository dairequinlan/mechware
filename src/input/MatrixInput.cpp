
#if defined(PICO)
 #include <stdio.h>
 #include "pico/stdlib.h"
#elif defined(TEENSY)
 #include <Arduino.h>
#endif

#include "MatrixInput.h"

extern uint8_t rowPins[NUM_ROWS];
extern uint8_t colPins[NUM_COLS];

MatrixInput::MatrixInput() {
  //set the row pins to 'input' which is the default 'disabled' state
  #if defined(PICO)
    for(int row=0; row < NUM_ROWS; row++) {
        gpio_init(rowPins[row]);
        gpio_set_dir(rowPins[row], GPIO_IN);
        gpio_pull_up(rowPins[row]);
    } 
    for (int col=0; col < NUM_COLS; col++) {
        gpio_init(colPins[col]);
        gpio_set_dir(colPins[col], GPIO_IN);
        gpio_pull_up(colPins[col]);
    }
  #elif defined(TEENSY)
    for(int row=0; row < NUM_ROWS; row++) {
          pinMode(rowPins[row], INPUT);
    } 
    for (int col=0; col < NUM_COLS; col++) {
        pinMode(colPins[col], INPUT_PULLUP);
    }
  #endif
}

bool MatrixInput::scan(KeyboardState* keyboardState) {

  //First loop runs through each of the rows,
  for (int row=0; row < NUM_ROWS; row++) {
        //for each row pin, set to OUTPUT and LOW 
       #if defined(PICO)
        gpio_set_dir(rowPins[row], GPIO_OUT);
        gpio_put(rowPins[row], false);
        //needed for some reason on the PICO, but not the teensy.
        //not to sure what difference a 1us difference makes, but 
        //if it's not done then the first colum often gives duplicates
        //as the previous row pin hasn't fully reset to an input yet
        busy_wait_us_32(1);
       #elif defined(TEENSY)
        pinMode(rowPins[row], OUTPUT);
        digitalWrite(rowPins[row], LOW);
       #endif 
        //now iterate through each of the columns, set to input_pullup, 
        //the Row is output and low, and we have input pullup on the column pins,
        //so a '1' is an un pressed switch, and a '0' is a pressed switch.
        for (int col=0; col < NUM_COLS; col++) {
           #if defined(PICO)
            bool boolVal = gpio_get(colPins[col]);
            unsigned char value = boolVal ? KEY_RELEASED : KEY_PRESSED;
           #elif defined(TEENSY) 
            unsigned char value = digitalRead(colPins[col]);
           #endif
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
                uint8_t scanCode = keyboardState->getScanCode(row,col);
                InputEvent* inputEvent = keyboardState->inputEventPool.getInputEvent(SCANCODE);
                inputEvent->scancode = scanCode;
                inputEvent->state = keyboardState->keyState[row][col];
                keyboardState->inputEvent(inputEvent);
                #if defined(PICO)
                 printf("%i %i %i\n",scanCode,row,col);
                #elif defined(TEENSY)
                 Serial.print("Debounced ");
                 Serial.print(scanCode);
                 Serial.print(" ");
                 Serial.print(inputEvent->state);
                 Serial.println("");
                #endif
              }
            }
        }
        //now just reset the pin mode (effectively disabling it)
       #if defined(PICO)
        gpio_set_dir(rowPins[row], GPIO_IN);
       #elif defined(TEENSY)
        pinMode(rowPins[row], INPUT);
       #endif
  }
  return true;
}
