#ifndef BATTERY_H
#define BATTERY_H

#define DRAWABLE_16X8_BATTERY_WIDTH 16 
#define DRAWABLE_16X8_BATTERY_HEIGHT  8

static const unsigned char PROGMEM drawable_16x8_battery_100[] =
{ B11111111, B11111110,
  B11111111, B11111110,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111110,
  B11111111, B11111110 };

static const unsigned char PROGMEM drawable_16x8_battery_80[] =
{ B11111111, B11111110,
  B11111111, B11110010,
  B11111111, B11110011,
  B11111111, B11110011,
  B11111111, B11110011,
  B11111111, B11110011,
  B11111111, B11110010,
  B11111111, B11111110 };
  
static const unsigned char PROGMEM drawable_16x8_battery_60[] =
{ B11111111, B11111110,
  B11111111, B11000010,
  B11111111, B11000011,
  B11111111, B11000011,
  B11111111, B11000011,
  B11111111, B11000011,
  B11111111, B11000010,
  B11111111, B11111110 };
  

static const unsigned char PROGMEM drawable_16x8_battery_40[] =
{ B11111111, B11111110,
  B11111100, B00000010,
  B11111100, B00000011,
  B11111100, B00000011,
  B11111100, B00000011,
  B11111100, B00000011,
  B11111100, B00000010,
  B11111111, B11111110 };
  
static const unsigned char PROGMEM drawable_16x8_battery_20[] =
{ B11111111, B11111110,
  B11110000, B00000010,
  B11110000, B00000011,
  B11110000, B00000011,
  B11110000, B00000011,
  B11110000, B00000011,
  B11110000, B00000010,
  B11111111, B11111110 };
  
static const unsigned char PROGMEM drawable_16x8_battery_charge[] =
{ B11111111, B11111110,
  B11111110, B01111110,
  B11111110, B01111111,
  B11111000, B00011111,
  B11111000, B00011111,
  B11111110, B01111111,
  B11111110, B01111110,
  B11111111, B11111110 };
  
static const uint8_t* getDrawableForBatteryLevel(uint8_t lvl)
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

#endif
