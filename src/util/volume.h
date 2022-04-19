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


#ifndef VOLUME_H
#define VOLUME_H


static unsigned char PROGMEM drawable_8x8_volume_mute[] =
{ B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001 };

static unsigned char PROGMEM drawable_8x8_volume_25[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11000000,
  B11111111 };

static unsigned char PROGMEM drawable_8x8_volume_50[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00110000,
  B00110000,
  B11110000,
  B11111111 };

static unsigned char PROGMEM drawable_8x8_volume_75[] =
{ B00000000,
  B00000000,
  B00001100,
  B00001100,
  B00111100,
  B00111100,
  B11111100,
  B11111111 };

static unsigned char PROGMEM drawable_8x8_volume_100[] =
{ B00000011,
  B00000011,
  B00001111,
  B00001111,
  B00111111,
  B00111111,
  B11111111,
  B11111111 };

static uint8_t* getDrawableForVolumeLevel(uint8_t lvl)
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
