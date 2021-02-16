#ifndef INPUT_EVENT
#define INPUT_EVENT
#include "KeyDefines.h"

enum InputEventType {FREE,SCANCODE,TIMER};

class InputEvent {
    public:
        InputEvent();
        InputEvent(int state, int scancode);
        void clear();

        int scancode = NOK;
        int state;
        unsigned int timestamp;

        InputEventType type = SCANCODE;
};

#endif