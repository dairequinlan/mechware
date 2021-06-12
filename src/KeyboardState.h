#ifndef KEYBOARD_STATE
#define KEYBOARD_STATE

#if defined(PICO)

#elif defined(TEENSY) 
 #include <arduino.h>
#endif 

#include "KeyDefines.h"
#include "InputEvent.h"
#include "InputEventPool.h"
#include "plugin/KeyPlugin.h"
#include "wire/WireHandler.h"


class KeyPlugin;
class WireHandler;

class KeyboardState {
    public:
        KeyboardState(  uint8_t keyMaps[2][NUM_ROWS][NUM_COLS], int nKeyMaps, 
                        KeyPlugin* keyPlugins[], int nKeyPlugins,
                        WireHandler* wireHandlers[], int nWireHandlers);
        void clearKeyStates();
        void resetKeyStates(uint8_t fromKeyMap[NUM_ROWS][NUM_COLS], uint8_t toKeyMap[NUM_ROWS][NUM_COLS]);
        void raise();
        void lower();
        uint8_t getScanCode(int row, int col);    
        uint8_t getScanCode(uint8_t keyMap[NUM_ROWS][NUM_COLS], int row, int col);
        void inputEvent(InputEvent *event);
        void runWireHandlers(InputEvent *event);
        void wirePrint(char *str);

        unsigned char keyIterCount[NUM_ROWS][NUM_COLS];
        uint8_t keyState[NUM_ROWS][NUM_COLS]; 
        uint8_t (*keyMaps)[NUM_ROWS][NUM_COLS];
        int nKeyMaps;
        KeyPlugin** keyPlugins;
        int nKeyPlugins;
        WireHandler** wireHandlers;
        int nWireHandlers;

        int activeKeyMapIndex = 0;

        InputEventPool inputEventPool;
};

#endif