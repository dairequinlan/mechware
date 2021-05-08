#include "../KeyDefines.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "WireHandler.h"
#include "I2CWireHandler.h"

I2CWireHandler::I2CWireHandler():WireHandler() {
    printf("Initialising I2C Wire Handler");

    busSpeed = i2c_init(i2c1, 100000);
    i2c_set_slave_mode(i2c1,true, 0x75);

    gpio_set_function(27, GPIO_FUNC_I2C);
    gpio_set_function(26, GPIO_FUNC_I2C);
    gpio_pull_up(27);
    gpio_pull_up(26);

}
   
bool I2CWireHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {
    if(event->type == SCANCODE && event->state == KEY_PRESSED){
        printf("scancode event, busSpeed: %d \n", busSpeed);
        uint8_t bob = (uint8_t)event->scancode;
        i2c_write_raw_blocking(i2c1, &bob, 1);
    }
    return true;
}
