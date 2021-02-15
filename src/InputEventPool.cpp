#include "InputEventPool.h"

InputEventPool::InputEventPool(){

}
    
InputEvent* InputEventPool::getInputEvent(InputEventType type) {
    for(int c1 = 0;c1 < INPUT_EVENT_POOL_SIZE;c1 ++) {
        if(inputEvents[c1].type == FREE) {
            inputEvents[c1].type = type;
            return &inputEvents[c1];
        }
    }
    return nullptr;
}