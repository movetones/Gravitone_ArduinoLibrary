// Gravitone.h Gravitone library header
// Matt Ruffner Sept 20 2019

#ifndef GRAVITONE_H
#define GRAVITONE_H

#include <Arduino.h>

// PIN DEFINITIONS FOR GRAVITONE V1.5 PCB
// CONNECTED TO A TEENSY 3.2
#define MCP23017_INTA	8  // MCP -> ARDUINO
#define MCP23017_INTB	7  // MCP -> ARDUINO
#define MAX98357_MODE	11 // LOW DISABLES, HIGH-Z ENABLES
#define OLED_CONTROL	12 // ACTIVE HIGH
#define BATTERY_SENSE	21 // ANALOG
#define BATTERY_STAT	20 // HIGH = CHARGING
#define ICM20948_INT	13 // ICM -> ARDUINO
#define ICM20948_SYNC	14 // ARDUINO -> ICM

#include "util/volume.h"
#include "util/battery.h"
#include "util/scale.h"
#include "util/chords.h"
#include "GravitoneButtonInterface.h"

#endif
