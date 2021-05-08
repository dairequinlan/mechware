#ifndef I2C_WIRE_HANDLER
#define I2C_WIRE_HANDLER
#include "WireHandler.h"

class I2CWireHandler : public WireHandler {
    public:
        I2CWireHandler();
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
    private:
        unsigned int busSpeed = 0;
};

#endif