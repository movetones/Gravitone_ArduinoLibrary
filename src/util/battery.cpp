/* Gravitone Library
 * Copyright (c) 2022, Matt Ruffner <matthew.ruffner@movetones.com>
 * MoveTones, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "battery.h"


unsigned char PROGMEM drawable_16x8_battery_100[16] =
{ B11111111, B11111110,
  B11111111, B11111110,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111110,
  B11111111, B11111110 };

unsigned char PROGMEM drawable_16x8_battery_80[16] =
{ B11111111, B11111110,
  B11111111, B11110010,
  B11111111, B11110011,
  B11111111, B11110011,
  B11111111, B11110011,
  B11111111, B11110011,
  B11111111, B11110010,
  B11111111, B11111110 };
  
unsigned char PROGMEM drawable_16x8_battery_60[16] =
{ B11111111, B11111110,
  B11111111, B11000010,
  B11111111, B11000011,
  B11111111, B11000011,
  B11111111, B11000011,
  B11111111, B11000011,
  B11111111, B11000010,
  B11111111, B11111110 };
  

unsigned char PROGMEM drawable_16x8_battery_40[16] =
{ B11111111, B11111110,
  B11111100, B00000010,
  B11111100, B00000011,
  B11111100, B00000011,
  B11111100, B00000011,
  B11111100, B00000011,
  B11111100, B00000010,
  B11111111, B11111110 };
  
unsigned char PROGMEM drawable_16x8_battery_20[16] =
{ B11111111, B11111110,
  B11110000, B00000010,
  B11110000, B00000011,
  B11110000, B00000011,
  B11110000, B00000011,
  B11110000, B00000011,
  B11110000, B00000010,
  B11111111, B11111110 };
  
unsigned char PROGMEM drawable_16x8_battery_charge[16] =
{ B11111111, B11111110,
  B11111110, B01111110,
  B11111110, B01111111,
  B11111000, B00011111,
  B11111000, B00011111,
  B11111110, B01111111,
  B11111110, B01111110,
  B11111111, B11111110 };
  
uint8_t* getDrawableForBatteryLevel(uint8_t lvl)
{
  switch ( lvl ) {
    case 5: return drawable_16x8_battery_charge; break;
    case 4: return drawable_16x8_battery_100; break;
    case 3: return drawable_16x8_battery_80; break;
    case 2: return drawable_16x8_battery_60; break;
    case 1: return drawable_16x8_battery_40; break;
    case 0: return drawable_16x8_battery_20; break;
  }
}


uint8_t getBatteryLevelFromVoltage(float v)
{
  if( v > 4.7 ){
    return 5;
  } 
  else if ( v >= 4.0 ) {
    return 4;
  }
  else if ( v >= 3.8 ) {
    return 3;
  }
  else if ( v >= 3.6 ) {
    return 2;
  }
  else if ( v >= 3.4 ) {
    return 1;
  }
  else if ( v < 3.4 ) {
    return 0;
  }
  return 0;
}

