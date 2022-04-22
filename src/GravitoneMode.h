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


#ifndef GRAVITONE_MODE_H
#define GRAVITONE_MODE_H

#include <Arduino.h>
#include <Audio.h>
#include "GravitoneHardware.h"
#include "util/buttons.h"
#include "util/battery.h"
#include "util/volume.h"
#include "config.h"


/**
 * @class The GravitoneMode class
 *
 * This class provides base functionality for a switchable mode to be built on.
 *
 */
class GravitoneMode {
public:
  /**
   * @brief GravitoneMode
   */
  GravitoneMode() : numPatches(0) {};
  
  /**
   *
   */
  ~GravitoneMode() { };
  
  /**
   * @brief addPatch
   * @param cable
   */
  void addPatch(AudioConnection *cable);

  /**
   * @brief reconnectPatches
   */
  void reconnectPatches();

  /**
   * @brief disconnectPatches
   */
  void disconnectPatches();
  
  /**
   * @brief setHardware
   * @param h
   */
  void setHardware(GravitoneHardware *h) { hardware = h; };
  
  /**
   * @brief begin
   */
  virtual void begin() {};

  /**
   * @brief start
   */
  virtual void start() {};

  /**
   * @brief stop
   */
  virtual void stop() { hardware->display.clearDisplay(); };
  
  /**
   * @brief functionality for button 1 to be overridden by a derived class
   * @param event
   */
  virtual void button1(butevent_t event) {};

  /**
   * @brief button2
   * @param event
   */
  virtual void button2(butevent_t event) {};

  /**
   * @brief button3
   * @param event
   */
  virtual void button3(butevent_t event) {};

  /**
   * @brief button4
   * @param event
   */
  virtual void button4(butevent_t event) {};

  /**
   * @brief button5
   * @param event
   */
  virtual void button5(butevent_t event) {};

  /**
   * @brief button6
   * @param event
   */
  virtual void button6(butevent_t event) {};

  /**
   * @brief button7
   * @param event
   */
  virtual void button7(butevent_t event) {};

  /**
   * @brief button8
   * @param event
   */
  virtual void button8(butevent_t event) {};

  /**
   * @brief button9
   * @param event
   */
  virtual void button9(butevent_t event) {};

  /**
   * @brief button10
   * @param event
   */
  virtual void button10(butevent_t event) {};

  /**
   * @brief button11
   * @param event
   */
  virtual void button11(butevent_t event) {};

  /**
   * @brief button12
   * @param event
   */
  virtual void button12(butevent_t event) {};
  
  /**
   * @brief onUpdateOrientation
   */
  virtual void onUpdateOrientation() {};

  /**
   * @brief onUpdateDisplay
   */
  virtual void onUpdateDisplay() {};
  
  /**
   * @brief getName
   * @return
   */
  virtual const char *getName() {};
  
  GravitoneHardware *hardware; //! our reference to the hardware
  
  AudioConnection *patchCoords[MAX_PATCHES]; //! any audio connections this mode makes
  int numPatches; //! how many patches were instantiated (not necessarily how many are active)
};


#endif
