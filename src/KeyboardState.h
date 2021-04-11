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
        KeyboardState(  int keyMaps[2][NUM_ROWS][NUM_COLS], int nKeyMaps, 
                        KeyPlugin* keyPlugins[], int nKeyPlugins,
                        WireHandler* wireHandlers[], int nWireHandlers);
        void clearKeyStates();
        void resetKeyStates(int fromKeyMap[NUM_ROWS][NUM_COLS], int toKeyMap[NUM_ROWS][NUM_COLS]);
        void raise();
        void lower();
        int getScanCode(int row, int col);    
        int getScanCode(int keyMap[NUM_ROWS][NUM_COLS], int row, int col);
        void inputEvent(InputEvent *event);
        void runWireHandlers(InputEvent *event);
        void wirePrint(char *str);

        unsigned char keyIterCount[NUM_ROWS][NUM_COLS];
        unsigned char keyState[NUM_ROWS][NUM_COLS]; 
        int (*keyMaps)[NUM_ROWS][NUM_COLS];
        int nKeyMaps;
        KeyPlugin** keyPlugins;
        int nKeyPlugins;
        WireHandler** wireHandlers;
        int nWireHandlers;

        int activeKeyMapIndex = 0;

        InputEventPool inputEventPool;
};

#endif