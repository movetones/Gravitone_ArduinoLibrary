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
