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

class GravitoneMode {
public:
  GravitoneMode() : numPatches(0) {};
  
  ~GravitoneMode() { };
  
  void addPatch(AudioConnection *cable);
  void reconnectPatches();
  void disconnectPatches();
  
  void setHardware(GravitoneHardware *h) { hardware = h; };
  
  virtual void begin() {};
  virtual void start() {};
  virtual void stop() { hardware->display.clearDisplay(); };
  
  virtual void button1(butevent_t event) {};
  virtual void button2(butevent_t event) {};
  virtual void button3(butevent_t event) {};
  virtual void button4(butevent_t event) {};
  virtual void button5(butevent_t event) {};
  virtual void button6(butevent_t event) {};
  virtual void button7(butevent_t event) {};
  virtual void button8(butevent_t event) {};
  virtual void button9(butevent_t event) {};
  virtual void button10(butevent_t event) {};
  virtual void button11(butevent_t event) {};
  virtual void button12(butevent_t event) {};
  
  virtual void onUpdateOrientation() {};
  virtual void onUpdateDisplay() {};
  
  virtual const char *getName() {};
  
  GravitoneHardware *hardware;
  
  AudioConnection *patchCoords[MAX_PATCHES];
  int numPatches;
};


#endif
