//stick some global key defines here, useful for a bunch of the other modules.
#ifndef GLOBAL_KEY_DEFINES
#define GLOBAL_KEY_DEFINES

#define KEY_RELEASED 1
#define KEY_PRESSED 0

//number of rows and columns
#define NUM_KEYMAPS 2
#define NUM_ROWS 5
#define NUM_COLS 9

//number of iterations of identical keyscan values before we trigger a keypress
#define DEBOUNCE_ITER 5

//microseconds between each scan. SCAN_PERIOD * DEBOUNCE_ITER = minimum response time
#define SCAN_PERIOD 3000
//do a timer event every 20 milliseconds.
#define TIMER_TICK_PERIOD 20000 

//some global key defines for function / transparent keys / custom defined etc.
#if defined(PICO)
 #define KEY_FUNCTION 0xE0 
 #define TRNS 0xE1
 #define KEY_SPECIAL 0xE2
#elif defined(TEENSY)
 #define KEY_FUNCTION -1
 #define TRNS -2
 #define KEY_SPECIAL -3

 #define KC_NONUS_BACKSLASH        ( 100  | 0xF000 )
 #define KC_NONUS_HASH             (  50  | 0xF000 )
#endif //key defines

 #define NOK 0

#endif