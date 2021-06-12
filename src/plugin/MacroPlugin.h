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
        MacroPlugin(uint8_t *scanCodes, int nCodes);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
        void replayMacro(KeyboardState* kbState);
        void storeCode(uint8_t state, uint8_t code);

        //int *stored;
        uint8_t *stored;
        int storedIndex = 8;
        int macroSize = 0;
        uint8_t lastKeyPressed = NOK;
        uint32_t lastKeyPressedTs;
        MacroPluginState pluginState = IDLE;
};

#endif