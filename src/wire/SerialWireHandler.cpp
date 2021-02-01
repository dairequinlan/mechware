#include "../KeyDefines.h"
#include "WireHandler.h"
#include "SerialWireHandler.h"

bool SerialWireHandler::keyEvent(int state, int scanCode, KeyboardState* kbState) {
    Serial.print(state == KEY_PRESSED ? "PRESSED ":"RELEASED ");
    Serial.println(scanCode);
    return true;
}