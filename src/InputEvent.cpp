
#include "InputEvent.h"

InputEvent::InputEvent(int state, int scancode) {
    type = SCANCODE;
    this->state = state;
    this->scancode = scancode;
}