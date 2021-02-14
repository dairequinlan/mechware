#ifndef MATRIX_INPUT
#define MATRIX_INPUT
#include "../KeyboardState.h"
#include "../InputEvent.h"
#include "InputDevice.h"

class MatrixInput: public InputDevice {
    public:
        MatrixInput();
        virtual bool scan(KeyboardState* keyboardState);
};

#endif