#ifndef GRAVITONE_BUTTON_DRIVER
#define GRAVITONE_BUTTON_DRIVER

#include "GravitoneButtonInterface.h"

static GravitoneButtonInterface buttons;

class GravitoneButtonDriver {
    
public:
    GravitoneButtonDriver(){}
    
    void begin(void (*callback)(GravitoneButtonEvent)) { 
        //Serial.println("Starting buttons");  
        buttons.begin();
        attachInterrupt(digitalPinToInterrupt(MCP23017_INTA), buttonISR, FALLING); 
        //attachInterrupt(digitalPinToInterrupt(MCP23017_INTB), buttonISR, FALLING); 
        buttons.setCallback(callback);
    }
    
    static void buttonISR() {
        //Serial.println("in isr");
        detachInterrupt(digitalPinToInterrupt(MCP23017_INTA));
        //detachInterrupt(digitalPinToInterrupt(MCP23017_INTB));
        buttons.processInterrupt();
        attachInterrupt(digitalPinToInterrupt(MCP23017_INTA), buttonISR, FALLING);
        //attachInterrupt(digitalPinToInterrupt(MCP23017_INTB), buttonISR, FALLING);
    }
    
    static GravitoneButtonDriver driver;
};

#endif
