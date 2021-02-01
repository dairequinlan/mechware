#ifndef INPUT_DEVICE
#define INPUT_DEVICE

#include "../KeyboardState.h"

class InputDevice {
    public:
        InputDevice();
        virtual bool scan(KeyboardState* keyboardState);

};

#endif