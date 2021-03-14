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


struct ButtonEvent {
    int id;
    int value;
};


enum EventType {
    EVENT_MENU,
    EVENT_EFFECT
    // ...
};

#endif
