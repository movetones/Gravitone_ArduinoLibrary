#ifndef GRAVITONE_DRIVER
#define GRAVITONE_DRIVER

#include "GravitoneButtonInterface.h"
#include "GravitoneDisplayInterface.h"

static GravitoneButtonInterface buttons;
static GravitoneDisplayInterface display;

class GravitoneDriver {
    
public:
    GravitoneDriver(){}
    
    void begin(void (*callback)(GravitoneButtonEvent)) { 
        //Serial.println("Starting display");
        display.begin();
        
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
    
    static GravitoneDriver driver;
};

#endif
