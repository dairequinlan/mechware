
#include "InputEvent.h"

InputEvent::InputEvent() {
    type = FREE; 
}

InputEvent::InputEvent(int state, int scancode) {
    type = SCANCODE;
    this->state = state;
    this->scancode = scancode;
}

void InputEvent::clear() {
    this->type = FREE;
}