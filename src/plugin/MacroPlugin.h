#ifndef MACRO_PLUGIN
#define MACRO_PLUGIN
#include "KeyPlugin.h"
#include "../InputEvent.h"

#define MACRO_TIMEOUT 200
#define NUKE_TIMEOUT 2000
#define MACRO_SIZE 200
enum MacroPluginState {IDLE, ACTIVATED, RECORDING};

class MacroPlugin:public KeyPlugin {
    public:
        MacroPlugin();
        MacroPlugin(int *scanCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
        void replayMacro(KeyboardState* kbState);
        void storeCode(int state, int code);

        //int *stored;
        int *stored;
        int storedIndex = 8;
        int macroSize = 0;
        int lastKeyPressed = NOK;
        unsigned long lastKeyPressedTs;
        MacroPluginState pluginState = IDLE;
};

#endif