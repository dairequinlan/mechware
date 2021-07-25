#ifndef I2C_WIRE_HANDLER
#define I2C_WIRE_HANDLER
#include "WireHandler.h"
#include "../input/InputDevice.h"
#include "../plugin/KeyPlugin.h" 

#define LOCAL_I2C_ADDRESS 0x00
#define PRIMARY_I2C_ADDRESS 0x75
#define SECONDARY_I2C_ADDRESS 0x76
#define I2C_TIMEOUT_US 100000
#define I2C_BUS_SPEED 100000

class I2CWireHandler : public InputDevice, public WireHandler {
    public:
        I2CWireHandler(bool primary);
        bool inputEvent(InputEvent* event, KeyboardState* kbState);
        bool scan(KeyboardState *keyboardState);
    private:
        unsigned int busSpeed = 0;
        bool primary = true;
};

#endif