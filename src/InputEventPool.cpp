#if defined(PICO)
 #include <stdio.h>
 #include "pico/stdlib.h"
#endif 
#include "InputEventPool.h"

InputEventPool::InputEventPool(){

}
    
InputEvent* InputEventPool::getInputEvent(InputEventType type) {
    for(int c1 = 0;c1 < INPUT_EVENT_POOL_SIZE;c1 ++) {
        if(inputEvents[c1].type == FREE) {
            inputEvents[c1].type = type;
            inputEvents[c1].timestamp = time_us_32();
            return &inputEvents[c1];
        }
    }
    printf("UNABLE TO FIND INPUT EVENT");
    return nullptr;
}