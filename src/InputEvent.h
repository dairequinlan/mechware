#ifndef INPUT_EVENT
#define INPUT_EVENT
#include "pico/stdlib.h"
#include "KeyDefines.h"

enum InputEventType {FREE,SCANCODE,TIMER};

class InputEvent {
    public:
        InputEvent();
        InputEvent(const InputEvent& event);
        InputEvent(uint8_t state, uint8_t scancode);
        InputEvent(uint8_t state, uint8_t scancode,uint8_t source);
        void clear();

        uint8_t  scancode = NOK;
        uint8_t  state = 0;
        uint32_t timestamp;
        uint8_t source = 0;

        InputEventType type = SCANCODE;
};

#endif