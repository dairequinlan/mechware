#ifndef INPUT_EVENT
#define INPUT_EVENT

enum InputEventType {FREE,SCANCODE};

class InputEvent {
    public:
        InputEvent();
        InputEvent(int state, int scancode);
        void clear();

        int scancode;
        int state;

        InputEventType type = SCANCODE;
};

#endif