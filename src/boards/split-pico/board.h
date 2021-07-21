#ifndef BOARD_CONFIG
#define BOARD_CONFIG

#include "../../KeyDefines.h"

#if defined(LEFT_HAND_SIDE)
 #define NUM_ROWS 5
 #define NUM_COLS 9
#elif defined(RIGHT_HAND_SIDE)
 #define NUM_ROWS 5
 #define NUM_COLS 9
#endif

#define NUM_KEYMAPS 2
#define NUM_INPUT_DEVICES 2
#define NUM_KEYPLUGINS 2

#if defined(PRIMARY)
 #define NUM_WIREHANDLERS 2
#else // secondary
 #define NUM_WIREHANDLERS 1
#endif 

#endif