#if defined(PICO)
 #include <stdio.h>
 #include "pico/stdlib.h"
#elif defined(TEENSY)
 #include <Arduino.h>
#endif 
#include "InputEventPool.h"

InputEventPool::InputEventPool(){

}

InputEvent* InputEventPool::getInputEvent(InputEvent event){
    InputEvent* poolEvent = getInputEvent(event.type);
    poolEvent->source = event.source;
    poolEvent->state = event.state;
    poolEvent->scancode = event.scancode;
    poolEvent->timestamp = event.timestamp;
    return poolEvent;
}

InputEvent* InputEventPool::getInputEvent(InputEventType type) {
    for(int c1 = 0;c1 < INPUT_EVENT_POOL_SIZE;c1 ++) {
        if(inputEvents[c1].type == FREE) {
            inputEvents[c1].type = type;
            #if defined(PICO) 
             inputEvents[c1].timestamp = time_us_32();
            #elif defined(TEENSY)
             inputEvents[c1].timestamp = micros();
            #endif
            
            return &inputEvents[c1];
        }
    }
    //printf("UNABLE TO FIND INPUT EVENT");
    return nullptr;
}