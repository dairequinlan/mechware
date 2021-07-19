
#include "InputEvent.h"

InputEvent::InputEvent() {
    type = FREE; 
}

InputEvent::InputEvent(const InputEvent& event) {
    source = event.source;
    state = event.state;
    scancode = event.scancode;
    type = event.type;
    timestamp = event.timestamp;
}

InputEvent::InputEvent(uint8_t state, uint8_t scancode, uint8_t source) {
    this->source = source;
    InputEvent(state, scancode);
}

InputEvent::InputEvent(uint8_t state, uint8_t scancode) {
    type = SCANCODE;
    this->state = state;
    this->scancode = scancode;
}

void InputEvent::clear() {
    this->type = FREE;
    this->scancode = KC_NONE;
    this->source = 0;
}