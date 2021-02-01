#include "WireHandler.h"

WireHandler::WireHandler() {
}

bool WireHandler::keyEvent(int state, int scanCode, KeyboardState* kbState){
    return true;
}

bool WireHandler::printStr(char* str, KeyboardState* kbState) {
    return true;
}