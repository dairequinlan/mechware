#ifndef INPUT_EVENT_POOL
#define INPUT_EVENT_POOL

#include "InputEvent.h"

#define INPUT_EVENT_POOL_SIZE 20

class InputEventPool {
    public:
        InputEventPool();
        InputEvent* getInputEvent(InputEventType type);

    private:
        InputEvent inputEvents[INPUT_EVENT_POOL_SIZE];
};
#endif