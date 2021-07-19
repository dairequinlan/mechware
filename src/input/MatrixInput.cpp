
#if defined(PICO)
 #include <stdio.h>
 #include "pico/stdlib.h"
#elif defined(TEENSY)
 #include <Arduino.h>
#endif

#include "MatrixInput.h"

//TODO get these pin assignments and put them someplace else along with the
//     key map defines and so on, the LEFT/RIGHT are VERY SPECIFIC to the split keyboard
//     that I've built with the PICOs, and the default case i.e. meant to represent
//     a single unit non-split keyboard, is specific to the particular keyboard I 
//     built with the teensy in it.
#if defined(LEFT_HAND_SIDE)
 unsigned char rowPins[NUM_ROWS] = {13,12,11,10,9};
 unsigned char colPins[NUM_COLS] = {15,14,8,7,6,5,4,3,2};
#elif defined(RIGHT_HAND_SIDE)
 unsigned char rowPins[NUM_ROWS] = {1,2,3,4,5};
 unsigned char colPins[NUM_COLS] = {13,12,11,10,9,8,7,6,0};
#else //will be triggered at the moment only in the teensy case.
  unsigned char rowPins[NUM_ROWS] = {13,21,20,18,19};
  unsigned char colPins[NUM_COLS] = {12,11,10,9,8,7,6,5,4,3,2,1,17,16,15,14};
#endif

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
