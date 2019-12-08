#ifndef GRAVITONE_BUTTON_INTERFACE
#define GRAVITONE_BUTTON_INTERFACE

#include <Adafruit_MCP23017.h>

// PIN DEFINITIONS FOR GRAVITONE V1.5 PCB
// CONNECTED TO A TEENSY 3.2
#define MCP23017_INTA	8  // MCP -> ARDUINO
#define MCP23017_INTB	7  // MCP -> ARDUINO

// BUTTON CONNECTIONS w.r.t. TO MCP23017
enum GravitoneButton {
    GPIN_A0,
    GPIN_A1,
    GPIN_A2,
    GPIN_A3,
    GPIN_A4,
    GPIN_A5,
    GPIN_B0,
    GPIN_A7,
    GPIN_A6,
    GPIN_B3,
    GPIN_B2,
    GPIN_B1
};


struct GravitoneButtonEvent {
    int id;
    int value;
};


enum GravitoneButtonEventType {
    EVENT_MENU,
    EVENT_KEYBOARD
};


enum GravitoneButtonMenuEvent {
    MENU_PREV,
    MENU_SELECT,
    MENU_NEXT,
    MENU_BACK
};


enum GravitoneButtonKeyboardEvent {
    GKEY_F1,
    GKEY_F2,
    GKEY_F3,
    GKEY_F4,
    GKEY_F5,
    GKEY_F6,
    GKEY_F7,
    GKEY_F8,
    GKEY_F9,
    GKEY_F10,
    GKEY_F11
};

class GravitoneButtonInterface {

public:
    GravitoneButtonInterface(void (*handler)(int, bool)) : eventHandler(handler) {};

    void begin();
    bool available();
    bool* changed();
    void update();

    void (*eventHandler)(int, bool);
    bool statesChanged[12];
    GravitoneButtonEvent states[12];
    
    Adafruit_MCP23017 mcp;
};

#endif
