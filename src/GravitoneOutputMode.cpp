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


#include "GravitoneOutputMode.h"

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void GravitoneOutputMode::drawVolume() {
  hardware->display.drawBitmap(102, 0,  getDrawableForVolumeLevel(volume), 8, 8, 1, 0);
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void GravitoneOutputMode::cycleVolume(int dir) {
  if ( dir > 0) {
    if ( volume >= 4 ) return;
    volume = (volume + 1) % 5;
  } else if (dir < 0) {
    if ( volume <= 0 ) return;
    volume = (volume - 1) % 5;
  }
  setVolume();
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void GravitoneOutputMode::setVolume() {
  setVolume(volume);
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void GravitoneOutputMode::setVolume(int vol) {
  if ( vol > 0 ) {
    hardware->enableAmp();
    switch ( vol ) {
      case 1: 
        amp1.gain(0.2);
        break;
      case 2: 
        amp1.gain(0.5);
        break;
      case 3: 
        amp1.gain(0.7);
        break;
      case 4: 
        amp1.gain(0.9);
        break;
    }
  } else {
    hardware->disableAmp();
  }
  drawVolume();
}
