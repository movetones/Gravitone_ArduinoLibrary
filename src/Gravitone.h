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

/** @file Gravitone.h
 *
 * Main Gravitone object
 * Matt Ruffner 2022
 * MoveTones, LLC
 *
 * \mainpage Gravitone Library
 *
 * \section Overiew
 * This library is l
 *
 * <img src="../images/button-mapping.png" width="500px">
 */

#ifndef GRAVITONE_H
#define GRAVITONE_H

#include <Audio.h>
#include <Arduino.h>

#include "config.h"
#include "GravitoneHardware.h"
#include "GravitoneMode.h"

/** @class Gravitone
 * The main object representing the Gravitone device.
 *
 * Instantiate one of these objects in a main arduino sketch to use the Gravitone.
 */
class Gravitone {
public:
  /**
   * @brief Gravitone
   * default constructor
   */
  Gravitone() : numModes(0), activeMode(-1) {
    for( int i=0; i<MAX_MODES; i++ ){
      init[i] = false;
    }
  };

  ~Gravitone() {};
  
  /**
   * @brief initialize the gravitone
   *
   * This initializes the hardware drivers and sets pin modes etc.
   */
  bool begin();
  
  /**
   * @brief eventLoop
   *
   * call this function often to process button, screen, and IMU
   * operations for the driver
   */
  void eventLoop();

  /**
   * @brief setActiveMode
   * @param id
   *
   * set the active mode to be the one at position id within
   * the modes array
   */
  void setActiveMode(int id);

  /**
   * @brief getActiveMode
   * @return the integer id corresponding to the active mode instance
   */
  int getActiveMode() { return activeMode; }
  

  /**
   * @brief addMode
   * @param m
   *
   * add a given pointer to a mode, add it to the array of modes
   * to switch between
   */
  void addMode(GravitoneMode *m);
  
  int numModes;   //! the total number of installed modes
  int activeMode; //! the
  
  bool init[MAX_MODES]; //! initialization status of installed modes

  GravitoneMode *mode; //! the current mode
  
  GravitoneMode *modes[MAX_MODES]; //! pointers to each installed mode
  
  GravitoneHardware hardware; //! the driver instance
  
private:
  void handleButtons(int bid);
};


#endif
