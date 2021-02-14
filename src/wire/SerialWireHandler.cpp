#include "../KeyDefines.h"
#include "WireHandler.h"
#include "SerialWireHandler.h"

bool SerialWireHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {
    Serial.print(event->state == KEY_PRESSED ? "PRESSED ":"RELEASED ");
    Serial.println(event->scancode);
    return true;
}