#ifndef INPUT_EVENT
#define INPUT_EVENT

enum InputEventType {SCANCODE};

class InputEvent {
    public:
        InputEvent(int state, int scancode);

        int scancode;
        int state;

        InputEventType type = SCANCODE;
};

#endif