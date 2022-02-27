#ifndef VOLUME_H
#define VOLUME_H

#define VOL_UP    1
#define VOL_DOWN -1

static const unsigned char PROGMEM drawable_8x8_volume_mute[] =
{ B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001 };

static const unsigned char PROGMEM drawable_8x8_volume_25[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11000000,
  B11111111 };

static const unsigned char PROGMEM drawable_8x8_volume_50[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00110000,
  B00110000,
  B11110000,
  B11111111 };

static const unsigned char PROGMEM drawable_8x8_volume_75[] =
{ B00000000,
  B00000000,
  B00001100,
  B00001100,
  B00111100,
  B00111100,
  B11111100,
  B11111111 };

static const unsigned char PROGMEM drawable_8x8_volume_100[] =
{ B00000011,
  B00000011,
  B00001111,
  B00001111,
  B00111111,
  B00111111,
  B11111111,
  B11111111 };

static const uint8_t* getDrawableForVolumeLevel(uint8_t lvl)
{
  switch ( lvl ) {
    case 0: return drawable_8x8_volume_mute; break;
    case 1: return drawable_8x8_volume_25; break;
    case 2: return drawable_8x8_volume_50; break;
    case 3: return drawable_8x8_volume_75; break;
    case 4: return drawable_8x8_volume_100; break;
  }
}


#endif
