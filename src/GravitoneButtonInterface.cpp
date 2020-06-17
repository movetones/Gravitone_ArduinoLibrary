#include "GravitoneButtonInterface.h"

void GravitoneButtonInterface::begin()
{
    // SET INTERRUPT PINS AS INPUTS
    pinMode(MCP23017_INTA, INPUT);
    pinMode(MCP23017_INTB, INPUT);

    mcp.begin();
    
    // INTA=INTB, NOT FLOATING, ACTIVE LOW
    //mcp.setupInterrupts(true, false, LOW);

    // GPIO DIRECTION AND PULLUP CONFIG
    // ALL INPUTS WITH PULLUPS ENABLED, FALLING EDGE TRIGGER
    mcp.pinMode(GPIN_A0, INPUT);
    mcp.pullUp(GPIN_A0, HIGH);
    //mcp.setupInterruptPin(GPIN_A0,CHANGE); 

    mcp.pinMode(GPIN_A1, INPUT);
    mcp.pullUp(GPIN_A1, HIGH);
    //mcp.setupInterruptPin(GPIN_A1,CHANGE); 

    mcp.pinMode(GPIN_A2, INPUT);
    mcp.pullUp(GPIN_A2, HIGH);
    //mcp.setupInterruptPin(GPIN_A2,CHANGE); 
    
    mcp.pinMode(GPIN_A3, INPUT);
    mcp.pullUp(GPIN_A3, HIGH);
    //mcp.setupInterruptPin(GPIN_A3,CHANGE); 
    
    mcp.pinMode(GPIN_A4, INPUT);
    mcp.pullUp(GPIN_A4, HIGH);
    //mcp.setupInterruptPin(GPIN_A4,CHANGE); 
    
    mcp.pinMode(GPIN_A5, INPUT);
    mcp.pullUp(GPIN_A5, HIGH);
    //mcp.setupInterruptPin(GPIN_A5,CHANGE); 
    
    mcp.pinMode(GPIN_A6, INPUT);
    mcp.pullUp(GPIN_A6, HIGH);
    //mcp.setupInterruptPin(GPIN_A6,CHANGE); 
    
    mcp.pinMode(GPIN_A7, INPUT);
    mcp.pullUp(GPIN_A7, HIGH);
    //mcp.setupInterruptPin(GPIN_A7,CHANGE); 
    

    mcp.pinMode(GPIN_B0, INPUT);
    mcp.pullUp(GPIN_B0, HIGH);
    //mcp.setupInterruptPin(GPIN_B0,CHANGE); 
    
    mcp.pinMode(GPIN_B1, INPUT);
    mcp.pullUp(GPIN_B1, HIGH);
    //mcp.setupInterruptPin(GPIN_B1,CHANGE); 
    
    mcp.pinMode(GPIN_B2, INPUT);
    mcp.pullUp(GPIN_B2, HIGH);
    //mcp.setupInterruptPin(GPIN_B2,CHANGE); 
    
    mcp.pinMode(GPIN_B3, INPUT);
    mcp.pullUp(GPIN_B3, HIGH);
    //mcp.setupInterruptPin(GPIN_B3,CHANGE); 

    mcp.readGPIOAB();

    
    int i;
    for( i=0; i<12; i++ )
    {
        states[i].id = i;
        states[i].value = 0;
    }

    
    update();
}

bool* GravitoneButtonInterface::changed()
{
    return statesChanged;
}

bool GravitoneButtonInterface::available()
{
    int i = 0;
    for( i=0; i<12; i++ ){
        if( statesChanged[i] ) return true;
    }
    return false;
}

void GravitoneButtonInterface::update()
{
    // READ CURRENT STATE
    uint16_t i, curPinVals = mcp.readGPIOAB();
    bool iVal;

    for( i=0; i<12; i++ )
    {
        // READ ITH BUTTON VALUE AND INVERT FOR NEG LOGIC
        iVal = !((curPinVals & (1 << i)) > 0);
        
        // SET STATE CHANGED BOOLEAN
        if( iVal != states[i].value ){
            statesChanged[i] = true;
        } else {
            statesChanged[i] = false;
        }
        
        // STORE CURRENT VALUE AND CALL EVENT HANDLER
        states[i].value = iVal;
        eventHandler(i, iVal);
    }
}
