#ifndef BATTERY_H
#define BATTERY_H

#define DRAWABLE_16X8_BATTERY_WIDTH 16 
#define DRAWABLE_16X8_BATTERY_HEIGHT  8

#include <Arduino.h>

extern unsigned char drawable_16x8_battery_100[16];
extern unsigned char drawable_16x8_battery_80[16];
extern unsigned char drawable_16x8_battery_60[16];  
extern unsigned char drawable_16x8_battery_40[16];
extern unsigned char drawable_16x8_battery_20[16];
extern unsigned char drawable_16x8_battery_charge[16];  
  
uint8_t* getDrawableForBatteryLevel(uint8_t lvl);

uint8_t getBatteryLevelFromVoltage(float v);

#endif
