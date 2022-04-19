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


#ifndef GRAVITONE_OUTPUT_MODE_H
#define GRAVITONE_OUTPUT_MODE_H

#include "GravitoneMode.h"

/** @class GravitoneOutputMode
 * A Gravitone mode that adds support for outputting over I2S and provides
 * volume control through an amp AudioObject
 *
 * TODO: add USB output support
 */
class GravitoneOutputMode : public GravitoneMode {
public:
  GravitoneOutputMode() : volume(1) {};
  ~GravitoneOutputMode() {  };

  /**
   * @brief begin
   * make the necessary audio connections for audio output
   * called by the containing Gravitone object
   */
  void begin() {
    addPatch( new AudioConnection(amp1, 0, hardware->i2s1, 0) );
    addPatch( new AudioConnection(amp1, 0, hardware->i2s1, 1) );
  }

  /**
   * @brief start
   */
  void start() {
    hardware->enableAmp();
    drawVolume();
    setVolume();
    reconnectPatches();
    Serial.print("End of GravitoneOutputMode start(), numPatches = ");
    Serial.println(numPatches);
  }
  
  /**
   * @brief stop
   * prepare this mode to be made inactive by disconnecting all of its
   * audio objects from the shared output.
   */
  void stop() {
    disconnectPatches();
    hardware->disableAmp();
  }
  
  virtual void button2(butevent_t event) {  
    if( event == BUTTON_PRESSED ){
      cycleVolume(VOL_DOWN);
    }
  };
  
  virtual void button3(butevent_t event) {
    if( event == BUTTON_PRESSED ){
      cycleVolume(VOL_UP);
    }
  }
  
  void drawVolume();
  void cycleVolume(int dir);
  void setVolume(int lvl); // 0 through 4 
  void setVolume(); // uses member var

  AudioAmplifier           amp1;           //xy=323,403
  
  int volume;                  
};


#endif
