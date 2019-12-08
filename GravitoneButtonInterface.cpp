#include "GravitoneButtonInterface.h"

void GravitoneButtonInterface::begin()
{
    // SET INTERRUPT PINS AS INPUTS
    pinMode(MCP23017_INTA, INPUT);
    pinMode(MCP23017_INTB, INPUT);

    mcp.begin();

    // INTA=INTB, NOT FLOATING, ACTIVE LOW
    mcp.setupInterrupts(true, false, LOW);

    // GPIO DIRECTION AND PULLUP CONFIG
    // ALL INPUTS WITH PULLUPS ENABLED, FALLING EDGE TRIGGER
    mcp.pinMode(GPIN_A0, INPUT);
    mcp.pullUp(GPIN_A0, HIGH);
    mcp.setupInterruptPin(GPIN_A0,CHANGE); 

    mcp.pinMode(GPIN_A1, INPUT);
    mcp.pullUp(GPIN_A1, HIGH);
    mcp.setupInterruptPin(GPIN_A1,CHANGE); 

    mcp.pinMode(GPIN_A2, INPUT);
    mcp.pullUp(GPIN_A2, HIGH);
    mcp.setupInterruptPin(GPIN_A2,CHANGE); 
    
    mcp.pinMode(GPIN_A3, INPUT);
    mcp.pullUp(GPIN_A3, HIGH);
    mcp.setupInterruptPin(GPIN_A3,CHANGE); 
    
    mcp.pinMode(GPIN_A4, INPUT);
    mcp.pullUp(GPIN_A4, HIGH);
    mcp.setupInterruptPin(GPIN_A4,CHANGE); 
    
    mcp.pinMode(GPIN_A5, INPUT);
    mcp.pullUp(GPIN_A5, HIGH);
    mcp.setupInterruptPin(GPIN_A5,CHANGE); 
    
    mcp.pinMode(GPIN_A6, INPUT);
    mcp.pullUp(GPIN_A6, HIGH);
    mcp.setupInterruptPin(GPIN_A6,CHANGE); 
    
    mcp.pinMode(GPIN_A7, INPUT);
    mcp.pullUp(GPIN_A7, HIGH);
    mcp.setupInterruptPin(GPIN_A7,CHANGE); 
    

    mcp.pinMode(GPIN_B0, INPUT);
    mcp.pullUp(GPIN_B0, HIGH);
    mcp.setupInterruptPin(GPIN_B0,CHANGE); 
    
    mcp.pinMode(GPIN_B1, INPUT);
    mcp.pullUp(GPIN_B1, HIGH);
    mcp.setupInterruptPin(GPIN_B1,CHANGE); 
    
    mcp.pinMode(GPIN_B2, INPUT);
    mcp.pullUp(GPIN_B2, HIGH);
    mcp.setupInterruptPin(GPIN_B2,CHANGE); 
    
    mcp.pinMode(GPIN_B3, INPUT);
    mcp.pullUp(GPIN_B3, HIGH);
    mcp.setupInterruptPin(GPIN_B3,CHANGE); 

    mcp.readGPIOAB();
}

void GravitoneButtonInterface::setCallback(void (*callback)(GravitoneButtonEvent))
{
    buttonEventCallback = callback;
}

void GravitoneButtonInterface::processInterrupt()
{

    // Get more information from the MCP from the INT
    uint8_t pin = mcp.getLastInterruptPin();
    uint8_t val = mcp.getLastInterruptPinValue();

    // create gravitoneButton event to send
    GravitoneButtonEvent e;
    e.id = pin;
    e.value = val;

    // call the user supplied callback
    buttonEventCallback(e);
}
