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


#define VOL_UP 1    //! volume up
#define VOL_DOWN -1 //! volume down


/** @class GravitoneOutputMode
 * A Gravitone mode that adds support for outputting over I2S and provides
 * volume control through an amp AudioObject
 *
 * TODO: add USB output support
 */
class GravitoneOutputMode : public GravitoneMode {
public:
  /**
   * @brief Default constructor - set initial volume to 1
   */
  GravitoneOutputMode() : volume(1) { setVolume(); };

  /**
   * @brief constructor with optional initial volume setting
   * @param vol the initial volume to set
   */
  GravitoneOutputMode(int vol) : volume(vol) { setVolume(); }

  /**
   * @brief default destructor
   */
  ~GravitoneOutputMode() {  };

  /**
   * @brief begin
   * make the necessary audio connections for audio output
   * called by the containing Gravitone object
   */
  void begin();

  /**
   * @brief make audio connections and do any other initialization.
   * Called by the gravitone object when swapping active modes.
   */
  void start();
  
  /**
   * @brief prepare this mode to be made inactive by disconnecting all of its
   * audio objects from the shared output.
   * Called by the Gravitone object when switching modes.
   */
  void stop();
  
  /**
   * @brief utilize button 2 for volume down
   * @param event the button event type passed from the hardware
   */
  virtual void button2(butevent_t event) {  
    if( event == BUTTON_PRESSED ){
      cycleVolume(VOL_DOWN);
    }
  };
  
  /**
   * @brief utilize button 3 for volume up
   * @param event the button event type passed from the hardware
   */
  virtual void button3(butevent_t event) {
    if( event == BUTTON_PRESSED ){
      cycleVolume(VOL_UP);
    }
  }
  
  /**
   * @brief draw the volume icon based on current volume
   */
  void drawVolume();

  /**
   * @brief cycle
   * @param dir: direction to change volume, VOL_UP or VOL_DOWN
   */
  void cycleVolume(int dir);

  /**
   * @brief directly set the volume. <=0 is muted. 1-4 are acceptable levels
   * @param lvl: 0 - 4 volume setting
   */
  void setVolume(int lvl); // 0 through 4 

  /**
   * @brief set amp1 gain based on member var volume
   */
  void setVolume(); // uses member var


  AudioAmplifier           amp1; //! amp to control overall mode output volume level
  
  int volume; //! the current volume setting
};


#endif
