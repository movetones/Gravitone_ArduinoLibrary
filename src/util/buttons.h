#ifndef GRAVITONE_BUTTON_INTERFACE
#define GRAVITONE_BUTTON_INTERFACE

// BUTTON CONNECTIONS w.r.t. TO MCP23017
enum ButtonMap {
    GB1,
    GB2,
    GB3,
    GB4,
    GB5,
    GB6,
    GB7,
    GB8,
    GB9,
    GB10,
    GB11,
    GB12
};

#define HOLD_DURATION 400 // milliseconds for hold to register

#define NUM_BUTTONS   12


typedef uint8_t butevent_t;

enum ButtonEvent {
  BUTTON_OPEN=8,
  BUTTON_PRESSED=9,  // this is a user pressing the button
  BUTTON_RELEASED=10, // this is the user releasing the button
  BUTTON_HELD=11      // the button has been held down
};
  

struct ButtonState {
  unsigned long t;
  uint8_t state[NUM_BUTTONS];
};


#endif
